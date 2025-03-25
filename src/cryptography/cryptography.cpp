#include <Arduino.h>
#include <cstdint>

#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/bignum.h>
#include <mbedtls/x509.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>
#include <base64.h>

#include <src/error/error.h>

#include "./cryptography.h"

//using namespace Cryptography;

Cryptography::Cryptography(void){
	this->clearState();
	this->aes.setState(this->state, CRYPTOGRAPHY_MAX_STATE);
	this->routingKey = (char *)CRYPTOGRAPHY_ROUTING_KEY;
}

void Cryptography::clearState(void){
	for(int i=0;i<CRYPTOGRAPHY_MAX_STATE; i++)
		this->state[i] = 0x0;
}


void Cryptography::setState(char *state, size_t stateSize){
	this->setState((uint8_t *)state, stateSize);
}
void Cryptography::setState(uint8_t *state, size_t stateSize){
	this->clearState();
	for(int i=0;i<CRYPTOGRAPHY_MAX_STATE && i<stateSize; i++)
		this->state[i] = state[i];
}
void Cryptography::setState(String state){
	this->clearState();
	for(int i=0; i<CRYPTOGRAPHY_MAX_STATE && i<state.length(); i++)
		this->state[i] = state[i];
}

