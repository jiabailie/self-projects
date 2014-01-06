#include "stdafx.h"
//#include <stdlib.h>
#include "math.h"
#include "imageutil.h"
#include "segment.h"
#include "imageprocess.h"
#include "histogram.h"
int _mae_convert_rgb2hsi(unsigned char * h_data, unsigned char * s_data,unsigned char * i_data, MAE_image* mae_image)
{
	int error = MAE_NO_ERROR;
	unsigned char *rgbdata;
	unsigned char *rgbtemp;
	unsigned char *rgbtemp1;
	unsigned char *htemp,*stemp,*itemp;
	unsigned char *htemp1,*stemp1,*itemp1;
	int width;
	int height;
	int depth;
	int widthbytes;
	int movedbits;
	int x,y;
	unsigned char r,g,b;
	unsigned char h,s,i;
	
	width = mae_image->w;
	height = mae_image->h;
	rgbdata = mae_image->data;
	depth = mae_image->depth;
	
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);
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
		//int a = 0;
		rgbtemp1 = rgbdata + widthbytes * y;
		htemp1 = h_data + y * width;
		stemp1 = s_data + y * width;
		itemp1 = i_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			//rgbtemp = rgbdata + widthbytes * y + movedbits * x;
			//htemp = h_data + y * width + x;
			//stemp = s_data + y * width + x;
			//itemp = i_data + y * width + x;
			rgbtemp = rgbtemp1 + movedbits * x;
			htemp   = htemp1   + x;
			stemp   = stemp1   + x;
			itemp   = itemp1   + x;
			r = *(rgbtemp + 0);
			g = *(rgbtemp + 1);
			b = *(rgbtemp + 2);
			_mae_rgb2hsi(r,g,b,&h,&s,&i);
			//if(h != 0 )
			//{
			//	a = 1;
			//}
			*htemp = h;
			*stemp = s;
			*itemp = i;
		}
	}

	return error;
}
int _mae_convert_rgb2hsv(unsigned char * h_data, unsigned char * s_data,unsigned char * v_data, MAE_image* mae_image)
{
	int error = MAE_NO_ERROR;
	unsigned char *rgbdata;
	unsigned char *rgbtemp;
	unsigned char *rgbtemp1;
	unsigned char *htemp,*stemp,*itemp;
	unsigned char *htemp1,*stemp1,*itemp1;
	int width;
	int height;
	int depth;
	int widthbytes;
	int movedbits;
	int x,y;
	unsigned char r,g,b;
	unsigned char h,s,v;
	
	width = mae_image->w;
	height = mae_image->h;
	rgbdata = mae_image->data;
	depth = mae_image->depth;
	
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);
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
		//int a = 0;
		rgbtemp1 = rgbdata + widthbytes * y;
		htemp1 = h_data + y * width;
		stemp1 = s_data + y * width;
		itemp1 = v_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			//rgbtemp = rgbdata + widthbytes * y + movedbits * x;
			//htemp = h_data + y * width + x;
			//stemp = s_data + y * width + x;
			//itemp = i_data + y * width + x;
			rgbtemp = rgbtemp1 + movedbits * x;
			htemp   = htemp1   + x;
			stemp   = stemp1   + x;
			itemp   = itemp1   + x;
			r = *(rgbtemp + 0);
			g = *(rgbtemp + 1);
			b = *(rgbtemp + 2);
			_mae_rgb2hsv(r,g,b,&h,&s,&v);
			//if(h != 0 )
			//{
			//	a = 1;
			//}
			*htemp = h;
			*stemp = s;
			*itemp = v;
		}
	}

	return error;
}

int _mae_convert_rgb2gray(unsigned char * i_data, MAE_image* open_image, MAE_image* high_image)
{
	int error = MAE_NO_ERROR;
	unsigned char *rgbdata;
	unsigned char *rgbtemp;
	unsigned char *rgbtemp1;
	unsigned char *itemp;
	unsigned char *itemp1;
	int width;
	int height;
	int depth;
	int widthbytes;
	int movedbits;
	int x,y;
	unsigned char r,g,b;
	unsigned char gray;

	MAE_image out_image;
   _mae_remove_mark_color3(open_image, high_image, &out_image );

	width = out_image.w;
	height = out_image.h;
	rgbdata = out_image.data;
	depth = out_image.depth;
	
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);
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
		//int a = 0;
		rgbtemp1 = rgbdata + widthbytes * y;
		itemp1 = i_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			rgbtemp = rgbtemp1 + movedbits * x;
			itemp   = itemp1   + x;
			r = *(rgbtemp + 0);
			g = *(rgbtemp + 1);
			b = *(rgbtemp + 2);
			gray = (r+g+b)/3;
			*itemp = gray;
		}
	}

	free(out_image.data);
	return error;
}

