class SetupPage : public WebPage{
	private:
		String createPage(){
			String pc = "<html><body>";
                                pc += "<h1>Palcom Relay Setup</h1>";
                                pc += "<form method=\"POST\" action=\"\">";
                                        pc += "<label>Target SSID: </label><input name=\"targetSSID\" type=\"text\" />";
					if(err_emptySSID){
						pc += this->craftErrorText("Field cannot be empty.");
						err_emptySSID = false;
					}
					pc += "<br>";
                                        pc += "<label>SSID Password: </label><input name=\"targetSSIDPassword\" type=\"password\" />";
					if(err_emptySSIDPassword){
						pc += this->craftErrorText("Field cannot be empty.");
						err_emptySSIDPassword = false;
					}
					pc += "<br>";
                                        pc += "<label>Relay Admin Name: </label><input name=\"relayUsername\" type=\"text\" value=\"admin\"/>";
					if(err_emptyUsername){
						pc += this->craftErrorText("Field cannot be empty.");
						err_emptyUsername = false;
					}
					pc += "<br>";
                                        pc += "<label>Relay Admin Password: </label><input name=\"relayPassword\" type=\"password\" />";
					if(err_emptyPassword){
						pc += this->craftErrorText("Field cannot be empty.");
						err_emptyPassword = false;
					}
					pc += "<br>";
                                        pc += "<label>Confirm Password: </label><input name=\"relayPasswordConf\" type=\"password\" />";
					if(err_emptyPasswordConfirm){
						pc += this->craftErrorText("Field cannot be empty.");
						err_emptyPasswordConfirm = false;
					}else if(err_passwordMissmatch){
						pc += this->craftErrorText("Passwords Don't Match.");
						err_passwordMissmatch = false;
					}
					pc += "<br>";
					pc += "<input type=\"submit\" name=\"setup\" value=\"Setup Relay\" />";
                                pc += "</form>";
                        pc += "</body></html>";
			return pc;

		}
		int subContext = 0;
		bool err_emptySSID = false;
		bool err_emptySSIDPassword = false;
		bool err_emptyUsername = false;
		bool err_emptyPassword = false;
		bool err_emptyPasswordConfirm = false;
		bool err_passwordMissmatch = false;

		String targetSSID = "";
                String targetSSIDPass = "";
                String relayUsername = "";
                String relayPassword = "";
                String relayPasswordConfirm = "";
	public:
		SetupPage(){
			this->setPageContent(this->createPage());
		}

		void finalizeSetup(){
			if(subContext == 0){ // if no errors
                                Serial.printf("Setting up Auth File.\n");
                                PalcomFS pfs;
                                Serial.printf("Checking if %s exists.\n", pfs_relayAuth);
                                if(pfs.exists(pfs_relayAuth)){
                                        Serial.printf("Removing old setup file.\n");
                                        pfs.rm(pfs_relayAuth);
                                }

                                Serial.printf("Building auth file.\n");
                                String data = targetSSID; data += "\n";
                                data += targetSSIDPass; data += "\n";
                                data += relayUsername; data += "\n";
                                data += relayPassword; data += "\n";

                                Serial.printf("Opening auth file.\n");
                                pfs.fd = SD.open(pfs_relayAuth, FILE_WRITE);
                                Serial.printf("Writing to auth file.\n");
                                pfs.write((unsigned char *)data.c_str(), data.length());
                                Serial.printf("Closing file.\n");
                                pfs.close();

                        }
		}

		int run(int context, HttpPacket httpPacket){
			targetSSID = "";
			targetSSIDPass = "";
			relayUsername = "";
			relayPassword = "";
			relayPasswordConfirm = "";
			
			err_emptySSID = false;
                	err_emptySSIDPassword = false;
                	err_emptyUsername = false;
                	err_emptyPassword = false;
                	err_emptyPasswordConfirm = false;
                	err_passwordMissmatch = false;

			
			for(int i=0; i<httpPacket.post_len; i++){
				if(i>=HTTP_PACKET_BUF_SIZE)
					break;
				if(httpPacket.post_key[i] == "targetSSID"){
					targetSSID = httpPacket.post_val[i];
					continue;
				}

				if(httpPacket.post_key[i] == "targetSSIDPassword"){
					targetSSIDPass = httpPacket.post_val[i];
					continue;
				}

				if(httpPacket.post_key[i] == "relayUsername"){
					relayUsername = httpPacket.post_val[i];
					continue;
				}

				if(httpPacket.post_key[i] == "relayPassword"){
					relayPassword = httpPacket.post_val[i];
					continue;
				}

				if(httpPacket.post_key[i] == "relayPasswordConf"){
					relayPasswordConfirm = httpPacket.post_val[i];
					continue;
				}
			}	

			this->subContext = 0;


			if(targetSSID == ""){
				err_emptySSID = true;
				this->subContext = -2; // value is empty
			}
                        if(targetSSIDPass == ""){
                		err_emptySSIDPassword = true;
				this->subContext = -3;
			}
                        if(relayUsername == ""){
                		err_emptyUsername = true;
				this->subContext = -4;
			}
                        if(relayPassword == ""){
                		err_emptyPassword = true;
				this->subContext = -5;
			}
                        if(relayPasswordConfirm == ""){
                		err_emptyPasswordConfirm = true;
				this->subContext = -6;
			}
			if(relayPassword != relayPasswordConfirm){
                		err_passwordMissmatch = true;
				this->subContext = -1;// passwords don't match.
			}

			Serial.printf("Setting Page Content.\n");
			this->setPageContent(this->createPage());
			return context;
		}	
};
