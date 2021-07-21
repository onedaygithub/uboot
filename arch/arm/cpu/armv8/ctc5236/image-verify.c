#include <common.h>
#include <asm/io.h>
#include <hash.h>
#include <u-boot/rsa-mod-exp.h>
#include <u-boot/sha256.h>
#include <u-boot/rsa-checksum.h>
#include <u-boot/rsa.h>
#include <uboot_aes.h>
#include <image-verify.h>
#include <ctcefuse.h>

int ctc_hash_calculate(const char *name,
		    const struct image_region region[],
		    int region_count, uint8_t *checksum);
		    
struct ctc_csum_algo g_ctc_csum_algos = {
		"sha256",
		SHA256_SUM_LEN,
		SHA256_DER_LEN,
		sha256_der_prefix,
		ctc_hash_calculate,
};

int ctc_get_pubkey_hash(struct ctc_cert *cert, u8 *hash)
{
    int i;
    int key_num;
    struct ctc_csum_algo *cksum_alogo;
    struct image_region region[CER_KEY_NUM];

    memset(hash, 0, CER_HASH_LEN);
    cksum_alogo = &g_ctc_csum_algos;
    key_num = cert->key_size/sizeof(struct rsa2048_pubkey_prop);
    for(i=0; i<key_num; i++){
        region[i].data = &cert->pkey_prop[i];
        region[i].size = sizeof(struct rsa2048_pubkey_prop);
    }    
    cksum_alogo->calculate("sha256", region, i, hash);

    return 0;
}

int ctc_get_image_hash(u8 *image, u32 image_size, u8 *hash)
{
    struct image_region region;
    struct ctc_csum_algo *cksum_alogo;
    
    memset(hash, 0, CER_HASH_LEN);
    cksum_alogo = &g_ctc_csum_algos;
    region.data = image;
    region.size = image_size;
    cksum_alogo->calculate("sha256", &region, 1, hash);

    return 0;
}

int ctc_get_keyindex(struct ctc_cert *cert, u8 *keyindex)
{
    switch(cert->nv){
        case 0:
            *keyindex = 0;
            break;
        case 1:
            *keyindex = 1;
            break;
        case 3:
            *keyindex = 2;
            break;
        case 7:
            *keyindex = 3;
            break;
        default:
            return -1;
    }

    return 0;
}

int ctc_hash_calculate(const char *name,
		    const struct image_region region[],
		    int region_count, uint8_t *checksum)
{
	struct hash_algo *algo;
	int ret = 0;
	void *ctx;
	uint32_t i;
	i = 0;

	ret = hash_progressive_lookup_algo(name, &algo);
	if (ret)
		return ret;

	ret = algo->hash_init(algo, &ctx);
	if (ret)
		return ret;

	for (i = 0; i < region_count - 1; i++) {
		ret = algo->hash_update(algo, ctx, region[i].data,
					region[i].size, 0);
		if (ret)
			return ret;
	}

	ret = algo->hash_update(algo, ctx, region[i].data, region[i].size, 1);
	if (ret)
		return ret;
	ret = algo->hash_finish(algo, ctx, checksum, algo->digest_size);
	if (ret)
		return ret;

	return 0;
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

int ctc_verify_sign(struct ctc_cert *cert, u8 *hash, int keyindex)
{
    u8 *p;
    u8 *sign;
    u32 sign_len;
    struct key_prop prop;
    struct rsa2048_pubkey_prop *pubkey_prop;
    struct ctc_csum_algo *cksum_alogo;

    p           = (u8 *)cert;
    sign        = (u8 *)(p + cert->sig_offset);
    sign_len    = cert->sig_size;
    pubkey_prop = (struct rsa2048_pubkey_prop *)(p + cert->key_offset);
    prop.rr              = pubkey_prop[keyindex].rr;
    prop.modulus         = pubkey_prop[keyindex].modulus;
    prop.public_exponent = &pubkey_prop[keyindex].public_exponent;
    prop.n0inv           = pubkey_prop[keyindex].n0inv;
    prop.num_bits        = pubkey_prop[keyindex].num_bits;
    prop.exp_len         = pubkey_prop[keyindex].exp_len;

    cksum_alogo = &g_ctc_csum_algos;
    return ctc_rsa_verify_key(&prop, sign, sign_len, hash, RSA2048_BYTES, cksum_alogo);
}

int ctc_aes_decrypt(u8 *key_ptr, u8 *src_ptr, u8 *dst_ptr, u32 len)
{
	u8 key_exp[AES_EXPAND_KEY_LENGTH];
	u32 aes_blocks;

	/* First we expand the key. */
	aes_expand_key(key_ptr, key_exp);
	/* Calculate the number of AES blocks to encrypt. */
	aes_blocks = DIV_ROUND_UP(len, AES_KEY_LENGTH);
    /* decrypt block */
	aes_cbc_decrypt_blocks(key_exp, src_ptr, dst_ptr, aes_blocks);

	return 0;
}

void ctc_store_pkey_hash(u8 *hash)
{
    memcpy((u8 *)CONFIG_SYS_HASH_BASE, hash, CER_HASH_LEN);
}

void ctc_load_pkey_hash(u8 *hash)
{
    memcpy(hash, (u8 *)CONFIG_SYS_HASH_BASE, CER_HASH_LEN);
}

