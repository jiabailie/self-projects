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
//double BLACK = 20;
//double YELLOW = 70;
#define  BLACK  20
#define YELLOW  70

float _mae_byte_per_pix(int depth)
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
int _mae_bits_per_pix(int depth)
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
int _mae_width_bytes( int depth, int width )
{
	int widthbytes;

	int bitsperpix = _mae_bits_per_pix(depth);

	widthbytes = ((((bitsperpix * width) + 31) / 32) * 4); 

	return( widthbytes );

}

long _mae_sizefromdepth(int pixwidth,int pixheight,int depth)
{
	long filesize;
	//float pixperbyte;
	filesize = _mae_width_bytes(depth,pixwidth ) * pixheight;
	return filesize;
}
unsigned char *_mae_allocate_image(int width, int height, int depth)
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
	_mae_calloc_uchar(&image, _mae_sizefromdepth(width, height, depth),
		"allocate_image : image");
	return(image);
}


unsigned char *_mae_mallocate_image(int width,int  height,int depth)
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
	_mae_malloc_uchar(&image, _mae_sizefromdepth(width, height, depth),
		"allocate_image : image");
	return(image);                 
}



void _mae_rgb2hsi(unsigned char i_r,unsigned char i_g,unsigned char i_b, unsigned char* o_h,unsigned char* o_s,unsigned char* o_i)
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

