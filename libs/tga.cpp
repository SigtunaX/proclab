// TGA.cpp: implementation of the TGA class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#include "TGA.h"

// TGA Header: 18 bytes
/*
typedef struct {
   char  idlength;
   char  colourmaptype;
   char  datatypecode;
   short int colourmaporigin;
   short int colourmaplength;
   char  colourmapdepth;
   short int x_origin;
   short int y_origin;
   short width;
   short height;
   char  bitsperpixel;
   char  imagedescriptor;
} TGA_HEADER;

typedef struct tagPIXEL{
   unsigned char r,g,b,a;
} PIXEL;
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TGA::TGA(const char file[])
{
	strcpy(filename, file);
}

TGA::~TGA()
{

}

int TGA::Save(int width, int height, unsigned char* &data)
{
	FILE			*fptr;			// Open file pointer
	int i,j;
	int cnt=0;
   
	// Write the result as a uncompressed TGA
	if ((fptr = fopen(filename,"wb")) == NULL)
		return(-1);
	putc(0,fptr);
	putc(0,fptr);
	putc(2,fptr);							// uncompressed RGB
	putc(0,fptr); putc(0,fptr);
	putc(0,fptr); putc(0,fptr);
	putc(0,fptr);
	putc(0,fptr); putc(0,fptr);				// X origin
	putc(0,fptr); putc(0,fptr);				// y origin
	putc((width & 0x00FF),fptr);
	putc((width & 0xFF00) / 256,fptr);
	putc((height & 0x00FF),fptr);
	putc((height & 0xFF00) / 256,fptr);
	putc(24,fptr);							// 24 bit bitmap
//	putc(32,fptr);							// Si afegim component alpha...
	putc(0,fptr);

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			putc(data[cnt+2],fptr);		// B
			putc(data[cnt+1],fptr);		// G
			putc(data[cnt  ],fptr);		// R
//			putc(255,fptr);				// A
			cnt+=3;
		}
	}
	fclose(fptr);	
	return 0;
}
