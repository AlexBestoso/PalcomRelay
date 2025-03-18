class PalcomInit{
	private:
		bool serialOn;
	public:
		PalcomInit(void);

		void initSerial(void);
		void initRadio(void);
		void initDisplay(void);
		void initUsb(void);
		void initSettings(void); // node configurations stored in partitions.
/*			
	sample of the sd card init process.

		if (SDCARD_CS >  0) {
                                display.clear();
                                sdSPI.begin(14, 2, 11, 13);
                                if (!SD.begin(13, sdSPI)) {
                                        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "SDCard  FAIL");
                                } else {
                                        display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, "SDCard  PASS");
                                        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
                                        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Size: " + String(cardSize) + "MB");
                                }
                                display.display();
                                delay(2000);
                        }*/
};
