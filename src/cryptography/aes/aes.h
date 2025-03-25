class PalcomAes{
	private:
		mbedtls_aes_context aes;

		String acceleratorPersona;
		uint8_t *state;
		size_t stateSize;

		uint8_t *initalizationVector;
		size_t initalizationVectorSize;

		uint8_t *encryptionKey;
		size_t encryptionKeySize;

		uint8_t *decryptionKey;
		size_t decryptionKeySize; 
		
		void clearState(void);
	
	public:
		PalcomAes(void);
		PalcomAes(uint8_t *state, size_t stateSize);
		PalcomAes(char *state, size_t stateSize);
		
		void setState(uint8_t *state, size_t stateSize);
                void setState(char *state, size_t stateSize);
		
		void setEncryptionKey(const unsigned char *key, size_t size);
		void setDecryptionKey(const unsigned char *key, size_t size);

		void setIv(unsigned char *iv, size_t ivSize);
		// mbedtls_aes_crypt_xts
		// mbedtls_aes_crypt_ofb
		// mbedtls_aes_crypt_ecb
		// mbedtls_aes_crypt_ctr
		// mbedtls_aes_crypt_cfb
		// mbedtls_aes_crypt_cbc

};
