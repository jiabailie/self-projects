/*
# proc: zoom - scales a binary bitmap up or down depending on the x & y factors
# proc:        passed.
# proc: zoom_x - scales a binary bitmap up or down based on the x factor
# proc:          passed.
# proc: zoom_y - scales a binary bitmap up or down based on the y factor
# proc:          passed.
# proc: enlarge_x - expand a binary bitmap's columns by the x factor passed.
# proc:
# proc: enlarge_y - expand a binary bitmap's rows by the y factor passed.
# proc:
# proc: shrink_x - down sample a binary bitmap's columns by the x factor
# proc:            passed.
# proc: shrink_y - down sample a binary bitmap's rows by the y factor passed.
# proc:
# proc: copy_cols - copies a single column of pixels to a range of column
# proc:             locations in a destination image.
# proc: copy_col - copies a column of pixels to a column location in a
# proc:            destination image.
# proc: copy_rows - copies a single row of pixels to a range of row locations
# proc:             in a destination image.
# proc: copy_row - copies a row of pixels to a row location in a destination
# proc:            image.
*/
#include "StdAfx.h"
#include <stdio.h>
#include <math.h>
#include "zoom.h"
#include "imageutil.h"

/************************************************************************/
void _zoom(unsigned char **zmdata, int *zw, int *zh, 
		  unsigned char *indata, int iw, int ih, int depth, 
		  float xfctr, float yfctr)
{

   unsigned char *chardata;
   unsigned char *xzmdata;
   unsigned char *yzmdata;
  // int xw, xh, yw, yh;
   int xw, xh;

   if((xfctr == 1.0) && (yfctr == 1.0))
   {
      *zmdata = _imagedup(indata, iw, ih, depth);
      *zw = iw;
      *zh = ih;
   } else{
      /* if shrinking y, then do it first to mimimize length of col copies */
      if(yfctr <= 1.0)
	  {
         chardata = (unsigned char *)_imagedup(indata, iw, ih, depth);
         _zoom_y(&yzmdata, &xw, &xh, chardata, iw, ih, depth, yfctr);
         free(chardata);
         _zoom_x(zmdata, zw, zh, yzmdata, xw, xh, depth, xfctr);
         free(yzmdata);
	  }
      /* if enlarging y, then do it last to mimimize length of col copies */
      if(yfctr > 1.0)
	  {
         chardata = (unsigned char *)_imagedup(indata, iw, ih, depth);
         _zoom_x(&xzmdata, &xw, &xh, chardata, iw, ih, depth, xfctr);
         free(chardata);
         _zoom_y(zmdata, zw, zh, xzmdata, xw, xh, depth, yfctr);
         free(xzmdata);
      }
   }
}

/************************************************************************/
void _zoom_x(unsigned char **xzmdata, int *zw, int *zh, 
			unsigned char *chardata, int iw, int ih, int depth, float xfctr)
{
   if(xfctr == 1.0)
   {
      *xzmdata = (unsigned char *)_imagedup(chardata, iw, ih, depth);
      *zw = iw;
      *zh = ih;
   }  else if(xfctr > 1.0)  {
      _enlarge_x(xzmdata, zw, zh, chardata, iw, ih, depth, xfctr);
   }  else  {
      _shrink_x(xzmdata, zw, zh, chardata, iw, ih, depth, xfctr);
   }
}

/************************************************************************/
void _zoom_y(unsigned char **yzmdata, int *zw, int *zh, 
			unsigned char *chardata, int iw, int ih, int depth, float yfctr)
{
   if(yfctr == 1.0)
   {
      *yzmdata = (unsigned char *)_imagedup(chardata, iw, ih, depth);
      *zw = iw;
      *zh = ih;
   }  else if(yfctr > 1.0) {
      _enlarge_y(yzmdata, zw, zh, chardata, iw, ih, depth, yfctr);
   }  else {
      _shrink_y(yzmdata, zw, zh, chardata, iw, ih, depth, yfctr);
   }
}

/************************************************************************/
void _enlarge_x(unsigned char **xzmdata, int *zw, int *zh, 
			   unsigned char *chardata, int iw, int ih, int depth, float xfctr)
{
   int from, to1, to2;
   float f_current;

   *zw = (int)((xfctr * iw) + 0.5);
   *zh = ih;
   *xzmdata = _allocate_image(*zw, *zh, depth);

   f_current = -1.0;
   from = 0;
   to1 = from;
   do{
      f_current += xfctr;
      to2 = (int)(f_current + 0.5);

      _copy_cols((*xzmdata), *zw, *zh, to1, to2, chardata, iw, ih, depth, from);

      from++;
      to1 = to2+1;
   } while(from < iw);
}

