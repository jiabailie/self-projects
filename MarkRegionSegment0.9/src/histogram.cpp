#include "stdafx.h"
#include "memalloc.h"
#include "histogram.h"
#include <stdlib.h>

#define  HIST_THRESHOLD   20
#define  HIST_ROW_GAP   3
#define  HIST_COLUM_GAP   3
void _mae_count_hist(unsigned char *data,int width,int height,
					  int orient,  int **hists,int *len)
{
    switch (orient){
       case 0:
            _mae_count_v_hist(data,width,height,hists,len);
            break;
       case 1:
            _mae_count_h_hist(data,width,height,hists,len);
            break;
       default:
			*hists = NULL;
			*len = 0;
		   break;
    }
}

void _mae_count_h_hist(unsigned char *data,int width, int height,
					int **hists,int *len)
{
	int x, y;
	unsigned char *temp;

	*len = height;

	_mae_malloc_int(hists, height, "_count_h_hist : hists");
	//memset((*hists),(int)0,height);
	for ( y = 0 ; y < height ; y++)
	{
		(*hists)[y] = 0;
	}
	for ( y = 0 ; y < height ; y++)
	{
	   temp = data + y * width;
	   for(x = 0; x < width; x ++ )
	   {	
			//if(*(temp+x) == 0 )
			//{
			//}else{
			//	(*hists)[y] += 1;
			//}
		   (*hists)[y] += *(temp+x);
	   }
	}

}

void _mae_count_v_hist(unsigned char *data,int width,int height,
					int **hists,int *len)
{
	int x, y;
	unsigned char *temp;

	_mae_malloc_int(hists, width, "_count_v_hist : hists");
	//memset((*hists), (int)0, width);
	for(x = 0; x < width; x ++ )
	{	
		(*hists)[x] = 0;
	}
	for ( y = 0 ; y < height ; y++)
	{
	   temp = data + y * width;
	   for(x = 0; x < width; x ++ )
	   {	

		   (*hists)[x] += *(temp+x);
	   }
	}

   *len = width;
}
void _mae_count_h_row_hist(unsigned char *data,int width, int height, 
						int x1, int x2,
					    int **hists,int *len)
{
	int x, y;
	unsigned char *temp;
	_mae_malloc_int(hists, height, "_count_h_rect_hist : hists");
	//memset((*hists),(int)0,height);
	for(y = 0; y < height; y ++ )
	{	
		(*hists)[y] = 0;
	}

	for ( y = 0 ; y < height ; y++)
	{
	   temp = data + y * width;
	   for(x = x1; x < x2; x ++ )
	   {	

		   (*hists)[y] += *(temp+x);
	   }
	}

	*len = height;
}
void _mae_count_h_rect_hist(unsigned char *data,int width, int height, 
						int x1, int y1, int w, int h,
					    int **hists,int *len)
{
	int x, y;
	unsigned char *temp;
	int length;

	int _height;
	_height = height;

	length = h;
	_mae_malloc_int(hists, length, "_count_h_rect_hist : hists");
	//memset((*hists),(int)0,height);
	for(y = 0; y < length; y ++ )
	{	
		(*hists)[y] = 0;
	}

	for ( y = 0 ; y < length ; y++)
	{
	   temp = data + (y + y1) * width;
	   for(x = 0; x < w; x ++ )
	   {	

		   (*hists)[y] += *(temp + x + x1);
	   }
	}
	*len = length;

}
void _mae_count_v_col_hist(unsigned char *data,int width,int height,
						int y1,int y2,
					    int **hists,int *len)
{
	int x, y;
	unsigned char *temp;
	int _height;
	_height = height;

	_mae_malloc_int(hists, width, "_count_v_rect_hist : hists");
	//memset((*hists), 0, width);
	for(x = 0; x < width; x ++ )
	{	
		(*hists)[x] = 0;
	}

	for ( y = y1 ; y < y2 ; y++)
	{
	   temp = data + y * width;
	   for(x = 0; x < width; x ++ )
	   {	

		   (*hists)[x] += *(temp+x);
	   }
	}

   *len = width;
}

