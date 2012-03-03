// ZNT.cpp: implementation of the ZNT class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"
#include "znt.h"
#ifdef TEXTURE_EDITOR
#include "../file.h"
#endif



//////////////////////////////////////////////////////////////////////
#ifdef TEXTURE_ENGINE	// En el engine no hi ha destructor, ni path (filename)!
//////////////////////////////////////////////////////////////////////
ZNT::ZNT(CTextGen &texture)
{
	t = &texture;
	data_size=0;
}
#endif


//////////////////////////////////////////////////////////////////////
#ifdef TEXTURE_EDITOR
//////////////////////////////////////////////////////////////////////
ZNT::ZNT(char file[], CTextGen &texture)
{
	t = &texture;
	strcpy(filename, file);
	data_size=0;
	data = NULL;
}

ZNT::~ZNT()
{
	if (data!=NULL)	// Potser no fa ni falta.... ens estalviariem 1 o 2 ks en memòria, però és mes codi! :D
		free(data);
}


// Generate data array
void ZNT::GenDataArray ()
{
	data = (unsigned char*) calloc (1, 2048);	// A little cutre :P
	SaveProcess();								// Guardem les textures al string
}
///////////////////
// Save texture
bool ZNT::SaveFile()
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

void ZNT::SaveProcess()
{
	SaveHeader();
	for (int i=0; i<t->dtexsize; i++)
	{
		SaveBody(i);
		if (t->dtex[i].type<100)
			SaveTexBody (i);
		else
			SaveEfxBody (i);

		switch (t->dtex[i].type)
		{
			case 0: SavePlain(i);           break;
			case 1: SaveNoise(i);           break;
			case 2: SavePerlin(i);          break;
			case 3: SaveCelular(i);         break;
			case 4: SavePlasma(i);          break;
			case 5: SaveBlob(i);            break;
			case 6:	SaveLine(i);            break;
			case 7: SaveText(i);            break;
			case 8: SaveXor(i);             break;
			case 150: SaveEffCustom(i);     break;
            case 151: SaveEffColor(i);      break;
            case 152: SaveEffGradBlur(i);   break;
		}
	}
}

void ZNT::SaveHeader(void)
{
	unsigned char width;
	unsigned char height;

	switch (t->t.w)
	{
		case  32: width=5; break;
		case  64: width=6; break;
		case 128: width=7; break;
		case 256: width=8; break;
		case 512: width=9; break;
		default: width=8; break;
	}
	switch (t->t.h)
	{
		case  32: height=5; break;
		case  64: height=6; break;
		case 128: height=7; break;
		case 256: height=8; break;
		case 512: height=9; break;
		default: height=8; break;
	}

	data[data_size++] = 'Z';			// Header
	data[data_size++] = ZNT_VERSION;	// Version
	data[data_size++] = width;
	data[data_size++] = height;
	data[data_size++] = t->dtexsize;	// Number of textures
}

// Generic Body
void ZNT::SaveBody(int numtex)
{
	data[data_size++] = t->dtex[numtex].type;
}

// Texture Body
void ZNT::SaveTexBody(int numtex)
{
	data[data_size++] = t->dtex[numtex].opacity;
	data[data_size++] = t->dtex[numtex].operation;
}

// Effects Body
void ZNT::SaveEfxBody(int numtex)
{
}


void ZNT::SavePlain(int numtex)
{
	*((T_PLAIN *)(data+data_size)) = t->dtex[numtex].plain;
	data_size += sizeof (T_PLAIN);
}

void ZNT::SaveNoise(int numtex)
{
	*((T_NOISE *)(data+data_size)) = t->dtex[numtex].noise;
	data_size += sizeof (T_NOISE);
}

void ZNT::SavePerlin(int numtex)
{
	*((T_PERLIN *)(data+data_size)) = t->dtex[numtex].perlin;
	data_size += sizeof (T_PERLIN);
}

void ZNT::SaveCelular(int numtex)
{
	*((T_CELULAR *)(data+data_size)) = t->dtex[numtex].celular;
	data_size += sizeof (T_CELULAR);
}


void ZNT::SavePlasma(int numtex)
{
	*((T_PLASMA *)(data+data_size)) = t->dtex[numtex].plasma;
	data_size += sizeof (T_PLASMA);
}


void ZNT::SaveBlob(int numtex)
{
	*((T_BLOB *)(data+data_size)) = t->dtex[numtex].blob;
	data_size += sizeof (T_BLOB);
}