int _mae_convert_rgb2gray2(unsigned char * i_data, MAE_image* image)
{
	int error = MAE_NO_ERROR;
	unsigned char *rgbdata;
	unsigned char *rgbtemp;
	unsigned char *rgbtemp1;
	unsigned char *itemp;
	unsigned char *itemp1;
	int width;
	int height;
	int depth;
	int widthbytes;
	int movedbits;
	int x,y;
	unsigned char r,g,b;
	unsigned char gray;

	width = image->w;
	height = image->h;
	rgbdata = image->data;
	depth = image->depth;
	
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);
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
		//int a = 0;
		rgbtemp1 = rgbdata + widthbytes * y;
		itemp1 = i_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			rgbtemp = rgbtemp1 + movedbits * x;
			itemp   = itemp1   + x;
			r = *(rgbtemp + 0);
			g = *(rgbtemp + 1);
			b = *(rgbtemp + 2);
			gray = (r+g+b)/3;
			*itemp = gray;
		}
	}

	return error;
}
void _mae_remove_gray_bg(unsigned char * data, int width, int height)
{	
	unsigned char * ptemp;
	unsigned char * ptemp1;
	unsigned char value;
	//long i;

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
			if(value == 0 )
			{
				*ptemp = 255;
			}
		}
	}

}
//
//void _mae_histogram(unsigned char * data, int width, int height, long * histogram)
//{	
//	unsigned char * ptemp;
//	unsigned char * ptemp1;
//	unsigned char value;
//	long i;
//	for ( i =0; i < 256; ++ i)
//	{
//		histogram[i] = 0;
//	}
//
//	for (int y = 0; y < height; y++)
//	{
//		ptemp1 = data;
//		ptemp1 += (height - 1 - y) * width;
//		for (int x = 0; x < width; x++)
//		{		
//			//指向源DIB第y行，第x个像素的指諄E
//			//同样要注意DIB上下倒置的问虂E
//			//ptemp = data;
//			//ptemp += (height - 1 - y) * width;
//			//ptemp += x;
//			ptemp = ptemp1 + x;
//			value = *(ptemp + 0 );
//			histogram[value]++;
//		}
//	}
//
//}


///////////////////////////////////////////////////////
// 函数脕E: GetThresholdByOTSU
// 参数		: LONG lHistogram[]    该块图像的直方图
//            LONG lWidth          该块图像的窥胰
//            LONG lHeight         该块图像的高度
// 返回值	: int  OTSU方法得到的阈值
// 功能		: 用OTSU方法得到小块灰度图的阈值
///////////////////////////////////////////////////////
int _mae_get_threshold_otsu(long histogram[], long width, long height)
{
	// 得到每个灰度级的概率，并将结果放到数组dlProbability[256]中
	double probability[256];
	int i;
	for (i = 0; i < 256; ++ i)
	{
		probability[i] = (double)histogram[i] / (height * width);
	}
	
	// 得到数学期蛠E
	double etotal = 0;
	for (i = 0; i < 256; ++ i)
	{
		etotal += (i + 1) * probability[i];
	}
	
	// 取i=0,1,2,3……254，对每个灰度级得到一个值，并放到数讈E
	// compare[255]中
	double compare[255];
	
	double ei = 0;
	double pi = 0;
	for (i = 0; i < 255; ++ i)
	{		
		ei += (i + 1) * probability[i];
		pi += probability[i];
		
		if (1 == pi || 0 == pi)
		{
			compare[i] = 0;
		}
		else
		{
			compare[i] = pow((etotal * pi - ei), 2) / (pi * 
				(1 - pi));
		}
	}
	
	int threshold = 0;
	for (i = 1; i < 255; ++ i)
	{
		if (compare[threshold] < compare[i])
		{
			threshold = i;
		}
	}
	return threshold;
}

