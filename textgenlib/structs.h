// Tipus de textures
#define ZN_MODULATE			0
#define	ZN_DECAL			1
#define ZN_BLEND			2

#define FONT_MAX 256

//#ifdef TEXTURE_EDITOR	// En el engine no fa falta definirla, ja està a un altre lloc
typedef struct tagTEXTURE
{
	unsigned int	tex_ID_OGL;		// Nombre de textura (a OpenGL)
	unsigned short 	w, h;			// Amplada i Llargada
	unsigned char	*data;			// Contingut de la textura
	unsigned char	param;			// Paràmetres: ZN_MODULATE, ZN_DECAL i ZN_BLEND
	unsigned char	iformat;		// Components de color (3,4...)
	int				format;			// Format (RGB, RGBA,...)
	unsigned char	filter;			// Activar filtre?
	unsigned char	loaded_OGL;		// La textura esta carregada ja en memòria de OpenGL?
} TEXTURE;


typedef struct tagUC_COLOR3 {
	unsigned char R,G,B;
} UC_COLOR3;
//#endif

typedef struct tagT_BLOB {
	UC_COLOR3		c;
	unsigned char	inv,mad,zoom,madvalor;
} T_BLOB;

typedef struct tagT_PLAIN {
	UC_COLOR3		c;
} T_PLAIN;

typedef struct tagT_NOISE {
	UC_COLOR3		c;
} T_NOISE;

typedef struct tagT_PERLIN {
	UC_COLOR3		c;
	unsigned char	zoom;
	unsigned char	persistence;
	unsigned char	octaves;
} T_PERLIN;

typedef struct tagT_CELULAR {
	unsigned char	type;
	UC_COLOR3		c,s;
} T_CELULAR;

typedef struct tagT_PLASMA {
	unsigned short	posX,posY;
	unsigned short	tam1,tam2,tam3,tam4;
	unsigned short	seed;
		
	unsigned char	zoom;
	UC_COLOR3		c,s;

} T_PLASMA;

typedef struct tagT_LINE {
	UC_COLOR3		c,s;
	unsigned char	rot1,rot2;
	unsigned char	n1,n2;
} T_LINE;

typedef struct tagT_FONT_PARAMS {
	UC_COLOR3		c;
	unsigned char	posX,posY;	// Text position
	unsigned char	type;		// Font type
	unsigned char	size;		// Font size
	unsigned char	numletters;
} T_FONT_PARAMS;

typedef struct tagT_FONT {
	T_FONT_PARAMS	p;
	char			text[FONT_MAX];
} T_FONT;

typedef struct tagT_XOR {
	UC_COLOR3		c;
} T_XOR;

typedef struct tagT_EFF_CUSTOM {
	char			filter[9];
	unsigned char	cfactor;
	unsigned char	bias;
} T_EFF_CUSTOM;

typedef struct tagT_EFF_COLOR {
	UC_COLOR3		c,s;
} T_EFF_COLOR;

typedef struct tagT_EFF_GRADBLUR {
	unsigned char	steps;
    unsigned short  stepX;
    unsigned short  stepY;
} T_EFF_GRADBLUR;

