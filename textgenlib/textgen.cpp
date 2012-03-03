/////////////////////////////////////////////
// ZNME :: Zona Neutra Mini Engine
//         Xphere
//         TEXTURE GENERATION
/////////////////////////////////////////////

#include "main.h"
#include "textlib.h"
#include <math.h>

CTextGen::CTextGen()
{
	dtexsize = 0;
	t.data = NULL;
#ifdef ENGINE
	dtex = NULL;
#endif
	Init();

}


CTextGen::~CTextGen()
{
	Free();
}

CTextGen &CTextGen::operator= (const CTextGen &ct)
{
	Init();
	COneTextGen void_tex;

	int i;
	dtexsize = ct.dtexsize;
#ifdef TEXTURE_EDITOR
	for (i=0; i<dtexsize; i++)
	{
		dtex.push_back(void_tex);
		dtex[i] = ct.dtex[i];
	}
#endif
#ifdef ENGINE
	dtex = new COneTextGen[dtexsize];
	for (i=0; i<dtexsize; i++)
		dtex[i] = ct.dtex[i];
#endif
	// Copy TEXTURE structure
	t.tex_ID_OGL = ct.t.tex_ID_OGL;
	t.w = ct.t.w;
	t.h = ct.t.h;
	t.param = ct.t.param;
	t.iformat = ct.t.iformat;
	t.format = ct.t.format;
	t.filter = ct.t.filter;
	t.loaded_OGL = ct.t.loaded_OGL;
	
	for (i=0; i<t.w*t.h*t.iformat; i++)
		t.data[i] = ct.t.data[i];
	return *this;
}

// Borrem tota la info emmagatzemada
void CTextGen::Free ()
{
	dtexsize = 0;
	if (t.data)
	{
		free(t.data);
		t.data = NULL;
	}
	// Clear layers
#ifdef TEXTURE_EDITOR
	dtex.clear();
#endif
#ifdef ENGINE
	if (dtex)
	{
		//free (dtex);
		delete [] dtex;
		dtex = NULL;
	}
#endif

}

// Borrem tot, i Inicialitza el contingut de la textura
void CTextGen::Init ()
{
	Free();

	// Init TEXTURE
	t.w = t.h = 256;	//256x256
	t.tex_ID_OGL = t.filter = 0;
	t.format = GL_RGB;
	t.iformat = 3;
	t.param = ZN_MODULATE;
	t.loaded_OGL = false;

	// Posem data a '0' data s'ha borrat quan hem fet el Free()
	t.data = (unsigned char *) calloc (t.w*t.h*t.iformat, sizeof(unsigned char));
}

// Regenera el contingut de la textura segons la info de textura
// data: buffer final on genera la textura
// dtex: llista amb la informació necessaria per generar la textura
// Regenerate the texture and store the results to the buffer given
void CTextGen::Regenerate ()
{
	int i,j;
	int data_len;				// longitud del buffer
	unsigned char* tmp_data;	// Buffer temporal on anem guardant la textura generada de cada capa

	data_len = t.w*t.h*t.iformat;
	tmp_data = (unsigned char*) malloc (data_len);
	memset(t.data, 0, data_len);	// Resetejem la textura

	for (j=0; j<dtexsize; j++)
	{
		if (dtex[j].type<100)	// Si no és un efecte...
		{
			dtex[j].Regenerate(t, tmp_data);	// Regenerem la textura: guardem a "tmp_data" la textura generada
			for (i=0; i<data_len; i++)
				t.data[i] = operation (dtex[j].operation, t.data[i], tmp_data[i]);
		}
		else
			dtex[j].ApplyEffect(t, t.data);
		
	}
	free (tmp_data);

}



