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
					pc += "<label>Usernmae: </label><input name=\"username\" type=\"text\" value=\"\" /><br>";
					pc += "<label>Password: </label><input name=\"password\" type=\"password\" value=\"\" />";
					pc += "<input name=\"login\" type=\"submit\" value=\"login\" />";
				pc += "</form>";
			}else{
				pc += "<h1>AUTHENTICATED</h1>";
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
				Serial.printf("User is authenticated.\n");
			}

			this->setPageContent(this->createPage());
			return context;
		}	
};
