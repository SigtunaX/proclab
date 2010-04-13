// ZNL.cpp: implementation of the ZNT class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"
#ifdef TEXTURE_ENGINE
//#include "textgen.h"
#endif
#include "ZNT.h"
#include "ZNL.h"
#ifdef TEXTURE_LIBRARY_EDITOR
#include "../file.h"
#endif


//////////////////////////////////////////////////////////////////////
#ifdef TEXTURE_ENGINE	// En el engine no hi ha destructor, ni path (filename)!
//////////////////////////////////////////////////////////////////////
ZNL::ZNL(CTextGenLib &texturegenlib)
{
	tglib = &texturegenlib;
	data_size=0;
}
#endif


//////////////////////////////////////////////////////////////////////
#ifdef TEXTURE_LIBRARY_EDITOR
//////////////////////////////////////////////////////////////////////
ZNL::ZNL(char file[], CTextGenLib &texturegenlib)
{
	tglib = &texturegenlib;
	strcpy(filename, file);
	data_size=0;
	data = NULL;
	checkHeader = true;
}


ZNL::~ZNL()
{
	if (data!=NULL)
		free(data);
}


void ZNL::GenDataArray()
{
	int cnt = 0;
	if (data)
	{
		free(data);
		data = NULL;
	}
	data = (unsigned char*) malloc (sizeof (unsigned char) * 2048);
	
	data[data_size++] = 'L';					// Descripció arxiu
	data[data_size++] = ZNL_VERSION;			// Versio
	data[data_size++] = tglib->dtexgensize;	// Cantitat de textures

	for (int i=0; i<tglib->dtexgensize; i++)
	{
		ZNT znt("", tglib->dtexgen[i]);
		znt.GenDataArray();
		for (cnt = 2; cnt<znt.data_size; cnt++) // Els primers 2 bytes no es copiem
		{
			data[data_size++] = znt.data[cnt];
		}

	}
}

///////////////////
// Save texture
bool ZNL::SaveFile()
{
	if ((fd=fopen (filename,"wb"))==NULL)
		return false;
	else
	{
		GenDataArray();
		for (int i=0; i<data_size; i++)
			fwrite (&data[i], sizeof(unsigned char), 1, fd);
		fclose(fd);
		return true;
	}
}


///////////////////
// Load texture
char ZNL::LoadFile()
{
	if ((file_size=file2char (filename, data))==-1)
		return false;
	else
	{
		data_size = 0;
		return LoadProcess();	// Llegim les textures del string
	}
}

//////////////////////////////////////////////////////////////////////
#endif // TEXTURE_EDITOR
//////////////////////////////////////////////////////////////////////

void ZNL::LoadOneTex (int tex_id)
{
	unsigned char *tg_buf;	// Buffer de la textura

#ifdef TEXTURE_LIBRARY_EDITOR
	ZNT znt("", (tglib->dtexgen[tex_id]));
#endif
#ifdef TEXTURE_ENGINE
	ZNT znt(tglib->dtexgen[tex_id]);
#endif
	

	tg_buf = (unsigned char*) malloc(1024);

	// Copiem el buffer temporal
	for (int j=0; j<(file_size-data_size); j++)
		tg_buf[j] = data[j+data_size];

	znt.LoadMem(tg_buf, false); //if TEXTURE_LIBRARY_EDITOR (ZNL) -> checkheader=false
	data_size+=znt.data_size;
	// NOTA: No fa falta fer el free() de tg_buf, ja que esta "linkat" amb el znt.data, i el propi destructor del ZNT ja fa el free (probat!)
}

int ZNL::LoadProcess()
{
	int msg=1;
#ifdef TEXTURE_LIBRARY_EDITOR
	msg=LoadHeader();
	if (msg<=0)
	{
		return msg;
	}
#endif
#ifdef TEXTURE_ENGINE
	LoadHeader();
#endif
	for (int i=0; i<tglib->dtexgensize; i++)
		LoadOneTex (i);
	return msg;
}

//////////////////////////////////////////////////////////////////////
#ifdef TEXTURE_LIBRARY_EDITOR
//////////////////////////////////////////////////////////////////////
char ZNL::LoadHeader()
{
	char msg = 1;

	if (checkHeader)
		msg = LoadCheckHeader();
	if (msg)
	{
		tglib->dtexgensize = data[data_size++];	// Number of textures
		tglib->Free();
		for (int i=0; i<tglib->dtexgensize; i++)
		{
			CTextGen *newtext = new CTextGen();
			tglib->dtexgen.push_back(*newtext);
		}
	}
	return msg;
}

// Checks if header is valid
char ZNL::LoadCheckHeader()
{
	if (data[data_size++] != 'L')	// Invalid file
		return -2;
	if (data[data_size++] != ZNL_VERSION)	// Invalid version
		return -1;
	return 1;
}
//////////////////////////////////////////////////////////////////////
#endif // TEXTURE_EDITOR
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#ifdef TEXTURE_ENGINE
//////////////////////////////////////////////////////////////////////
void ZNL::LoadHeader()
{
	tglib->dtexgensize = data[data_size++];	// Number of textures
	tglib->Free();
	tglib->dtexgen = new CTextGen[tglib->dtexgensize];
}
//////////////////////////////////////////////////////////////////////
#endif // ENGINE
//////////////////////////////////////////////////////////////////////


// Loads a texture from memory buffer
void ZNL::LoadMem(unsigned char* &buffer, unsigned short fileSize) // TODO: Es pot treure el fileSize??
{
	data = buffer;
	file_size = fileSize;
	LoadProcess(); 			// Llegim les textures del string i gravem el nou tamany del file_size
}