void _mae_count_v_rect_hist(unsigned char *data,int width,int height,
						int x1, int y1, int w, int h,
					    int **hists,int *len)
{
	int x, y;
	unsigned char *temp;
	int length;
	
	int _height;
	_height = height;

	length = w;
	_mae_malloc_int(hists, width, "_count_v_rect_hist : hists");
	//memset((*hists), 0, width);
 	for(x = 0; x < length; x ++ )
	{	
		(*hists)[x] = 0;
	}

	for ( y = 0 ; y < h ; y++)
	{
	   temp = data + (y + y1 ) * width;
//	   for(x = 0; x < width; x ++ )
	   for(x = 0; x < w; x ++ )
	   {	

		   (*hists)[x] += *(temp + x + x1);
	   }
	}

   *len = length;
}
void _mae_compute_hist_blocks(int *hists, int len, int**blockst, int**blocked,int *blocknum)
{
	int i;
	int j;
	
	int *_hists = hists;

	//去除短 投影
	for(i = 0; i < len; i ++ )
	{
		if(_hists[i] < HIST_THRESHOLD )
		{
			_hists[i] = 0;
		}
	}
	//计算区域行的个数
	int* _block_;
	_mae_malloc_int(&_block_, len, "_compute_hist_peaks : _block_s");
	//memset(_block_, (int)0, len);
	for(i = 0; i < len; i ++ )
	{	
		_block_[i] = 0;
	}

	int _blocknum = 0;
	int _prev_row = 0;
	int _cur_row;
	//int _next_row;
	for(i = 0; i < len; i ++)
	{
		_cur_row = _hists[i];
		if(  _prev_row == 0  && _cur_row  > 0  )
		{
			_blocknum ++;
			_block_[i] = 1;
		}
		_prev_row = _cur_row;
	}
	//
	_mae_malloc_int(blockst, _blocknum, "_compute_hist_peaks : peaks");
	_mae_malloc_int(blocked, _blocknum, "_compute_hist_peaks : peaks");
//	memset((*blockst), (int)0, _blocknum);
//	memset((*blocked), (int)0, _blocknum);
	for(i = 0; i < _blocknum; i ++ )
	{
		blockst[i] = 0;
		blocked[i] = 0;
	}

	int _idx = 0;
	int _count = 0;
	for(i = 0; i < len; i ++)
	{
		if( _block_[i] == 1 )
		{
			(*blockst)[_idx] = i;
			_count = 0;
			for(j = i; j < len; j ++ )
			{
				if(_hists[j] == 0 ) break;
				_count++;
			}
			(*blocked)[_idx] = _count;
			_idx++;
		}

	}
	
	free(_block_);

	*blocknum = _blocknum;
}



void _mae_hist_h_runs( int *rx,int * ry,int * rlen, 
					  int nruns, int w, int h, 
					  int **h_hist,int * nh_hist)
{
   int i;
   int y;
	int _w;
	int *_rx;
	_w = w;
	_rx = rx;

   *nh_hist = h;
   _mae_malloc_int(h_hist ,  h, "_hist_nruns_hori : h_hist");
   
   for(i = 0; i < h; i ++ )
   {
	   (*h_hist)[i] = 0;
   }
   for(i = 0; i < nruns; i ++ )
   {
	   y =  ry[i];
	   (*h_hist)[y] += rlen[i];
   }

}


void _mae_histogram(unsigned char * data, int width, int height, long * histogram)
{	
	unsigned char * ptemp;
	unsigned char * ptemp1;
	unsigned char value;
	long i;
	for ( i =0; i < 256; ++ i)
	{
		histogram[i] = 0;
	}

	for (int y = 0; y < height; y++)
	{
		ptemp1 = data;
		ptemp1 += (height - 1 - y) * width;
		for (int x = 0; x < width; x++)
		{		
			//指向源DIB第y行，第x个像素的指諄E
			//同样要注意DIB上下倒置的问虂E
			//ptemp = data;
			//ptemp += (height - 1 - y) * width;
			//ptemp += x;
			ptemp = ptemp1 + x;
			value = *(ptemp + 0 );
			histogram[value]++;
		}
	}

}


