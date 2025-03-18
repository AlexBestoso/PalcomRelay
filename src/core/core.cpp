#include <Arduino.h>
#include <cstdint>
#include <src/error/error.h>
#include <src/taskQueue/taskQueue.h>
#include <src/init/init.h>


#include "./core.h"
PalcomCore::PalcomCore(void){ }

void PalcomCore::startCore(void){
	this->initer.initSerial();
    	this->initer.initRadio();
    	this->initer.initDisplay();
    	this->initer.initUsb();
    	this->initer.initSettings();
}
