#include <Arduino.h>
#include <cstdint>
#include <ds3231.h>
#include <SPI.h>
#include <SD.h>

#include <src/error/error.h>
#include <src/LoRaSnake/LoRaSnake.class.h>
#include <usbEventCallback.h>

#include "./init.h"
extern OLED_CLASS_OBJ display;
extern LoRaSnake loraSnake;
extern int relayMode;
extern SPIClass sdSPI;
extern SPIClass loraSPI;
extern bool sdActive;

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

void PalcomInit::initStorage(void){
	Serial.printf("Starting SD Storage...\n");
	sdSPI.begin(14, 2, 11, 13);
	if(SD.begin(13, sdSPI)){
		uint8_t cardType = SD.cardType();
                                uint32_t cardSize = SD.cardSize() / (1024 * 1024);
                                uint32_t cardTotal = SD.totalBytes() / (1024 * 1024);
                                uint32_t cardUsed = SD.usedBytes() / (1024 * 1024);

                                if(cardType == CARD_NONE){
                                        Serial.println("No SD_MMC card attached");
					sdActive = false;
                                }else{
					sdActive = true;
                                        Serial.print("SD_MMC Card Type: ");
                                        if(cardType == CARD_MMC){
                                                Serial.println("MMC");
                                        }else if(cardType == CARD_SD) {
                                                Serial.println("SDSC");
                                        }else if(cardType == CARD_SDHC) {
                                                Serial.println("SDHC");
                                        }else{
                                                Serial.println("UNKNOWN");
                                        }
                                        Serial.printf("SD Card Size: %lu MB\n", cardSize);
                                        Serial.printf("Total space: %lu MB\n",  cardTotal);
                                        Serial.printf("Used space: %lu MB\n",   cardUsed);
				}
	}else{
		Serial.printf("SPI begin failed\n");
	}
		/*if (SDCARD_CS >  0) {
	sdSPI.begin(14, 2, 11, 13);
	if (!SD.begin(13, sdSPI)) {
	display.drawString(display.getWidth() / 2, display.getHeight() / 2, "SDCard  FAIL");
	} else {
	display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, "SDCard  PASS");
	uint32_t cardSize = SD.cardSize() / (1024 * 1024);
	display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Size: " + String(cardSize) + "MB");
	}
	display.display();
	delay(2000);*/
}
