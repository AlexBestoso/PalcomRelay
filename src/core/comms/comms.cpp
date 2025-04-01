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

void CoreComms::deriveRoutingIv(void){
	for(int i=0, j=0; i<CORE_ROUTING_KEY_SIZE && i+1<CORE_ROUTING_KEY_SIZE && j<16; i+=2, j++){
		this->iv[j] = CORE_ROUTING_KEY[i] ^ CORE_ROUTING_KEY[i+1];
	}
}

void CoreComms::deriveRoutingPadding(void){
	for(int i=0, j=CORE_ROUTING_KEY_SIZE-1, q=CORE_ROUTING_KEY_SIZE/2, p=(CORE_ROUTING_KEY_SIZE/2)-1; (i<=q) && (j >= p); i++, j--, q--, p++){
		int a = this->padding[i];
		int b = this->padding[j];
		int c = this->padding[p]; 
		int d = this->padding[q]; 

		int big = 0b1111<<4;
		int small = 0b1111;
		this->padding[i] = (a&small) + ((d&small)<<4);
		this->padding[j] = (b&big) + ((c&big)>>4);
		this->padding[p] = (c&small) + ((b&small)<<4);
		this->padding[q] = (d&big) + ((a&big)>>4);
	}
}


void CoreComms::deriveRoutingRecvKey(void){
	for(int i=0; i<CORE_ROUTING_KEY_SIZE; i++)
		this->inKey[i] = CORE_ROUTING_KEY[i];
}
void CoreComms::deriveRoutingSendKey(void){
	for(int i=CORE_ROUTING_KEY_SIZE-1, j=0; i>0; i--, j++)
		this->outKey[j] = CORE_ROUTING_KEY[i];
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
			this->cryptography->setAesMode(CRYPTOGRAPHY_AES_MODE_CBC);
			this->cryptography->setAesKey((uint8_t *)this->outKey, 32, (uint8_t *)this->inKey, 32);
			this->cryptography->setAesIv((uint8_t *)this->iv, 16);
			this->cryptography->clearState();

			for(int i=0; i<32; i+=16){
				this->cryptography->setState(loraSnake.lrsPacket.data+i, 16);
				this->cryptography->aesDecrypt((unsigned char *)tsk.msg+i);
			}
			
			if(this->validatePadding((unsigned char *)tsk.msg, 256)){
				taskQueue.push(tsk);
				Serial.printf("Valid message received.\n");
			}else{
				Serial.printf("Invalid message. not storing.\n");
			}
                }
                return true;
                //default: Do nothing.
        }
        return false;
}

bool CoreComms::validatePadding(unsigned char *data, size_t dataSize){
	if(dataSize != 256) return false;
	for(int i=0; i<256; i++){
		if((data[i]-this->padding[i] ^ this->padding[i]) != 0) return false;
	}
	return true;
}

void CoreComms::init(Cryptography *c, unsigned char*CORE_ROUTING_KEY, size_t CORE_ROUTING_KEY_SIZE){
	this->CORE_ROUTING_KEY = CORE_ROUTING_KEY;
	this->CORE_ROUTING_KEY_SIZE = CORE_ROUTING_KEY_SIZE;

	this->cryptography = c;
	this->deriveRoutingIv();
	this->deriveRoutingRecvKey();
	this->deriveRoutingSendKey();
	this->deriveRoutingPadding();
}
