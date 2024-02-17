class WebServer{
	private:
		bool accessPointMode = true;
		String ap_ssid = "LoRaSig";
		String ap_password = "LoRaSig!";
		IPAddress relayIp;
		//WiFiServer server(80);
	public:
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
			server.begin();
			return true;
		}
};
