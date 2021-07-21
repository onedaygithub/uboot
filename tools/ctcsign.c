#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <uboot_aes.h>
#include <u-boot/rsa-mod-exp.h>
#include <u-boot/sha256.h>
#include <u-boot/rsa.h>
#include <u-boot/rsa-checksum.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/engine.h>

#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))

#define CER_VER      0x10

#define CER_RSA_LEN  256
#define CER_SIG_LEN  256
#define CER_HASH_LEN 32
#define CER_KEY_NUM  4

#define CER_ID_KEY   0
#define CER_ID_IMG   1

#define OPR_HASH  0
#define OPR_CERT  1
#define OPR_AUTH  2
#define OPR_ENCR  3
#define OPR_DECR  4

struct ctcsign_params {
    char *cmdname;
	int key_num;
    int nv_count;
    int opr;
    int key_level;
    int hash_level;
    int cert_level;
    int auth_level;
    int encrypt;
    char *key_dir;
    char *privkey_file;
    char *pubkey_file;
    char *src_file;
    char *dst_file;
    char *aes_key;
};

struct rsa2048_pubkey_prop {
	uint32_t num_bits;	/* Key length in bits */
	uint32_t n0inv;		/* -1 / modulus[0] mod 2^32 */
	uint32_t exp_len;	/* Exponent length in number of uint8_t */
	uint64_t public_exponent; /* public exponent as byte array */
	char rr[CER_RSA_LEN];		/* R^2 can be treated as byte array */
	char modulus[CER_RSA_LEN];	/* modulus as byte array */
};

struct ctc_csum_algo {
	const char *name;
	const int checksum_len;
	const int der_len;
	const uint8_t *der_prefix;
    const EVP_MD *(*calculate_sign)(void);
	int (*calculate)(const char *name,
			 const struct image_region region[],
			 int region_count, uint8_t *checksum);
};

struct ctc_cert {
    uint8_t ver;
    uint8_t id;
    uint8_t nv;
    uint8_t encrypt;
    uint32_t img_size;
    uint32_t key_offset;
    uint32_t key_size;
    uint32_t sig_offset;
    uint32_t sig_size;
    struct rsa2048_pubkey_prop pkey_prop[CER_KEY_NUM];
    uint8_t signature[CER_SIG_LEN];
};

struct ctc_cert g_cert;
struct key_prop g_prop;
struct crypto_algo *g_p_crypto_algo;
struct ctc_csum_algo *g_p_cksum_alogo;
struct ctcsign_params g_ctcparams;

uint64_t g_exponent;
char g_modulus[CER_RSA_LEN];
char g_r_squared[CER_RSA_LEN];

struct ctc_csum_algo g_ctc_csum_algos = {
		"sha256",
		SHA256_SUM_LEN,
		SHA256_DER_LEN,
		sha256_der_prefix,
		EVP_sha256,
		hash_calculate,
};

struct crypto_algo g_ctc_crypto_algos = {
		"rsa4096",
		RSA4096_BYTES,
		rsa_sign,
		rsa_add_verify_data,
		rsa_verify,
};

static void ctcsign_usage(const char *msg)
{
    if(msg)
	    fprintf(stderr, "Error: %s\n", msg);
    
	fprintf(stderr,
		"          -H ==> gen level N public key hash to file:hash1.txt or hash2.txt\n"
		"                 %s -H level_N -n PubKeyNum -d PubKeyDir\n"
		"          -C ==> gen level N certificate to file:cert1.txt or cert2.txt\n"
		"                 %s -C 1 -n PubKeyNum -c NVCount -e Encrypt -d PubKeyDir\n"
		"                 %s -C 2 -n PubKeyNum -c NVCount -e Encrypt -s SrcImage -d PubKeyDir\n"
		"          -A ==> perform level N authentication\n"
		"                 %s -A 1 -n PubKeyNum -c NVCount -d PubKeyDir\n"
		"                 %s -A 2 -c NVCount -s SrcImage -d PubKeyDir\n"
		"          -E ==> Perform image AES-128 encrypt\n"
		"                 %s -E -s SrcImage -k AES128Key\n"
		"          -D ==> perform image AES-128 decrypt\n"
		"                 %s -D -s SrcImage -k AES128Key\n"
		"          -n ==> rsa key pair number\n"
		"          -c ==> nv count\n"
		"          -d ==> set key directory\n"
		"          -p ==> set private key file\n"
		"          -P ==> set public key file\n"
		"          -s ==> set source file to sign or encrypt\n"
		"          -o ==> set output file name\n"
		"          -k ==> set AES-128 key\n"
		"          -e ==> enable AES encrypt or not\n"
		"          -h ==> usage\n", 
		g_ctcparams.cmdname, g_ctcparams.cmdname, g_ctcparams.cmdname, g_ctcparams.cmdname,
		g_ctcparams.cmdname, g_ctcparams.cmdname, g_ctcparams.cmdname);

	exit(EXIT_FAILURE);
}

