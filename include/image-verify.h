#ifndef __IMAGE_VERIFY_H__
#define __IMAGE_VERIFY_H__

#define CER_RSA_LEN  256
#define CER_SIG_LEN  256
#define CER_HASH_LEN 32
#define CER_KEY_NUM  4

#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
struct ctc_csum_algo {
	const char *name;
	const int checksum_len;
	const int der_len;
	const uint8_t *der_prefix;
	int (*calculate)(const char *name,
			 const struct image_region region[],
			 int region_count, uint8_t *checksum);
};

struct rsa2048_pubkey_prop {
	uint32_t num_bits;	/* Key length in bits */
	uint32_t n0inv;		/* -1 / modulus[0] mod 2^32 */
	uint32_t exp_len;	/* Exponent length in number of uint8_t */
	uint64_t public_exponent; /* public exponent as byte array */
	char rr[CER_RSA_LEN];		/* R^2 can be treated as byte array */
	char modulus[CER_RSA_LEN];	/* modulus as byte array */
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

int ctc_get_pubkey_hash(struct ctc_cert *cert, u8 *hash);
int ctc_get_image_hash(u8 *image, u32 image_size, u8 *hash);
int ctc_get_keyindex(struct ctc_cert *cert, u8 *keyindex);
int ctc_verify_sign(struct ctc_cert *cert, u8 *hash, int keyindex);
int ctc_aes_decrypt(u8 *key_ptr, u8 *src_ptr, u8 *dst_ptr, u32 len);
void ctc_store_pkey_hash(u8 *hash);
void ctc_load_pkey_hash(u8 *hash);
#endif
