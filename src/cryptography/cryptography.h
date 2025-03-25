#ifndef LORA_SNAKE_MAX_BUFFER
	#define CRYPTOGRAPHY_MAX_STATE 256
#else
	#define CRYPTOGRAPHY_MAX_STATE LORA_SNAKE_MAX_BUFFER
#endif

#define CRYPTOGRAPHY_ROUTING_KEY_SIZE 32

const uint8_t CRYPTOGRAPHY_ROUTING_KEY[CRYPTOGRAPHY_ROUTING_KEY_SIZE]= {
        162, 232, 0xff, 0x3a, 0x69, 0x22, 0xff, 0x82, 0x43 , 0x87,

        0x23, 0xe8, 0x18, 0x08, 0x17, 0x76, 0x6E, 0x69, 0x67, 0x67,

        0x65, 0x72, 0x10, 0x3a, 0x4a, 0x50, 0x03, 0x18, 0x07, 0x21,
        145, 33
};

#include <src/cryptography/aes/aes.h>

class Cryptography{
	private:
		char *routingKey;
		mbedtls_ctr_drbg_context ctr_drbg;
		mbedtls_entropy_context entropy;

		uint8_t state[CRYPTOGRAPHY_MAX_STATE];

		void clearState(void);
	public:
		PalcomAes aes;
		
		Cryptography(void);
		
		void setState(uint8_t *state, size_t stateSize);
		void setState(char *state, size_t stateSize);
		void setState(String state);
		int randomByte(void);
};
