class WebServer{
	private:
		bool accessPointMode = true;
		String ap_ssid = "LoRaSig";
		String ap_password = "LoRaSig!";
		IPAddress relayIp;

		SetupPage setupPage;
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
			if(client){
				Serial.printf("Client Available.\n");
				String currentLine = "";
				while(client.connected()){
					if(client.available()){
						char c = client.read();
						Serial.write(c);
						if(c == '\n'){
						/*	if(currentLine.length() == 0){
								if(hasPostContent){
									hasPostContent = false;
									continue;
								}
								String response = "";
								switch(context){
									default: // Setup Page
										response = setupPage.getResponseHeader();
										response += setupPage.getPageContent();
								}
								response += "\r\n";
								client.print(response);
								break;
							}*/
							
							currentLine += "\r\n";
							if(currentLine.startsWith("Content-Length: ") && currentLine.endsWith("\r\n")){
								Serial.printf("Extracting content length : %s\n", currentLine.c_str());
								String grabber = "";
								int i=16;
								while(currentLine[i] != '\r'){
									grabber += currentLine[i];
									i++;
								}
								postSize = grabber.toInt();
								if(postSize > 0)
									hasPostContent = true;
								currentLine = "";
							}else{
								currentLine = "";
							}
						}else if(c != '\r'){
							currentLine += c;
						}

						// Process get / post request.
						switch(context){
							default: // Setup Page
								 setupPage.run(currentLine);
						}
					}else{
						String response = "";
						switch(context){
							default: // Setup Page
								response = setupPage.getResponseHeader();
								response += setupPage.getPageContent();
						}
						response += "\r\n";
						client.print(response);
						break;
					}
				}
			}
			client.stop();
			Serial.println("Client Disconnected.");
			return context;
		}
};