void _mae_histogram_only_mark_pixs(unsigned char * data, int width, int height, long * histogram, long *pixs, int flag)
{	
	unsigned char * ptemp;
	unsigned char * ptemp1;
	unsigned char value;
	long i;
	long count;
	long max = 0;
	int max_color_idx = 0;
//	int temp;	
	//double probability[256];
	//double etotal;
	int min_color_idx;

	min_color_idx = 0;

	for ( i =0; i < 256; ++ i)
	{
		histogram[i] = 0;
	}
	count = 0;
	for (int y = 0; y < height; y++)
	{
		ptemp1 = data;
		ptemp1 += (height - 1 - y) * width;
		for (int x = 0; x < width; x++)
		{		
			ptemp = ptemp1 + x;
			value = *(ptemp + 0 );
			if(value != 255 )
			{
				histogram[value]++;
				count ++;
			}
		}
	}
	//
	//for (i = 0; i < 256; ++ i)
	//{
	//	probability[i] = (double)(histogram[i]) / (double)(count);
	//}
	//// 得到数学期蛠E
	//etotal = 0;
	//for (i = 0; i < 256; ++ i)
	//{
	//	etotal += (i + 1) * probability[i];
	//}
	for(i = 0; i < 256; i ++ )
	{
		if( histogram[i] != 0 )
		{
		
			min_color_idx = i;
			break;
		}

	}

	if( flag == 1 ) // segment for remove high color 
	{
		max = 0;
		max_color_idx = 0;
		for (i = 0; i < 256; ++ i)
		{
			if( max < histogram[i])
			{
				max = histogram[i];
				max_color_idx = i;
			}
		}
		for(i = max_color_idx + 1; i < 256; i ++ )
		{
			histogram[i] = 0;
		}
		count = 0;
		for(i = 0; i < 256; i ++ )
		{
			count += histogram[i];
		}
		//追加黑色像素
		//histogram[0] += (height + width)*3;
		//count +=  (height + width)*3;
		int _temp = (max - histogram[min_color_idx]) / 3;	
		histogram[min_color_idx] += _temp;
		count +=  _temp;
	}else{ // segment for detect background color region
		//histogram[min_color_idx] += (height + width)*3;
		//count +=  (height + width)*3;
		histogram[0] += count / 10;
		count +=  count / 10;

	}
	*pixs = count;
}
void _mae_histogram_text_pixnum(unsigned char * data, int width, int height, long *pix_num)
{
	unsigned char * ptemp;
	unsigned char * ptemp1;
	unsigned char value;
	long count;
	count = 0;
	for (int y = 0; y < height; y++)
	{
		ptemp1 = data;
		ptemp1 +=  y * width;
		for (int x = 0; x < width; x++)
		{		
			ptemp = ptemp1 + x;
			value = *(ptemp + 0 );
			if(value == 1 )
			{
				count ++;
			}
		}
	}
	*pix_num = count;
}

void _mae_histogram_mark_pixnum(unsigned char * data, int width, int height, long *pix_num)
{
	unsigned char * ptemp;
	unsigned char * ptemp1;
	unsigned char value;
	long count;
	count = 0;
	for (int y = 0; y < height; y++)
	{
		ptemp1 = data;
		ptemp1 += y * width;
		for (int x = 0; x < width; x++)
		{		
			ptemp = ptemp1 + x;
			value = *(ptemp + 0 );
			if(value != 255 )
			{
				count ++;
			}
		}
	}
	*pix_num = count;
}

int _mae_hist_rect_pixs(unsigned char *data,int width, int height, 
							int x1, int y1, int w, int h)
{
	int x, y;
	long count;
	unsigned char *temp;

	int _height;
	_height = height;

	count = 0;
	for(y = y1; y < y1+h; y ++ )
	{
	   temp = data + y * width;
	   for ( x = x1 ; x < x1 + w; x++)
	   {	
		   if( *(temp+x) != 0 )
		   {
			   count++;
		   }
	   }
	}
	return count;
}

