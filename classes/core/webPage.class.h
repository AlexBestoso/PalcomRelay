class WebPage{
	private:
		String pageContent = "";
		String cookie = "";
		bool sendCookie = false;
	public:

		void setSendCookie(bool v){
			sendCookie = v;
		}
		void setPageContent(String v){
			this->pageContent = v;
		}

		String getPageContent(void){
			return this->pageContent;
		}

		String getResponseHeader(void){
			 String ret = "HTTP/1.1 200 OK\r\n";
                         ret += "Content-type:text/html\r\n";
			 if(this->cookie != "" && sendCookie){
				 ret += "Set-Cookie: auth="; ret += this->cookie; ret += "\r\n";
				 sendCookie = false;
			 }
                         ret += "\r\n";
			 return ret;
		}

		String craftErrorText(String text){
			String ret = "<font color=\"red\">";
			ret += text;
			ret += "</font>";
			return ret;
		}

		String generateAuthCookie(void){
			String cookie = "";
			PalcomFS pfs;

			randomSeed(analogRead(0));
			char encoder[16] = {
				'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
			};
			for(int i=0; i<32; i++){
				cookie += encoder[(char)random(16)];
			}

			/*if(!pfs.exists(pfs_cookies))
				pfs.mkdir(pfs_cookies);
			sprintf((char *)compBuffer, "%s/%s", pfs_cookies, cookie.c_str());
			pfs.fd = SD.open(compBuffer, FILE_WRITE);
			pfs.fd.write((unsigned char *)cookie.c_str(), 32);
			pfs.close();*/

			this->cookie = cookie;
			this->sendCookie = true;

			return cookie;
		}

		bool validateAuthCookie(String cookie){
			if(cookie == "" || this->cookie == ""){
				return false;
			}
			return (cookie == this->cookie) ? true : false;
		}
};
