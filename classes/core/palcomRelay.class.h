
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
