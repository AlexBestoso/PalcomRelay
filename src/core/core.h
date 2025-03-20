
class PalcomCore{
	private:
		PalcomInit initer;
		CoreGraphics subCoreGraphics;
		CoreComms subCoreComms;
		
	public:
		PalcomCore(void);
		
		void startCore(void);
		void driveCore(void);
};