int _mae_segmention_by_threshold(unsigned char *in_data, int width, int height,long nThreshold, unsigned char * out_data)
{
	unsigned char * p_old_data;
	unsigned char * p_new_data;
	unsigned char * p_old_temp;
	unsigned char * p_new_temp;
	unsigned char gray;
	int x,y;

	//memset(out_data, (unsigned char)0, width * height );

	//每行
	for(y = 0; y < height; y++)
	{
		p_new_temp =  out_data + (height - 1 - y) * width;
		p_old_temp = in_data + (height - 1 - y) * width;
		for(x = 0; x < width; x++)
		{
			//p_new_data =  out_data + (height - 1 - y) * width + x;
			//p_old_data = in_data + (height - 1 - y) * width + x;
			p_new_data = p_new_temp + x;
			p_old_data = p_old_temp + x;

			gray = *p_old_data;

			if( gray < nThreshold )
			{
				*p_new_data = 0;
			}
			else
			{
				*p_new_data = 1;
			}
		}
	}

	return 0;
}
int _mae_segmention_by_threshold2(unsigned char *in_data, int width, int height,long nThreshold, unsigned char * out_data)
{
	unsigned char * p_old_data;
	unsigned char * p_new_data;
	unsigned char * p_old_temp;
	unsigned char * p_new_temp;
	unsigned char gray;
	int x,y;

	//memset(out_data, (unsigned char)0, width * height );

	//每行
	for(y = 0; y < height; y++)
	{
		p_new_temp =  out_data + (height - 1 - y) * width;
		p_old_temp = in_data + (height - 1 - y) * width;
		for(x = 0; x < width; x++)
		{
			//p_new_data =  out_data + (height - 1 - y) * width + x;
			//p_old_data = in_data + (height - 1 - y) * width + x;
			p_new_data = p_new_temp + x;
			p_old_data = p_old_temp + x;

			gray = *p_old_data;

			if( gray < nThreshold )
			{
				*p_new_data = 1;
			}
			else
			{
				*p_new_data = 0;
			}
		}
	}

	return 0;
}

int _mae_segment(unsigned char *in_data, int width, int height, unsigned char *out_data)
{
	long hist[256];
	int threshold;
	int error = 0;
	for(int i = 0; i < 256; i ++ )
	{
		hist[i] = 0;
	}
	//remove gray background color
	//remove_gray_bg(in_data, width, height);
	_mae_histogram(in_data, width, height, hist);

	threshold = _mae_get_threshold_otsu(hist, width, height);

	error = _mae_segmention_by_threshold(in_data, width, height,threshold, out_data);

	//unsigned char * _tmpdata;
	//_tmpdata = _mae_mallocate_image(width,height,8);
	//int   mode = 2;
	//int stru_size3=3;
	//int structure3[3][3]={ {-1, 0, -1},
 //                          {0, 0, 0},
 //                         {-1, 0, -1}
 //                       };
	//int stru_size5=5;
	//int structure5[5][5]={  {-1, -1, 0, -1, -1},
 //                          {-1,   0, 0, 0, -1},
 //                          {0,   0, 0,  0, 0},
 //                          {-1,   0, 0, 0, -1},
	//					   {-1, -1, 0, -1, -1}
 //                       };

	//_mae_erosion(out_data, width, height, mode, (int**)structure3, stru_size3,_tmpdata);
	//_mae_dilation(_tmpdata, width, height, mode, (int**)structure5, stru_size5, out_data);
	//free(_tmpdata);

	return error;

}

