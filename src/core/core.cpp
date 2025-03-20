#include <Arduino.h>
#include <cstdint>
#include <src/error/error.h>
#include <src/taskQueue/taskQueue.h>
#include <src/init/init.h>
#include <src/core/graphics/graphics.h>
#include <src/core/comms/comms.h>


#include "./core.h"
extern TaskQueue taskQueue;
PalcomCore::PalcomCore(void){ }



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
