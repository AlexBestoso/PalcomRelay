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
		void initStorage(void);
};
