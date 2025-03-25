
class PalcomCore{
	private:
		PalcomInit initer;
		CoreGraphics subCoreGraphics;
		CoreComms subCoreComms;
		Cryptography *cryptography;
		
	public:
		PalcomCore(void);
		PalcomCore(Cryptography *crypto);
		
		void startCore(void);
		void driveCore(void);
};
