#include <Arduino.h>
#include <cstdint>

#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/bignum.h>
#include <mbedtls/x509.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>

#include <src/error/error.h>

#include "./aes.error.h"
#include "./aes.h"

void PalcomAes::clearState(void){ // might rename this to "free state"
	this->state = NULL;
	this->stateSize = 0;
}
                
PalcomAes::PalcomAes(void){
	this->state = NULL;
	this->stateSize = 0;
}
PalcomAes::PalcomAes(uint8_t *state, size_t stateSize){
	this->setState(state, stateSize);
}
PalcomAes::PalcomAes(char *state, size_t stateSize){
	this->setState(state, stateSize);
}

void PalcomAes::setState(uint8_t *state, size_t stateSize){
	this->state = state;
	this->stateSize = stateSize;
}
void PalcomAes::setState(char *state, size_t stateSize){
	this->state = (uint8_t *)state;
	this->stateSize = stateSize;
}
/*
void PalcomAes::initNumberGen(void){
	mbedtls_ctr_drbg_init(&ctr_drbg);
}
void PalcomAes::initEntropy(void){
	mbedtls_entropy_init(&entropy);
}
void PalcomAes::seedNumberGen(unsigned char* seed, size_t seedLen){
	int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, seed, seedLen);
	if(ret != 0){
		this->free();
		throw AesError("seedNumberGen", "mbedtls_ctr_drbg_seed failed, resources freed.", 0);
	}
}*/


void PalcomAes::setEncryptionKey(const unsigned char *key, size_t size){
	if(size != 256/8 && size != 192/8 && size != 128/8){
		String msg = "The Provided key is not an AES key. Size is '";
		msg += String(size);
		msg += "' but can only be 16, 24, or 32";
		throw AesError("setEncryptionKey", msg.c_str(), 0);
	}
	mbedtls_aes_setkey_enc(&this->aes, (const unsigned char *)key, size);
}

void PalcomAes::setDecryptionKey(const unsigned char *key, size_t size){
	if(size != 256/8 && size != 192/8 && size != 128/8){
                String msg = "The Provided key is not an AES key. Size is '";
                msg += String(size);
                msg += "' but can only be 16, 24, or 32";
                throw AesError("setDecryptionKey", msg.c_str(), 0);
        }
        mbedtls_aes_setkey_dec(&this->aes, (const unsigned char *)key, size);
}
        
void PalcomAes::setIv(unsigned char *iv, size_t ivSize){
	if(ivSize != 16)
		throw AesError("setIv", "Provided IV size is invalid. It must be 16 bytes in size.", 0);
	this->initalizationVector = iv;
	this->initalizationVectorSize = ivSize;	
}
