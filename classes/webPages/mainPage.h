class MainPage : public WebPage{
	private:
		bool username = false;
		bool  password = false;
		String authCookie = "";
		AuthFile authFile;

		String createPage(){
			String pc = "<html><body>";
			if(!this->validateAuthCookie(authCookie)){
				pc += "<h1>Palcom Relay Login</h1>";
				pc += "<form method=\"POST\">";
					pc += "<label>Username: </label><input name=\"username\" type=\"text\" value=\"\" /><br>";
					pc += "<label>Password: </label><input name=\"password\" type=\"password\" value=\"\" />";
					pc += "<input name=\"login\" type=\"submit\" value=\"login\" />";
				pc += "</form>";
			}else{
				RelayConfig rc;
				PalcomFS pfs;
				if(pfs.exists(pfs_relayConfig)){
					pfs.fd = SD.open(pfs_relayConfig, FILE_READ);
					size_t fSize = pfs.fd.size();
					pfs.clearFileBuffer();
					pfs.fd.read(fileData, fSize);
					pfs.close();

					int ctx = 0;
					for(int i=0; i<fSize; i++){
						if(ctx == 0){
							if(fileData[i] == '\n'){
								ctx = 1;
								continue;
							}
							rc.relayIP += (char)fileData[i];
						}else{
							if(fileData[i] == '\n'){
                                                                ctx = 1;
                                                                break;
                                                        }
                                                        rc.relayPort += (char)fileData[i];
						}
					}
				}
				pc += "<h1>Palcom Relay Control Panel</h1>";
				pc += "<table>";
				pc += "<tr>";
					// top bar
					pc += "<td><a href=\"logout.pal\">logout</a></td>";
				pc += "</tr>";
				pc += "<tr>";
					// main section
					pc += "<td>";
						// user / pass update form
						pc += "<h2>Update User</h2><br>";
						pc += "<form action=\"updateUser.pal\" method=\"POST\" >";
							pc += "<label>Username: </label><input name=\"username\" value=\"";pc += authFile.relayUsername; pc += "\" type=\"text\"/><br />";
							pc += "<label>Current Password: </label><input name=\"currentPassword\" value=\"\" type=\"password\" /><br />";
							pc += "<label>New Password: </label><input name=\"newPassword\" value=\"\" type=\"password\" /><br />";
							pc += "<label>Confirm Password: </label><input name=\"confPassword\" value=\"\" type=\"password\" /><br />";
							pc += "<input type=\"submit\" value=\"Update\" name=\"updateUser\" />";
						pc += "</form> <br><hr /><br />";

						// relay configuration form
						pc += "<h2>Relay Config</h2><br>";
						pc += "<form action=\"updateConfig.pal\" method=\"POST\">";
							pc += "<label>Relay IP: </label><input name=\"relayIP\" type=\"text\" value=\""; pc += rc.relayIP.c_str(); pc += "\" /><br>";
							pc += "<label>Relay Port: </label><input name=\"relayPort\" type=\"text\" value=\""; pc += rc.relayPort.c_str(); pc += "\" /></br>";
							pc += "<input type=\"submit\" value=\"Update\" name=\"updateRelay\" />";
						pc += "</form><br /><hr /><br />";

						// Factory Reset Form
						pc += "<h2>Factory Reset</h2>";
						pc += "<form action=\"reset.pal\" method=\"POST\">";
							pc += "<label>Admin Password: </label><input type=\"password\" name=\"password\" value=\"\" /><br />";
							pc += "<input type=\"submit\" value=\"Reset\" name=\"factoryReset\" />";
						pc += "</form>";
					pc += "</td>";
				pc += "</tr>";
				pc += "</table>";
			}
                        pc += "</body></html>";
			return pc;

		}
		int subContext = 0;

	public:
		MainPage(){
			this->setPageContent(this->createPage());
		}

		void finalizeMain(void){

		}

		int run(int context, HttpPacket httpPacket, AuthFile authFile){
			this->authFile = authFile;
			// Check for auth cookie
			bool authed = false;
			authCookie = "";
			username = false;
			password = false;

			for(int i=0; i<httpPacket.cookie_len; i++){
				if(httpPacket.cookie_key[i] == "auth"){
					authCookie = httpPacket.cookie_val[i];
					authed = this->validateAuthCookie(authCookie);
					break;
				}
			}

			if(!authed){
				for(int i=0; i<httpPacket.post_len; i++){
					 if(httpPacket.post_key[i] == "username"){
						if(httpPacket.post_val[i] == authFile.relayUsername){
							username = true;
						}
					 }
	
					 if(httpPacket.post_key[i] == "password"){
						if(httpPacket.post_val[i] == authFile.relayPassword){
                        	                        password = true;
                        	                }
                        	         }
				}

				if(username && password){
					Serial.printf("Generating Cookie\n");
					this->authCookie = this->generateAuthCookie();
				}
			}else{ // Authenticated Context.
				if(httpPacket.uri == "/reset.pal"){
					for(int i=0; i<httpPacket.post_len; i++){
                        		        if(httpPacket.post_key[i] == "password"){
							if(httpPacket.post_val[i] == authFile.relayPassword){
								PalcomFS pfs;
								pfs.rm("/");
								esp_restart();
							}
                        		                break;
                        		        }
                        		}
				}else  if(httpPacket.uri == "/updateUser.pal"){
					String currentPass = "";
					String usr = "";
					String newPass = "";
					String confPass = "";
					for(int i=0; i<httpPacket.post_len; i++){
                                                if(httpPacket.post_key[i] == "currentPassword"){
							currentPass = httpPacket.post_val[i];
                                                }else if(httpPacket.post_key[i] == "username"){
							usr = httpPacket.post_val[i];
						}else if(httpPacket.post_key[i] == "newPassword"){
							newPass = httpPacket.post_val[i];
						}else if(httpPacket.post_key[i] = "confPassword"){
							confPass = httpPacket.post_val[i];
						}
                                        }

					if(currentPass == authFile.relayPassword){
						PalcomFS pfs;
                                		if(pfs.exists(pfs_relayAuth)){
                                		        Serial.printf("Removing old setup file.\n");
                                		        pfs.rm(pfs_relayAuth);
                                		}

                                		String data = authFile.ssid; data += "\n";
                                		data += authFile.ssidPassword; data += "\n";
                                		data += usr; data += "\n";
						if(newPass != "" && (newPass == confPass)){
							data += newPass; data += "\n";
						}else{
                                			data += authFile.relayPassword; data += "\n";
						}

                                		pfs.fd = SD.open(pfs_relayAuth, FILE_WRITE);
                                		pfs.write((unsigned char *)data.c_str(), data.length());
                                		pfs.close();
					}
				}else if(httpPacket.uri == "/updateConfig.pal"){
					RelayConfig rc;
					for(int i=0; i<httpPacket.post_len; i++){
                                                if(httpPacket.post_key[i] == "relayIP"){
                                                        rc.relayIP = httpPacket.post_val[i];
                                                }else if(httpPacket.post_key[i] == "relayPort"){
                                                        rc.relayPort = httpPacket.post_val[i];
                                                }
                                        }
					PalcomFS pfs;
					if(pfs.exists(pfs_relayConfig)){
						pfs.rm(pfs_relayConfig);
					}

					String data = rc.relayIP; data += "\n";
					data += rc.relayPort; data += "\n";
					pfs.fd = SD.open(pfs_relayConfig, FILE_WRITE);
                                        pfs.write((unsigned char *)data.c_str(), data.length());
                                        pfs.close();
				}
			}

			this->setPageContent(this->createPage());
			return context;
		}	
};