static void process_ctcsign_args(int argc, char **argv)
{
	char *ptr;
	int opt;

    g_ctcparams.cmdname = *argv;
    
	while ((opt = getopt(argc, argv, "H:C:A:EDn:c:d:p:P:s:o:k:e:h")) != -1) {
		switch (opt) {
		case 'H':
            g_ctcparams.opr = OPR_HASH;
			g_ctcparams.hash_level = strtoul(optarg, &ptr, 16);
            g_ctcparams.key_level = g_ctcparams.hash_level;
			if ((g_ctcparams.hash_level != 1) && (g_ctcparams.hash_level != 2)) {
				goto para_check_fail;
			}
			break;
		case 'C':
            g_ctcparams.opr = OPR_CERT;
			g_ctcparams.cert_level = strtoul(optarg, &ptr, 16);
            g_ctcparams.key_level = g_ctcparams.cert_level;
			if ((g_ctcparams.cert_level != 1) && (g_ctcparams.cert_level != 2)) {
				goto para_check_fail;
			}
			break;
		case 'A':
            g_ctcparams.opr = OPR_AUTH;
			g_ctcparams.auth_level = strtoul(optarg, &ptr, 16);
            g_ctcparams.key_level = g_ctcparams.auth_level;
			if ((g_ctcparams.auth_level != 1) && (g_ctcparams.auth_level != 2)) {
				goto para_check_fail;
			}
			break;
		case 'E':
            g_ctcparams.opr = OPR_ENCR;
			break;
		case 'D':
            g_ctcparams.opr = OPR_DECR;
			break;
		case 'n':
			g_ctcparams.key_num = strtoul(optarg, &ptr, 16);
			if ((g_ctcparams.key_num > 4) || (g_ctcparams.key_num < 1)) {
				goto para_check_fail;
			}
			break;
		case 'c':
			g_ctcparams.nv_count = strtoul(optarg, &ptr, 16);
			if ((g_ctcparams.nv_count != 0) && (g_ctcparams.nv_count != 1)
                && (g_ctcparams.nv_count != 3) &&(g_ctcparams.nv_count != 7)) {
				fprintf(stderr, "Invalid nv count, only 0,1,3,7 is valid\n");
				goto para_check_fail;
			}
			break;
		case 'e':
			g_ctcparams.encrypt = strtoul(optarg, &ptr, 16);
			break;
		case 'd':
			g_ctcparams.key_dir = optarg;
			break;
		case 'p':
			g_ctcparams.privkey_file = optarg;
			break;
		case 'P':
			g_ctcparams.pubkey_file = optarg;
			break;
		case 's':
			g_ctcparams.src_file = optarg;
			break;
		case 'o':
			g_ctcparams.dst_file = optarg;
			break;
		case 'k':
			g_ctcparams.aes_key = optarg;
			break;
		case 'h':
            goto para_check_fail;
		default:
			goto para_check_fail;
		}
	}

    switch(g_ctcparams.opr){
        case OPR_HASH :
            if(!g_ctcparams.key_dir)
                goto para_check_fail;
            break;
        case OPR_CERT :
            if(!g_ctcparams.key_dir)
                goto para_check_fail;
            if((g_ctcparams.cert_level == 2) && !g_ctcparams.src_file)
                goto para_check_fail;
            break;
        case OPR_AUTH :
            if(!g_ctcparams.key_dir)
                goto para_check_fail;
            if((g_ctcparams.auth_level == 2) && !g_ctcparams.src_file)
                goto para_check_fail;
            break;
        case OPR_ENCR :
        case OPR_DECR :
            if(!g_ctcparams.src_file || !g_ctcparams.aes_key)
                goto para_check_fail;
            break;
        default:
            goto para_check_fail;
    }

    return;
    
para_check_fail:
        ctcsign_usage("Invalid option");
        exit(EXIT_FAILURE);
}

static int ctc_rsa_err(const char *msg)
{
	unsigned long sslErr = ERR_get_error();

	fprintf(stderr, "%s", msg);
	fprintf(stderr, ": %s\n",
		ERR_error_string(sslErr, 0));

	return -1;
}

static void ctc_rsa_remove(void)
{
	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
#ifdef HAVE_ERR_REMOVE_THREAD_STATE
	ERR_remove_thread_state(NULL);
#else
	ERR_remove_state(0);
#endif
	EVP_cleanup();
}

