// System includes
#include <SPI.h>
#include <LoRa.h>
#include "./ds3231.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <SD.h>
#include <string.h>
#include <string>

// Global Variables
OLED_CLASS_OBJ display(OLED_ADDRESS, OLED_SDA, OLED_SCL);
WiFiServer server(80);
SPIClass sdSPI(VSPI);
#define WIFI_SSID       "Morning Star"
#define WIFI_PASSWORD   "Bnddff6&"
#define RELAY_CONTEXT_SETUP 0
#define RELAY_CONTEXT_MAIN 1

//const char *ssid = "LoRaSig";
//const char *password = "LoRaSig!";

// Custom Includes
#include "./classes/classLinker.h"

PalcomRelay palcomRelay;

void setup()
{
  palcomRelay.setup();
    /*WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        display.clear();
        Serial.println("WiFi Connect Fail");
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "WiFi Connect Fail");
        display.display();
        delay(2000);
        esp_restart();
    }
    Serial.print("Connected : ");
    Serial.println(WiFi.SSID());
    Serial.print("IP:");
    Serial.println(WiFi.localIP().toString());
    display.clear();
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, "IP:" + WiFi.localIP().toString());
    display.display();
    delay(2000);

    SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
    LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    if (!LORA_SENDER) {
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoraRecv Ready");
        display.display();
    }*/
}

int count = 0;

void loop(){
  WiFiClient client = server.available();
  if(client)
    palcomRelay.run(client);
/*#if LORA_SENDER
    int32_t rssi;
    if (WiFi.status() == WL_CONNECTED) {
        rssi = WiFi.RSSI();
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Send RSSI:" + String(rssi));
        display.display();
        LoRa.beginPacket();
        LoRa.print("WiFi RSSI: ");
        LoRa.print(rssi);
        LoRa.endPacket();
    } else {
        Serial.println("WiFi Connect lost ...");
    }
    delay(2500);
#else
    if (LoRa.parsePacket()) {
        String recv = "";
        while (LoRa.available()) {
            recv += (char)LoRa.read();
        }
        count++;
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, recv);
        String info = "[" + String(count) + "]" + "RSSI " + String(LoRa.packetRssi());
        display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, info);
        display.display();
    }
#endif*/
}
