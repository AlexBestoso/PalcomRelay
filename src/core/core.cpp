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
		this->cryptography->setAesMode(CRYPTOGRAPHY_AES_MODE_OFB);
		this->cryptography->setAesKey((uint8_t *)&CORE_ROUTING_KEY, CORE_ROUTING_KEY_SIZE, NULL, CORE_ROUTING_KEY_SIZE);
		unsigned char get[16] = {0};
		unsigned char iv[16] = {0};
		String mgs = "my butthole itch";
		String ivs = "1234567891234567";
		for(int i=0; i<16; i++)
			iv[i] = ivs[i];

		
		this->cryptography->setAesIv(iv, 16);
		this->cryptography->setState((uint8_t *)mgs.c_str(), 16);
		mgs = "";

		this->cryptography->aesEncrypt((unsigned char *)&get);	
		
		Serial.printf("enc get : ");
		for(int i=0; i<16; i++) Serial.printf("%c ", get[i]);
		Serial.printf("\n");

		this->cryptography->setAesKey((uint8_t *)&CORE_ROUTING_KEY, CORE_ROUTING_KEY_SIZE, NULL, CORE_ROUTING_KEY_SIZE);
		this->cryptography->setState(get, 16);
		for(int i=0; i<16; i++)
                        iv[i] = ivs[i];
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