/************************************************************************/
void _enlarge_y(unsigned char ** yzmdata, int *zw, int *zh, 
			   unsigned char *chardata, int iw, int ih, int depth, float yfctr)
{
   int from, to1, to2;
   float f_current;

   *zw = iw;
   *zh = (int)((yfctr * ih) + 0.5);
   *yzmdata = _allocate_image(*zw, *zh, depth);

   f_current = -1.0;
   from = 0;
   to1 = from;
   do{
      f_current += yfctr;
      to2 = (int)(f_current + 0.5);

      _copy_rows((*yzmdata), *zw, *zh, to1, to2, chardata, iw, ih, depth, from);

      from++;
      to1 = to2+1;
   } while(from < ih);
}

/************************************************************************/
void _shrink_x(unsigned char **xzmdata, int *zw, int *zh, 
			  unsigned char *chardata, int iw, int ih, int depth, float xfctr)
{
   int from, to;
   float recip, f_current;

   recip = (float)1.0 / xfctr;
   *zw = (int)((xfctr * iw) + 0.5);
   *zh = ih;
   *xzmdata = _mallocate_image(*zw, *zh, depth);

   f_current = 0.0;
   from = 0;
   to = from;
   do{
      _copy_col((*xzmdata), *zw, *zh, to, chardata, iw, ih, depth, from);
      f_current += recip;
      from = (int)(f_current + 0.5);
      to++;
   } while(to < *zw);

}

/************************************************************************/
void _shrink_y(unsigned char **yzmdata, int *zw, int *zh, 
			  unsigned char *chardata, int iw, int ih, int depth, float yfctr)
{
   int from, to;
   float recip, f_current;


   recip = (float)1.0 / yfctr;
   *zw = iw;
   *zh = (int)((yfctr * ih) + 0.5);
   *yzmdata = _mallocate_image(*zw, *zh, depth);

   f_current = 0.0;
   from = 0;
   to = from;

   do{
      _copy_row((*yzmdata), *zw, *zh, to, chardata, iw, ih, depth, from);
      f_current += recip;
      from = (int)(f_current + 0.5);
      to++;
   } while(to < *zh);
}

/************************************************************************/
void _copy_cols(unsigned char *xzmdata, int zw, int zh, 
			  int to1, int to2, 
			  unsigned char *chardata, int iw, int ih, int depth, int from)
{
   int i;

   for(i = to1; i <= to2; i++)
   {
      _copy_col(xzmdata, zw, zh, i, chardata, iw, ih, depth, from);
   }
}

/************************************************************************/
void _copy_col(unsigned char *xzmdata, int zw, int zh, int to, 
			  unsigned char *chardata,int  iw, int ih, int depth, int from)
{
   unsigned char *fptr, *tptr;
   int i;
   int j;
   int movedbits;
   int iw_size = _width_bytes( depth, iw);
   int zw_size = _width_bytes( depth, zw);

   if(zh != ih)
   {
	   //fatalerr("copy_col", "length of columns not equal", NULL);
	   ;
   }
	movedbits = 1;
	switch( depth )
	{
	case 8:	
		movedbits = 1;
		break;
	case 16:
		movedbits = 2;
		break;
	case 24:
		movedbits = 3;
		break;
	case 32:
		movedbits = 4;
		break;
	}
   fptr = chardata + from * movedbits;
   tptr = xzmdata + to * movedbits;
   for(i = 0; i < zh; i++)
   {
	   for(j = 0; j < movedbits; j ++ )
	   {
		   *(tptr + j) = *(fptr + j);
	   }
      tptr += zw_size;
      fptr += iw_size;
   }
}

/************************************************************************/
void _copy_rows(unsigned char *yzmdata, int zw, int zh, int to1, int to2, 
			   unsigned char *chardata, int iw, int ih, int depth, int from)
{
   int i;

   for(i = to1; i <= to2; i++)
   {
      _copy_row(yzmdata, zw, zh, i, chardata, iw, ih, depth, from);
   }
}

/************************************************************************/
void _copy_row(unsigned char *yzmdata, int zw, int zh, int to, 
			  unsigned char *chardata, int iw, int ih, int depth, int from)
{
   unsigned char *fptr, *tptr;
   
   //for warning 
   int _zh;
   int _ih;
   _zh = zh;
   _ih = ih;
	//
   if(zw != iw)
   {
	 //  fatalerr("copy_row", "length of rows not equal", NULL);
	   ;
   }
   int iw_size = _width_bytes( depth, iw);
   int zw_size = _width_bytes( depth, zw);

   fptr = chardata + (from * iw_size);
   tptr = yzmdata + (to * zw_size);
   memcpy(tptr, fptr, zw_size);
}

/************************************************************/