static int ctc_rsa_pem_get_pub_key(const char *keydir, const char *name, RSA **rsap, int keylevel)
{
	char path[1024];
	EVP_PKEY *key;
	X509 *cert;
	RSA *rsa;
	FILE *f;
	int ret;

	*rsap = NULL;

	snprintf(path, sizeof(path), "%s/key%d/%s.crt", keydir, keylevel, name);    
	f = fopen(path, "r");
	if (!f) {
		fprintf(stderr, "Couldn't open RSA certificate: '%s': %s\n",
			path, strerror(errno));
		return -EACCES;
	}

	/* Read the certificate */
	cert = NULL;
	if (!PEM_read_X509(f, &cert, NULL, NULL)) {
		ctc_rsa_err("Couldn't read certificate");
		ret = -EINVAL;
		goto err_cert;
	}

	/* Get the public key from the certificate. */
	key = X509_get_pubkey(cert);
	if (!key) {
		ctc_rsa_err("Couldn't read public key\n");
		ret = -EINVAL;
		goto err_pubkey;
	}

	/* Convert to a RSA_style key. */
	rsa = EVP_PKEY_get1_RSA(key);
	if (!rsa) {
		ctc_rsa_err("Couldn't convert to a RSA style key");
		ret = -EINVAL;
		goto err_rsa;
	}
	fclose(f);
	EVP_PKEY_free(key);
	X509_free(cert);
	*rsap = rsa;

	return 0;

err_rsa:
	EVP_PKEY_free(key);
err_pubkey:
	X509_free(cert);
err_cert:
	fclose(f);
	return ret;
}

static int cal_bignum(char *data, BIGNUM *num, int num_bits)
{
	int nwords = num_bits / 32;
	int size;
	uint32_t *buf, *ptr;
	BIGNUM *tmp, *big2, *big32, *big2_32;
	BN_CTX *ctx;
	int ret;

	tmp = BN_new();
	big2 = BN_new();
	big32 = BN_new();
	big2_32 = BN_new();
	if (!tmp || !big2 || !big32 || !big2_32) {
		fprintf(stderr, "Out of memory (bignum)\n");
		return -ENOMEM;
	}
	ctx = BN_CTX_new();
	if (!tmp) {
		fprintf(stderr, "Out of memory (bignum context)\n");
		return -ENOMEM;
	}
	BN_set_word(big2, 2L);
	BN_set_word(big32, 32L);
	BN_exp(big2_32, big2, big32, ctx); /* B = 2^32 */

	size = nwords * sizeof(uint32_t);
	buf = malloc(size);
	if (!buf) {
		fprintf(stderr, "Out of memory (%d bytes)\n", size);
		return -ENOMEM;
	}

	/* Write out modulus as big endian array of integers */
	for (ptr = buf + nwords - 1; ptr >= buf; ptr--) {
		BN_mod(tmp, num, big2_32, ctx); /* n = N mod B */
		*ptr = cpu_to_fdt32(BN_get_word(tmp));
		BN_rshift(num, num, 32); /*  N = N/B */
	}

	/*
	 * We try signing with successively increasing size values, so this
	 * might fail several times
	 */

    memcpy(data, (char *)buf, size);
    
	free(buf);
	BN_free(tmp);
	BN_free(big2);
	BN_free(big32);
	BN_free(big2_32);

	return ret;
}

static int ctc_rsa_init(void)
{
	int ret;

	ret = SSL_library_init();
	if (!ret) {
		fprintf(stderr, "Failure to init SSL library\n");
		return -1;
	}
	SSL_load_error_strings();

	OpenSSL_add_all_algorithms();
	OpenSSL_add_all_digests();
	OpenSSL_add_all_ciphers();

	return 0;
}

static int ctc_rsa_pem_get_priv_key(const char *keydir, const char *name,
				RSA **rsap)
{
	char path[1024];
	RSA *rsa;
	FILE *f;

	*rsap = NULL;
	snprintf(path, sizeof(path), "%s/key%d/%s.key", keydir, g_ctcparams.key_level, name);
	f = fopen(path, "r");
	if (!f) {
		fprintf(stderr, "Couldn't open RSA private key: '%s': %s\n",
			path, strerror(errno));
		return -ENOENT;
	}

	rsa = PEM_read_RSAPrivateKey(f, 0, NULL, path);
	if (!rsa) {
		ctc_rsa_err("Failure reading private key");
		fclose(f);
		return -EPROTO;
	}
	fclose(f);
	*rsap = rsa;

	return 0;
}

