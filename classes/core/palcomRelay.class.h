/*
 * Move these into their own usb class one day
 * */

LoRaSnake loraSnake; 

#define RELAY_CMD_MODE 0

static int getCommandId(String cmd){
	String grabber = "";
	for(int i=0; i<cmd.length(); i++){
		grabber += cmd[i];
		if(grabber == "mode "){
			return RELAY_CMD_MODE;
		}
	}
	return -1;
}

static void relayCmd_mode(String cmd){
	String grabber = "";
	for(int i=5; i<cmd.length(); i++)
		grabber += cmd[i];

	PalcomPartition pp;
	palcom_partition_t pt;
	if(grabber == "disable"){
		Serial.printf("Disabling relay...\n");
		if(!pp.read(&pt)){
			Serial.printf("Failed to read from flash.\n");
		}
		pt.mode = RELAY_MODE_DISABLED;
		if(!pp.write(pt)){
			Serial.printf("Failed to write to flash.\n");
			return;
		}
		relayMode = pt.mode;
	}else if(grabber == "repeat"){
		Serial.printf("Enabling repeater mode...\n");
		if(!pp.read(&pt)){
			Serial.printf("Failed to read from flash.\n");
		}
                pt.mode = RELAY_MODE_REPEAT;
                if(!pp.write(pt)){
			Serial.printf("Failed to write to flash.\n");
			return;
		}
		relayMode = pt.mode;
	}else{
		Serial.printf("Invalid relay mode\n");
	}
}

static void runSerialCommand(uint8_t *buf, size_t bufSize){
  	if(bufSize <= 4){
    		// Invalid command size
    		return;
  	}else if(buf[0] != 'C' && buf[1] != 'M' && buf[2] != 'D' && buf[3] != ' '){
    		// invalid command structure
    		return;
  	}

  	String cmd = "";
  	for(int i=4; i<bufSize; i++){
		cmd += (char)buf[i];
	}
	
	switch(getCommandId(cmd)){
		case RELAY_CMD_MODE:
			relayCmd_mode(cmd);
		break;
	}
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
		size_t msg_max = 256;
		uint8_t msg[256] = {0};
		size_t msg_size = 0;		

		void clearMsg(int full=1){
			for(int i=0; i<msg_max; i++){
				msg[i] = 0;
				if(full == 1)
				loraSnake.lrsPacket.data[i] = 0;
			}
			if(full == 1)
			loraSnake.lrsPacket.data_size = 0;
			msg_size = 0;
		}

		bool parseMessage(void){
			size_t s = loraSnake.lrsPacket.data_size;
			uint8_t *d = loraSnake.lrsPacket.data;
			
			if(s <= 4 || s >= 256){
				clearMsg();
				return false;
			}

			if(d[0] != 'L' && d[1] != 'A' && d[2] != 'P' && d[3] != ' '){
				clearMsg();
				return false;
			}
			
			clearMsg(0);
			msg[0] = 'P';
			msg[1] = 'A';
			msg[2] = 'L';
			msg[3] = ' ';
			msg_size = s;
			for(int i=4; i<s; i++){
				msg[i] = d[i];
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
			
			USBSerial.onEvent(usbEventCallback);
			USBSerial.begin();
			return true;
		}

		bool fetchPacket(void){
			int v = loraSnake.readRecv();
			bool ret = v == 0 ? false : true;
			if(!ret || loraSnake.lrsPacket.data_size <= 0 || v == 2)
				return false;

			if(!parseMessage()){
				return false;
			}
			return ret;
		}

		bool executeRelay(int mode){
			bool ret = false;
			switch(mode){
				case RELAY_MODE_REPEAT:
					Serial.printf("LOG repeating message: [%ld]\n", msg_size);
					ret = loraSnake.send(msg, msg_size);
					if(!ret){
						Serial.printf("Failed to relay message...\n");
					}else{
						Serial.printf("Sent...\n");
					}
					clearMsg();
					return ret;
				break;
				default: // RELAY_MODE_DISABLED
					Serial.printf("LOG Relay is disabled.\n");
					clearMsg();
					return true;
			}
			
			return false;
		}
};		
