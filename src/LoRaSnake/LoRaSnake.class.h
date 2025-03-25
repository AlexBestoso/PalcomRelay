#include <RadioLib.h>

#define LORA_SNAKE_CHIP_SX1262 "SX1262"

#define LORA_SNAKE_BOARD_TDECK 0
#define LORA_SNAKE_BOARD_T3S3 1

#define LORA_SNAKE_MODE LORA_SNAKE_CHIP_SX1262
//#define LORA_SNAKE_BOARD LORA_SNAKE_BOARD_TDECK
#define LORA_SNAKE_BOARD LORA_SNAKE_BOARD_T3S3

#define LORA_SNAKE_MAX_BUFFER 256

static void loraSnakeSetTxFlag(void);
static void loraSnakeSetRxFlag(void);

struct LoRaSnakePacket{
  	uint8_t data[LORA_SNAKE_MAX_BUFFER] = {0};
  	size_t data_size = 0;
  	float rssi = 0.0;
  	float snr = 0.0;
  	float freq_err = 0.0;
};

class LoRaSnake{
  	private: 
    		int _NSS;
    		int _DIO1;
    		int _NRST;
    		int _BUSY;
    		int _sck;
    		int _miso;
    		int _mosi;

    		bool running;

  	public:
    		LoRaSnake(void);
    		~LoRaSnake();
    		int txState; 
    		int rxState;
    		struct LoRaSnakePacket lrsPacket;

    		bool init(void);
    		bool listenStart(void);

    		int readRecv(void);
	
    		void listenStop(void);
      
    		void forceSendMode(void);
    		void sendStart(String v);
    		void sendStart(unsigned char *v, size_t l);

    		bool send(uint8_t *d, size_t s);
    		void sendStop(void);

		void modeSend(void);
		void modeRecv(void);
};