void _mae_rgb2hsv(unsigned char R, unsigned char G,unsigned char B, unsigned char *h, unsigned char *s, unsigned char *v )
{
	//  float min1, max1, delta;
	//  float r,g,b;
	// 
	//  r = (float)r1;
	//  g = (float)g1;
	//  b = (float)b1;
	//   
	//  min1 = min( r, min(g, b) );    
	//  max1 = max( r, max(g, b) );  

	//   *v = (unsigned char)max1;    
	//   delta = max1 - min1;    
	//   if( max1 != 0 )    
	//  // if( delta != 0 )    
	//       *s = (unsigned char) (delta / max1);     
	//   else    
	//   {    
	//       // r = g = b = 0    
	//       // s = 0, v ﾎｴｶｨﾒ・   
	//       *s = 0;    
	//       *h = -1;    
	//       return;    
	//   }    
	//           
	//   if( r == max1 )    
	//       *h = (unsigned char)(( g - b ) / delta);         // ﾔﾚ yellow &amt; magenta ﾖｮｼ・   
	//   else if( g == max1 )    
	//       *h = (unsigned char)(2 + ( b - r ) / delta);     // ﾔﾚ cyan &amt; yellow ﾖｮｼ・
	//   else    
	//       *h = (unsigned char)(4 + ( r - g ) / delta);     // ﾔﾚ magenta &amt; cyan ﾖｮｼ・   
	//       
	//   *h *= 60;                               // degrees    
	//   if( *h < 0 ) 
	//{
	//       *h += 360;
	//	*h = *h;
	//}
	//   *h = *h;

	//*h = (unsigned char)(*h * 255 / 360);
	//*s = (unsigned char)(*s * 255);
	//*v = (unsigned char)((r+g+b) / 3);

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
//
//void _mae_rgb2hsL(unsigned char i_r,unsigned char i_g,unsigned char i_b, unsigned char* o_h,unsigned char* o_s,unsigned char* o_i)
//{
//	//float var_r;
//	//float var_G;
//	//float var_B;
//	//float var_Min;
//	//float var_Max;
//	//float del_Max;
//	//float H;
//	//float S;
//	//float L;
//
//	//var_R = ( R / 255 ) //RGB from 0 to 255
//	//var_G = ( G / 255 )
//	//var_B = ( B / 255 )
//
//	//var_Min = min( var_R, var_G, var_B ) //Min. value of RGB
//	//var_Max = max( var_R, var_G, var_B ) //Max. value of RGB
//	//del_Max = var_Max - var_Min //Delta RGB value
//
//	//L = ( var_Max + var_Min ) / 2
//
//	//if ( del_Max == 0 ) //This is a gray, no chroma...
//	//{
//	//H = 0 //HSL results from 0 to 1
//	//S = 0
//	//}
//	//else //Chromatic data...
//	//{
//	//if ( L < 0.5 ) S = del_Max / ( var_Max + var_Min )
//	//else S = del_Max / ( 2 - var_Max - var_Min )
//
//	//del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max
//	//del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max
//	//del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max
//
//	//if ( var_R == var_Max ) H = del_B - del_G
//	//else if ( var_G == var_Max ) H = ( 1 / 3 ) + del_R - del_B
//	//else if ( var_B == var_Max ) H = ( 2 / 3 ) + del_G - del_R
//
//	//if ( H < 0 ) ; H += 1
//	//if ( H > 1 ) ; H -= 1
//	//}
//
//}
//
//void _mae_RGB2Lab(double R, double G, double B, double *_L, double *_a, double *_b)
//{
//    double X, Y, Z, fX, fY, fZ;
//	double L,a,b;
//
//    X = 0.412453*R + 0.357580*G + 0.180423*B;
//    Y = 0.212671*R + 0.715160*G + 0.072169*B;
//    Z = 0.019334*R + 0.119193*G + 0.950227*B;
//    X /= (255 * 0.950456);
//    Y /=  255;
//    Z /= (255 * 1.088754);
//    if (Y > 0.008856)
//    {
//     fY = pow(Y, 1.0/3.0);
//     L = 116.0*fY - 16.0;
//    }
//    else
//    {
//     fY = 7.787*Y + 16.0/116.0;
//     L = 903.3*Y;
//    }
//    if (X > 0.008856)
//     fX = pow(X, 1.0/3.0);
//    else
//     fX = 7.787*X + 16.0/116.0;
//    if (Z > 0.008856)
//     fZ = pow(Z, 1.0/3.0);
//    else
//     fZ = 7.787*Z + 16.0/116.0;
//    a = 500.0*(fX - fY);
//    b = 200.0*(fY - fZ);
//    if (L < BLACK)
//    {
//     a *= exp((L - BLACK) / (BLACK / 4));
//     b *= exp((L - BLACK) / (BLACK / 4));
//     L = BLACK;
//    }
//    if (b > YELLOW)
//     b = YELLOW;
//
//	*_L = L;
//	*_a = a;
//	*_b = b;
//}
//
//void _mae_Lab2RGB(double L, double a, double b, double *R, double *G, double *B)
//{
// double X, Y, Z, fX, fY, fZ;
// double RR, GG, BB;
//
//    fY = pow((L + 16.0) / 116.0, 3.0);
//    if (fY < 0.008856)
//     fY = L / 903.3;
//    Y = fY;
//    if (fY > 0.008856)
//     fY = pow(fY, 1.0/3.0);
//    else
//     fY = 7.787 * fY + 16.0/116.0;
//    fX = a / 500.0 + fY;
//    if (fX > 0.206893)
//     X = pow(fX, 3.0);
//    else
//     X = (fX - 16.0/116.0) / 7.787;
//    fZ = fY - b /200.0;
//    if (fZ > 0.206893)
//     Z = pow(fZ, 3.0);
//    else
//     Z = (fZ - 16.0/116.0) / 7.787;
//    X *= (0.950456 * 255);
//    Y *=             255;
//    Z *= (1.088754 * 255);
//    RR =  3.240479*X - 1.537150*Y - 0.498535*Z;
//    GG = -0.969256*X + 1.875992*Y + 0.041556*Z;
//    BB =  0.055648*X - 0.204043*Y + 1.057311*Z;
//    *R = (float)(RR < 0 ? 0 : RR > 255 ? 255 : RR);
//    *G = (float)(GG < 0 ? 0 : GG > 255 ? 255 : GG);
//    *B = (float)(BB < 0 ? 0 : BB > 255 ? 255 : BB);
//}

void _mae_grayimage_rotate_90(unsigned char *idata, unsigned char *odata, int w, int h)
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

int _mae_rgb_variance(unsigned char _r,unsigned char _g,unsigned char _b)
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


unsigned char *_mae_imagedup(unsigned char *image,int  width,int height, int depth)
{
	unsigned char *dup = NULL;

	dup = _mae_mallocate_image(width, height, depth);
	memcpy(dup, image, _mae_sizefromdepth(width, height, depth));
	return(dup);
}

unsigned char *_mae_cut_image(unsigned char *data,int sw,int sh,
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

	_mae_malloc_uchar(&odata, w * h, "_mae_cut_image : cdata");
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

//void _mae_cut_image(unsigned char *data,int sw,int sh,
//					int l,int t,int w,int h , unsigned char **out_data)
//{
//   unsigned char *cdata,*cptr;
//   int y,source_width,dest_width,horiz_offset;
//
//   /* byte width of source raster buffer */
//   source_width = sw;
//   /* byte width of sub-image */
//   dest_width = w;
//   /* allocate sub-image buffer */
//   _mae_malloc_uchar(&cdata, dest_width * h, "_mae_cut_image : cdata");
//   cptr = cdata;
//   /* horizontal byte offset into source data */
//   horiz_offset = l;
//   /* foreach line to be clipped ... */
//   for(y = t; y < (t+h); y++)
//   {
//      /* foreach byte to be clipped ... */
//      memcpy(cptr,(data + (y * source_width) + horiz_offset), dest_width);
//      cptr += dest_width;
//   }
//   /* return the sub-image */
//   *out_data = cdata;
//   return;
//}