static int ctc_rsa_sign_with_key(RSA *rsa, struct ctc_csum_algo *ctc_csum_algo,
		const struct image_region region[], int region_count,
		uint8_t **sigp, uint *sig_size)
{
	EVP_PKEY *key;
	EVP_MD_CTX *context;
	int size, ret = 0;
	uint8_t *sig;
	int i;

	key = EVP_PKEY_new();
	if (!key)
		return ctc_rsa_err("EVP_PKEY object creation failed");

	if (!EVP_PKEY_set1_RSA(key, rsa)) {
		ret = ctc_rsa_err("EVP key setup failed");
		goto err_set;
	}

	size = EVP_PKEY_size(key);
	sig = malloc(size);
	if (!sig) {
		fprintf(stderr, "Out of memory for signature (%d bytes)\n",
			size);
		ret = -ENOMEM;
		goto err_alloc;
	}

	context = EVP_MD_CTX_create();
	if (!context) {
		ret = ctc_rsa_err("EVP context creation failed");
		goto err_create;
	}
	EVP_MD_CTX_init(context);
	if (!EVP_SignInit(context, ctc_csum_algo->calculate_sign())) {
		ret = ctc_rsa_err("Signer setup failed");
		goto err_sign;
	}

	for (i = 0; i < region_count; i++) {
		if (!EVP_SignUpdate(context, region[i].data, region[i].size)) {
			ret = ctc_rsa_err("Signing data failed");
			goto err_sign;
		}
	}

	if (!EVP_SignFinal(context, sig, sig_size, key)) {
		ret = ctc_rsa_err("Could not obtain signature");
		goto err_sign;
	}
	EVP_MD_CTX_cleanup(context);
	EVP_MD_CTX_destroy(context);
	EVP_PKEY_free(key);

	*sigp = sig;
	*sig_size = size;

	return 0;

err_sign:
	EVP_MD_CTX_destroy(context);
err_create:
	free(sig);
err_alloc:
err_set:
	EVP_PKEY_free(key);
	return ret;
}

int ctc_rsa_sign(const struct image_region region[], int region_count,
	     uint8_t **sigp, uint *sig_len)
{
	RSA *rsa;
	int ret;
    int keyindex;
    char keyname[1024];

    memset(keyname, 0, 1024);
    
	ret = ctc_rsa_init();
	if (ret)
		return ret;

    if(g_ctcparams.nv_count == 0){
        keyindex = 0;
    }else if(g_ctcparams.nv_count == 1){
        keyindex = 1;
    }else if(g_ctcparams.nv_count == 3){
        keyindex = 2;
    }else{
        keyindex = 3;
    }
    snprintf(keyname, sizeof(keyname), "priv%d_%d",g_ctcparams.key_level, keyindex+1);
	ret = ctc_rsa_pem_get_priv_key(g_ctcparams.key_dir, keyname, &rsa);
	if (ret)
		goto err_priv;
    
	ret = ctc_rsa_sign_with_key(rsa, g_p_cksum_alogo, region,
				region_count, sigp, sig_len);
	if (ret)
		goto err_sign;

	RSA_free(rsa);
	ctc_rsa_remove();

	return ret;

err_sign:
	RSA_free(rsa);
err_priv:
	return ret;
}

/*
 * rsa_get_exponent(): - Get the public exponent from an RSA key
 */
static int ctc_rsa_get_exponent(RSA *key, uint64_t *e)
{
	int ret;
	BIGNUM *bn_te;
	uint64_t te;

	ret = -EINVAL;
	bn_te = NULL;

	if (!e)
		goto cleanup;

	if (BN_num_bits(key->e) > 64)
		goto cleanup;

	*e = BN_get_word(key->e);

	if (BN_num_bits(key->e) < 33) {
		ret = 0;
		goto cleanup;
	}

	bn_te = BN_dup(key->e);
	if (!bn_te)
		goto cleanup;

	if (!BN_rshift(bn_te, bn_te, 32))
		goto cleanup;

	if (!BN_mask_bits(bn_te, 32))
		goto cleanup;

	te = BN_get_word(bn_te);
	te <<= 32;
	*e |= te;
	ret = 0;

cleanup:
	if (bn_te)
		BN_free(bn_te);

	return ret;
}

/*
 * rsa_get_params(): - Get the important parameters of an RSA public key
 */
int ctc_rsa_get_params(RSA *key, uint64_t *exponent, uint32_t *n0_invp,
		   BIGNUM **modulusp, BIGNUM **r_squaredp)
{
	BIGNUM *big1, *big2, *big32, *big2_32;
	BIGNUM *n, *r, *r_squared, *tmp;
	BN_CTX *bn_ctx = BN_CTX_new();
	int ret = 0;

	/* Initialize BIGNUMs */
	big1 = BN_new();
	big2 = BN_new();
	big32 = BN_new();
	r = BN_new();
	r_squared = BN_new();
	tmp = BN_new();
	big2_32 = BN_new();
	n = BN_new();
	if (!big1 || !big2 || !big32 || !r || !r_squared || !tmp || !big2_32 ||
	    !n) {
		fprintf(stderr, "Out of memory (bignum)\n");
		return -ENOMEM;
	}

	if (0 != ctc_rsa_get_exponent(key, exponent))
		ret = -1;

	if (!BN_copy(n, key->n) || !BN_set_word(big1, 1L) ||
	    !BN_set_word(big2, 2L) || !BN_set_word(big32, 32L))
		ret = -1;

	/* big2_32 = 2^32 */
	if (!BN_exp(big2_32, big2, big32, bn_ctx))
		ret = -1;

	/* Calculate n0_inv = -1 / n[0] mod 2^32 */
	if (!BN_mod_inverse(tmp, n, big2_32, bn_ctx) ||
	    !BN_sub(tmp, big2_32, tmp))
		ret = -1;
	*n0_invp = BN_get_word(tmp);

	/* Calculate R = 2^(# of key bits) */
	if (!BN_set_word(tmp, BN_num_bits(n)) ||
	    !BN_exp(r, big2, tmp, bn_ctx))
		ret = -1;

	/* Calculate r_squared = R^2 mod n */
	if (!BN_copy(r_squared, r) ||
	    !BN_mul(tmp, r_squared, r, bn_ctx) ||
	    !BN_mod(r_squared, tmp, n, bn_ctx))
		ret = -1;

	*modulusp = n;
	*r_squaredp = r_squared;

	BN_free(big1);
	BN_free(big2);
	BN_free(big32);
	BN_free(r);
	BN_free(tmp);
	BN_free(big2_32);
	if (ret) {
		fprintf(stderr, "Bignum operations failed\n");
		return -ENOMEM;
	}

	return ret;
}