int _mae_detect_high_color(MAE_image *in_image, MAE_image * out_image, int distance)
{
	int error = MAE_NO_ERROR;
	int depth;
	int width;
	int height;
	int movedbits;
	long widthbytes;
	int x,y;
	unsigned char *src;
	unsigned char *dst;
	unsigned char *srctmp;
	unsigned char *dsttmp;
	unsigned char r,g,b;
	int varience;

	out_image->depth = in_image->depth;
	out_image->w = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = in_image->stride ;
	out_image->type = in_image->type;
	
	out_image->data  = _mae_mallocate_image(in_image->w,in_image->h,in_image->depth);

	depth = in_image->depth;
	width = in_image->w;
	height = in_image->h;
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);
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
		dsttmp = out_image->data + widthbytes * y;
		srctmp = in_image->data  + widthbytes * y;
		for(x = 0; x < width; x ++ )
		{
			dst = dsttmp + movedbits * x;
			src = srctmp + movedbits * x;
			if( movedbits == 1 )
			{
				*dst = *src;
			}else{
				r = *(src + 0);
				g = *(src + 1);
				b = *(src + 2);
				//r=g=b图像专为全灰
				varience = _mae_rgb_variance(r,g,b);
				if(varience > distance )
				{
					*(dst + 0 ) = *(src + 0);
					*(dst + 1 ) = *(src + 1);
					*(dst + 2 ) = *(src + 2);
				}else{
					*(dst + 0 ) = 255;
					*(dst + 1 ) = 255;
					*(dst + 2 ) = 255;
				}
			}
		}
	}

	//return 0;	
	return error;
}
//
//int _mae_segment_rgb(MAE_image *in_image, MAE_image * out_image)
//{
//	int error = MAE_NO_ERROR;
//	int depth;
//	int width;
//	int height;
//	int movedbits;
//	long widthbytes;
//	int x,y;
//	unsigned char *src;
//	unsigned char *dst;
//	unsigned char *srctmp;
//	unsigned char *dsttmp;
//	unsigned char r,g,b;
//	int varience;
//
//	out_image->depth = in_image->depth;
//	out_image->w = in_image->w;
//	out_image->h = in_image->h;
//	out_image->stride = in_image->stride ;
//	out_image->type = in_image->type;
//	
//	out_image->data  = _mae_mallocate_image(in_image->w,in_image->h,in_image->depth);
//
//	depth = in_image->depth;
//	width = in_image->w;
//	height = in_image->h;
//	movedbits = 1;
//	widthbytes = _mae_width_bytes(depth, width);
//	switch( depth )
//	{
//	case 8:	
//		movedbits = 1;
//		break;
//	case 16:
//		movedbits = 2;
//		break;
//	case 24:
//		movedbits = 3;
//		break;
//	case 32:
//		movedbits = 4;
//		break;
//	}
//
//	for(y = 0; y < height; y ++ )
//	{
//		dsttmp = out_image->data + widthbytes * y;
//		srctmp = in_image->data  + widthbytes * y;
//		for(x = 0; x < width; x ++ )
//		{
//			dst = dsttmp + movedbits * x;
//			src = srctmp + movedbits * x;
//			if( movedbits == 1 )
//			{
//				*dst = *src;
//			}else{
//				r = *(src + 0);
//				g = *(src + 1);
//				b = *(src + 2);
//				varience = _mae_rgb_variance(r,g,b);
//				if(varience > RGB_SEGMENT_VARIENCE )
//				{
//					*(dst + 0 ) = *(src + 0);
//					*(dst + 1 ) = *(src + 1);
//					*(dst + 2 ) = *(src + 2);
//				}else{
//					*(dst + 0 ) = 255;
//					*(dst + 1 ) = 255;
//					*(dst + 2 ) = 255;
//				}
//			}
//		}
//	}
//
//	return 0;	
//	return error;
//}
//
//int _mae_remove_high_color(MAE_image *in_image, MAE_image * out_image)
//{
//	int error = MAE_NO_ERROR;
//	int depth;
//	int width;
//	int height;
//	int movedbits;
//	long widthbytes;
//	int x,y;
//	unsigned char *src;
//	unsigned char *dst;
//	unsigned char *srctmp;
//	unsigned char *dsttmp;
//	unsigned char r,g,b;
//	int varience;
//
//	out_image->depth = in_image->depth;
//	out_image->w = in_image->w;
//	out_image->h = in_image->h;
//	out_image->stride = in_image->stride ;
//	out_image->type = in_image->type;
//	
//	out_image->data  = _mae_mallocate_image(in_image->w,in_image->h,in_image->depth);
//
//	depth = in_image->depth;
//	width = in_image->w;
//	height = in_image->h;
//	movedbits = 1;
//	widthbytes = _mae_width_bytes(depth, width);
//	switch( depth )
//	{
//	case 8:	
//		movedbits = 1;
//		break;
//	case 16:
//		movedbits = 2;
//		break;
//	case 24:
//		movedbits = 3;
//		break;
//	case 32:
//		movedbits = 4;
//		break;
//	}
//
//	for(y = 0; y < height; y ++ )
//	{
//		dsttmp = out_image->data + widthbytes * y;
//		srctmp = in_image->data  + widthbytes * y;
//		for(x = 0; x < width; x ++ )
//		{
//			dst = dsttmp + movedbits * x;
//			src = srctmp + movedbits * x;
//			if( movedbits == 1 )
//			{
//				*dst = *src;
//			}else{
//				r = *(src + 0);
//				g = *(src + 1);
//				b = *(src + 2);
//				varience = _mae_rgb_variance(r,g,b);
//				if(varience < RGB_REMOVE_VARIENCE )
//				{
//					*(dst + 0 ) = *(src + 0);
//					*(dst + 1 ) = *(src + 1);
//					*(dst + 2 ) = *(src + 2);
//				}else{
//					*(dst + 0 ) = 255;
//					*(dst + 1 ) = 255;
//					*(dst + 2 ) = 255;
//				}
//			}
//		}
//	}
//
//	return 0;	
//	return error;
//}

