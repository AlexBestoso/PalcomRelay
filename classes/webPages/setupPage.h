class SetupPage : public WebPage{
	private:
		String createPage(int context){
			String pc = "<html><body>";
                                pc += "<h1>Palcom Relay Setup</h1>";
                                pc += "<form method=\"POST\" action=\"\">";
                                        pc += "<label>Target SSID: </label><input name=\"targetSSID\" type=\"text\" /><br>";
                                        pc += "<label>SSID Password: </label><input name=\"targetSSIDPassword\" type=\"password\" /><br>";
                                        pc += "<label>Relay Admin Name: </label><input name=\"relayUsername\" type=\"text\" value=\"admin\"/><br>";
                                        pc += "<label>Relay Admin Password: </label><input name=\"relayPassword\" type=\"password\" /><br>";
                                        pc += "<label>Confirm Password: </label><input name=\"relayPasswordConf\" type=\"password\" /><br>";
					pc += "<input type=\"submit\" name=\"setup\" value=\"Setup Relay\" />";
                                pc += "</form>";
                        pc += "</body></html>";
			return pc;
		}
	public:
		SetupPage(){
			this->setPageContent(this->createPage(0));
		}
		void run(String clientRequest){
			Serial.printf("CLIENT REQUEST : %s\n", clientRequest.c_str());
		}	
};
