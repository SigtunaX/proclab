/////////////////////////////////////////////
// ZNME :: Zona Neutra Mini Engine
//         Xphere
//         TEXTURE GENERATION
/////////////////////////////////////////////


#include "main.h"
#ifdef ENGINE
//#include "textgen.h"
//#include "textgenlib.h"
#endif
#include "textlib.h"

CTextGenLib::CTextGenLib()
{
	dtexgensize = 0;
#ifdef ENGINE
	dtexgen=NULL;
#endif
}


// Borrem tota la info emmagatzemada
CTextGenLib::~CTextGenLib()
{
	Free();
}


void CTextGenLib::Free ()
{
#ifdef TEXTURE_EDITOR
	dtexgen.clear();
#endif
#ifdef ENGINE
	if (dtexgen)
	{
		//free(dtexgen);
		delete [] dtexgen;
		dtexgen=NULL;
	}
	size=0;
#endif
}

// Regenera tota la llibrería
void CTextGenLib::Regenerate ()
{
	int i;
	for (i=0; i<dtexgensize; i++)
	{
		dtexgen[i].Regenerate();
	}
}