int	_mae_segment_gray(unsigned char *in_data, int width, int height, unsigned char *out_data)
{
	long hist[256];
	int threshold;
	int error = 0;
	for(int i = 0; i < 256; i ++ )
	{
		hist[i] = 0;
	}
	_mae_histogram(in_data, width, height, hist);

	//otsu分割
	threshold = _mae_get_threshold_otsu(hist, width, height);

	error = _mae_segmention_by_threshold2(in_data, width, height,threshold, out_data);

	//unsigned char * _tmpdata;
	//_tmpdata = _mae_mallocate_image(width,height,8);
//	int   mode = 2;
	//int stru_size=3;
	//int structure[3][3]={ {-1, 0, -1},
 //                          {0, 0, 0},
 //                         {-1, 0, -1}
 //                       };
	//int stru_size=5;
	//int structure[5][5]={  {-1, -1, 0, -1, -1},
 //                          {-1,   0, 0, 0, -1},
 //                          {0,   0, 0,  0, 0},
 //                          {-1,   0, 0, 0, -1},
	//					   {-1, -1, 0, -1, -1}
 //                       };

	//_mae_erosion(out_data, width, height, mode, (int**)structure, stru_size,_tmpdata);
	//_mae_dilation(_tmpdata, width, height, mode, (int**)structure, stru_size, out_data);

	//free(_tmpdata);

	return error;
}

int	_mae_segment_grayforchecktext(unsigned char *in_data, int width, int height, unsigned char *out_data)
{
	unsigned char * p_old_data;
	unsigned char * p_new_data;
	unsigned char * p_old_temp;
	unsigned char * p_new_temp;
	unsigned char gray;
	int x,y;

	//memset(out_data, (unsigned char)0, width * height );

	//每行
	for(y = 0; y < height; y++)
	{
		p_new_temp =  out_data + (height - 1 - y) * width;
		p_old_temp = in_data + (height - 1 - y) * width;
		for(x = 0; x < width; x++)
		{
			p_new_data = p_new_temp + x;
			p_old_data = p_old_temp + x;

			gray = *p_old_data;

			if( gray < 240 )
			{
				*p_new_data = 1;
			}
			else
			{
				*p_new_data = 0;
			}
		}
	}

	return 0;
}


int _mae_remove_mark_color3(MAE_image *open_image, MAE_image *high_image, MAE_image *out_image )
{
	int error = MAE_NO_ERROR;

	int depth;
	int width;
	int height;
	int movedbits;
	long widthbytes;
	int x,y;
	unsigned char *highsrc;
	unsigned char *opensrc;
	unsigned char *dst;
	unsigned char *highsrctmp;
	unsigned char *opensrctmp;
	unsigned char *dsttmp;
	//unsigned char r,g,b;
	int red,green,blue;
	
	out_image->depth = open_image->depth;
	out_image->w = open_image->w;
	out_image->h = open_image->h;
	out_image->stride = open_image->stride ;
	out_image->type = open_image->type;
	
	out_image->data  = _mae_mallocate_image(open_image->w,open_image->h,open_image->depth);


	depth = out_image->depth;
	width = out_image->w;
	height = out_image->h;
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);


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
		dsttmp = out_image->data + widthbytes * y;
		opensrctmp = open_image->data  + widthbytes * y;
		highsrctmp = high_image->data  + widthbytes * y;
		for(x = 0; x < width; x ++ )
		{
			dst = dsttmp + movedbits * x;
			opensrc = opensrctmp + movedbits * x;
			highsrc = highsrctmp + movedbits * x;
			
			red = *(highsrc + 0 );
			green = *(highsrc + 1 );
			blue = *(highsrc + 2 );
			if( red != 255 || green != 255 && blue != 255 )
			{
				*(dst + 0 ) = 255;
				*(dst + 1 ) = 255;
				*(dst + 2 ) = 255;
			}else{
				*(dst + 0 ) = *(opensrc + 0 );
				*(dst + 1 ) = *(opensrc + 1 );
				*(dst + 2 ) = *(opensrc + 2 );
			}
		}
	}
	return error;

}

