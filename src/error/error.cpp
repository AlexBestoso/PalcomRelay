#include <Arduino.h>
#include <cstdint>
#include "./error.h"

CoreException::CoreException(String msg, uint8_t errorCode){
	this->from = "";
	this->errorMessage = msg;
        this->className = "";
        this->functionName = "";
	this->_errorCode = errorCode;
}

CoreException::CoreException(String fName, String msg, uint8_t errorCode){
	this->from = "";
	this->errorMessage = msg;
	this->_errorCode = errorCode;
	this->functionName = fName;
}

CoreException::CoreException(String msg, String classN, String funcN, uint8_t errorCode){
	this->from = "";
	this->errorMessage = msg;
        this->className = classN;
        this->functionName = funcN;
        this->_errorCode = errorCode;
}

CoreException::CoreException(const CoreException& e, String fName, String msg, uint8_t errorCode){
	this->from = "From : ";
	this->from += this->where();
	this->from += " - ";
	this->from += this->what();
	this->errorMessage = msg;
        this->functionName = fName;
        this->_errorCode = errorCode;
}

String CoreException::what(void){
	return errorMessage;
}

String CoreException::where(void){
	String ret = this->className;
	ret += "::";
	ret += this->functionName;
	ret += "()";
	return ret;
}

uint32_t CoreException::errorCode(void){
	return _errorCode;
}

const char *CoreException::codeTranslate(void){
	switch(this->errorCode()){
		case CORE_ERR_TASK:
			return "CORE_ERR_TASK";
		case ERR_TASK_CREATE:
			return "ERR_TASK_CREATE";
		case ERR_TASK_SEMAPHORE:
			return "ERR_TASK_SEMAPHORE";
		case ERR_TASK_NULL:
			return "ERR_TASK_NULL";
		case CORE_ERR_TQ:
			return "CORE_ERR_TQ";
		case ERR_TQ_SEMAPHORE:
			return "ERR_TQ_SEMAPHORE";
		case ERR_TQ_POP:
			return "ERR_TQ_POP";
		case ERR_TQ_PUSH:
			return "ERR_TQ_PUSH";
		case CORE_ERR_DEBUG:
			return "CORE_ERR_DEBUG";
		case ERR_DEBUG_TEST:
			return "ERR_DEBUG_TEST";
		case CORE_ERR_INIT:
			return "CORE_ERR_INIT";
		case ERR_INIT_LORA:
			return "ERR_INIT_LORA";
	}
	return "ERR_UNKNOWN";
}
	
void CoreException::out(void){
	String From = this->from == "" ? "" : "\n\t";
	From += this->from;
	Serial.printf("[%ld:%s] %s %s\n", this->_errorCode, this->codeTranslate(), this->where().c_str(), this->what().c_str(), From.c_str());
}

void CoreException::halt(void){
	this->out();
	while(1){
		delay(1000);
	}
}

void CoreException::log(void){
	Serial.printf("Log Not implemented.\n");
}

void CoreException::setClassName(const char *name){
	this->className = name;
}
