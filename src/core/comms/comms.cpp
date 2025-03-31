#include <Arduino.h>
#include <cstdint>
#include <ds3231.h> 

#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/bignum.h>
#include <mbedtls/x509.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>

#include <src/error/error.h>
#include <src/taskQueue/taskQueue.h>
#include <src/cryptography/cryptography.h>
#include <src/LoRaSnake/LoRaSnake.class.h>
                
#include "./comms.h"
                
extern OLED_CLASS_OBJ display;
extern TaskQueue taskQueue;
extern LoRaSnake loraSnake;
                
CoreComms::CoreComms(void){
        this->taskType = TASK_SPACE_COMMS;
}

bool CoreComms::pop(void){
        struct task_queue_task t = taskQueue.pop(this->taskType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

bool CoreComms::fetchTask(void){
        return this->pop();
}

bool CoreComms::runTask(void){
        switch(this->task.instruction){
                case COMMS_INSTR_RECV:{
			if(loraSnake.readRecv() != 1){
				return false;
			}
			struct task_queue_task tsk = taskQueue.buildTask(TASK_SPACE_STORAGE, TASK_SPACE_COMMS, STORAGE_INSTR_RECVED);
			for(int i=0; i<loraSnake.lrsPacket.data_size && i<256; i++){
				tsk.msg[i] = loraSnake.lrsPacket.data[i];
			}
			taskQueue.push(tsk);
                }
                return true;
                //default: Do nothing.
        }
        return false;
}
