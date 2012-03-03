// ZNT.h: interface for the ZNT class.
//
//////////////////////////////////////////////////////////////////////

// File version
#define ZNT_VERSION		16

class ZNT  
{
public:
#ifdef TEXTURE_EDITOR
	ZNT(char file[], CTextGen &texture);
	virtual ~ZNT();
	char LoadFile(char checkHeader);
	bool SaveFile();
	void GenDataArray();
#endif

#ifdef TEXTURE_ENGINE
	ZNT(CTextGen &texture);
#endif
	void LoadMem(unsigned char* &buffer, char checkHeader);
	unsigned short data_size;	// Size of the data to save
	unsigned char *data;		// This is the data of the file to save/read
	char checkHeader;	// Checks headers, set this to false, if our file does not have the first 2 bytes

private:
	CTextGen *t;

#ifdef TEXTURE_EDITOR
	void SaveProcess();
	void SaveHeader();
	void SaveBody(int numtex);
	void SaveTexBody(int numtex);
	void SaveEfxBody(int numtex);

	void SavePlain(int numtex);
	void SaveNoise(int numtex);
	void SavePerlin(int numtex);
	void SaveCelular(int numtex);
	void SavePlasma(int numtex);
	void SaveBlob(int numtex);
	void SaveLine(int numtex);
	void SaveText(int numtex);
	void SaveXor(int numtex);
	void SaveEffCustom(int numtex);
	void SaveEffColor(int numtex);
	void SaveEffGradBlur(int numtex);
    
#endif

	char LoadProcess();
#ifdef TEXTURE_EDITOR
	char LoadCheckHeader();
	char LoadHeader();
#endif
#ifdef TEXTURE_ENGINE
	void LoadHeader();
#endif
	void LoadBody(int numtex);
	void LoadTexBody(int numtex);
	void LoadEfxBody(int numtex);

	void LoadPlain(int numtex);
	void LoadNoise(int numtex);
	void LoadPerlin(int numtex);
	void LoadCelular(int numtex);
	void LoadPlasma(int numtex);
	void LoadBlob(int numtex);
	void LoadLine(int numtex);
	void LoadText(int numtex);
	void LoadXor(int numtex);
	void LoadEffCustom(int numtex);
	void LoadEffColor(int numtex);
	void LoadEffGradBlur(int numtex);
    
#ifdef TEXTURE_EDITOR
protected:
	FILE *fd;
	char filename[1024];
#endif
};

