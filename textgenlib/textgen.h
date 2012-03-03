/////////////////////////////////////////////
// ZNME :: Zona Neutra Mini Engine
//         Xphere
//         TEXTURE GENERATION
/////////////////////////////////////////////

/////////////////
// Classes:
//		COneTextGen:	Una textura
//		CTextGen:		Un grup de textures (aka, tipus mescla)



class COneTextGen  
{
public:
	COneTextGen();
	virtual			~COneTextGen() {};
	COneTextGen &operator= (const COneTextGen &ot); 
	void			Init();
	void			Regenerate (TEXTURE &t, unsigned char* &data);		// Regenerem la textura i ho posem a *data
	void			ApplyEffect (TEXTURE &t, unsigned char* &data);

#ifdef TEXTURE_EDITOR
	char			name[32];		// Name of the Texture
#endif
	unsigned char	type;			// Tipus
	unsigned char	operation;
	unsigned char	opacity;

	T_BLOB          blob;
	T_PLAIN         plain;
	T_NOISE         noise;
	T_PERLIN        perlin;
	T_CELULAR       celular;
	T_PLASMA        plasma;
	T_LINE          line;
	T_FONT          font;
	T_XOR           t_xor;
	T_EFF_CUSTOM    eff_cust;
    T_EFF_COLOR     eff_color;
    T_EFF_GRADBLUR  eff_gradblur;
};


#ifdef TEXTURE_EDITOR
	typedef	deque<COneTextGen> dCOneTextGen;
#endif
#ifdef TEXTURE_ENGINE
	typedef COneTextGen*		dCOneTextGen;
#endif


class CTextGen
{
public:
	CTextGen();
	virtual	~CTextGen();
	CTextGen &operator= (const CTextGen &ct); 
	void	Init();
	void	Free();
	void	Regenerate ();			// Regenerem la textura i es posa a *data
#ifdef TEXTURE_EDITOR
	void SaveRAW(char file[]);
#endif
	dCOneTextGen		dtex;		// Grup de textures que conformen la mescla
	int					dtexsize;
	TEXTURE				t;			// Conté la info preparada per pujarla al OpenGL (tambe hi ha la data final)
private:
	int operation (int oper, unsigned char val1, unsigned char val2);
};

