class UrlEncoding{
	private:

	public:
		bool plusIsWhitespace = true;

		String encode(char *decoded, size_t decodedSiz){
			String ret = "";

			return ret;
		}
		String decode(char *encoded, size_t encodedSize){
			String ret = "";
			char translateArray[16] = {
				'0', '1', '2', '3', '4',
				'5', '6', '7', '8', '9',
				'a', 'b', 'c', 'd', 'e',
				'f'
			};

			// Ascii codes for capital A-F
			int capMin = 64;
			int capMax = 70;
	
			// distance between uppercase alphabet and lowercase alphabet.
			int lowercaseOffset = 32;
	
			// Parsing variables
			int parseContext = 0;
			char parsedChar;
	
			// decoding loop
			for(int i=0; i<encodedSize; i++){
				char currentChar = encoded[i];
				if(parseContext == 0){
					if(currentChar == '+' && plusIsWhitespace){
						ret += " ";
					}else if(currentChar == '%'){
						parseContext++;
					}else{
						ret += currentChar;
					}
				}else{
					if(currentChar >= capMin && currentChar <= capMax){
						currentChar = (int)currentChar + lowercaseOffset;
					}
					for(int j=0; j<16; j++){
						if(currentChar == translateArray[j]){
							if(parseContext == 1){
								parsedChar = (j<<4);
								parseContext++;
							}else{
								parsedChar += j;
								ret += parsedChar;
								parseContext = 0;
							}
							break;
						}
					}
				}
	
			}
			return ret;
		}
};
