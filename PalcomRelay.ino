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
#include <ds3231.h>
#include <SD.h>
#include <string.h>
#include <string>
#include <USB.h>
#include <RadioLib.h>
//#include "mbedtls/config.h"
#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/bignum.h>
#include <mbedtls/x509.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>
#include <base64.h>


int relayMode = 0; // RELAY_MODE_DISABLED

#include <src/error/error.h>

#include <src/taskQueue/taskQueue.h>
TaskQueue taskQueue;

//#include <src/partition/partition.h>
#include <usbEventCallback.h> // <-- partition is included in here.




// Global Variables





OLED_CLASS_OBJ display(OLED_ADDRESS, 18, 17);

SPIClass sdSPI(SPI);

#if !ARDUINO_USB_CDC_ON_BOOT
USBCDC USBSerial;
#endif

bool loraSnakeTransmit = false;
bool loraSnakeReceive = false;
SX1262 _radio = new Module(7, 33, 8, 34);
#include <src/LoRaSnake/LoRaSnake.class.h>
LoRaSnake loraSnake;

#include <src/cryptography/cryptography.h>
Cryptography cryptography;

// Core Includes
#include <src/core/graphics/graphics.h>
#include <src/core/comms/comms.h>
#include <src/init/init.h>
#include "./classes/classLinker.h"

#include <src/core/core.h>
PalcomCore core(&cryptography);

PalcomRelay palcomRelay;




void setup(void){
  try{
    core.startCore();

  }catch(CoreException &e){
    e.out();
    e.halt();
  }
}


void loop(void){
  try{
    core.driveCore();
  }catch(CoreException &e){
    e.out();
  }
  /*if(palcomRelay.fetchPacket()){
    palcomRelay.executeRelay(relayMode);
  }*/
  delay(1000);
  
}
