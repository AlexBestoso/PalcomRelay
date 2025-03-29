#include <Arduino.h>
#include <cstdint>
#include <src/error/error.h>

#include "./aes.error.h"
AesError::AesError(const char *function, const char *msg, int errorCode) : CoreException(function, msg, errorCode){
	this->setClassName("PalcomAes");
}
AesError::AesError(const AesError &e, const char *function, const char *msg, int errorCode) : CoreException(e, function, msg, errorCode){
	this->setClassName("PalcomAes");
}
