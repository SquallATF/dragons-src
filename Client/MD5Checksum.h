#ifndef MD5Checksum_H
#define MD5Checksum_H

class CMD5Checksum {
public:
	//interface functions for the RSA MD5 calculation
	static unsigned char *GetMD5(BYTE* pBuf, UINT nLength);
	static unsigned char *GetMD5(FILE *File);
	static unsigned char *GetMD5(const char *strFilePath);

protected:
	//constructor/destructor
	CMD5Checksum();
	virtual ~CMD5Checksum() {};

	//RSA MD5 implementation
	void Transform(BYTE Block[64]);
	void Update(BYTE* Input, ULONG nInputLen);
	unsigned char *Final();
	inline DWORD RotateLeft(DWORD x, int n);
	inline void FF(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void GG(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void HH(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);
	inline void II(DWORD& A, DWORD B, DWORD C, DWORD D, DWORD X, DWORD S, DWORD T);

	//utility functions
	void DWordToByte(BYTE* Output, DWORD* Input, UINT nLength);
	void ByteToDWord(DWORD* Output, BYTE* Input, UINT nLength);

private:
	BYTE  m_lpszBuffer[64];  //input buffer
	ULONG m_nCount[2];   //number of bits, modulo 2^64 (lsb first)
	ULONG m_lMD5[4];   //MD5 checksum
};

#endif // MD5Checksum_H ///:~