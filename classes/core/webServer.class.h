class WebServer{
	private:
		bool accessPointMode = true;
		String ap_ssid = "LoRaSig";
		String ap_password = "LoRaSig!";
		IPAddress relayIp;

		SetupPage setupPage;

		String httpRequest = "";

		void fetchHttpRequest(WiFiClient client){
			this->httpRequest = "";
			while(client.connected()){
				if(client.available()){
                                        char c = client.read();
					this->httpRequest += c;	
				}else{
					break;
				}
			}
		}

		HttpPacket processRequest(void){
			HttpPacket ret;
			if(this->httpRequest == ""){
				ret.error = true;
				return ret;
			}

			/*
			 * Check if the HTTP request is a POST/GET request.
			 * */
			int uriOffset = 0;
			if(this->httpRequest.startsWith("POST ")){
				ret.post_request = true;
				ret.get_request = false;
				uriOffset = 5;
			}else if(this->httpRequest.startsWith("GET ")){
				ret.post_request = false;
                                ret.get_request = true;
				uriOffset = 4;
			}else{
				ret.error = true;
				return ret;
			}


			/*
			 * Get the URI from the above request.
			 * */
			ret.uri = "";
			while(this->httpRequest[uriOffset] != ' ' && this->httpRequest[uriOffset] != '\n' && this->httpRequest[uriOffset] != '\r' && ret.uri.length() < 512){
				ret.uri += this->httpRequest[uriOffset];
				uriOffset++;
			}

			/* Do POST specific shit */
			if(ret.post_request){
				// fetch Content-Length
				String line = "";
				bool christ = false;
				for(int i=0; i<this->httpRequest.length(); i++){
					line += httpRequest[i];
					if(line.endsWith("\r\n")){
						if(line.startsWith("\r\n") && line.length() == 2){
							christ = true;
							line = "";
							continue;
						}
						if(line.startsWith("Content-Length:")){
							String grab = "";
							for(int j=16; j<line.length(); j++){
								if(line[j] == '\r' || line[j] == '\n'){
									ret.content_length = grab.toInt();
									break;
								}
								grab += line[j];
							}
						}
						line = "";
					}

				}
				if(christ){
					ret.post_len = 0;
					//ret.post_val
					String sacrament = "";
					christ = false;
					for(int j=0; j<line.length(); j++){
						if(ret.post_len >= HTTP_PACKET_BUF_SIZE){
							break;
						}
						char c = line[j];

						if(c == '\r' || c == '\n'){
							break;
						}
						if(c == '&'){
							ret.post_val[ret.post_len] = sacrament;
							ret.post_len++;
							sacrament = "";
							continue;
						}
						if(c == '='){
							ret.post_key[ret.post_len] = sacrament;
							sacrament = "";
							continue;
						}
						sacrament += c;
					}
				}
			}
			
			// Process URI


			return ret;
		}
	public:
		String getIpAsString(void){
  			return String(relayIp[0]) + String(".") + String(relayIp[1]) + String(".") + String(relayIp[2]) + String(".") + String(relayIp[3]);
		}
		void setApSsid(String v){
			ap_ssid = v;
		}
		void setApPassword(String v){
			ap_password = v;
		}
		bool getAccessPointMode(void){
			return accessPointMode;
		}
		bool setAccessPointMode(bool v){
			accessPointMode = v;
		}

		bool setupAccessPoint(void){
			if(!WiFi.softAP(this->ap_ssid.c_str(), this->ap_password.c_str())){
                                Serial.printf("Failed to create ap\n");
                                return false;
                        }

                        relayIp = WiFi.softAPIP();
			display.clear();
                        display.setFont(ArialMT_Plain_16);
                        display.setTextAlignment(TEXT_ALIGN_CENTER);
                        display.drawString(display.getWidth() / 2, display.getHeight() / 2, getIpAsString());
                        display.display();
                        delay(2000);
			server.begin();
			return true;
		}

		int run(WiFiClient client, int context){
			bool hasPostContent = false;
			int postSize = 0;
			String targetURI = "";
			String postContent = "";
			if(client){
				this->fetchHttpRequest(client);
				HttpPacket httpPacket = this->processRequest();
				if(httpPacket.error){
					Serial.printf("Failed to process HTTP Packet.\n");
				}


				// Process get / post request.
				switch(context){
					default: // Setup Page
						 context = setupPage.run(context, httpPacket);
				}

				String response = "";		
				switch(context){
					default: // Setup Page
						response = setupPage.getResponseHeader();
						response += setupPage.getPageContent();
				}
				response += "\r\n";
				client.print(response);
			}

			client.stop();
			Serial.println("Client Disconnected.");
			setupPage.finalizeSetup();
			return context;
		}
};
