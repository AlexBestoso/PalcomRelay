#include <Arduino.h>
#include <cstdint>

#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/bignum.h>
#include <mbedtls/x509.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>
#include <base64.h>

#include <src/error/error.h>
//#include <src/cryptography/aes/aes.h>

#include "./cryptography.h"
#include "./cryptography.error.h"

//using namespace Cryptography;

Cryptography::Cryptography(void){
	this->clearState();
}

void Cryptography::clearState(void){
	for(int i=0;i<CRYPTOGRAPHY_MAX_STATE; i++)
		this->state[i] = 0x0;
	this->stateSize = 0;
}


void Cryptography::setState(uint8_t *state, size_t stateSize){
	this->clearState();
	this->stateSize = stateSize;
	for(int i=0; i<stateSize && i<CRYPTOGRAPHY_MAX_STATE; i++){
		this->state[i] = state[i];
	}
	
}

void Cryptography::setAesIv(uint8_t *iv, size_t ivS){
	this->aes.setIv(iv, ivS);
}

void Cryptography::setAesMode(int mode){
	if(mode <= 0 || mode > CRYPTOGRAPHY_AES_MAX_MODE)
		throw CryptographyError("setAesMode", "Unsupported mode requested.", 0x00);

	this->aes_mode = mode;
}

void Cryptography::setAesKey(uint8_t *eKey, size_t eKeySize, uint8_t *dKey, uint8_t dKeySize){
	if((eKeySize + dKeySize) == 0 || (eKey == NULL && dKey == NULL))
		throw CryptographyError("setAesKey", "No keys provided.", 0);

	bool runE = eKeySize > 0, runD = dKeySize > 0;
	
	if(eKey == NULL && eKeySize == dKeySize){
		eKey = dKey;
		runE = true;
		runD = true;
	}else if(dKey == NULL && dKeySize == eKeySize){
		dKey = eKey;
		runE = true;
		runD = true;
	}else if(!runE && !runD){
		throw CryptographyError("setAesKey", "Not setting keys.", 0);
	}

	if(runE) this->aes.setEncryptionKey(eKey, eKeySize);
	if(runD) this->aes.setDecryptionKey(dKey, dKeySize);	
}

void Cryptography::aesEncrypt(unsigned char *out){
	this->aes.setState(this->state, this->stateSize);
	switch(this->aes_mode){
		case CRYPTOGRAPHY_AES_MODE_XTS:
			throw CryptographyError("aesEncrypt", "Mode not Implemented.", 0);
		break;
		case CRYPTOGRAPHY_AES_MODE_OFB:
			this->aes.encrypt_ofb(out);
			//throw CryptographyError("aesEncrypt", "MODE OFB not implemented.\n", 0);
		break;
		default:
			throw CryptographyError("aesEncrypt", "Invalid mode provided.", 0x00);
	}
}

void Cryptography::aesDecrypt(unsigned char *out){
        switch(this->aes_mode){
                case CRYPTOGRAPHY_AES_MODE_XTS:
			throw CryptographyError("aesEncrypt", "Mode not Implemented.", 0);
                break;
		case CRYPTOGRAPHY_AES_MODE_OFB:
			//throw CryptographyError("aesEncrypt", "MODE OFB not implemented.\n", 0);
			this->aes.decrypt_ofb(out);
		break;
                default:
                        throw CryptographyError("aesDecrypt", "Invalid mode provided.", 0x00);
        }
}
