class MainPage : public WebPage{
	private:
		String createPage(){
			String pc = "<html><body>";
				pc += "<h1>Palcom Relay Login</h1>";
                        pc += "</body></html>";
			return pc;

		}
		int subContext = 0;

	public:
		MainPage(){
			this->setPageContent(this->createPage());
		}

		void finalizeSetup(){
		}

		int run(int context, HttpPacket httpPacket){

			this->setPageContent(this->createPage());
			return context;
		}	
};
