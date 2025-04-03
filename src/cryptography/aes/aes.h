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

class PalcomAes{
	private:
		mbedtls_aes_context aes;
		
		int mode;
			
		size_t ctr;
		unsigned char hyperX[16];
		size_t hyperW;

		unsigned char hyperY[16];
		size_t hyperH;
		
		uint8_t *initalizationVector;
		size_t initalizationVectorSize;

		uint8_t *encryptionKey;
		size_t encryptionKeySize;

		uint8_t *decryptionKey;
		size_t decryptionKeySize; 

		void clearState(void);
		void mbedErrorHandle(int err);
	
	public:
		uint8_t *state;
		size_t stateSize;

		PalcomAes(void);
		PalcomAes(bool mode);
		PalcomAes(uint8_t *state, size_t stateSize);

		void setMode(int mode);

		void setDataUnit(unsigned char *data, size_t dataSize);
		
		void setState(uint8_t *state, size_t stateSize);
		void reset(void);	
		
		void setEncryptionKey(const unsigned char *key, size_t size);
		void setDecryptionKey(const unsigned char *key, size_t size);

		void setCtr(size_t v);
		void setIv(unsigned char *iv, size_t ivSize);
		void setNonce(unsigned char *nonce, size_t nsize, size_t nonceOffset);
		void setCubit(unsigned char *streamBlock, size_t streamBlockSize);
		
		void init(void);

		void encrypt_xts(unsigned char *out); // xts is for faggots
		void decrypt_xts(unsigned char *out); // xts is for faggots

		void encrypt_ofb(unsigned char *out);
		void decrypt_ofb(unsigned char *out);

		void encrypt_ctr(unsigned char *out);
		void decrypt_ctr(unsigned char *out);

		void encrypt_ecb(unsigned char *out);
		void decrypt_ecb(unsigned char *out);
		
		void encrypt_cbc(unsigned char *out);
		void decrypt_cbc(unsigned char *out);
		// mbedtls_aes_crypt_cfb

};