void _mae_hist_high_rgb(unsigned char *data,int width, int height, int depth, int widthbytes,
				   int *rhist,int *ghist, int *bhist, long *pixs)
{
	int movedbits;
	int x,y;
	unsigned char *src;
	unsigned char *srctmp;
	unsigned char r,g,b;
//	int varience;
	long pix_num = 0;
	for(int i = 0; i < 256; i ++ )
	{
		rhist[i] = 0;
		ghist[i] = 0;
		bhist[i] = 0;
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

	for(y = 0; y < height; y ++ )
	{
		srctmp = data  + widthbytes * y;
		for(x = 0; x < width; x ++ )
		{
			src = srctmp + movedbits * x;
			if( movedbits == 1 )
			{
				r = *(src + 0);
				g = *(src + 0);
				b = *(src + 0);
				if(r != 255 )
				{	
					rhist[r] ++;
					ghist[r] ++;
					bhist[r] ++;
					pix_num++;
				}
			}else{
				r = *(src + 0);
				g = *(src + 1);
				b = *(src + 2);
				//if(r != 255 || g != 255 || b != 255 )
				if( (r == 250 && g == 250 && b == 250) ||
					(r < 5  && g < 5 && b < 5) )
				{
					;
				}else{
					rhist[r] ++;
					ghist[g] ++;
					bhist[b] ++;
					pix_num++;
				}
			}
		}
	}
	*pixs = pix_num;
}


void _mae_histogram_mark_(unsigned char * data, int width, int height, long *pix_num)
{
	unsigned char * ptemp;
	unsigned char * ptemp1;
	unsigned char value;
	long count;
	count = 0;
	for (int y = 0; y < height; y++)
	{
		ptemp1 = data;
		ptemp1 += y * width;
		for (int x = 0; x < width; x++)
		{		
			ptemp = ptemp1 + x;
			value = *(ptemp + 0 );
			if(value != 255 )
			{
				count ++;
			}
		}
	}
	*pix_num = count;
}


void _mae_histogram_mark(unsigned char * data, int width, int height, long * histogram, long *pixs)
{	
	unsigned char * ptemp;
	unsigned char * ptemp1;
	unsigned char value;
	long i;
	long num = 0;
	for ( i =0; i < 256; ++ i)
	{
		histogram[i] = 0;
	}

	for (int y = 0; y < height; y++)
	{
		ptemp1 = data;
		ptemp1 += (height - 1 - y) * width;
		for (int x = 0; x < width; x++)
		{		
			ptemp = ptemp1 + x;
			value = *(ptemp + 0 );
			if( value != 0 && value != 255)
			//if( value >=2  && value <= 253)
			{
				histogram[value]++;
				num++;
			}
		}
	}
	*pixs = num;

}


void _mae_hist_high_color_pixs(unsigned char *data,int width, int height, int depth, int widthbytes,
							   int x1, int y1, int w, int h, long *pixs)
{
	int movedbits;
	int x,y;
	unsigned char *src;
	unsigned char *srctmp;
	unsigned char r,g,b;
	//int varience;
	long pix_num = 0;

	int _height;
	int _width;
	_height = height;
	_width = width;

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

	for(y = y1; y < y1+h; y ++ )
	{
		srctmp = data  + widthbytes * y;
		for(x = x1; x < x1 + w; x ++ )
		{
			src = srctmp + movedbits * x;
			if( movedbits == 1 )
			{
				r = *(src + 0);
				if(r != 255 )
				{	
					pix_num++;
				}
			}else{
				r = *(src + 0);
				g = *(src + 1);
				b = *(src + 2);
				if(r != 255 || g != 255 || b != 255 )
				{
					pix_num++;
				}
			}
		}
	}
	*pixs = pix_num;
}


void _mae_hist_contour_pixs(unsigned char *data,int width, int height, int depth, int widthbytes, int bkcol,
							   int x1, int y1, int w, int h, long *pixs)
{
	int movedbits;
	int x,y;
	unsigned char *temp;
	unsigned char r,g,b;
	int left;
	int top;
	int right;
	int bottom;	//int varience;
	long pix_num = 0;
	int _height;
	int _width;
	_height = height;
	_width = width;
	//unsigned char r,g,b;
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

	//up line
	//do not show region deleted

	left = x1;
	top = y1;
	right = left +  w -1;
	bottom = top +  h -1;
	//left edge of box
	for(y = top; y < bottom; y ++ )
	{
		x = left;
		temp = data + widthbytes * y + movedbits * x;
		if( movedbits == 1 )
		{
			if( *temp != 255 )
			{
				pix_num ++;
			}
		}else{
			r = *(temp + 0);
			g = *(temp + 1);
			b = *(temp + 2);
			if( (r+g+b) / 3 < bkcol )
			{
				pix_num ++;
			}
		}
	}
	//right edge of box
	for(y = top; y < bottom; y ++ )
	{
		x = right;
		temp = data + widthbytes * y + movedbits * x;
		if( movedbits == 1 )
		{
			if( *temp != 255 )
			{
				pix_num ++;
			}
		}else{
			r = *(temp + 0);
			g = *(temp + 1);
			b = *(temp + 2);
			if( (r+g+b) / 3 < bkcol )
			{
				pix_num ++;
			}
		}
	}
	//top edge of box
	for(x = left; x < right; x ++ )
	{
		y = top;
		temp = data + widthbytes * y + movedbits * x;
		if( movedbits == 1 )
		{
			if( *temp != 255 )
			{
				pix_num ++;
			}
		}else{
			r = *(temp + 0);
			g = *(temp + 1);
			b = *(temp + 2);
			if( (r+g+b) / 3 < bkcol )
			{
				pix_num ++;
			}
		}
	}
	//bottom edge of box
	for(x = left; x < right; x ++ )
	{
		y = bottom;
		temp = data + widthbytes * y + movedbits * x;
		if( movedbits == 1 )
		{
			if( *temp != 255 )
			{
				pix_num ++;
			}
		}else{
			r = *(temp + 0);
			g = *(temp + 1);
			b = *(temp + 2);
			if( (r+g+b) / 3 < bkcol )
			{
				pix_num ++;
			}
		}
	}
	*pixs = pix_num;
}


int _mae_mass_center_region(unsigned char *data,int width, int height, int flag,
							int x1, int y1, int w, int h, int *cx, int *cy, int *linewidth)
{
	//int x, y;
	//long count;
	//unsigned char *temp;
	//long sumx;
	//long sumy;
	//long mass_pixs;
	//mass_pixs = 0;
	//sumx = 0;
	//sumy = 0;
	//for(y = y1; y < y1+h; y ++ )
	//{
	//   temp = data + y * width;
	//   for ( x = x1 ; x < x1 + w; x++)
	//   {	
	//	   if( *(temp+x) != 0 )
	//	   {
	//		   mass_pixs++;
	//		   sumx += x;
	//		   sumy += y;
	//	   }
	//   }
	//}
	//if( mass_pixs == 0 )
	//{
	//	*cx = 0;
	//	*cy = 0;
	//}else{
	//	*cx = sumx / mass_pixs;
	//	*cy = sumy / mass_pixs;
	//}

	
	int *hists;
	int len;
	int *avhists;
	int i;
	int k;
	int max;
	int x,y;
	int lw;
	int dx,dy;

	if( flag == 0 )    //hori
	{
		if( h == 0 ) return 0;
		_mae_count_h_rect_hist(data,width, height, 
								x1, y1, w, h,
								&hists, &len);
		_mae_malloc_int(&avhists, len, "_mae_mass_center_region : avhists");

		memset(avhists, (int)0, len*sizeof(int));
		for(i = 1; i < len-1; i ++ )
		{
			avhists[i] = (hists[i-1] + hists[i] + hists[i+1])/3;
		}
///////////////////
 //    FILE *fp = fopen("E:\\sharp2011\\mark\\test_hist.txt","w");
	//if(fp!=NULL)
	//{
	//	for(i = 0; i < len; i ++ )
	//	{
	//		fprintf(fp,"%d \n,", hists[i] );
	//	}
	//	fclose(fp);
	//}
///////////////////////////

		max = avhists[0];
		y = 0;
		for(i = 1; i < len; i ++ )
		{
			if(max < avhists[i] )
			{
				max = avhists[i];
				y = i;
			}
		}
		dy = y;
		y = y + y1;
		x = x1 + w / 2;
		*cx = x;
		*cy = y;
		lw = 0;
		for(k = dy; k < len; k ++ )
		{
			if( avhists[k] < max / 2  )
			{
				break;
			}
			lw ++;
		}
		for(k = dy; k > 0; k -- )
		{
			if( avhists[k] < max / 2  )
			{
				break;
			}
			lw ++;
		}
		*linewidth = lw;
		free(hists);
		free(avhists);
	}else{
		if( w == 0 ) return 0;
		_mae_count_v_rect_hist(data,width, height, 
								x1, y1, w, h,
								&hists, &len);
		_mae_malloc_int(&avhists, len, "_mae_mass_center_region : avhists");

		memset(avhists, (int)0, len*sizeof(int));
		for(i = 1; i < len-1; i ++ )
		{
			avhists[i] = (hists[i-1] + hists[i] + hists[i+1])/3;
		}
		max = avhists[0];
		x = 0;
		for(i = 1; i < len; i ++ )
		{
			if(max < avhists[i] )
			{
				max = avhists[i];
				x = i;
			}
		}
		dx = x;
		x = x + x1; 
		y = y1 + h / 2;

		*cx = x;
		*cy = y;

		lw = 0;
		for(k = dx; k < len; k ++ )
		{
			if( avhists[k] < max / 2  )
			{
				break;
			}
			lw ++;
		}
		for(k = dx; k > 0; k -- )
		{
			if( avhists[k] < max / 2  )
			{
				break;
			}
			lw ++;
		}
		*linewidth = lw;

		free(hists);
		free(avhists);
	}
	return 0;
}

void _mae_com_line_width(int w, int h,int *h_rx, int *h_ry, int *h_rlen, int h_nruns, 
						 int *v_rx, int *v_ry, int *v_rlen, int v_nruns, 
						 int *h_line_width, int *v_line_width)
{
	int *hist1;
	int  num1;
	int *hist2;
	int  num2;
	int *tmphist1;
	int *tmphist2;
	int i;
	int maxlen;
	int idx;
	int count1;
	int count2;

	 _mae_hist_h_runs(h_rx, h_ry,h_rlen,h_nruns, w, h, &hist1, &num1);
	 _mae_hist_h_runs(v_rx, v_ry,v_rlen,v_nruns, h, w, &hist2, &num2);
	
   _mae_malloc_int(&tmphist1 ,  num1, "_mae_com_line_width : tmphist1");
   _mae_malloc_int(&tmphist2 ,  num2, "_mae_com_line_width : tmphist1");

   memset(tmphist1, (int)0, num1);
   memset(tmphist2, (int)0, num2);
   for(i = 1; i < num1 - 1 ; i ++ )
   {
		tmphist1[i] = (hist1[i-1] + hist1[i] + hist1[i+1]) / 3;
   }
   for(i = 1; i < num2 - 1 ; i ++ )
   {
		tmphist2[i] = (hist2[i-1] + hist2[i] + hist2[i+1]) / 3;
   }
   maxlen = 0;
   idx = 0;
   for(i = 0; i < num1; i ++ )
   {
		if( maxlen < tmphist1[i] )
		{
			maxlen = tmphist1[i];
			idx = i;
		}
   }
	count1 = 0;
	for( i = idx; i > 0; i -- )
	{
		if( tmphist1[i] <  (maxlen/3) )
		{
			break;
		}
		count1 ++;
	}
	for( i = idx; i < num1; i ++ )
	{
		if( tmphist1[i] <  (maxlen/3) )
		{
			break;
		}
		count1 ++;
	}

   maxlen = 0;
   idx = 0;
   for(i = 0; i < num2; i ++ )
   {
		if( maxlen < tmphist2[i] )
		{
			maxlen = tmphist2[i];
			idx = i;
		}
   }
	count2 = 0;
	for( i = idx; i > 0; i -- )
	{
		if( tmphist2[i] <  (maxlen/3) )
		{
			break;
		}
		count2 ++;
	}
	for( i = idx; i < num2; i ++ )
	{
		if( tmphist2[i] <  (maxlen/3) )
		{
			break;
		}
		count2 ++;
	}

	*h_line_width = count1 + 4;
	*v_line_width = count2 + 4;

}


void _mae_hist_high_rgb2(unsigned char *data,int width, int height, int depth, int widthbytes,
				   int *rhist,int *ghist, int *bhist)
{
	int movedbits;
	int x,y;
	unsigned char *src;
	unsigned char *srctmp;
	unsigned char r,g,b;
//	int varience;
	long pix_num = 0;
	for(int i = 0; i < 256; i ++ )
	{
		rhist[i] = 0;
		ghist[i] = 0;
		bhist[i] = 0;
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

	for(y = 0; y < height; y ++ )
	{
		srctmp = data  + widthbytes * y;
		for(x = 0; x < width; x ++ )
		{
			src = srctmp + movedbits * x;
			if( movedbits == 1 )
			{
				r = *(src + 0);
				g = *(src + 0);
				b = *(src + 0);
				if(r != 255 )
				{	
					rhist[r] ++;
					ghist[r] ++;
					bhist[r] ++;
					pix_num++;
				}
			}else{
				r = *(src + 0);
				g = *(src + 1);
				b = *(src + 2);
				rhist[r] ++;
				ghist[g] ++;
				bhist[b] ++;
			}
		}
	}
}