int _mae_remove_mark_color2(MAE_image *out_image, unsigned char *in_data )
{
	int error = MAE_NO_ERROR;

	int depth;
	int width;
	int height;
	int movedbits;
	long widthbytes;
	int x,y;
	unsigned char *src;
	unsigned char *dst;
	unsigned char *srctmp;
	unsigned char *dsttmp;
//	unsigned char r,g,b;
	int color;
	

	depth = out_image->depth;
	width = out_image->w;
	height = out_image->h;
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);


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
		dsttmp = out_image->data + widthbytes * y;
		srctmp = in_data  + width * y;
		for(x = 0; x < width; x ++ )
		{
			dst = dsttmp + movedbits * x;
			src = srctmp +  x;
			color = *src;
			if( color == 1)
			{
				*(dst + 0 ) = 255;
				*(dst + 1 ) = 255;
				*(dst + 2 ) = 255;
			}
		}
	}
	return error;

}


//int _mae_remove_mark_color(MAE_image *io_image, MAE_image *h_image, MAE_region * regions, int nregions)
int _mae_remove_mark_color(MAE_image *io_image, MAE_region * regions, int nregions)
{
	int error = MAE_NO_ERROR;
	int i;
//	int idx;
//	MAE_region region;
//	int row_width1;
///	int row_width2;
//	int col_width1;
//	int col_width2;
	
	//int step;
	//step = 5;
	for(i = 0; i < nregions; i ++ )
	{
		//do not show region deleted
		if( regions[i].label == -1 ) continue;
		//_mae_remove_region_high_color(io_image, h_image, &regions[i]);
		_mae_remove_region_high_color(io_image, &regions[i]);
	}
	return error;
}

//void _mae_remove_region_high_color(MAE_image *io_image, MAE_image *h_image, MAE_region* region)
void _mae_remove_region_high_color(MAE_image *io_image,MAE_region* region)
{
	MAE_image h_image; 
	MAE_image region_image; 

//	unsigned char * h_data;
//	unsigned char * s_data;
//	unsigned char * i_data;
	unsigned char * gray_data;
	unsigned char * bi_data;
//	long mk_pix_num = 0;
//	long mt_pix_num = 0;
	int width;
	int height;

	//get region image
	//_mae_get_region_image(h_image, region->l, region->t, region->w, region->h, &region_image );

	_mae_get_region_image(io_image, region->l, region->t, region->w, region->h, &h_image );
//	_mae_get_region_image(io_image, region->l, region->t, region->w, region->h, &region_image );
	////detect hight color
	_mae_detect_high_color(&h_image, &region_image, RGB_SEGMENT_VARIENCE);
//	free(tmp_region_image.data);
	//_mae_get_region_image(&high_image, region->l, region->t, region->w, region->h, &region_image );
	//free(high_image.data);

	 //_mae_save_image("test_region.bmp", &region_image );
////////////////////////test usage start
	 //mae_save_image("test0.bmp", &region_image );
////////////////////////test usage end

	width = region->w;
	height = region->h;
	//h_data = _mae_mallocate_image(width,height,8);
	//s_data = _mae_mallocate_image(width,height,8);
	//i_data = _mae_mallocate_image(width,height,8);
	gray_data = _mae_mallocate_image(width,height,8);
	bi_data = _mae_mallocate_image(width,height,8);
	//
	//_mae_convert_rgb2hsi(h_data, s_data, i_data, &region_image);
	_mae_convert_rgb_to_gray(gray_data, &region_image);
	//_mae_convert_mark_to_gray(gray_data, &region_image);
////////////////////////test usage start
	//MAE_image testimage;
	//_mae_save_gray_image(&testimage, gray_data, width, height, &region_image );
	// _mae_save_image("test1.bmp", &testimage );
	// free(testimage.data);
////////////////////////test usage end
	 //20100709 for histgram mark pixs number start for detect color image region
//	_mae_histogram_mark_pixnum(gray_data, width, height, &mk_pix_num);
//	region->mk_pix_num = mk_pix_num;
	 //20100709 for histgram mark pixs number end
	//_mae_segment_gray(gray_data, width, height, bi_data);
	_mae_segment_only_mark_pixs(gray_data, width, height, bi_data, 1);
////////////////////////test usage start
	//_mae_save_segment_image(&testimage, bi_data, width, height, &region_image );
	// _mae_save_image("test2.bmp", &testimage );
	// free(testimage.data);

////////////////////////test usage end
	 //20100709 for histgram mark pixs number start for detect color image region
//	_mae_histogram_text_pixnum(bi_data, width, height, &mt_pix_num);
//	region->mt_pix_num = mt_pix_num;
	 //20100709 for histgram mark pixs number end
	
	//gray_data minus bi_data
	_mae_minus_image(bi_data, width, height, gray_data);

////////////////////////test usage start
	//_mae_save_segment_image(&testimage, bi_data, width, height, &region_image );
	// _mae_save_image("test3.bmp", &testimage );
	// free(testimage.data);
////////////////////////test usage end
	//MAE_image testimage;
	//_mae_save_segment_image(&testimage,bi_data, width, height, &region_image );
	// mae_save_image("test1.bmp", &testimage );
	 //free(testimage.data);
	//second segment
	//_mae_segment(gray_data, width, height, bi_data);
	//_mae_save_segment_image(&testimage,bi_data, width, height, &region_image );
	 //mae_save_image("test2.bmp", &testimage );
	 //free(testimage.data);

	//test start
////////////////////////test usage start
	//_mae_save_segment_image(&testimage, bi_data, width, height, &region_image );
	// mae_save_image("test4.bmp", &testimage );
	// free(testimage.data);
////////////////////////test usage end
	//test end
	_mae_set_region_to_image2(io_image, region, bi_data, &h_image);

	free(region_image.data );
	//free(h_data);
	//free(s_data);
	//free(i_data);
	free(gray_data);
	free(bi_data);
	free(h_image.data);
	return ;
}