int ctc_get_key_prop(struct key_prop *prop, int keylevel, int keyindex)
{
    char keyname[1024];
	BIGNUM *modulus, *r_squared;
	uint64_t exponent;
	uint32_t n0_inv;
	int ret;
	int bits;
	RSA *rsa;

    memset(keyname, 0, 1024);
    memset(g_modulus, 0, sizeof(g_modulus));
    memset(g_r_squared, 0, sizeof(g_r_squared));
    snprintf(keyname, sizeof(keyname), "pub%d_%d",keylevel, keyindex+1);
	ret = ctc_rsa_pem_get_pub_key(g_ctcparams.key_dir, keyname, &rsa, keylevel);
	if (ret)
		goto err_get_pub_key;
	ret = ctc_rsa_get_params(rsa, &exponent, &n0_inv, &modulus, &r_squared);
	if (ret)
		goto err_get_params;
	bits = BN_num_bits(modulus);
    
    cal_bignum(g_modulus, modulus, bits);
    cal_bignum(g_r_squared, r_squared, bits);
    
    g_exponent    = cpu_to_be64(exponent);
    prop->num_bits = bits;
    prop->modulus = g_modulus;
    prop->public_exponent = &g_exponent;
    prop->exp_len = sizeof(uint64_t);
    prop->n0inv = n0_inv;
    prop->rr = g_r_squared;

err_get_params:
	RSA_free(rsa);
err_get_pub_key:

	return ret;
}

static int ctc_rsa_verify_padding(const uint8_t *msg, const int pad_len,
			      struct ctc_csum_algo *algo)
{
	int ff_len;
	int ret;

	/* first byte must be 0x00 */
	ret = *msg++;
	/* second byte must be 0x01 */
	ret |= *msg++ ^ 0x01;
	/* next ff_len bytes must be 0xff */
	ff_len = pad_len - algo->der_len - 3;
	ret |= *msg ^ 0xff;
	ret |= memcmp(msg, msg+1, ff_len-1);
	msg += ff_len;
	/* next byte must be 0x00 */
	ret |= *msg++;
	/* next der_len bytes must match der_prefix */
	ret |= memcmp(msg, algo->der_prefix, algo->der_len);

	return ret;
}

static int ctc_rsa_verify_key(struct key_prop *prop, const uint8_t *sig,
			  const uint32_t sig_len, const uint8_t *hash,
			  const uint32_t key_len, struct ctc_csum_algo *algo)
{
	int pad_len;
	int ret;

	if (!prop || !sig || !hash || !algo)
		return -EIO;

	if (sig_len != (prop->num_bits / 8)) {
		printf("Signature is of incorrect length %d\n", sig_len);
		return -EINVAL;
	}

	/* Sanity check for stack size */
	if (sig_len > RSA_MAX_SIG_BITS / 8) {
		printf("Signature length %u exceeds maximum %d\n", sig_len,
		      RSA_MAX_SIG_BITS / 8);
		return -EINVAL;
	}

	uint8_t buf[sig_len];

    ret = rsa_mod_exp_sw(sig, sig_len, prop, buf);
	if (ret) {
		printf("Error in Modular exponentation\n");
		return ret;
	}

	pad_len = key_len - algo->checksum_len;

	/* Check pkcs1.5 padding bytes. */
	ret = ctc_rsa_verify_padding(buf, pad_len, algo);
	if (ret) {
		printf("In RSAVerify(): Padding check failed!\n");
		return -EINVAL;
	}

	/* Check hash. */
	if (memcmp((uint8_t *)buf + pad_len, hash, sig_len - pad_len)) {
		printf("In RSAVerify(): Hash check failed!\n");
		return -EACCES;
	}

	return 0;
}