void ZNT::SaveLine(int numtex)
{
	*((T_LINE *)(data+data_size)) = t->dtex[numtex].line;
	data_size += sizeof (T_LINE);

}


void ZNT::SaveText(int numtex)
{
	*((T_FONT_PARAMS *)(data+data_size)) = t->dtex[numtex].font.p;
	data_size += sizeof(T_FONT_PARAMS);;	// 8 bytes mes el que ocupi la font

	for (int i=0; i<(t->dtex[numtex].font.p.numletters-1); i++)	// "-1" ja que el ultim byte sempre es el mateix, el terminador '\0', i no el posem
		data[data_size++] = t->dtex[numtex].font.text[i];
}

void ZNT::SaveXor(int numtex)
{
	*((T_XOR *)(data+data_size)) = t->dtex[numtex].t_xor;
	data_size += sizeof (T_XOR);
}

void ZNT::SaveEffCustom(int numtex)
{
	*((T_EFF_CUSTOM *)(data+data_size)) = t->dtex[numtex].eff_cust;
	data_size += sizeof (T_EFF_CUSTOM);
}

void ZNT::SaveEffColor(int numtex)
{
	*((T_EFF_COLOR *)(data+data_size)) = t->dtex[numtex].eff_color;
	data_size += sizeof (T_EFF_COLOR);
}

void ZNT::SaveEffGradBlur(int numtex)
{
	*((T_EFF_GRADBLUR *)(data+data_size)) = t->dtex[numtex].eff_gradblur;
	data_size += sizeof (T_EFF_GRADBLUR);
}

///////////////////
// Load texture
char ZNT::LoadFile(char checkHeader)
{
	char msg = -1;
	this->checkHeader = checkHeader;
	if ((data_size=(unsigned short)file2char (filename, data))==-1)
		return msg;
	else
	{
		data_size = 0;
		msg = LoadProcess();	// Llegim les textures del string
	}
	return msg;
}

//////////////////////////////////////////////////////////////////////
#endif // TEXTURE_EDITOR
//////////////////////////////////////////////////////////////////////

// Loads a texture from memory buffer
void ZNT::LoadMem(unsigned char* &buffer, char checkHeader)
{
	this->checkHeader = checkHeader;
	data = buffer;
	LoadProcess();	// Llegim les textures del string
}

char ZNT::LoadProcess()
{
	char msg=1;

#ifdef TEXTURE_EDITOR
	msg=LoadHeader();
#endif
#ifdef TEXTURE_ENGINE
	LoadHeader();
#endif
	if (msg==1)
	{
		for (int i=0; i<t->dtexsize; i++)
		{
			LoadBody(i);
			if (t->dtex[i].type<100)
				LoadTexBody (i);
			else
				LoadEfxBody (i);

			switch (t->dtex[i].type)
			{
				case 0: LoadPlain(i);           break;
				case 1: LoadNoise(i);           break;
				case 2: LoadPerlin(i);          break;
				case 3: LoadCelular(i);         break;
				case 4: LoadPlasma(i);          break;
				case 5: LoadBlob(i);            break;
				case 6:	LoadLine(i);            break;
				case 7: LoadText(i);            break;
				case 8: LoadXor(i);             break;
				case 150: LoadEffCustom(i);     break;
				case 151: LoadEffColor(i);      break;
				case 152: LoadEffGradBlur(i);	break;
            }
		}
	}
	return msg;
}

#ifdef TEXTURE_EDITOR
// Checks if header is valid
char ZNT::LoadCheckHeader(void)
{
	if (data[data_size++] != 'Z')	// Invalid file
		return -2;
	if (data[data_size++] != ZNT_VERSION)	// Invalid version
		return -1;
	return 1;
}

char ZNT::LoadHeader(void)
{
	char msg = 1;
	int width;
	int height;

	if (checkHeader)
		msg = LoadCheckHeader();
	if (msg==1)
	{
		switch (data[data_size++])	// W
		{
			case  5: width=32; break;
			case  6: width=64; break;
			case  7: width=128; break;
			case  8: width=256; break;
			case  9: width=512; break;
			default: width=256; break;
		}
		switch (data[data_size++])	// H
		{
			case  5: height=32; break;
			case  6: height=64; break;
			case  7: height=128; break;
			case  8: height=256; break;
			case  9: height=512; break;
			default: height=256; break;
		}
		//width	= (int) pow (2, data[data_size++];		// Substituir per lo de dalt.... pero s'ha d'incloure el math!
		//height	= (int) pow (2, data[data_size++];
		t->Init();
		t->dtexsize = data[data_size++];	// Number of textures
		for (int i=0; i<t->dtexsize; i++)
		{
			COneTextGen *newtext = new COneTextGen();
			t->dtex.push_back(*newtext);
		}
		t->t.w = width;
		t->t.h = height;
	}
	return msg;
}