int _mae_convert_rgb_to_gray(unsigned char * out_data, MAE_image* in_image)
{
	int error = MAE_NO_ERROR;
	unsigned char *rgbdata;
	unsigned char *rgbtemp;
	unsigned char *rgbtemp1;
	unsigned char *itemp;
	unsigned char *itemp1;
	int width;
	int height;
	int depth;
	int widthbytes;
	int movedbits;
	int x,y;
	unsigned char r,g,b;
	unsigned char gray;


	width = in_image->w;
	height = in_image->h;
	rgbdata = in_image->data;
	depth = in_image->depth;
	
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);
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
		//int a = 0;
		rgbtemp1 = rgbdata + widthbytes * y;
		itemp1 = out_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			rgbtemp = rgbtemp1 + movedbits * x;
			itemp   = itemp1   + x;
			r = *(rgbtemp + 0);
			g = *(rgbtemp + 1);
			b = *(rgbtemp + 2);
			gray = (r+g+b)/3;
			*itemp = gray;
		}
	}

	return error;
}

int	_mae_segment_only_mark_pixs(unsigned char *in_data, int width, int height, unsigned char *out_data, int flag)
{
	long hist[256];
	int threshold;
	int error = 0;
	long pixs = 0;

	for(int i = 0; i < 256; i ++ )
	{
		hist[i] = 0;
	}
	_mae_histogram_only_mark_pixs(in_data, width, height, hist, &pixs, flag);
	threshold = _mae_get_threshold_otsu_only_mark(hist, pixs);
	
	//threshold = _mae_get_threshold_otsu(hist, width, height);
	//_mae_histogram(in_data, width, height, hist);

	error = _mae_segmention_by_threshold2(in_data, width, height,threshold, out_data);

	return error;
}

int _mae_get_threshold_otsu_only_mark(long histogram[], long pixs)
{
	// 得到每个灰度级的概率，并将结果放到数组dlProbability[256]中
	double probability[256];
	int i;

	for (i = 0; i < 256; ++ i)
	{
		probability[i] = (double)histogram[i] / (double)(pixs);
	}

	// 得到数学期蛠E
	double etotal = 0;
	for (i = 0; i < 256; ++ i)
	{
		etotal += (i + 1) * probability[i];
	}
	
	// 取i=0,1,2,3……254，对每个灰度级得到一个值，并放到数讈E
	// compare[255]中
	double compare[255];
	
	double ei = 0;
	double pi = 0;
	for (i = 0; i < 255; ++ i)
	{		
		ei += (i + 1) * probability[i];
		pi += probability[i];
		
		if (1 == pi || 0 == pi)
		{
			compare[i] = 0;
		}
		else
		{
			compare[i] = pow((etotal * pi - ei), 2) / (pi * 
				(1 - pi));
		}
	}
	
	int threshold = 0;
	for (i = 1; i < 255; ++ i)
	{
		if (compare[threshold] < compare[i])
		{
			threshold = i;
		}
	}

	//if( etotal < threshold )
	//{
	//	threshold =(int)etotal;
	//}
	return threshold;
}

