#include <Arduino.h>
#include <cstdint>
#include <src/error/error.h>

#include "./cryptography.error.h"
CryptographyError::CryptographyError(const char *function, const char *msg, int errorCode) : CoreException(function, msg, errorCode){
	this->setClassName("Cryptography");
}

CryptographyError::CryptographyError(const CryptographyError& e, String fName, String msg, uint8_t errorCode) : CoreException(e, fName, msg, errorCode){
	this->setClassName("Cryptography");
}