int CTextGen::operation (int oper, unsigned char val1, unsigned char val2)
{
	int value = 0;
	switch (oper)
	{			
	case 0:			//////////////////////////// SUMA
		value =	val1 + val2;
		break;
	case 1:			//////////////////////////// RESTA
		value = val1 - val2;
		break;
	case 2:			//////////////////////////// AVERAGE
		value = (int)((val1 + val2) / 2.0f);
		break;
	case 3:			//////////////////////////// MULT
		value = int(255 * (val2 / 255.0 * val1 / 255.0));
		break;
	case 4:			//////////////////////////// MIN
		value = min(val1, val2);
		break;
	case 5:			//////////////////////////// MAX
		value = max(val1, val2);
		break;
	case 6:			//////////////////////////// AMPLITUDE: Depends: math.h
		value = int(sqrt(int(val1 * val1 + val2 * val2)) / sqrt(2.0));;
		break;
	case 7:			//////////////////////////// AND
		value = val1 & val2;
		break;
	case 8:			//////////////////////////// OR
		value = val1 | val2;
		break;
	case 9:			//////////////////////////// XOR
		value = val1 ^ val2;
		break;
	
	}
	if (value>255)
		value=255;
	if (value<0)
		value=0;
	return value;
}

#ifdef TEXTURE_EDITOR
	void CTextGen::SaveRAW(char file[])
	{
		unsigned char *data_tmp;
		int x, y;
		int cnt = 0;
		FILE *fo;
		
		data_tmp = (unsigned char*) malloc (t.w*t.h*t.iformat);

		// Hem d'invertir les X's (per aixo usem el buffer temporal data_tmp)
		for (x=t.h-1; x>=0; x--)
		{
			for (y=0; y<t.w; y++)
			{
				data_tmp[x*(t.h*3)+(y*3)  ]=t.data[cnt++];
				data_tmp[x*(t.h*3)+(y*3)+1]=t.data[cnt++];
				data_tmp[x*(t.h*3)+(y*3)+2]=t.data[cnt++];
			}
		}
		fo=fopen(file,"wb");
		fwrite (data_tmp, sizeof (unsigned char), t.w*t.h*t.iformat, fo);
		fclose (fo);
		free (data_tmp);
	}
#endif

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COneTextGen::COneTextGen()
{
	Init();
}

void COneTextGen::Init()
{
	type=0;						// Per defecte, plain


	// Plain
	plain.c.R=plain.c.G=plain.c.B=0;
	// Noise
	noise.c.R=noise.c.G=noise.c.B=0;
	// Perlin
	perlin.c.R=perlin.c.G=perlin.c.B=0;
	perlin.zoom = 100;
	perlin.persistence = 200;
	perlin.octaves=8;
	// Cell
	celular.c.R=celular.c.G=celular.c.B=1;
	celular.s.R=celular.s.G=celular.s.B=0;
	celular.type=0;
	// Plasma
	plasma.s.R=plasma.s.G=plasma.s.B=0;
	plasma.c.R=plasma.c.G=plasma.c.B=0;
	plasma.seed=1;
	plasma.tam1=plasma.tam2=plasma.tam3=plasma.tam4=16;
	plasma.posX=plasma.posY=0;
	plasma.zoom=1;
	// Blob
	blob.c.R=blob.c.G=blob.c.B=0;
	blob.inv=0;
	blob.mad=0;
	blob.madvalor=1;
	//blob.zoom=t.width/2;
	blob.zoom=128;//TODO: Canviat pq ja no tenim width aqui

	// Line
	line.c.R=line.c.G=line.c.B=0;
	line.s.R=line.s.G=line.s.B=0;
	line.rot1 = line.rot2 = 0;
	line.n1 = line.n2 = 1;
    
	// Font
	font.p.c.R=font.p.c.G=font.p.c.B=255;
	font.p.posX = font.p.posY = 0;
	font.p.type = 0;
	font.p.size = 50;
	font.p.numletters=0;
	font.text[0] = '\0';


	// Xor
	t_xor.c.R=t_xor.c.G=t_xor.c.B=0;

	// Custom filter
	eff_cust.filter[0] = eff_cust.filter[1] = eff_cust.filter[2] = eff_cust.filter[3] = eff_cust.filter[4] = eff_cust.filter[5] = eff_cust.filter[6] = eff_cust.filter[7] = eff_cust.filter[8] = 1;
	eff_cust.bias = 0;
	eff_cust.cfactor = 9;
    
    // Colorize
    eff_color.c.R = eff_color.c.G = eff_color.c.B = 10;
    eff_color.s.R = eff_color.s.G = eff_color.s.B = 200;
    
    // Gradient Blur
    eff_gradblur.steps = 50;
    eff_gradblur.stepX = eff_gradblur.stepY = 256;

	// Nom, opacitat i operacio
#ifdef TEXTURE_EDITOR
	strcpy(name,"");
#endif
	opacity	=	255;
	operation	=	0;
}


