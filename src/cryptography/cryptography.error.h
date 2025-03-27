class CryptographyError : public CoreException{
	private:

	public:
		CryptographyError(const char *function, const char *msg, int errorCode);
};
