class PalcomAes{
	private:
		mbedtls_aes_context aes;

		String acceleratorPersona;
		
		size_t ctr;
		unsigned char data_unit[16];
		
		uint8_t *initalizationVector;
		size_t initalizationVectorSize;

		uint8_t *encryptionKey;
		size_t encryptionKeySize;

		uint8_t *decryptionKey;
		size_t decryptionKeySize; 

		void clearState(void);
	
	public:
		uint8_t *state;
		size_t stateSize;

		PalcomAes(void);
		PalcomAes(bool mode);
		PalcomAes(uint8_t *state, size_t stateSize);

		void setDataUnit(unsigned char *data, size_t dataSize);
		
		void setState(uint8_t *state, size_t stateSize);
		
		void setEncryptionKey(const unsigned char *key, size_t size);
		void setDecryptionKey(const unsigned char *key, size_t size);

		void setCtr(size_t v);
	
		void setIv(unsigned char *iv, size_t ivSize);
		
		void init(void);

		void encrypt_xts(unsigned char *out); // xts is for faggots
		void decrypt_xts(unsigned char *out); // xts is for faggots

		void encrypt_ofb(unsigned char *out);
		void decrypt_ofb(unsigned char *out);

		// mbedtls_aes_crypt_ofb
		// mbedtls_aes_crypt_ecb
		// mbedtls_aes_crypt_ctr
		// mbedtls_aes_crypt_cfb
		// mbedtls_aes_crypt_cbc

};