void _mae_minus_image(unsigned char *dst_data, int width, int height, unsigned char *src_data )
{
	unsigned char * out_data;
	unsigned char * in_data;
	unsigned char * out_temp;
	unsigned char * in_temp;
	unsigned char gray;
	int x,y;

	//每行
	for(y = 0; y < height; y++)
	{
		out_data =  dst_data + y * width;
		in_data  =  src_data +  y * width;
		for(x = 0; x < width; x++)
		{
			out_temp = out_data + x;
			in_temp = in_data + x;

			gray = *in_temp;

			if( gray == 255 )
			{
				*out_temp = 1;
			}
		}
	}
}

void _mae_plus_image(unsigned char *dst_data, int width, int height, unsigned char *src_data )
{
	unsigned char * out_data;
	unsigned char * in_data;
	unsigned char * out_temp;
	unsigned char * in_temp;
	unsigned char gray;
	int x,y;

	memset(dst_data, (unsigned char)0, width * height );
	//每行
	for(y = 0; y < height; y++)
	{
		out_data =  dst_data + y * width;
		in_data  =  src_data +  y * width;
		for(x = 0; x < width; x++)
		{
			out_temp = out_data + x;
			in_temp = in_data + x;

			gray = *in_temp;

			if( gray != 0 )
			{
				*out_temp = 1;
			}
		}
	}
}
//
//int _mae_save_gray_image(MAE_image *out_image,unsigned char *in_data, int w, int h, MAE_image *in_image )
//{
//	int error = MAE_NO_ERROR;
//	out_image->depth = in_image->depth;
//	out_image->w = in_image->w;
//	out_image->h = in_image->h;
//	out_image->stride = in_image->stride ;
//	out_image->type = in_image->type;
//	
//	out_image->data  = _mae_mallocate_image(in_image->w,in_image->h,in_image->depth);
//	error = _mae_set_gray_to_image(in_data, w, h, out_image);
//	return error;
//}
//
//int _mae_set_gray_to_image(unsigned char* in_data,int width, int height, MAE_image *mae_image)
//{
//	unsigned char *mae_data;
//	int depth;
//	int movedbits;
//	long widthbytes;
//	int x,y;
//	unsigned char *src;
//	unsigned char *dst;
//	unsigned char *srctmp;
//	unsigned char *dsttmp;
//
//	unsigned char gray;
//	depth = mae_image->depth;
//	mae_data = mae_image->data;
//	
//	movedbits = 1;
//	widthbytes = _mae_width_bytes(depth, width);
//	switch( depth )
//	{
//	case 8:	
//		movedbits = 1;
//		break;
//	case 16:
//		movedbits = 2;
//		break;
//	case 24:
//		movedbits = 3;
//		break;
//	case 32:
//		movedbits = 4;
//		break;
//	}
//	for(y = 0; y < height; y ++ )
//	{
//		dsttmp = mae_data + widthbytes * y;
//		srctmp = in_data + y * width;
//		for(x = 0; x < width; x ++ )
//		{
//			dst = dsttmp + movedbits * x;
//			src = srctmp + x;
//			//dst = mae_data + widthbytes * y + movedbits * x;
//			//src = in_data + y * width + x;
//			gray = *src; 
//			if( movedbits == 1 )
//			{
//				*dst = gray;
//			}else{
//				*(dst + 0) = gray;
//				*(dst + 1) = gray;
//				*(dst + 2) = gray;
//			}
//		}
//	}
//
//	return 0;
//}

int _mae_segmention_from_high_color(MAE_image *h_image, unsigned char * out_data)
{
	int error = MAE_NO_ERROR;
	unsigned char *rgbdata;
	unsigned char *rgbtemp;
	unsigned char *rgbtemp1;
	unsigned char *otemp;
	unsigned char *otemp1;
	int width;
	int height;
	int depth;
	int widthbytes;
	int movedbits;
	int x,y;
	unsigned char r,g,b;
	
	width = h_image->w;
	height = h_image->h;
	rgbdata = h_image->data;
	depth = h_image->depth;
	
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, width);
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
		//int a = 0;
		rgbtemp1 = rgbdata + widthbytes * y;
		otemp1 = out_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			rgbtemp = rgbtemp1 + movedbits * x;
			otemp   = otemp1   + x;
			if( movedbits == 3 )
			{
				r = *(rgbtemp + 0);
				g = *(rgbtemp + 1);
				b = *(rgbtemp + 2);
			}else{
				r = *(rgbtemp + 0);
				g = *(rgbtemp + 0);
				b = *(rgbtemp + 0);
			}
			if(r == 255 && g == 255 && b == 255 )
			{
				*otemp = 0;
			}else{
				*otemp = 1;
			}
		}
	}

	return error;
}
