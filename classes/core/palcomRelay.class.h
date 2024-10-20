/*
 * Move these into their own usb class one day
 * */

LoRaSnake loraSnake; 

static void runSerialCommand(uint8_t *buf, size_t bufSize){
  if(bufSize <= 4){
    // Invalid command size
    Serial.printf("Invalid command size\n");
    return;
  }else if(buf[0] != 'C' && buf[1] != 'M' && buf[2] != 'D' && buf[3] != ' '){
    // invalid command structure
    Serial.printf("Invalid command structure\n");
    return;
  }

  Serial.printf("Executing command [%ld] : ", bufSize);
  String cmd = "";
  for(int i=4; i<bufSize; i++)
    Serial.printf("%c", buf[i]);
	Serial.printf("\n");

}

static void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  if (event_base == ARDUINO_USB_EVENTS) {
    arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
    switch (event_id) {
      case ARDUINO_USB_STARTED_EVENT: Serial.println("USB PLUGGED"); break;
      case ARDUINO_USB_STOPPED_EVENT: Serial.println("USB UNPLUGGED"); break;
      case ARDUINO_USB_SUSPEND_EVENT: Serial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en); break;
      case ARDUINO_USB_RESUME_EVENT:  Serial.println("USB RESUMED"); break;

      default: break;
    }
  } else if (event_base == ARDUINO_USB_CDC_EVENTS) {
    arduino_usb_cdc_event_data_t *data = (arduino_usb_cdc_event_data_t *)event_data;
    switch (event_id) {
      case ARDUINO_USB_CDC_CONNECTED_EVENT:    Serial.println("CDC CONNECTED"); break;
      case ARDUINO_USB_CDC_DISCONNECTED_EVENT: Serial.println("CDC DISCONNECTED"); break;
      case ARDUINO_USB_CDC_LINE_STATE_EVENT:   Serial.printf("CDC LINE STATE: dtr: %u, rts: %u\n", data->line_state.dtr, data->line_state.rts); break;
      case ARDUINO_USB_CDC_LINE_CODING_EVENT:
        Serial.printf(
          "CDC LINE CODING: bit_rate: %lu, data_bits: %u, stop_bits: %u, parity: %u\n", data->line_coding.bit_rate, data->line_coding.data_bits,
          data->line_coding.stop_bits, data->line_coding.parity
        );
        break;
      case ARDUINO_USB_CDC_RX_EVENT:
	Serial.printf("Processing Serial Command...");
	{
        	uint8_t buffer[data->rx.len];
        	size_t len = USBSerial.read(buffer, data->rx.len);
        	runSerialCommand(buffer, len);
	}
        break;
      case ARDUINO_USB_CDC_RX_OVERFLOW_EVENT: Serial.printf("CDC RX Overflow of %d bytes", data->rx_overflow.dropped_bytes); break;

      default: break;
    }
  }
}



class PalcomRelay{
	private:
		String msg = "";
		
		bool parseMessage(void){
			size_t s = loraSnake.lrsPacket.data_size;
			uint8_t *d = loraSnake.lrsPacket.data;
			
			if(s <= 4 || s >= 255)
				return false;

			if(d[0] != 'L' && d[1] != 'A' && d[2] != 'P' && d[4] != ' '){
				return false;
			}
			
			msg = "";
			for(int i=5; i<s; i++){
				msg += (char)d[i];
			}

			return true;
		}
	public:
		PalcomRelay(void){
	
		}

		bool setup(void){
			Serial.begin(115200);
			if(!loraSnake.init()){
				return false;
			}

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
	
    			/*if (SDCARD_CS >  0) {
        			display.clear();
        			sdSPI.begin(14, 2, 11, 13);
        			if (!SD.begin(13, sdSPI)) {
            				display.drawString(display.getWidth() / 2, display.getHeight() / 2, "SDCard  FAIL");
        			} else {
            				display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, "SDCard  PASS");
            				uint32_t cardSize = SD.cardSize() / (1024 * 1024);
            				display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Size: " + String(cardSize) + "MB");
        			}
        			display.display();
        			delay(2000);
    			}*/


    			String info = ds3231_test();
    			if (info != "") {
        			display.clear();
        			display.setFont(ArialMT_Plain_16);
        			display.setTextAlignment(TEXT_ALIGN_CENTER);
        			display.drawString(display.getWidth() / 2, display.getHeight() / 2, info);
        			display.display();
        			delay(2000);
    			}

			if(!loraSnake.listenStart()){
				display.clear();
        			display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoRa Err 1");
				display.display();
				while(1){
					Serial.printf("Failed to listen for connections.\n");
					delay(1000);
				}
			}

			display.clear();
    			display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Palnode");
    			display.display();
    			delay(2000);
			
		//	USB.onEvent(usbEventCallback);
			USBSerial.onEvent(usbEventCallback);

		//	USB.begin();
			USBSerial.begin();
			return true;
		}

		bool fetchPacket(void){
			bool ret = loraSnake.readRecv() == 0 ? false : true;
			if(!ret)
				return false;

			if(!parseMessage()){
				return false;
			}
			return ret;
		}

		bool executeRelay(int mode){
			switch(mode){
				default: // RELAY_MODE_DISABLED
					Serial.printf("LOG Relay is disabled.\n");
					return true;
			}
		}
};		
