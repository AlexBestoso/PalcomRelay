class WebServer{
	private:
		bool accessPointMode = true;
		String ap_ssid = "LoRaSig";
		String ap_password = "LoRaSig!";
		IPAddress relayIp;
		//WiFiServer server(80);
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

		void setupPage(WiFiClient client){
			if(client){
				Serial.printf("Client Available.\n");
				String currentLine = "";
				while(client.connected()){
					if(client.available()){
						char c = client.read();
						Serial.write(c);
						if(c == '\n'){
							if(currentLine.length() == 0){
								client.println("HTTP/1.1 200 OK");
            							client.println("Content-type:text/html");
            							client.println();

								client.print("<h1>Place holder setup page.</h1>");

								client.println();
								break;
							}else{
								currentLine = "";
							}
						}else if(c != '\r'){
							currentLine += c;
						}

						// Process get / post request.
					}
				}
			}
			client.stop();
			Serial.println("Client Disconnected.");
		}
};