COneTextGen &COneTextGen::operator= (const COneTextGen &ot)
{
//	Init();

#ifdef TEXTURE_EDITOR
	strcpy (name, ot.name);
#endif
	type = ot.type;
	operation = ot.operation;
	opacity = ot.opacity;

	blob = ot.blob;
	plain = ot.plain;
	noise = ot.noise;
	perlin = ot.perlin;
	celular = ot.celular;
	plasma = ot.plasma;
	line = ot.line;
	font.p = ot.font.p;
	strcpy (font.text, ot.font.text);
//	for (int i=0; i<font.p.numletters; i++)
//		font.text[i] = ot.font.text[i];
	t_xor = ot.t_xor;
	eff_cust = ot.eff_cust;
    eff_color = ot.eff_color;
    eff_gradblur = ot.eff_gradblur;


	return *this;
}

// Regenerate the texture and store the results to the buffer given
// t: Texture info (width and height mainly)
// data: a pointer not initialised of the buffer
void COneTextGen::Regenerate (TEXTURE &t, unsigned char* &data)
{
	// Clear texture data
	if (data!=NULL)
		free (data);
	data = (unsigned char*) calloc(t.w*t.h*t.iformat, sizeof(unsigned char));

	switch (type)
	{
		case 0: Text_generate_plain		(t, plain, data);	break;
		case 1: Text_generate_noise		(t, noise, data);	break;
		case 2:	Text_generate_perlin	(t, perlin, data);	break;
		case 3: Text_generate_celular	(t, celular, data);	break;
		case 4: Text_generate_plasma	(t, plasma, data);	break;
		case 5: Text_generate_blob		(t, blob, data);	break;
		case 6: Text_generate_line		(t, line, data);	break;
		case 7: Text_generate_font		(t, font, data);	break;
		case 8: Text_generate_xor		(t, t_xor, data);		break;
		default:	break;
	}
	Text_opp (t, opacity, data);
}



// Apply the effect to the data buffer
void COneTextGen::ApplyEffect (TEXTURE &t, unsigned char* &data)
{
	switch (type)
	{
		case 100:	Text_effect_bw			(t, data);	break;
		case 101:	Text_effect_rect2polar	(t, data);	break;
		case 102:	Text_effect_blur		(t, data);	break;
		case 103:	Text_effect_motionblur	(t, data);	break;
		case 104:	Text_effect_edges1		(t, data);	break;
		case 105:	Text_effect_edges2		(t, data);	break;
		case 106:	Text_effect_sharpen1	(t, data);	break;
		case 107:	Text_effect_sharpen2	(t, data);	break;
		case 108:	Text_effect_sharpen3	(t, data);	break;
		case 109:	Text_effect_emboss1		(t, data);	break;
		case 110:	Text_effect_emboss2		(t, data);	break;
		case 111:	Text_effect_mean		(t, data);	break;
		case 112:	Text_effect_mean2		(t, data);	break;

		case 150:	Text_effect_custom3		(t, eff_cust, data); break;
		case 151:	Text_effect_color		(t, eff_color, data); break;
        case 152:	Text_effect_gradblur	(t, eff_gradblur, data); break;
		default:	break;
	}
}




