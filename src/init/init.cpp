#include <Arduino.h>
#include <cstdint>
#include <ds3231.h>
#include <SPI.h>

#include <src/error/error.h>
#include <src/LoRaSnake/LoRaSnake.class.h>
#include <usbEventCallback.h>

#include "./init.h"
extern OLED_CLASS_OBJ display;
extern LoRaSnake loraSnake;
extern int relayMode;

PalcomInit::PalcomInit(void){
	this->serialOn = false;
}

void PalcomInit::initSerial(void){
	Serial.begin(115200);
	this->serialOn = true;
}

void PalcomInit::initRadio(void){
	if(!this->serialOn)
		this->initSerial();


	SPI.end();
	SPI.begin(5, 3, 6);
	bool err = loraSnake.init();
	if(err == false){
		throw CoreException("Failed to initalize the LoRa chip", "PalcomInit", "initRadio", ERR_INIT_LORA);
	}
}

void PalcomInit::initDisplay(void){
	if (OLED_RST > 0) {
		pinMode(OLED_RST, OUTPUT);
		digitalWrite(OLED_RST, HIGH);
		delay(100);
		digitalWrite(OLED_RST, LOW);
		delay(100);
		digitalWrite(OLED_RST, HIGH);
	}
	display.init();
	display.flipScreenVertically();
	display.clear();
	display.setFont(ArialMT_Plain_16);
	display.setTextAlignment(TEXT_ALIGN_CENTER);

	String info = ds3231_test();
	if (info != "") {
		display.clear();
		display.setFont(ArialMT_Plain_16);
		display.setTextAlignment(TEXT_ALIGN_CENTER);
		display.drawString(display.getWidth() / 2, display.getHeight() / 2, info);
		display.display();
		delay(2000);
	}

	display.clear();
	display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Starting...");
	display.display();
	delay(2000);
}

void PalcomInit::initUsb(void){
	USBSerial.onEvent(usbEventCallback);
    	USBSerial.begin();
}

void PalcomInit::initSettings(void){
	PalcomPartition pp;
    	palcom_partition_t pt;
    	pp.read(&pt);

    	if(pt.mode >= RELAY_MODE_START && pt.mode <= RELAY_MODE_END){
      		relayMode = pt.mode;
    	}else{
      		relayMode = RELAY_MODE_DISABLED;
      		pt.mode = relayMode;
      		pp.write(pt);
    	}
}
