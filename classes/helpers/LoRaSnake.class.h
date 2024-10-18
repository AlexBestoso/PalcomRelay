#include <RadioLib.h>

#define LORA_SNAKE_CHIP_SX1262 "SX1262"

#define LORA_SNAKE_BOARD_TDECK 0
#define LORA_SNAKE_BOARD_T3S3 1

#define LORA_SNAKE_MODE LORA_SNAKE_CHIP_SX1262
//#define LORA_SNAKE_BOARD LORA_SNAKE_BOARD_TDECK
#define LORA_SNAKE_BOARD LORA_SNAKE_BOARD_T3S3

volatile bool loraSnakeTransmit = false;
volatile bool loraSnakeReceive = false;

void loraSnakeSetTxFlag(void){
  loraSnakeTransmit = true;
}
void loraSnakeSetRxFlag(void){
  loraSnakeReceive = true;
}

struct LoRaSnakePacket{
  uint8_t data[256] = {0};
  size_t data_size=0;
  float rssi=0.0;
  float snr=0.0;
  float freq_err=0.0;
};

class LoRaSnake{
  private:
  /*
  * Chip mode set at compilation time
  */
  #if LORA_SNAKE_BOARD == LORA_SNAKE_BOARD_T3S3
    int _NSS = 7;
    int _DIO1 = 33;
    int _NRST = 8;
    int _BUSY = 34;
    int _sck = 5;
    int _miso = 3;
    int _mosi = 6;
  #elif LORA_SNAKE_BOARD == LORA_SNAKE_BOARD_TDECK
    int _NSS = 9;
    int _DIO1 = 45;
    int _NRST = 17;
    int _BUSY = 13;
    int _sck = 40;
    int _miso = 38;
    int _mosi = 41;
  #endif

  #ifdef LORA_SNAKE_MODE == LORA_SNAKE_CHIP_SX1262
    //SX1262 _radio = new Module(9, 45, 17, 13);
    SX1262 _radio = new Module(_NSS, _DIO1, _NRST, _BUSY);
  #endif
  
  bool running = false;

  public:

    int txState=RADIOLIB_ERR_NONE, rxState=RADIOLIB_ERR_NONE;
    struct LoRaSnakePacket lrsPacket;

    /*
     * Use for unsupported boards
     */
    bool init(int NSS, int DIO1, int NRST, int BUSY, int sck, int miso, int mosi, float freq){
      this->_radio = new Module(NSS, DIO1, NRST, BUSY);
      SPI.end();
      SPI.begin(sck, miso, mosi);
      if(this->_radio.begin(freq) == RADIOLIB_ERR_NONE){ 
        return true;
      }
      return false;
    }

    bool init(float freq=868.0){ 
    #ifdef LORA_SNAKE_MODE == LORA_SNAKE_CHIP_SX1262
          //this->_radio = new Module(9, 45, 17, 13);
          SPI.end();
          SPI.begin(_sck, _miso, _mosi);
          if(this->_radio.begin(freq) == RADIOLIB_ERR_NONE){
            this->_radio.setPacketSentAction(loraSnakeSetTxFlag);
            this->_radio.setPacketReceivedAction(loraSnakeSetRxFlag);
            return true;
          }
    #endif
    
      return false;
    }

    bool listenStart(void){
      int s = this->_radio.startReceive();
      if (s == RADIOLIB_ERR_NONE){
        running = true;
        return true;
      }
      return false;
    }

    int readRecv(void){
      if(loraSnakeReceive){
        loraSnakeReceive = false;
        lrsPacket.data_size = this->_radio.getPacketLength();
        this->rxState = this->_radio.readData(lrsPacket.data, lrsPacket.data_size);

        if(this->rxState == RADIOLIB_ERR_NONE){
          lrsPacket.rssi = this->_radio.getRSSI();
          lrsPacket.snr = this->_radio.getSNR();
          lrsPacket.freq_err = this->_radio.getFrequencyError();
          return 1;
        }else if(this->rxState == RADIOLIB_ERR_CRC_MISMATCH){
          //Serial.printf("Malformed packet received.");
          return 2; // malformed packet
        }else{
          Serial.printf("Unknown Error");
          return 0; // unknown error
        }
      }else if(!running){
        this->listenStart();
        return 0;
      }else{
        return 0;
      }
    }

    void listenStop(void){
        this->_radio.standby();
        running = false;
    }

    void forceSendMode(void){
     loraSnakeTransmit = true; 
    }

    void sendStart(String v){
      txState = this->_radio.startTransmit(v.c_str());
    }
    void sendStart(byte *v, size_t l){
      txState = this->_radio.startTransmit(v, l);
    }

    bool send(uint8_t *d, size_t s){
      sendStart(d, s);
      delay(250);
      
      if(txState == RADIOLIB_ERR_NONE){
          sendStop();
          return true;
        }else{
          Serial.printf("Radio Error %d\n", txState);
        }
        loraSnakeTransmit = false;
        sendStop();
        
      
      return false;
    }

    void sendStop(void){
      this->_radio.finishTransmit();
    }
};
