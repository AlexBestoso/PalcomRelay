class CryptographyError : public CoreException{
	private:

	public:
		CryptographyError(const char *function, const char *msg, int errorCode);
		CryptographyError(const CryptographyError& e, String fName, String msg, uint8_t errorCode);
};
