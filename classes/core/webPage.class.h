class WebPage{
	private:
		String pageContent = "";
	public:
		void setPageContent(String v){
			this->pageContent = v;
		}

		String getPageContent(void){
			return this->pageContent;
		}

		String getResponseHeader(void){
			 String ret = "HTTP/1.1 200 OK\r\n";
                         ret += "Content-type:text/html\r\n";
                         ret += "\r\n";
			 return ret;
		}
};