void ctc_fill_pubkey_prop(struct rsa2048_pubkey_prop *pubkey_prop, struct key_prop *prop)
{
    pubkey_prop->num_bits = prop->num_bits;
    pubkey_prop->n0inv    = prop->n0inv;
    pubkey_prop->exp_len  = prop->exp_len;
    pubkey_prop->public_exponent = g_exponent;
    memcpy(pubkey_prop->rr, prop->rr, CER_RSA_LEN);
    memcpy(pubkey_prop->modulus, prop->modulus, CER_RSA_LEN);
}

static int ctc_store_hash(uint8_t *hash, int size)
{
    FILE *f;
    char path[1024];

    snprintf(path, sizeof(path), "./hash%d.txt", g_ctcparams.hash_level);
    f = fopen(path, "w+");
    if (!f) {
        return -EACCES;
    }

    fwrite(hash, size, 1, f);

    fclose(f);

    printf("Store key hash to %s\n", path);

    return 0;
}

static int ctc_store_cert(uint8_t *cert, int size)
{
    FILE *f;
    char path[1024];

    snprintf(path, sizeof(path), "./cert%d.txt", g_ctcparams.cert_level);
    f = fopen(path, "w+");
    if (!f) {
        return -EACCES;
    }

    fwrite(cert, size, 1, f);

    fclose(f);

    printf("Store certificate to %s\n", path);

    return 0;
}

