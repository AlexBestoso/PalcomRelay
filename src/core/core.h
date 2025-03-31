#define CORE_ROUTING_KEY_SIZE 32

const uint8_t CORE_ROUTING_KEY[CORE_ROUTING_KEY_SIZE]= {
        162, 232, 0xff, 0x3a, 0x69, 0x22, 0xff, 0x82, 0x43 , 0x87,

        0x23, 0xe8, 0x18, 0x08, 0x17, 0x76, 0x6E, 0x69, 0x67, 0x67,

        0x65, 0x72, 0x10, 0x3a, 0x4a, 0x50, 0x03, 0x18, 0x07, 0x21,
        145, 33
};
class PalcomCore{
	private:
		PalcomInit initer;
		CoreGraphics subCoreGraphics;
		CoreComms subCoreComms;
		CoreStorage subCoreStorage;
		Cryptography *cryptography;
		
	public:
		PalcomCore(void);
		PalcomCore(Cryptography *crypto);
		
		void startCore(void);
		void driveCore(void);
};
