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
#include <src/taskQueue/taskQueue.h>
#include <src/init/init.h>
#include <src/cryptography/cryptography.h>

#include <src/core/graphics/graphics.h>
#include <src/core/comms/comms.h>


#include "./core.h"
extern TaskQueue taskQueue;
PalcomCore::PalcomCore(void){ }

PalcomCore::PalcomCore(Cryptography *crypto){
	this->cryptography = crypto;
}

void PalcomCore::startCore(void){
	this->initer.initSerial();
    	this->initer.initRadio();
    	this->initer.initDisplay();
    	this->initer.initUsb();
    	this->initer.initSettings();

	taskQueue.push(
		taskQueue.buildTask(TASK_SPACE_GRAPHICS, TASK_SPACE_GOD, GRAPHICS_INSTR_HOMEPAGE)
	);
}


void PalcomCore::driveCore(void){
	try{
		uint8_t keyA[32], keyB[32];
		for(int i=0; i<16; i++){
			keyA[i] = CORE_ROUTING_KEY[i];
			keyB[i] = CORE_ROUTING_KEY[i];
		}
		this->cryptography->setAesMode(CRYPTOGRAPHY_AES_MODE_CTR);
		this->cryptography->setAesKey((uint8_t *)&keyA, 16, (uint8_t *)&keyB, 16);
		unsigned char get[16] = {0};
		unsigned char iv[16] = {0};
		unsigned char Nonce[16] = {0};
		unsigned char streamBlock[16] = {0};
		
		String mgs = "my butthole itch";
		String ivs = "1234567891234567";
		for(int i=0; i<16; i++){
			iv[i] = ivs[i];
			Nonce[i] = 0;
			streamBlock[i] = 0;
		}

		
		this->cryptography->setAesIv(iv, 16);
		this->cryptography->setState((uint8_t *)mgs.c_str(), 16);
		mgs = "";

		this->cryptography->setAesNonce(Nonce, 16, 0);
		this->cryptography->setAesStreamBlock(streamBlock, 16);
		this->cryptography->aesEncrypt((unsigned char *)&get);	
		
		Serial.printf("enc get : ");
		for(int i=0; i<16; i++) Serial.printf("%c ", get[i]);
		Serial.printf("\n");
		
		for(int i=0; i<CORE_ROUTING_KEY_SIZE; i++){
                        keyA[i] = CORE_ROUTING_KEY[i];
                        keyB[i] = CORE_ROUTING_KEY[i];
                }

		this->cryptography->setAesKey((uint8_t *)&keyA, 16, (uint8_t *)&keyB, 16);
		this->cryptography->setState(get, 16);

		for(int i=0; i<16; i++){
			iv[i] = ivs[i];
			Nonce[i] = 0;
			streamBlock[i] = 0;
		}

		this->cryptography->setAesNonce(Nonce, 16, 0);
		this->cryptography->setAesStreamBlock(streamBlock, 16);

		this->cryptography->setAesIv(iv, 16);
		this->cryptography->aesDecrypt((unsigned char *)&get);

		Serial.printf("dec get : ");
		for(int i=0; i<16; i++) Serial.printf("%c ", get[i]);
		Serial.printf("\n\n");


		
	}catch(CoreException &e){
		e.out();
		e.halt();
	}
	if(this->subCoreGraphics.fetchTask()){
		this->subCoreGraphics.runTask();
	}

	// comms core
	if(this->subCoreComms.fetchTask()){
		this->subCoreComms.runTask();
	}	

	// storage core

	// usb core ? 
}