#endif

#ifdef TEXTURE_ENGINE
// It is the same as LoadHeader, but without reading the first 2 bytes ('Z' and version)
void ZNT::LoadHeader(void)
{
	unsigned short width;
	unsigned short height;
	COneTextGen cotg;
	switch (data[data_size++])	// W
	{
		case  5: width=32; break;
		case  6: width=64; break;
		case  7: width=128; break;
		case  8: width=256; break;
		case  9: width=512; break;
		default: width=256; break;
	}
	switch (data[data_size++])	// H
	{
		case  5: height=32; break;
		case  6: height=64; break;
		case  7: height=128; break;
		case  8: height=256; break;
		case  9: height=512; break;
		default: height=256; break;
	}
	//width	= (int) pow (2, data[data_size++];		// Substituir per lo de dalt.... pero s'ha d'incloure el math!
	//height	= (int) pow (2, data[data_size++];
	t->Init();
	t->dtexsize = data[data_size++];	// Number of textures
	t->dtex = new COneTextGen[t->dtexsize];
	t->t.w = width;
	t->t.h = height;
}
#endif

// Generic Body
void ZNT::LoadBody(int numtex)
{
	t->dtex[numtex].type = data[data_size++];
}

// Texture Body
void ZNT::LoadTexBody(int numtex)
{
	t->dtex[numtex].opacity	= data[data_size++];
	t->dtex[numtex].operation = data[data_size++];
}

// Effects Body
void ZNT::LoadEfxBody(int numtex)
{
}


void ZNT::LoadPlain(int numtex)
{
	t->dtex[numtex].plain = *((T_PLAIN *) (data+data_size));
	data_size += sizeof (T_PLAIN);
}

void ZNT::LoadNoise(int numtex)
{
	t->dtex[numtex].noise = *((T_NOISE *) (data+data_size));
	data_size += sizeof (T_NOISE);
}

void ZNT::LoadPerlin(int numtex)
{
	t->dtex[numtex].perlin = *((T_PERLIN *) (data+data_size));
	data_size += sizeof (T_PERLIN);
}

void ZNT::LoadCelular(int numtex)
{
	t->dtex[numtex].celular = *((T_CELULAR *) (data+data_size));
	data_size += sizeof (T_CELULAR);
}


void ZNT::LoadPlasma(int numtex)
{
	t->dtex[numtex].plasma = *((T_PLASMA *) (data+data_size));
	data_size += sizeof (T_PLASMA);
}


void ZNT::LoadBlob(int numtex)
{
	t->dtex[numtex].blob = *((T_BLOB *) (data+data_size));
	data_size += sizeof (T_BLOB);
}


void ZNT::LoadLine(int numtex)
{
	t->dtex[numtex].line = *((T_LINE *) (data+data_size));
	data_size += sizeof (T_LINE);
}


void ZNT::LoadText(int numtex)
{
	t->dtex[numtex].font.p = *((T_FONT_PARAMS *) (data+data_size));
	data_size += sizeof(T_FONT_PARAMS);
	int i;
	for (i=0; i<(t->dtex[numtex].font.p.numletters-1); i++)	// Posem el "-1" ja que el terminador no hi és
		t->dtex[numtex].font.text[i]	= data[data_size++];
	t->dtex[numtex].font.text[i] = '\0';	// Afegim el temrinador a mà

}

void ZNT::LoadXor(int numtex)
{
	t->dtex[numtex].t_xor = *((T_XOR *) (data+data_size));
	data_size += sizeof (T_XOR);
}

void ZNT::LoadEffCustom(int numtex)
{
	t->dtex[numtex].eff_cust = *((T_EFF_CUSTOM *)(data+data_size));
	data_size += sizeof (T_EFF_CUSTOM);
}

void ZNT::LoadEffColor(int numtex)
{
	t->dtex[numtex].eff_color = *((T_EFF_COLOR *)(data+data_size));
	data_size += sizeof (T_EFF_COLOR);
}

void ZNT::LoadEffGradBlur(int numtex)
{
	t->dtex[numtex].eff_gradblur = *((T_EFF_GRADBLUR *)(data+data_size));
	data_size += sizeof (T_EFF_GRADBLUR);
}

