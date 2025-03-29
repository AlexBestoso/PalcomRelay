class AesError : public CoreException{
	private:

	public:
		AesError(const char *function, const char *msg, int errorCode);
		AesError(const AesError &e, const char *function, const char *msg, int errorCode);
};
