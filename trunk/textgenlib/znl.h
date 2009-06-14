// ZNL.h: interface for the ZNL class.
//
//////////////////////////////////////////////////////////////////////

// File version
#define ZNL_VERSION		15

class ZNL
{
public:
#ifdef TEXTURE_LIBRARY_EDITOR
	ZNL(char file[], CTextGenLib &texturegenlib);
	virtual ~ZNL();
	char LoadFile();
	bool SaveFile();
	void GenDataArray();
	char checkHeader;	// Checks headers, set this to false, if our file does not have the first 2 bytes
#endif
#ifdef TEXTURE_ENGINE
	ZNL(CTextGenLib &texturegenlib);
#endif
	void LoadOneTex (int tex_id);
	void LoadMem(unsigned char* &buffer, unsigned short fileSize);
	unsigned short data_size;	// Size of the data to save
	unsigned short file_size;	// Size of the file readed
	unsigned char *data;		// This is the data of the file to save/read

private:
	CTextGenLib *tglib;

	int LoadProcess();
#ifdef TEXTURE_EDITOR
	char LoadCheckHeader();
	char LoadHeader();
#endif
#ifdef TEXTURE_ENGINE
	void LoadHeader();
#endif
#ifdef TEXTURE_LIBRARY_EDITOR
protected:
	FILE *fd;
	char filename[1024];
#endif

};

