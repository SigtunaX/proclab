// TGA.h: interface for the TGA class.
//
//////////////////////////////////////////////////////////////////////

class TGA  
{
public:
	TGA(const char file[]);
	virtual ~TGA();
	int Save(int width, int height, unsigned char* &data);

protected:
	FILE *fd;
	char filename[1024];

};
