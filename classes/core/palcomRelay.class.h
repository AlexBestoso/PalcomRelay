class PalcomRelay{
	private:
		WebServer webServer;
		int context = 0;
		/*
		 * Checks for certain configuration files to determine if we should be
		 * operating in Access-point mode or client mode.
		 * */
		bool checkWiFiOperationMode(void){
			
			return webServer.getAccessPointMode();
		}
	public:
		PalcomRelay(void){
	
		}

		bool setup(void){
			Serial.begin(115200);
    			while (!Serial);

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
    			display.drawString(display.getWidth() / 2, display.getHeight() / 2, LORA_SENDER ? "LoRa Sender" : "LoRa Receiver");
    			display.display();
    			delay(2000);
	
    			if (SDCARD_CS >  0) {
        			display.clear();
        			SPIClass sdSPI(VSPI);
        			sdSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);
        			if (!SD.begin(SDCARD_CS, sdSPI)) {
            				display.drawString(display.getWidth() / 2, display.getHeight() / 2, "SDCard  FAIL");
        			} else {
            				display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, "SDCard  PASS");
            				uint32_t cardSize = SD.cardSize() / (1024 * 1024);
            				display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Size: " + String(cardSize) + "MB");
        			}
        			display.display();
        			delay(2000);
    			}


    			String info = ds3231_test();
    			if (info != "") {
        			display.clear();
        			display.setFont(ArialMT_Plain_16);
        			display.setTextAlignment(TEXT_ALIGN_CENTER);
        			display.drawString(display.getWidth() / 2, display.getHeight() / 2, info);
        			display.display();
        			delay(2000);
    			}

			if(this->checkWiFiOperationMode()){
				if(!webServer.setupAccessPoint()){
					Serial.printf("Failed to create access point.\n");
					while(1){delay(100);}
				}
			}else{
			
			}

			Serial.printf("Setup Successful.\n");
			return true;
		}

		void run(WiFiClient client){
			switch(this->context){
				default:
					webServer.setupPage(client);
			}
		}
};
