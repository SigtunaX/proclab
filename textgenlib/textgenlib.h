/////////////////////////////////////////////
// ZNME :: Zona Neutra Mini Engine
//         Xphere
//         TEXTURE GENERATION
/////////////////////////////////////////////

/////////////////
// Classes:
//		CTextGenLib:	TextureGen Library


#ifdef TEXTURE_EDITOR
	typedef	deque<CTextGen> dCTextGen;
#endif
#ifdef TEXTURE_ENGINE
	typedef CTextGen*		dCTextGen;
#endif

class CTextGenLib
{
public:
	CTextGenLib();
	virtual		~CTextGenLib();
	void		Free();
	void		Regenerate ();			// Regenerem la textura i es posa a *data
	dCTextGen	dtexgen;				// Llibrería de textures
	int			dtexgensize;
};