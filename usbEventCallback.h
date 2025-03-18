
#include <src/partition/partition.h>
#include <USB.h>
#define RELAY_CMD_MODE 0

#define RELAY_MODE_DISABLED 0
#define RELAY_MODE_REPEAT 1
#define RELAY_MODE_START RELAY_MODE_DISABLED
#define RELAY_MODE_END RELAY_MODE_REPEAT
extern int relayMode;


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