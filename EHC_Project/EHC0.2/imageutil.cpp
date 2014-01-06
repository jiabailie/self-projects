#include "StdAfx.h"
#include <stdio.h>
#include "memalloc.h"
#include <math.h>
#include "imageutil.h"
#include "math.h"
#define PI 3.1415926
#ifndef max
#define max(a,b,c)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b,c)            (((a) < (b)) ? (a) : (b))
#endif

#define eps  0.0000000001
#define  BLACK  20
#define YELLOW  70

float _byte_per_pix(int depth)
{
   float byteperpix = 0;

   switch(depth){
   case 1:
      byteperpix = 0.125;
      break;
   case 4:
      byteperpix = 0.5;
      break;
   case 8:
      byteperpix = 1.0;
      break;
   case 16:
      byteperpix = 2;
      break;
   case 24:
      byteperpix = 3;
      break;
   case 32:
      byteperpix = 4;
      break;
   default:
 	   break;
   }
   return byteperpix;
}
int _bits_per_pix(int depth)
{
   int bitsperpix = 0;

   switch(depth){
   case 1:
      bitsperpix = 1;
      break;
   case 4:
      bitsperpix = 4;
      break;
   case 8:
      bitsperpix = 8;
      break;
   case 24:
      bitsperpix = 24;
      break;
   case 32:
      bitsperpix = 32;
      break;
   default:
	   break;
   }
   return bitsperpix;
}
int _width_bytes( int depth, int width )
{
	int widthbytes;
	
	int bitsperpix = _bits_per_pix(depth);
	
	widthbytes = ((((bitsperpix * width) + 31) / 32) * 4); 

	return( widthbytes );

}

long _sizefromdepth(int pixwidth,int pixheight,int depth)
{
   long filesize;
   //float pixperbyte;
	filesize = _width_bytes(depth,pixwidth ) * pixheight;
   return filesize;
}
unsigned char *_allocate_image(int width, int height, int depth)
{
//  char error[64];
  unsigned char *image = NULL;

  if (width < 1){
	  return NULL;
  }
  if (height < 1){
	  return NULL;
  }
  if (depth < 1){
	  return NULL;
  }
  _calloc_uchar(&image, _sizefromdepth(width, height, depth),
               "allocate_image : image");
  return(image);
}


unsigned char *_mallocate_image(int width,int  height,int depth)
{
//  char error[64];
  unsigned char *image = NULL;
 
  if (width < 1){
	  return NULL;
  }
  if (height < 1){
	  return NULL;
  }
  if (depth < 1){  
	  return NULL;
  }                    
  _malloc_uchar(&image, _sizefromdepth(width, height, depth),
               "allocate_image : image");
  return(image);                 
}



void _rgb2hsi(unsigned char i_r,unsigned char i_g,unsigned char i_b, unsigned char* o_h,unsigned char* o_s,unsigned char* o_i)
{
	double r = (double)i_r / 255;
	double g = (double)i_g / 255;
	double b = (double)i_b / 255;
	double h;
	double s;
	double i;
	double num;
	double den;

	double min = r;
	if (g < min)
		min = g;
	if (b < min)
		min = b;

	i = (r+g+b)/3.0;
	s = 1 - min/i;


	if (s == 0.0)
	{
		h = 0.0;
    }
	else
	{
		num = ((r-g)+(r-b))/2.0;
		den = sqrt((r-g)*(r-g) + (r-b)*(g-b));
		//H = H/sqrt((R-G)*(R-G) + (R-B)*(G-B));
		//h = num/(den + eps);
		h = num/den;
		h = acos(h);
		if (b > g)
		{
			h = 2*PI - h;
		}
		h = h/(2*PI);
	}

	*o_h = (unsigned char)(h * 255);
	*o_s = (unsigned char)(s * 255);
	*o_i = (unsigned char)(i * 255);
}

void _rgb2hsv(unsigned char R, unsigned char G,unsigned char B, unsigned char *h, unsigned char *s, unsigned char *v )
{
 
	float var_R;
	float var_G;
	float var_B;
	float del_R;
	float del_G;
	float del_B;
	float var_Min;
	float var_Max;
	float del_Max;
	float H;
	float S;
	float V;

	S = 0;

	var_R = ( (float)R / (float)255 ); //RGB from 0 to 255
	var_G = ( (float)G / (float)255 );
	var_B = ( (float)B / (float)255 );

	var_Min = min( var_R, var_G, var_B ); //Min. value of RGB
	var_Max = max( var_R, var_G, var_B ); //Max. value of RGB
	del_Max = var_Max - var_Min; //Delta RGB value 

	V = var_Max;

	if ( del_Max == 0 ) //This is a gray, no chroma...
	{
		H = 0 ;//HSV results from 0 to 1
		S = 0;
	}
	else //Chromatic data...
	{
		S = del_Max / var_Max;

		del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max;
		del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max;
		del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max;

		if ( var_R == var_Max ) 
		{
			H = del_B - del_G;
		}
		else if ( var_G == var_Max )
		{
			H = ( 1 / 3 ) + del_R - del_B;
		}
		else if ( var_B == var_Max )
		{
			H = ( 2 / 3 ) + del_G - del_R;
		}

		if ( H < 0 ) 
		{
			 H += 1;
		}
		if ( H > 1 )
		{
			H -= 1;
		}
	}

	*h = (unsigned char)((int)(H * 255));
	*s = (unsigned char)((int)(S * 255));
	*v = (unsigned char)((int)(V * 255));
}

void _grayimage_rotate_90(unsigned char *idata, unsigned char *odata, int w, int h)
{
   int x, y;
   unsigned char *iptr, *siptr, *optr, *soptr;

   siptr = idata;
   soptr = odata + h - 1;
   for(y = 0; y < h; y++){
      iptr = siptr;
      optr = soptr;
      for(x = 0; x < w; x++){
         *optr = *iptr;
         optr += h;
         iptr++;
      }
      siptr += w;
      soptr--;
   }
}

int _rgb_variance(unsigned char _r,unsigned char _g,unsigned char _b)
{
	int r = (int)_r;
	int g = (int)_g;
	int b = (int)_b;
	int e;
	int v;
	e = ( r + g + b )/3;
	v = ((e-r)*(e-r) + (e-g)*(e-g) + (e-b)*(e-b));
	return v;
}


unsigned char *_imagedup(unsigned char *image,int  width,int height, int depth)
{
  unsigned char *dup = NULL;

  dup = _mallocate_image(width, height, depth);
  memcpy(dup, image, _sizefromdepth(width, height, depth));
  return(dup);
}

unsigned char *_cut_image(unsigned char *data,int sw,int sh,
						      int x,int y,int w, int h)
{
//   unsigned char *idata, *iptr;
   unsigned char *iptr;
   unsigned char *odata, *optr;
   int i, j;
	int _sw;
	int _sh;
	_sw = sw;
	_sh = sh;

   _malloc_uchar(&odata, w * h, "_mae_cut_image : cdata");
   //memset(odata, 0, w*h);
   //optr = odata;
   //for(i = y; i < (y+h); i++)
   //{
   //   memcpy(optr, (data + i * sw + x), w);
   //   optr += w;
   //}
   memset(odata, 0, w*h);
   for(i = y; i < (y+h); i++)
   {
	   for(j = x; j < (x+w); j ++ )
	   {
			iptr = data + 	i * sw + j;
			optr = odata + (i-y)*w + (j-x);
			*optr = *iptr;
	   }
   }
   return(odata);
}
