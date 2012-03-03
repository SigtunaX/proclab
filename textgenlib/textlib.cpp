/////////////////////////////////////////////
// ZNME :: Zona Neutra Mini Engine
//         Xphere
//         TEXTURE LIBRARY
/////////////////////////////////////////////


#include "main.h"
#include <math.h>

/////////////////////////////////////////////
// Texture copy: text_o -> Txt_d
void Text_swap (TEXTURE &Txt_d, unsigned char *text_o)
{
	int cnt;
	int cnt_max = Txt_d.w*Txt_d.h*3;

	for (cnt=0; cnt<cnt_max; cnt++)
		Txt_d.data[cnt] = text_o[cnt];
}

void Text_add (TEXTURE &Txt, unsigned char *text)
{
	int i,j,k;
	int valor;

	for (i=0; i<Txt.w; i++)
		for (j=0; j<Txt.h; j++)
			for (k=0; k<3; k++) {
				valor = Txt.data[ i*(Txt.h*3) + (j*3) +k ] + text[ i*(Txt.h*3) + (j*3) +k ];
				if (valor>255)
					valor=255;
				if (valor<0)
					valor=0;
				Txt.data[ i*(Txt.h*3) + (j*3) +k ] = valor;
			}
}

void Text_sub (TEXTURE &Txt, unsigned char *text)
{
	int i,j,k;
	int valor;

	for (i=0; i<Txt.w; i++)
		for (j=0; j<Txt.h; j++)
			for (k=0; k<3; k++) {
				valor = Txt.data[ i*(Txt.h*3) + (j*3) +k ] - text[ i*(Txt.h*3) + (j*3) +k ];
				if (valor>255)
					valor=255;
				if (valor<0)
					valor=0;
				Txt.data[ i*(Txt.h*3) + (j*3) +k ] = valor;
			}
}

// Textura: per valors com l'altura i l'amplada
// text: textura a on s'aplica (la que es llegeix i modifica)
// opp: Opacitat (de 0 transparent, a 255 opac)
void Text_opp (TEXTURE &Txt, unsigned char opp, unsigned char *data)
{
	int cnt;
	int cnt_max = Txt.w*Txt.h*3;

	for (cnt=0; cnt<cnt_max; cnt++)
		data[cnt] = (unsigned char) ( data[cnt] * (float)opp/255.0f );
}


/////////////////////////////////////////////
// PLAIN TEXTURE
/////////////////////////////////////////////
void Text_generate_plain (TEXTURE Txt, T_PLAIN plain, unsigned char *data)
{
	int max_cnt = Txt.w*Txt.h*3;
	
	int cnt = 0;
    while (cnt<max_cnt)
	{
		data[cnt++]=plain.c.R;
		data[cnt++]=plain.c.G;
		data[cnt++]=plain.c.B;
	}
}

/////////////////////////////////////////////
// NOISE FUNC
/////////////////////////////////////////////
inline double findnoise2(double x,double y)
{
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
}

inline double interpolate1(double a,double b,double x)
{
	double ft=x * 3.1415927;
	double f=(1.0-cos(ft))* 0.5;
	return a*(1.0-f)+b*f;
}