int ctc_map_file(char *fname, void **data, struct stat *sbuf)
{
	int fd;
	void *ptr;

	/* Load FIT blob into memory (we need to write hashes/signatures) */
	fd = open(fname, O_RDWR | O_BINARY);

	if (fd < 0) {
		fprintf(stderr, "%s: Can't open %s: %s\n",
			g_ctcparams.cmdname, fname, strerror(errno));
		goto err;
	}

	if (fstat(fd, sbuf) < 0) {
		fprintf(stderr, "%s: Can't stat %s: %s\n",
			g_ctcparams.cmdname, fname, strerror(errno));
		goto err;
	}

	errno = 0;
	ptr = mmap(0, sbuf->st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if ((ptr == MAP_FAILED) || (errno != 0)) {
		fprintf(stderr, "%s: Can't read %s: %s\n",
			g_ctcparams.cmdname, fname, strerror(errno));
		goto err;
	}

	*data = ptr;
	return fd;

err:
	if (fd >= 0)
		close(fd);

	return -1;
}

void ctc_gen_pubkey_prop(struct rsa2048_pubkey_prop *pubkey_prop, int keylevel)
{
    int i = 0;
    struct key_prop prop;

    memset(pubkey_prop, 0, sizeof(struct rsa2048_pubkey_prop));
    for(i=0; i<g_ctcparams.key_num; i++){
        memset(&prop, 0, sizeof(prop));
        ctc_get_key_prop(&prop, keylevel, i);
        ctc_fill_pubkey_prop(&pubkey_prop[i], &prop);
    }
}

void _ctc_gen_keyhash(uint8_t *hash, int keylevel)
{
    int i = 0;
    struct rsa2048_pubkey_prop pubkey_prop[CER_KEY_NUM];
    struct image_region region[CER_KEY_NUM];

    memset(hash, 0, CER_HASH_LEN);
    memset(pubkey_prop, 0, sizeof(pubkey_prop));

    ctc_gen_pubkey_prop(pubkey_prop, keylevel);
    for(i=0; i<g_ctcparams.key_num; i++){
        region[i].data = &pubkey_prop[i];
        region[i].size = sizeof(struct rsa2048_pubkey_prop);
    }
    g_p_cksum_alogo->calculate("sha256", region, i, hash);
}

void ctc_gen_hash(void)
{
    uint8_t hash[CER_HASH_LEN];
    
    _ctc_gen_keyhash(hash, g_ctcparams.key_level);

    ctc_store_hash(hash, CER_HASH_LEN);
}

void ctc_gen_cert_level1(void)
{
    int i;
	uint8_t *sign;
	uint sign_len;
    struct image_region region[CER_KEY_NUM];
    struct rsa2048_pubkey_prop pubkey_prop[CER_KEY_NUM];
        
    g_cert.id = CER_ID_KEY;
    memset(pubkey_prop, 0, sizeof(pubkey_prop));
    ctc_gen_pubkey_prop(pubkey_prop, 2);
    for(i=0; i < g_ctcparams.key_num; i++){
        region[i].data = &pubkey_prop[i];
        region[i].size = sizeof(struct rsa2048_pubkey_prop);
    }
    ctc_rsa_sign(region, i, &sign, &sign_len);
    memcpy(g_cert.signature, sign, CER_SIG_LEN);
}

void ctc_gen_cert_level2(void)
{
    int fd;
    void *data;
    struct stat sbuf;
	uint8_t *sign;
	uint sign_len;
    struct image_region region;
    
    fd = ctc_map_file(g_ctcparams.src_file, &data, &sbuf);
    if(fd<0){
        fprintf(stderr, "%s: Can't map source file %s: %s\n",
        			g_ctcparams.cmdname, g_ctcparams.src_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    region.data = data;
    region.size = sbuf.st_size;
    ctc_rsa_sign(&region, 1, &sign, &sign_len);

    g_cert.id = CER_ID_IMG;
    g_cert.img_size = sbuf.st_size;
    memcpy(g_cert.signature, sign, CER_SIG_LEN);

	if (data) {
		munmap(data, sbuf.st_size);
		close(fd);
	}
}

void ctc_gen_cert(void)
{    
    g_cert.ver = CER_VER;
    g_cert.nv  = g_ctcparams.nv_count;
    g_cert.encrypt = g_ctcparams.encrypt;
    g_cert.key_offset = (uint32_t)((uint64_t)g_cert.pkey_prop - (uint64_t)&g_cert.ver);
    g_cert.key_size = sizeof(struct rsa2048_pubkey_prop)*g_ctcparams.key_num;
    g_cert.sig_offset = (uint32_t)((uint64_t)g_cert.signature - (uint64_t)&g_cert.ver);
    g_cert.sig_size = CER_SIG_LEN;
    ctc_gen_pubkey_prop(g_cert.pkey_prop, g_ctcparams.key_level);
    
    if(g_ctcparams.cert_level == 1){
        ctc_gen_cert_level1();
    }else{
        ctc_gen_cert_level2();
    }

    ctc_store_cert((uint8_t *)&g_cert, sizeof(g_cert));
}

void ctc_auth_verify(uint8_t *hash)
{
    int fd;
    void *data;
    struct stat sbuf;
    uint8_t *sign;
    uint sign_len;
    int keyindex;
	char cert_path[1024];
    struct ctc_cert *cert;
    struct key_prop prop;
    struct rsa2048_pubkey_prop *pubkey_prop;
    
    snprintf(cert_path, sizeof(cert_path), "./cert%d.txt", g_ctcparams.auth_level);
    fd = ctc_map_file(cert_path, &data, &sbuf);
    if(fd<0){
        fprintf(stderr, "%s: Can't map source file %s: %s\n",
        			g_ctcparams.cmdname, g_ctcparams.src_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(g_ctcparams.nv_count == 0){
        keyindex = 0;
    }else if(g_ctcparams.nv_count == 1){
        keyindex = 1;
    }else if(g_ctcparams.nv_count == 3){
        keyindex = 2;
    }else{
        keyindex = 3;
    }
    
    cert = (struct ctc_cert *)data;
    sign = (uint8_t *)(data + cert->sig_offset);
    sign_len = cert->sig_size;
    pubkey_prop = (struct rsa2048_pubkey_prop *)(data + cert->key_offset);
    prop.rr = pubkey_prop[keyindex].rr;
    prop.modulus = pubkey_prop[keyindex].modulus;
    prop.public_exponent = &pubkey_prop[keyindex].public_exponent;
    prop.n0inv = pubkey_prop[keyindex].n0inv;
    prop.num_bits = pubkey_prop[keyindex].num_bits;
    prop.exp_len = pubkey_prop[keyindex].exp_len;
    
    if (ctc_rsa_verify_key(&prop, sign, sign_len, hash, RSA2048_BYTES, g_p_cksum_alogo)){
        fprintf(stderr, "%s: Verify fail\n", g_ctcparams.cmdname);
        return;
    }

    fprintf(stderr, "%s: Verify Pass\n", g_ctcparams.cmdname);

	if (data) {
		munmap(data, sbuf.st_size);
		close(fd);
	}
}

void ctc_auth_level1(void)
{
    uint8_t hash[CER_HASH_LEN];

    _ctc_gen_keyhash(hash, 2);    
    ctc_auth_verify(hash);
}

void ctc_auth_level2(void)
{
    int fd;
    void *data;
    struct stat sbuf;
    struct image_region region;
    uint8_t hash[32];

    fd = ctc_map_file(g_ctcparams.src_file, &data, &sbuf);
    if(fd<0){
        fprintf(stderr, "%s: Can't map source file %s: %s\n",
        			g_ctcparams.cmdname, g_ctcparams.src_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    region.data = data;
    region.size = sbuf.st_size;
    g_p_cksum_alogo->calculate("sha256", &region, 1, hash);

	if (data) {
		munmap(data, sbuf.st_size);
		close(fd);
	}

    ctc_auth_verify(hash);
}

void ctc_auth(void)
{
    if(g_ctcparams.auth_level == 1){
        ctc_auth_level1();
    }else{
        ctc_auth_level2();
    }
}

void ctc_aes_encrypt(void)
{
    int dst_fd;
    void *dst;
    char cmd[1024];
    char dst_file[1024];
    uint32_t aes_blocks;
    struct stat dst_sbuf;
    uint8_t key_exp[AES_EXPAND_KEY_LENGTH];

    memset(cmd, 0, 1024);
    sprintf (cmd, "cp %s %s_enc", g_ctcparams.src_file, g_ctcparams.src_file);
    system(cmd);

    memset(cmd, 0, 1024);
    sprintf (cmd, "cp %s %s_pad", g_ctcparams.src_file, g_ctcparams.src_file);
    system(cmd);
    
    memset(dst_file, 0, 1024);
    snprintf(dst_file, sizeof(dst_file), "%s_enc", g_ctcparams.src_file);
    dst_fd = ctc_map_file(dst_file, &dst, &dst_sbuf);
    if(dst_fd<0){
        fprintf(stderr, "%s: Can't map dst file %s: %s\n",
        			g_ctcparams.cmdname, dst_file, strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* First we expand the key. */
    aes_expand_key((u8 *)g_ctcparams.aes_key, key_exp);
    /* Calculate the number of AES blocks to encrypt. */
    aes_blocks = DIV_ROUND_UP(dst_sbuf.st_size, AES_KEY_LENGTH);

	if (dst_fd >= 0)
		close(dst_fd);
    
    memset(cmd, 0, 1024);
    sprintf (cmd, "objcopy -I binary -O binary --pad-to=%d --gap-fill=0xff %s_enc", aes_blocks*AES_KEY_LENGTH, g_ctcparams.src_file);
    system(cmd);

    memset(cmd, 0, 1024);
    sprintf (cmd, "objcopy -I binary -O binary --pad-to=%d --gap-fill=0xff %s_pad", aes_blocks*AES_KEY_LENGTH, g_ctcparams.src_file);
    system(cmd);

    dst_fd = ctc_map_file(dst_file, &dst, &dst_sbuf);
    if(dst_fd<0){
        fprintf(stderr, "%s: Can't map dst file %s: %s\n",
        			g_ctcparams.cmdname, dst_file, strerror(errno));
        exit(EXIT_FAILURE);
    }
        
    aes_cbc_encrypt_blocks(key_exp, dst, dst, aes_blocks);
    
	if (dst_fd >= 0)
		close(dst_fd);
}

void ctc_aes_decrypt(void)
{
    int dst_fd, src_fd;
    void *dst, *src;
    char cmd[1024];
    char dst_file[1024];
    uint32_t aes_blocks;
    struct stat dst_sbuf, src_sbuf;
    uint8_t key_exp[AES_EXPAND_KEY_LENGTH];

    memset(cmd, 0, 1024);
    sprintf (cmd, "cp %s_enc %s_dec", g_ctcparams.src_file, g_ctcparams.src_file);
    system(cmd);
    
    memset(dst_file, 0, 1024);
    snprintf(dst_file, sizeof(dst_file), "%s_dec", g_ctcparams.src_file);
    dst_fd = ctc_map_file(dst_file, &dst, &dst_sbuf);
    if(dst_fd<0){
        fprintf(stderr, "%s: Can't map dst file %s: %s\n",
        			g_ctcparams.cmdname, dst_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* First we expand the key. */
    aes_expand_key((u8 *)g_ctcparams.aes_key, key_exp);

    /* Calculate the number of AES blocks to encrypt. */
    aes_blocks = DIV_ROUND_UP(dst_sbuf.st_size, AES_KEY_LENGTH);  

    aes_cbc_decrypt_blocks(key_exp, dst, dst, aes_blocks);

    src_fd = ctc_map_file(g_ctcparams.src_file, &src, &src_sbuf);
    if(src_fd<0){
        fprintf(stderr, "%s: Can't map src file %s: %s\n",
        			g_ctcparams.cmdname, g_ctcparams.src_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(memcmp(src, dst, src_sbuf.st_size)){
        fprintf(stderr, "%s: decrypt image is not consistent with original image!\n",
        			g_ctcparams.cmdname);
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "%s: decrypt image is consistent with original image!\n",
        			g_ctcparams.cmdname);
}

int main(int argc, char **argv)
{   
    process_ctcsign_args(argc, argv);

    g_p_crypto_algo = &g_ctc_crypto_algos;
    g_p_cksum_alogo = &g_ctc_csum_algos;

    if(g_ctcparams.opr == OPR_HASH){
        ctc_gen_hash();
    }else if(g_ctcparams.opr == OPR_CERT){
        ctc_gen_cert();
    }else if(g_ctcparams.opr == OPR_AUTH){
        ctc_auth();
    }else if(g_ctcparams.opr == OPR_ENCR){
        ctc_aes_encrypt();
    }else if(g_ctcparams.opr == OPR_DECR){
        ctc_aes_decrypt();
    }

    return 0;
}

