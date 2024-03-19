#define PAL_LORA_CTX_LISTEN 0
class PalcomLoRa{
	private:
		bool sendMode = false;
		int loRaBand = BAND;
		int loRaSyncWord = 0xAB;
		String recvBuffer = "";
		int context = PAL_LORA_CTX_LISTEN;
	public:
		void setContext(int ctx){
			this->context = ctx;
		}
		int getContext(void){
			return this->context;
		}
		bool setup(void){
			SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
			LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
			LoRa.setSyncWord(loRaSyncWord);
			LoRa.setCodingRate4(6);
			LoRa.setSignalBandwidth(250);
			LoRa.disableCrc();
			LoRa.setPreambleLength(15);
			LoRa.setSpreadingFactor(10);
			LoRa.setTxPower(17);
			if(!LoRa.begin(loRaBand)){
				return false;
			}
			return true;
		}

		void send(const char *msg){
			LoRa.beginPacket();
			LoRa.print(msg);
			LoRa.endPacket();
		}

		void DebugRecvBuf(void){
			Serial.printf("Received : %s\n", recvBuffer.c_str());
		}

		String recv(void){
			recvBuffer = "";
			if (LoRa.parsePacket()) {
				Serial.printf("Debug : Packet Parsed.\n");
				while(LoRa.available()){
					recvBuffer += (char)LoRa.read();
				}
			}
			return recvBuffer;
		}
};