double noise2(double x,double y)
{
	double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
	double floory=(double)((int)y);
	double s,t,u,v;//Integer declaration
	s=findnoise2(floorx,floory); 
	t=findnoise2(floorx+1,floory);
	u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
	v=findnoise2(floorx+1,floory+1);
	double int1=interpolate1(s,t,x-floorx);//Interpolate between the values.
	double int2=interpolate1(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return interpolate1(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}


void Text_generate_noise (TEXTURE Txt, T_NOISE noise, unsigned char *data)
{
	int x,y;
	int cnt=0;

	for (y=0; y<Txt.h; y++)
		for (x=0; x<Txt.w; x++)
		{
			data [cnt++]=(unsigned char)(255.0f*findnoise2(x, y+noise.c.R));
			data [cnt++]=(unsigned char)(255.0f*findnoise2(x, y+noise.c.G));
			data [cnt++]=(unsigned char)(255.0f*findnoise2(x, y+noise.c.B));
		}
}

/////////////////////////////////////////////
// PERLIN TEXTURE
/////////////////////////////////////////////

void Text_generate_perlin (TEXTURE Txt, T_PERLIN perlin, unsigned char *data)
{	
	int x,y;
	int cnt=0;
	double zoom = perlin.zoom;
	double p=(double)perlin.persistence/255.0;
	int octaves = perlin.octaves;
	
	for (x=0; x<Txt.w; x++)
		for (y=0; y<Txt.h; y++)
		{
			double getnoise =0;
			for(int a=0;a<octaves-1;a++)//This loops trough the octaves.
			{
				double frequency = pow(2,a);//This increases the frequency with every loop of the octave.
				double amplitude = pow(p,a);//This decreases the amplitude with every loop of the octave.
				getnoise += noise2(((double)x)*frequency/zoom,((double)y)/zoom*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
			}
			int color= (int)((getnoise*128.0)+128.0);//Convert to 0-256 values.
			if(color>255)
				color=255;
			if(color<0)
				color=0;
			
			data [cnt++] = (int)((perlin.c.R/255.0)*(double)color);
			data [cnt++] = (int)((perlin.c.G/255.0)*(double)color);
			data [cnt++] = (int)((perlin.c.B/255.0)*(double)color);
		}
}



/////////////////////////////////////////////
// CELLULAR TEXTURE
/////////////////////////////////////////////

float WrapDist (int width, int height, int x, int y, int px, int py)
{
	int dx = abs(x-px);
	int dy = abs(y-py);
	if (dx > width/2)
		dx = width-dx;
	if (dy > height/2)
		dy = height-dy;
	return sqrtf( dx*dx + dy*dy );
}

// Calculate distance to nearest point
float DistToNearestPoint(int w, int h, int x, int y, int* xcoords, int* ycoords, int numpoints)
{
	float dist;
	float mindist = 100000;
	for (int cnt = 0; cnt<numpoints; cnt++)
	{
		dist = WrapDist(w, h, xcoords[cnt], ycoords[cnt], x, y);
//		dist = sqrtf( (xcoords[cnt]-x)*(xcoords[cnt]-x) + (ycoords[cnt]-y)*(ycoords[cnt]-y) );
		if (dist < mindist)
			mindist = dist;
	}

	return mindist;
}


// Calculate distance to nearest point
float DistToNearestPoint2(int w, int h, int x, int y, int* xcoords, int* ycoords, int numpoints)
{
	float dist, dist2;
	float mindist = 100000;
	for (int cnt = 0; cnt<numpoints; cnt++)
	{
		dist = WrapDist(w, h, xcoords[cnt], ycoords[cnt], x, y);
		//dist = sqrtf( (xcoords[cnt]-x)*(xcoords[cnt]-x) + (ycoords[cnt]-y)*(ycoords[cnt]-y) );
		if (dist < dist2)
		{
			dist2 = dist;
			if (dist2 < mindist)
				mindist = dist2;
		}
	}
	
	return dist2;
}

void Text_generate_celular (TEXTURE Txt, T_CELULAR celular, unsigned char *data)
{
	int x,y;
	int cnt=0;
	
	
	float *distBuffer;
	int *xcoords;
	int *ycoords;
	float mindist =  100000;
	float maxdist = -100000;
	int numPoints = 10;

	distBuffer = (float*) malloc (Txt.w*Txt.h*sizeof (float));
	xcoords = (int*) malloc (numPoints*sizeof (int));
	ycoords = (int*) malloc (numPoints*sizeof (int));
	
	for (cnt=0; cnt<numPoints; cnt++)
	{
		xcoords[cnt] = abs(Txt.h * noise2(cnt*celular.s.R,celular.s.G*cnt));
		ycoords[cnt] = abs(Txt.w * noise2(celular.s.B*cnt,(celular.s.G*celular.s.B)));
	}
		
						
	cnt = 0;
	for (y=0; y<Txt.h; y++)
		for (x=0; x<Txt.w; x++)
		{
			if (celular.type==0)
				distBuffer[x+y*Txt.h] = DistToNearestPoint (Txt.w, Txt.h, x, y, xcoords, ycoords, numPoints);
			else
				distBuffer[x+y*Txt.h] = DistToNearestPoint2(Txt.w, Txt.h, x, y, xcoords, ycoords, numPoints) - DistToNearestPoint (Txt.w, Txt.h, x, y, xcoords, ycoords, numPoints)*2;

			float distance = distBuffer[x+y*Txt.h];
			
			if (distance < mindist)
				mindist = distance;
			if (distance > maxdist)
				maxdist = distance;

			data [cnt++] = (unsigned char) (celular.c.R*((distance-mindist)/(maxdist-mindist)));
			data [cnt++] = (unsigned char) (celular.c.G*((distance-mindist)/(maxdist-mindist)));
			data [cnt++] = (unsigned char) (celular.c.B*((distance-mindist)/(maxdist-mindist)));
		}

	free (distBuffer);
	free (xcoords);
	free (ycoords);
}


/////////////////////////////////////////////
// PLASMA TEXTURE
/////////////////////////////////////////////
void Text_generate_plasma (TEXTURE Txt, T_PLASMA plasma, unsigned char *data)
{
	int x,y;
	int cnt=0;
	int i=0;
	int t4=plasma.posX;
	int t3=plasma.posY;
	unsigned char	*tableX;
	unsigned char	*tableY;

	//////////////////////////////////////////////////////// Creem taules del plasma
	tableX = (unsigned char*) malloc (Txt.w*sizeof (unsigned char));
	tableY = (unsigned char*) malloc (Txt.w*sizeof (unsigned char));
	// Generem les dues taules auxiliars (unes taules amb cercles)
	for(i = 0; i < Txt.w; i++)
	{
		tableX[i] = (unsigned char)
					( plasma.tam1*sin(i*PI*2/(Txt.w/(float)plasma.zoom)) +
					  plasma.tam2*sin(i*PI*2/(Txt.w/(float)plasma.zoom)) +
					  50);
		tableY[i] = (unsigned char)
					( plasma.tam3*cos(i*PI*2/(Txt.w/(float)plasma.zoom)) +
					  plasma.tam4*cos(i*PI*2/(Txt.w/(float)plasma.zoom)) +
					  25);
	}
	////////////////////////////////////////////////////////


	for (x=0; x<Txt.w; x++)
	{
		int temp = tableY[plasma.posX] + tableY[plasma.posY];

		for (y=0; y<Txt.h; y++)
		{
			int color = (temp + tableX[t3] + tableX[t4] + plasma.seed) * 2;
			color = color % 512;

			if(color >= 255)
			{
				color -= 255;
				color = 256-color;
			}
			data [cnt++] = (int)plasma.c.R + (unsigned char)((((float)plasma.s.R - (float)plasma.c.R) / 255 ) * color);
			data [cnt++] = (int)plasma.c.G + (unsigned char)((((float)plasma.s.G - (float)plasma.c.G) / 255 ) * color);
			data [cnt++] = (int)plasma.c.B + (unsigned char)((((float)plasma.s.B - (float)plasma.c.B) / 255 ) * color);

			t3 = (t3 + 1) % Txt.w;
			t4 = (t4 + 2) % Txt.w;
		}
		plasma.posX = (plasma.posX + 1) % Txt.h;
		plasma.posY = (plasma.posY + 2) % Txt.h;
	}
	free (tableX);
	free (tableY);
	tableX=tableY= NULL;
}

/////////////////////////////////////////////
// BLOB TEXTURE
/////////////////////////////////////////////
void Text_generate_blob (TEXTURE Txt, T_BLOB blob, unsigned char *data)
{
	int X_orig=Txt.w/2;
	int Y_orig=Txt.h/2;
	int x, y;
	int cnt=0;
	double dx, dy;
	double dist;
	double valor;
	
	// Ens patejem la textura i calculem el potencial
	for(x=0; x<Txt.w; x++)
	{
		for (y=0; y<Txt.h; y++)
		{
			dx=X_orig-x;
			dy=Y_orig-y;
			dist=sqrt(dx*dx+dy*dy)/(Txt.w/2);
			valor=(dist*blob.zoom)/Txt.w;
			if (valor>=1.0)
				valor=1.0;
			if (valor<0.1)
				valor=0.1;
			valor=fabs(log10(valor));

			if (blob.inv==1)
				valor=1.0-valor;		// Invertim Blob
			if (blob.mad==1)
				valor=blob.madvalor/valor;	// Mad Effect
		
			data [cnt++]	= (unsigned char) (valor*blob.c.R);
			data [cnt++]	= (unsigned char) (valor*blob.c.G);
			data [cnt++]	= (unsigned char) (valor*blob.c.B);
		}
	}
}

/////////////////////////////////////////////
// LINEAR TEXTURE
/////////////////////////////////////////////
void Text_generate_line (TEXTURE Txt, T_LINE line, unsigned char *data)
{
	int i=0, j=0;
	int cnt=0;
	float s,c;
	int val_c;
	int val_s;
	int valor[3];

	for (i=0; i<Txt.w; i++)
	{
		for (j=0; j<Txt.h; j++)
		{
			if (line.rot1==0)
				val_s=i;
			else
				val_s=j;
			if (line.rot2==0)
				val_c=i;
			else
				val_c=j;

			c = 0.5f - 0.5f*cosf((((float)val_c/(float)Txt.h)*2*PIF) * (float)(line.n1));
			s = 0.5f + 0.5f*cosf((((float)val_s/(float)Txt.h)*2*PIF) * (float)(line.n2));			
			//Efecte curiÛs, un cercle
			//c = 0.5f + 0.5f*cosf(sqrt((i - width / 2.0) * (i - width / 2.0) + (j - height / 2.0) * (j - height / 2.0)) / (float)(line.n1));
			valor[0]=(unsigned char) (line.s.R*s) + (unsigned char) (line.c.R*c);
			valor[1]=(unsigned char) (line.s.G*s) + (unsigned char) (line.c.G*c);
			valor[2]=(unsigned char) (line.s.B*s) + (unsigned char) (line.c.B*c);
			if (valor[0]<0) valor[0]=0;
			if (valor[1]<0) valor[1]=0;
			if (valor[2]<0) valor[2]=0;
			if (valor[0]>255) valor[0]=255;
			if (valor[1]>255) valor[1]=255;
			if (valor[2]>255) valor[2]=255;
			data[cnt++]=valor[0];
			data[cnt++]=valor[1];
			data[cnt++]=valor[2];
		}
	}
}

#ifdef __APPLE__
	#import "apple_font.h"
#endif
/////////////////////////////////////////////
// FONT TEXTURE
/////////////////////////////////////////////
void RenderFont(int x, int y, int size, char *str, char *fontname, UC_COLOR3 textcol,
				unsigned char *cbuf, int w)
{
#ifdef __APPLE__
	RenderFontApple (x, y, size, w, str, cbuf);
#endif
#ifdef WIN32
	int total=w*w;
	int *buf;

	buf = (int*)malloc(sizeof(int)*total);

    HDC hdc=GetDC(NULL);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bm = CreateCompatibleBitmap(hdc,w,w);

    HFONT hf=CreateFont(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
    ANTIALIASED_QUALITY, DEFAULT_PITCH, fontname);         

    RECT r;
    r.top=0;
    r.left=0;
    r.right=w;
    r.bottom=w;

    SelectObject(mdc, bm);
	FillRect(mdc, &r, CreateSolidBrush(RGB(0,0,0))); // Background

    SelectObject(mdc, hf);      
    SetBkMode(mdc, TRANSPARENT);
    SetTextColor(mdc, RGB(textcol.R,textcol.G,textcol.B));
    TextOut(mdc, x, y, str, strlen(str));
    
    BITMAPINFO bmi;    

    bmi.bmiHeader.biSize=sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth=w;
    bmi.bmiHeader.biHeight=w;
    bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biBitCount=32;
    bmi.bmiHeader.biCompression=BI_RGB;

    GetDIBits(mdc,bm,0,w,buf,&bmi,DIB_RGB_COLORS);

    DeleteObject(hf);
    DeleteObject(bm);
	DeleteObject(mdc);
	DeleteObject(&bmi);
	// Convert int buffer to char buffer
	int c;
	unsigned char R,G,B;

    for (int i=0; i < total; i++)
    {
            c=buf[i];
            R=(c>>16)&0xFF;
            G=(c>>8)&0xFF;
            B=(c)&0xFF;
            *cbuf++=R;
            *cbuf++=G;
            *cbuf++=B;
    }
	free (buf);
#endif
}

void Text_generate_font (TEXTURE Txt, T_FONT font, unsigned char *data)
{
	unsigned char *tmpbuf;
	tmpbuf = (unsigned char*) malloc(Txt.w*Txt.h*3);

	if (!font.text)
		return;
	switch (font.p.type)
	{
	case 1:
		RenderFont(font.p.posX, font.p.posY, font.p.size, font.text, "Verdana", font.p.c, tmpbuf, Txt.h);
		break;
	case 2:
		RenderFont(font.p.posX, font.p.posY, font.p.size, font.text, "Courier", font.p.c, tmpbuf, Txt.h);
		break;
	default:
		RenderFont(font.p.posX, font.p.posY, font.p.size, font.text, "Arial", font.p.c, tmpbuf, Txt.h);
		break;
	}

	memcpy (data, tmpbuf,Txt.w*Txt.h*3);
	
	free (tmpbuf);
}

/////////////////////////////////////////////
// XOR TEXTURE
/////////////////////////////////////////////
void Text_generate_xor (TEXTURE Txt, T_XOR t_xor, unsigned char *data)
{
	int x, y;
	int cnt=0;
	unsigned char c;

	for(x=0; x<Txt.w; x++)
	{
		for (y=0; y<Txt.h; y++)
		{
			c = x ^ y;
			data[cnt++]	= t_xor.c.R ^ c;
			data[cnt++]	= t_xor.c.G ^ c;
			data[cnt++]	= t_xor.c.B ^ c;
		}
	}
}


/////////////////////////////////////////////
// BLACK AND WHITE EFFECT
/////////////////////////////////////////////
void Text_effect_bw (TEXTURE Txt, unsigned char *data)
{
	int max_cnt = Txt.w*Txt.h*3;
	int value;

	int cnt = 0;
    while (cnt<max_cnt)
	{
		value = (data[cnt] + data[cnt+1] + data[cnt+2])/3.0f;
		data[cnt++]=value;
		data[cnt++]=value;
		data[cnt++]=value;
	}
}

/////////////////////////////////////////////
// Rectangular 2 polar effect
/////////////////////////////////////////////
void Text_effect_rect2polar (TEXTURE Txt, unsigned char *data)
{
	int x,y;
	int dx, dy;
	int newX,newY;
	unsigned char *data2;
	data2 = (unsigned char*) malloc(Txt.w*Txt.h*3);

	for(x=0; x<Txt.w; x++)
	{
		for(y=0; y<Txt.h; y++)
		{
			dx = x - Txt.w/2;
			dy = y - Txt.h/2;
			newX = (atan2 (dy,dx) * (Txt.w/2)/PIF) + Txt.w/2;
			newY = sqrt(dx*dx+dy*dy);
			if (newX<0)	newX=0;
			if (newY<0)	newY=0;
			if (newX>Txt.w)	newX=Txt.w;
			if (newY>Txt.h)	newY=Txt.h;
			data2[ x*(Txt.h*3) + (y*3)   ] = data[ newX*(Txt.h*3) + (newY*3)   ];
			data2[ x*(Txt.h*3) + (y*3)+1 ] = data[ newX*(Txt.h*3) + (newY*3)+1 ];
			data2[ x*(Txt.h*3) + (y*3)+2 ] = data[ newX*(Txt.h*3) + (newY*3)+2 ];
		}
	}

	memcpy(data, data2, Txt.w*Txt.h*3);

	free (data2);
}


/////////////////////////////////////////////
// Apply a filter
/////////////////////////////////////////////


void applyfilter (char *filter, unsigned char filterW, unsigned char cfactor, unsigned char bias, TEXTURE Txt, unsigned char *data)
{
	int x,y,dx,dy;
	int fx, fy;	// Coordenades del filtre
	unsigned char *data2;
	data2 = (unsigned char*) calloc(Txt.w*Txt.h*3, 1);
	float r,g,b;
	float factor = 1.0f/(float)cfactor;

	for(x=0; x<Txt.w; x++)
		for(y=0; y<Txt.h; y++)
		{ 
			r = g = b =0.0;
			//multiply every value of the filter with corresponding image pixel
			for(fx = 0; fx < filterW; fx++)
			for(fy = 0; fy < filterW; fy++)
			{
				dx = (x - filterW / 2 + fx + Txt.w) % Txt.w;
				dy = (y - filterW / 2 + fy + Txt.h) % Txt.h;
				
				r += data[ dx*(Txt.h*3) + (dy*3)   ] * filter[fx*(filterW)+(fy)]; 
				g += data[ dx*(Txt.h*3) + (dy*3)+1 ] * filter[fx*(filterW)+(fy)]; 
				b += data[ dx*(Txt.h*3) + (dy*3)+2 ] * filter[fx*(filterW)+(fy)]; 
			} 
         
			//truncate values smaller than zero and larger than 255 
			data2[ x*(Txt.h*3) + (y*3)   ] = min(max(int(factor * r + bias), 0), 255); 
			data2[ x*(Txt.h*3) + (y*3)+1 ] = min(max(int(factor * g + bias), 0), 255); 
			data2[ x*(Txt.h*3) + (y*3)+2 ] = min(max(int(factor * b + bias), 0), 255);
		}    

	memcpy(data, data2, Txt.w*Txt.h*3);
	
	
	free (data2);
}

void Text_effect_blur (TEXTURE Txt, unsigned char *data)
{
	// Blur
	char filter[25] =  
	{
		0, 0, 1, 0, 0,
		0, 1, 1, 1, 0,
		1, 1, 1, 1, 1,
		0, 1, 1, 1, 0,
		0, 0, 1, 0, 0,
	};
	applyfilter (filter, 5, 13, 0, Txt, data);
}

void Text_effect_motionblur (TEXTURE Txt, unsigned char *data)
{
	// Motion Blur
	char filter[81] =
	{
		1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1
	};
	applyfilter (filter, 9, 9, 0, Txt, data);

}

void Text_effect_edges1 (TEXTURE Txt, unsigned char *data)
{
	char filter[25] =
	{
		-1,  0,  0,  0,  0,
		 0, -2,  0,  0,  0,
		 0,  0,  6,  0,  0,
		 0,  0,  0, -2,  0,
		 0,  0,  0,  0, -1,
	};
	applyfilter (filter, 5, 1, 0, Txt, data);

}

void Text_effect_edges2 (TEXTURE Txt, unsigned char *data)
{
	char filter[9] =
	{
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	applyfilter (filter, 3, 1, 0, Txt, data);
}

void Text_effect_sharpen1 (TEXTURE Txt, unsigned char *data)
{
	char filter[9] =
	{
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	};
	applyfilter (filter, 3, 1, 0, Txt, data);
}
void Text_effect_sharpen2 (TEXTURE Txt, unsigned char *data)
{
	char filter[25] =
	{
		-1, -1, -1, -1, -1,
		-1,  2,  2,  2, -1,
		-1,  2,  8,  2, -1,
		-1,  2,  2,  2, -1,
		-1, -1, -1, -1, -1,
	};
	applyfilter (filter, 5, 8, 0, Txt, data);
}

void Text_effect_sharpen3 (TEXTURE Txt, unsigned char *data)
{
	char filter[9] =
	{
		 1,  1,  1,
		 1, -7,  1,
		 1,  1,  1
	};
	applyfilter (filter, 3, 1, 0, Txt, data);
}

void Text_effect_emboss1 (TEXTURE Txt, unsigned char *data)
{
	// Emboss	 (emboss+greyscale = bumpmap)!!!
	char filter[9] =
	{
	    -1, -1,  0,
		-1,  0,  1,
		0,  1,  1
	};
	applyfilter (filter, 3, 1, 128, Txt, data);
}

void Text_effect_emboss2 (TEXTURE Txt, unsigned char *data)
{
	char filter[25] =
	{
		-1, -1, -1, -1,  0,
		-1, -1, -1,  0,  1,
		-1, -1,  0,  1,  1,
		-1,  0,  1,  1,  1,
		 0,  1,  1,  1,  1
	};
	applyfilter (filter, 5, 1, 128, Txt, data);
}

void Text_effect_mean (TEXTURE Txt, unsigned char *data)
{
	char filter[9] =
	{
	    1,  1,  1,
		1,  1,  1,
		1,  1,  1
	};
	applyfilter (filter, 3, 9, 0, Txt, data);
}

void Text_effect_mean2 (TEXTURE Txt, unsigned char *data)
{
	char filter[81] =
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	applyfilter (filter, 9, 81, 0, Txt, data);
}


void Text_effect_custom3 (TEXTURE Txt, T_EFF_CUSTOM t_eff_cust, unsigned char *data)
{
	applyfilter (t_eff_cust.filter, 3, t_eff_cust.cfactor, t_eff_cust.bias, Txt, data);
}

// COLORIZE
void Text_effect_color (TEXTURE Txt, T_EFF_COLOR t_eff_color, unsigned char *data)
{
	int x,y;
	int r,g,b;
	int sr,sg,sb;
	int er,eg,eb;
	unsigned char *data2;
    
	sr = t_eff_color.c.R;
	sg = t_eff_color.c.G;
	sb = t_eff_color.c.B;
	er = t_eff_color.s.R;
	eg = t_eff_color.s.G;
	eb = t_eff_color.s.B;
	data2 = (unsigned char*) malloc(Txt.w*Txt.h*3);
	
	for(x=0; x<Txt.w; x++)
	{
		for(y=0; y<Txt.h; y++)
		{
			r = data[ (x)*(Txt.h*3) + ((y)*3)  ];
			g = data[ (x)*(Txt.h*3) + ((y)*3)+1];
			b = data[ (x)*(Txt.h*3) + ((y)*3)+2];
			float lum = ((r+g+b)/3.0f)/255.0f;
			r = (sr + (er - sr)*lum);
			g = (sg + (eg - sg)*lum);
			b = (sb + (eb - sb)*lum);
			data2[ x*(Txt.h*3) + (y*3)   ] = r;
			data2[ x*(Txt.h*3) + (y*3)+1 ] = g;
			data2[ x*(Txt.h*3) + (y*3)+2 ] = b;
		}
	}
	
	memcpy(data, data2, Txt.w*Txt.h*3);
	
	free (data2);
}


// GRADIENT BLUR
void Text_effect_gradblur (TEXTURE Txt, T_EFF_GRADBLUR t_eff_gradblur, unsigned char *data)
{
    
    int steps = t_eff_gradblur.steps;
    int stepX = t_eff_gradblur.stepX;
    int stepY = t_eff_gradblur.stepY;
    
    int x,y;
    float dx, dy;
    float px, py;
    int r,g,b;
    int val;
    unsigned char *data2;
    int n;
    data2 = (unsigned char*) malloc(Txt.w*Txt.h*3);
    
    for(x=0; x<Txt.w; x++)
    {
        for(y=0; y<Txt.h; y++)
        {
            r=g=b=0;
            val = data[ x*(Txt.h*3) + (y*3)   ];
            dx = sinf(2.0f*PIF*val/(float)stepX);
            dy = cosf(2.0f*PIF*val/(float)stepY);
            px = x;
            py = y;
            for (n=0; n<steps;n++)
            {
                if (px<0) px=0;
				if (py<0) py=0;
                if (px>=Txt.w) px=Txt.w-1;
                if (py>=Txt.h) py=Txt.h-1;
                r += data[ ((int)px)*(Txt.h*3) + (((int)py)*3)  ];
                g += data[ ((int)px)*(Txt.h*3) + (((int)py)*3)+1];
                b += data[ ((int)px)*(Txt.h*3) + (((int)py)*3)+2];
                px +=dx;
                py +=dy;
            }
            r = r/steps;
            g = g/steps;
            b = b/steps;
            
            data2[ x*(Txt.h*3) + (y*3)   ] = r;
            data2[ x*(Txt.h*3) + (y*3)+1 ] = g;
            data2[ x*(Txt.h*3) + (y*3)+2 ] = b;
        }
    }
    
    memcpy(data, data2, Txt.w*Txt.h*3);
    
    free (data2);
}


