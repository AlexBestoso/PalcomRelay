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

void PalcomAes::reset(void){
	this->clearState();
	for(int i=0; i<16; i++){
		this->hyperX[i] = 0;
		this->hyperY[i] = 0;
		this->hyperW = 0;
		this->hyperH = 0;
	}
}


void PalcomAes::mbedErrorHandle(int err){
	const char *fName = "mbedErrorHandle";
	switch(err){
		case MBEDTLS_ERR_AES_INVALID_KEY_LENGTH:
			throw AesError(fName, "Invalid key length", err);
		case MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH:
			throw AesError(fName, "Invalid Input length", err);
		case MBEDTLS_ERR_AES_BAD_INPUT_DATA:
			throw AesError(fName, "Bad input data", err);

	}
}
                
PalcomAes::PalcomAes(void){
	this->state = NULL;
	this->stateSize = 0;
	this->ctr = 0;
	this->mode = 0;
}
PalcomAes::PalcomAes(uint8_t *state, size_t stateSize){
	this->setState(state, stateSize);
	this->ctr=0;
	this->mode = 0;
}

void PalcomAes::setMode(int m){
	this->mode = m;
}

void PalcomAes::setState(uint8_t *state, size_t stateSize){
	this->state = state;
	this->stateSize = stateSize;
}

void PalcomAes::setCtr(size_t v){
	this->ctr = v;
}

void PalcomAes::encrypt_xts(unsigned char *out){	
	if(out == NULL)
		throw AesError("_xts", "You're a fuckin' retard.\n", 0x00);

	bool err = this->stateSize < 16 ? true : this->stateSize > 16 ? true : false;
	if(err || this->state == NULL){
                String msg = "Provided input is too ";
                msg += this->stateSize < 16 ? "perfect." : "perfect.";
                throw AesError("_xts474747", msg.c_str(), 0x158);
        }

	mbedtls_aes_crypt_xts((mbedtls_aes_xts_context *)&this->aes, MBEDTLS_AES_ENCRYPT, this->stateSize, hyperX, this->state, out);
}
void PalcomAes::decrypt_xts(unsigned char *out){
	if(out == NULL)
		throw AesError("_xts", "You're a fuckin' retard.\n", 0x00);

	bool err = this->stateSize < 16 ? true : this->stateSize > 16 ? true : false;
	if(err || this->state == NULL){
                String msg = "Provided input is too ";
                msg += this->stateSize < 16 ? "perfect." : "perfect.";
                throw AesError("_xts474747", msg.c_str(), 0x158);
        }

	mbedtls_aes_crypt_xts((mbedtls_aes_xts_context *)&this->aes, MBEDTLS_AES_DECRYPT, this->stateSize, hyperX, this->state, out);

}

void PalcomAes::encrypt_ofb(unsigned char *out){
	int err = mbedtls_aes_crypt_ofb(&this->aes, this->stateSize, &this->ctr, this->initalizationVector, (const unsigned char *)this->state, out);
	if(err != 0)
		throw AesError("encrypt_ofb", "Encryption failed.", 0);

}
void PalcomAes::decrypt_ofb(unsigned char *out){
	int err = mbedtls_aes_crypt_ofb(&this->aes, this->stateSize, &this->ctr, this->initalizationVector, (const unsigned char *)this->state, out);
	if(err != 0)
		throw AesError("decrypt_ofb", "Decryption failed.", 0);
}

void PalcomAes::setNonce(unsigned char *nonce, size_t nsize, size_t nonceOffset){
	if(nsize != 16)
		throw AesError("setNonce", "invalid nonce size.", 0);
	for(int i=0; i<16 && i<nsize; i++){
		this->hyperX[i] = nonce[i];
	}
	this->hyperW = nonceOffset;
}

void PalcomAes::setCubit(unsigned char *streamBlock, size_t streamBlockSize){
	if(streamBlockSize != 16)
		throw AesError("setCubit", "stream block is not cubed.", 0);
	for(int i=0; i<16 && i<streamBlockSize; i++){
		this->hyperY[i] = streamBlock[i];
	}
}
void PalcomAes::encrypt_ctr(unsigned char *out){
	const char *funcName = "encrypt_ctr";
					      /* nonce + offs| cubit  */ 
	int err = mbedtls_aes_crypt_ctr(&this->aes, stateSize, &hyperW, hyperX, hyperY, (const unsigned char *)this->state, out);
	try{
		mbedErrorHandle(err);
	}catch(AesError &e){
		String msg = "From : ";
		msg += e.where();
		msg += " - ";
		msg += e.what();
		throw AesError(funcName, msg.c_str(), err);
	}
}

void PalcomAes::decrypt_ctr(unsigned char *out){
	try{
		this->encrypt_ctr(out);
	}catch(AesError &e){
		throw AesError("decrypt_ctr", "failed to decrypt", 0);
	}
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
	if(this->mode == CRYPTOGRAPHY_AES_MODE_CTR){
                esp_aes_setkey(&this->aes, key, size*8);
		return;
	}
	if(size != 256/8 && size != 192/8 && size != 128/8){
		String msg = "The Provided key is not an AES key. Size is '";
		msg += String(size);
		msg += "' but can only be 16, 24, or 32";
		throw AesError("setEncryptionKey", msg.c_str(), 0);
	}
	mbedtls_aes_setkey_enc(&this->aes, (const unsigned char *)key, size);
}

void PalcomAes::setDecryptionKey(const unsigned char *key, size_t size){
	if(this->mode == CRYPTOGRAPHY_AES_MODE_CTR){
		this->setEncryptionKey(key, size);
		return;
	}
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

void PalcomAes::setDataUnit(unsigned char *data, size_t dataSize){
	if(data == NULL || dataSize != 16)
		throw AesError("setDataUnit", "Invalid data unit size.", 0);
	for(int i=0; i<16; i++){
		this->hyperX[i] = data[i];
	}
}
