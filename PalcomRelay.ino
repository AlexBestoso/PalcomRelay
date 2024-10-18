/*
T3-S3 	Value
Board 	ESP32S3 Dev Module
Port 	Your port
USB CDC On Boot 	Enable
CPU Frequency 	240MHZ(WiFi)
Core Debug Level 	None
USB DFU On Boot 	Disable
Erase All Flash Before Sketch Upload 	Disable
Events Run On 	Core1
Flash Mode 	QIO 80MHZ
Flash Size 	4MB(32Mb)
Arduino Runs On 	Core1
USB Firmware MSC On Boot 	Disable
Partition Scheme 	Default 4M Flash with spiffs(1.2M APP/1.5MB SPIFFS)
PSRAM 	QSPI PSRAM
Upload Mode 	UART0/Hardware CDC
Upload Speed 	921600
USB Mode 	CDC and JTAG
Programmer 	Esptool
*/

// System includes
#include <SPI.h>
#include <LoRa.h>
#include "./ds3231.h"
#include <SD.h>
#include <string.h>
#include <string>


// Global Variables
OLED_CLASS_OBJ display(OLED_ADDRESS, 18, 17);

SPIClass sdSPI(SPI);

// Custom Includes
#include "./classes/classLinker.h"

PalcomRelay palcomRelay;

void setup(void){
  palcomRelay.setup();
   
}


void loop(void){

}
