#include <Arduino.h>
#include <cstdint>
#include <SPI.h>
#include <SD.h>

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
#include <src/core/storage/storage.h>

#include <src/LoRaSnake/LoRaSnake.class.h>


#include "./core.h"
extern TaskQueue taskQueue;
extern SPIClass sdSPI;
extern LoRaSnake loraSnake;

PalcomCore::PalcomCore(void){ }

PalcomCore::PalcomCore(Cryptography *crypto){
	this->cryptography = crypto;
	this->subCoreComms.init(crypto, (unsigned char*)CORE_ROUTING_KEY, CORE_ROUTING_KEY_SIZE);
}

void PalcomCore::startCore(void){
	this->initer.initSerial();
	this->initer.initStorage();
    	this->initer.initRadio();
    	this->initer.initDisplay();
    	this->initer.initUsb();
    	this->initer.initSettings();

	taskQueue.push(
		taskQueue.buildTask(TASK_SPACE_GRAPHICS, TASK_SPACE_GOD, GRAPHICS_INSTR_HOMEPAGE)
	);
}


void PalcomCore::driveCore(void){
	if(this->subCoreGraphics.fetchTask()){
		this->subCoreGraphics.runTask();
	}

	// comms core
	if(this->subCoreComms.fetchTask()){
		this->subCoreComms.runTask();
	}	

	// storage core
	if(this->subCoreStorage.fetchTask()){
		this->subCoreStorage.runTask();
	}
	// usb core ? 
}
