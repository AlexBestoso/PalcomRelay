#ifndef LORA_SNAKE_MAX_BUFFER
	#define CRYPTOGRAPHY_MAX_STATE 256
#else
	#define CRYPTOGRAPHY_MAX_STATE LORA_SNAKE_MAX_BUFFER
#endif

#define CRYPTOGRAPHY_AES_MAX_MODE 6
#define CRYPTOGRAPHY_AES_MODE_XTS 1
#define CRYPTOGRAPHY_AES_MODE_OFB 2
#define CRYPTOGRAPHY_AES_MODE_CTR 3
#define CRYPTOGRAPHY_AES_MODE_ECB 4
#define CRYPTOGRAPHY_AES_MODE_CBC 5

#include <src/cryptography/aes/aes.h>

class Cryptography{
	private:
		int aes_mode;
		PalcomAes aes;

		mbedtls_ctr_drbg_context ctr_drbg;
		mbedtls_entropy_context entropy;


	public:
		uint8_t state[CRYPTOGRAPHY_MAX_STATE];
		size_t stateSize;
		void clearState(void);
		
		Cryptography(void);
		
		void setState(uint8_t *state, size_t stateSize);
		int randomByte(void);

		void setAesMode(int mode);
		void setAesKey(uint8_t *eKey, size_t eKeySize, uint8_t *dKey, uint8_t dKeySize);
		void setAesIv(uint8_t *iv, size_t ivS);
		void setAesNonce(uint8_t *nonce, size_t nsize, int offset);
		void setAesStreamBlock(uint8_t *streamBlock, size_t size);
		void aesEncrypt(unsigned char *out);
		void aesDecrypt(unsigned char *out);
};
