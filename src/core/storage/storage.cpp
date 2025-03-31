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
#include <src/cryptography/cryptography.h>

#include <SD.h>
                
#include "./storage.h"

extern TaskQueue taskQueue;
extern SPIClass sdSPI;
                
CoreStorage::CoreStorage(void){
        this->taskType = TASK_SPACE_STORAGE;
}

bool CoreStorage::pop(void){
        struct task_queue_task t = taskQueue.pop(this->taskType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

bool CoreStorage::fetchTask(void){
        return this->pop();
}

bool CoreStorage::runTask(void){
        switch(this->task.instruction){
                case STORAGE_INSTR_RECVED:{

			Serial.printf("Storing the message '%s'\n", (const char *)this->task.msg);
			taskQueue.push(
                		taskQueue.buildTask(TASK_SPACE_GRAPHICS, TASK_SPACE_STORAGE, GRAPHICS_INSTR_HOMEPAGE)
        		);
                }
                return true;
                //default: Do nothing.
        }
        return false;
}
