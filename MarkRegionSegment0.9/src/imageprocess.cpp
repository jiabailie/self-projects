#include "stdafx.h"
//#include <stdlib.h>
#include "imageprocess.h"
#include "imageutil.h"
#include "zoom.h"
#include "memalloc.h"
#include "maedefs.h"
#include "histogram.h"

#ifdef MAE_USE_GDIPLUS
#include   <Objbase.h>  
#include   <Gdiplus.h>
#include   <GdiPlusEnums.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	GdiplusStartupInput gdiplusStartupInput;	
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure
	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			return j;  // Success
		}    
	} // for

	GdiplusShutdown(gdiplusToken);

	return -1;  // Failure
}

#endif

int _mae_open_image_bitmap(unsigned char * data, int width, int height, MAE_image* mae_image)
{
	int error = MAE_NO_ERROR;
	int depth = 24;
	//int i;
	//	unsigned char * temp1;
	//	unsigned char * temp2;
	int rowsize;
	mae_image->depth = depth;
	mae_image->w  = width;
	mae_image->h  = height;
	mae_image->stride  = _mae_width_bytes(depth, width);
	mae_image->data = _mae_mallocate_image(width,height,depth);
	//memcpy(mae_image->data, data, 	_mae_sizefromdepth(width, height, depth));
	rowsize = _mae_width_bytes(depth, width );
	//for(i = 0; i < height; i ++ )
	//{
	//	temp1 = mae_image->data + (height - i - 1) * rowsize;
	//	temp2 = data + i * rowsize;
	//	memcpy(temp1, temp2, rowsize );
	//}
	memcpy(mae_image->data, data, rowsize * height);
	//_mae_inverse_image(data, width, height, rowsize, mae_image->data);

	return error;
}


int _mae_open_image(const char* file_name, MAE_image* mae_image)
{
	int error = MAE_NO_ERROR;
#ifdef MAE_USE_GDIPLUS

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput; 
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 

	WCHAR w_file_name[1024];
	memset(w_file_name, 0, 1024);
	//LPCSTR w_file_name;
	long  len = (long)strlen( file_name ) + 1;
	//  
	int wLen;
	wLen = MultiByteToWideChar( 
		CP_ACP, 0, file_name, len, 
		w_file_name,  len);

	Bitmap *bitmap = Bitmap::FromFile(w_file_name);
	//Bitmap bitmap = Bitmap(w_file_name);
	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();
	if(width == 0 || height == 0 )
	{
		delete(bitmap);
		GdiplusShutdown(gdiplusToken); 
		return MAE_OPEN_IMAGE_FAIL ;
	}
	//20100901 add start
	mae_image->xdpi = (int)bitmap->GetHorizontalResolution();
	mae_image->ydpi = (int)bitmap->GetVerticalResolution();
	if(mae_image->xdpi == 0 )
	{
		mae_image->xdpi = 96;
	}
	if(mae_image->ydpi == 0 )
	{
		mae_image->ydpi = 96;
	}
	//20100901 add end

	Rect rect;
	rect.X = 0;
	rect.Y = 0;
	rect.Width = width;
	rect.Height = height;

	BitmapData bmpData;
	bitmap->LockBits(&rect,ImageLockModeRead, PixelFormat24bppRGB,&bmpData);
	unsigned char* pBuf = (unsigned char*)bmpData.Scan0;
	error = _mae_open_image_bitmap( pBuf, width, height, mae_image);
	bitmap->UnlockBits(&bmpData);
	delete(bitmap);
	GdiplusShutdown(gdiplusToken); 
#else
	FILE* hFile;	//file handle to read the image

	if ((hFile=fopen(file_name,"rb"))==NULL){
		return MAE_FILE_NOT_EXIST;
	}

	error = _mae_decode_bmp( hFile, mae_image);
	fclose(hFile);

#endif
	return error;
}


int _mae_decode_bmp(FILE * fp, MAE_image* mae_image)
{
	unsigned char * outdata;

	int error = MAE_NO_ERROR;
	if (fp == NULL) {
		return MAE_FILE_NOT_EXIST;
	}

	BITMAPFILEHEADER   bf;
	DWORD off = ftell(fp);


	if(fread(&bf, min(14,sizeof(bf)), 1, fp)==0){
		return MAE_FILE_UNSUPPORT_FORMAT;
	}
	if (bf.bfType != BFT_BITMAP) { //do we have a RC HEADER?
		bf.bfOffBits = 0L;
		fseek(fp,off,SEEK_SET);
	}

	BITMAPINFOHEADER bmpHeader;
	if (_mae_read_bitmap_info(fp,&bmpHeader) != MAE_NO_ERROR ){
		return MAE_READ_IMAGE_FAIL;
	}
	DWORD dwCompression=bmpHeader.biCompression;
	DWORD dwBitCount=bmpHeader.biBitCount; //preserve for BI_BITFIELDS compression <Thomas Ernst>
	bool bIsOldBmp;
	bIsOldBmp = bmpHeader.biSize == sizeof(BITMAPCOREHEADER);


	bool bTopDownDib = bmpHeader.biHeight<0; //<Flanders> check if it's a top-down bitmap
	if (bTopDownDib) bmpHeader.biHeight=-bmpHeader.biHeight;

	//20100901 add start
	mae_image->xdpi = bmpHeader.biXPelsPerMeter;
	mae_image->ydpi = bmpHeader.biYPelsPerMeter;
	if(mae_image->xdpi == 0 )
	{
		mae_image->xdpi = 96;
	}
	if(mae_image->ydpi == 0 )
	{
		mae_image->ydpi = 96;
	}
	//20100901 add end


	//if (info.nEscape == -1) {
	//	// Return output dimensions only
	//	head.biWidth = bmpHeader.biWidth;
	//	head.biHeight = bmpHeader.biHeight;
	//	throw "output dimensions returned";
	//}
	// Make sure bits per pixel is valid

	//if (!Create(bmpHeader.biWidth,bmpHeader.biHeight,bmpHeader.biBitCount,CXIMAGE_FORMAT_BMP))
	//	throw "Can't allocate memory";

	outdata = _mae_mallocate_image(bmpHeader.biWidth,bmpHeader.biHeight,bmpHeader.biBitCount);

	//head.biXPelsPerMeter = bmpHeader.biXPelsPerMeter;
	//head.biYPelsPerMeter = bmpHeader.biYPelsPerMeter;
	//info.xDPI = (long) floor(bmpHeader.biXPelsPerMeter * 254.0 / 10000.0 + 0.5);
	//info.yDPI = (long) floor(bmpHeader.biYPelsPerMeter * 254.0 / 10000.0 + 0.5);

	//if (info.nEscape) throw "Cancelled"; // <vho> - cancel decoding
	////////
	//	
	//RGBQUAD *pRgb  = GetPalette();
	//     if (pRgb){
	//       if (bIsOldBmp){
	//            // convert a old color table (3 byte entries) to a new
	//            // color table (4 byte entries)
	//           hFile->Read((void*)pRgb,DibNumColors(&bmpHeader) * sizeof(RGBTRIPLE),1);
	//           for (int i=DibNumColors(&head)-1; i>=0; i--){
	//               pRgb[i].rgbRed      = ((RGBTRIPLE *)pRgb)[i].rgbtRed;
	//               pRgb[i].rgbBlue     = ((RGBTRIPLE *)pRgb)[i].rgbtBlue;
	//               pRgb[i].rgbGreen    = ((RGBTRIPLE *)pRgb)[i].rgbtGreen;
	//               pRgb[i].rgbReserved = (BYTE)0;
	//           }
	//       } else {
	//           hFile->Read((void*)pRgb,DibNumColors(&bmpHeader) * sizeof(RGBQUAD),1);
	//		//force rgbReserved=0, to avoid problems with some WinXp bitmaps
	//		for (unsigned int i=0; i<head.biClrUsed; i++) pRgb[i].rgbReserved=0;
	//       }
	//   }

	//	if (info.nEscape) throw "Cancelled"; // <vho> - cancel decoding

	switch (dwBitCount) {
		case 24 :
			if (bf.bfOffBits != 0L) {
				fseek(fp, off + bf.bfOffBits, SEEK_SET);
			}
			if (dwCompression == BI_RGB){
				fread(outdata, bmpHeader.biSizeImage,1,fp); //read in the pixels
			} else {
				error = MAE_FILE_UNSUPPORT_FORMAT;
				free(outdata);
				return error;
			}
			break;
		default:
			error = MAE_FILE_UNSUPPORT_FORMAT;
			free(outdata);
			return error;
			break;
	}


	mae_image->depth = bmpHeader.biBitCount;
	mae_image->w  = bmpHeader.biWidth;
	mae_image->h  = bmpHeader.biHeight;
	mae_image->stride  = _mae_width_bytes(bmpHeader.biBitCount, bmpHeader.biWidth);
	mae_image->data  = outdata;

	return error;
}

int  _mae_read_bitmap_info(FILE* fp, BITMAPINFOHEADER *bmpinfo)
{
	int error = MAE_NO_ERROR;

	if ((fp == NULL)||(bmpinfo==NULL)){
		return MAE_FILE_NOT_EXIST;
	}

	if (fread(bmpinfo,sizeof(BITMAPINFOHEADER),1,fp)==0){
		return MAE_FILE_NOT_EXIST;
	}

	switch (bmpinfo->biSize) // what type of bitmap info is this?
	{
	case sizeof(BITMAPINFOHEADER):
		break;

	case 64: //sizeof(OS2_BMP_HEADER):
		fseek(fp, (long)(64 - sizeof(BITMAPINFOHEADER)), SEEK_CUR);
		break;
	default:
		return MAE_FILE_NOT_EXIST;
	}
	bmpinfo->biSizeImage = _mae_sizefromdepth(bmpinfo->biWidth,bmpinfo->biHeight,bmpinfo->biBitCount);
	return error;
}


int _mae_save_image(const char* file_name, const MAE_image* src)
{
	int error = MAE_NO_ERROR;
#ifdef MAE_USE_GDIPLUS

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput; 
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 

	WCHAR w_file_name[1024];
	memset(w_file_name, 0, 1024);
	long  len = (long)strlen( file_name ) + 1;
	int wLen;
	wLen = MultiByteToWideChar( 
		CP_ACP, 0, file_name, len, 
		w_file_name,  len);

	Bitmap *  bitmap=new Bitmap(src->w,src->h,PixelFormat24bppRGB);

	//20100901 add
	bitmap->SetResolution((Gdiplus::REAL)src->xdpi,(Gdiplus::REAL)src->ydpi);
	//
	Rect rect;
	rect.X = 0;
	rect.Y = 0;
	rect.Width = src->w;
	rect.Height = src->h;
	BitmapData bmpData;
	bitmap->LockBits(&rect,ImageLockModeRead, PixelFormat24bppRGB,&bmpData);

	unsigned char* pBuf = (unsigned char*)bmpData.Scan0;

	memcpy(pBuf,src->data, src->h * src->stride );
	//_mae_inverse_image(src->data, src->w, src->h, src->stride, pBuf);


	CLSID clsid;
	if( GetEncoderClsid(L"image/jpeg", &clsid) == -1 )
	{
		error = MAE_SAVE_IMAGE_FAIL;
		return error;
	}

	//LONG lQuality = 80;
	//EncoderParameters params; 
	//params.Parameter[0].Guid = EncoderQuality;
	//params.Parameter[0].NumberOfValues = 1;
	//params.Parameter[0].Type = EncoderParameterValueTypeLong;
	//params.Parameter[0].Value = (VOID*) &lQuality;
	//params.Count = 1;

	bitmap->Save(w_file_name, &clsid, NULL );
	delete bitmap;
	GdiplusShutdown(gdiplusToken); 
#else
	FILE* hFile;	//file handle to write the image

	if ((hFile=fopen(file_name,"wb"))==NULL)
	{
		return MAE_OPEN_IMAGE_FAIL;
	}

	error = _mae_encode_bmp(hFile, src);
	fclose(hFile);
#endif
	return error;
}

int _mae_encode_bmp(FILE * fp, const MAE_image * mae_image)
{
	int error = MAE_NO_ERROR;
	int bits_size;
	if (fp==NULL) {
		return MAE_FILE_NOT_EXIST;
	}

	if (mae_image->data ==NULL){
		return MAE_ERROR_NUMS;
	}
	BITMAPFILEHEADER	hdr;

	bits_size = mae_image->stride * mae_image->h;

	hdr.bfType = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
	hdr.bfSize = sizeof(BITMAPINFOHEADER)+ bits_size + 14 /*sizeof(BITMAPFILEHEADER)*/;
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = 14 /*sizeof(BITMAPFILEHEADER)*/ + sizeof(BITMAPINFOHEADER) + _mae_get_palette_size();

	BITMAPINFOHEADER  bmpinfoheader;
	_mae_set_bmp_header(&bmpinfoheader,mae_image );
	// Write the file header
	fwrite(&hdr,min(14,sizeof(BITMAPFILEHEADER)),1, fp);
	// Write the bitmap info header
	fwrite(&bmpinfoheader,min(40,sizeof(bmpinfoheader)),1, fp);
	// Write the DIB header and the pixels
	fwrite(mae_image->data,bits_size,1, fp);
	return error;
}
void _mae_set_bmp_header(BITMAPINFOHEADER *bmpinfoheader,const MAE_image* mae_image )
{

	bmpinfoheader->biSize = 40;

	bmpinfoheader->biWidth = mae_image->w;

	bmpinfoheader->biHeight = mae_image->h;

	bmpinfoheader->biPlanes = 1;

	bmpinfoheader->biBitCount = 24;

	bmpinfoheader->biCompression = 0;

	bmpinfoheader->biSizeImage = 0;

	//bmpinfoheader->biXPelsPerMeter = 3790;
	if( mae_image->xdpi != 0 )
	{
		bmpinfoheader->biXPelsPerMeter = mae_image->xdpi;
	}else{
		bmpinfoheader->biXPelsPerMeter = 3790;
	}

	//bmpinfoheader->biYPelsPerMeter = 3790;
	if( mae_image->ydpi != 0 )
	{
		bmpinfoheader->biYPelsPerMeter = mae_image->ydpi;
	}else{
		bmpinfoheader->biYPelsPerMeter = 3790;
	}

	bmpinfoheader->biClrUsed = 0;

	bmpinfoheader->biClrImportant = 0;

}


int _mae_get_palette_size()
{
	return 0;
}

int _mae_set_segment_to_image(unsigned char* in_data,int width, int height, MAE_image *mae_image)
{
	unsigned char *mae_data;
	int depth;
	int movedbits;
	long widthbytes;
	int x,y;
	unsigned char *src;
	unsigned char *dst;
	unsigned char *srctmp;
	unsigned char *dsttmp;

	unsigned char gray;
	depth = mae_image->depth;
	mae_data = mae_image->data;

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
		dsttmp = mae_data + widthbytes * y;
		srctmp = in_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			dst = dsttmp + movedbits * x;
			src = srctmp + x;
			//dst = mae_data + widthbytes * y + movedbits * x;
			//src = in_data + y * width + x;
			gray = (*src)?0:255; 
			if( movedbits == 1 )
			{
				*dst = gray;
			}else{
				*(dst + 0) = gray;
				*(dst + 1) = gray;
				*(dst + 2) = gray;
			}
		}
	}

	return 0;
}

int _mae_save_segment_image(MAE_image *out_image,unsigned char *in_data, int w, int h, MAE_image *in_image )
{
	int error = MAE_NO_ERROR;
	out_image->depth = in_image->depth;
	out_image->w = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = in_image->stride ;
	out_image->type = in_image->type;

	out_image->data  = _mae_mallocate_image(in_image->w,in_image->h,in_image->depth);
	error = _mae_set_segment_to_image(in_data, w, h, out_image);
	return error;
}
int _mae_save_h_runs_image(MAE_image *out_image,int *rx, int *ry, int *rlen, int nruns )
{
	int error = MAE_NO_ERROR;
	int x;
	int y;
	int len;
	int i,j;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char *data;
	unsigned char *temp;

	memset(out_image->data,(BYTE)255,_mae_sizefromdepth(out_image->w,out_image->h,out_image->depth));

	depth = out_image->depth;
	data = out_image->data;
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, out_image->w);
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

	for(i = 0; i < nruns; i ++ )
	{
		x = rx[i];
		y = ry[i];
		len = rlen[i];
		for(j=0; j < len; j ++ )
		{
			temp = data + widthbytes * y + movedbits * (x+j);
			if( movedbits == 1 )
			{
				*temp = 0;
			}else{
				*(temp + 0) = 0;
				*(temp + 1) = 0;
				*(temp + 2) = 0;
			}

		}
	}

	return error;

}

int _mae_save_v_runs_image(MAE_image *out_image,int *rx, int *ry, int *rlen, int nruns )
{
	int error = MAE_NO_ERROR;
	int x;
	int y;
	int len;
	int i,j;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char *data;
	unsigned char *temp;
	int height;

	height = out_image->h;

	memset(out_image->data,(BYTE)255,_mae_sizefromdepth(out_image->w,out_image->h,out_image->depth));

	depth = out_image->depth;
	data = out_image->data;
	movedbits = 1;
	widthbytes = _mae_width_bytes(depth, out_image->w);
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

	for(i = 0; i < nruns; i ++ )
	{
		x = rx[i];
		y = ry[i];
		len = rlen[i];
		for(j=0; j < len; j ++ )
		{
			temp = data + widthbytes * ( height - 1 - (y+j)) + movedbits * x;
			if( movedbits == 1 )
			{
				*temp = 0;
			}else{
				*(temp + 0) = 0;
				*(temp + 1) = 0;
				*(temp + 2) = 0;
			}

		}
	}

	return error;

}

// image erosion
//int _mae_image_erosion(unsigned char *in_data, int width, int height, unsigned char * out_data)
//{
//	int   mode = 2;
//	int structure[3][3];
//	structure[0][0]=-1;
//	structure[0][1]=0;
//	structure[0][2]=-1;
//	structure[1][0]=0;
//	structure[1][1]=0;
//	structure[1][2]=0;
//	structure[2][0]=-1;
//	structure[2][1]=0;
//	structure[2][2]=-1;
//	
//
//	if (_mae_erosion(in_data, width, height, mode, structure, out_data))
//	{
//		return 1;
//	}
//	return 0;
//}

int _mae_erosion(unsigned char *in_data, int width, int height, int mode, int **structure, int stru_size, unsigned char * out_data)
{
	int i,j,n,m,sum;

	int minus;

	minus = (stru_size -1)  / 2;

	sum = height * width * sizeof(unsigned char); 
	unsigned char *tmpdata = (unsigned char*)malloc(sum);  
	memcpy((unsigned char*)tmpdata, (unsigned char*)in_data, sum);
	//memcpy((unsigned char*)out_data,(unsigned char)255, height*width);



	if (mode==0)     // horizontal structure 
	{
		for(i=0;i<height;i++)
		{
			for(j=minus;j<width-minus;j++)
			{
				out_data[i*width+j]=1;
				for (n=0;n<stru_size;n++)
				{
					if(tmpdata[i*width+j+(n-minus)]==0)
					{
						out_data[i*width+j]=0;
						break;
					}
				}

			}
		}
	}
	else if(mode==1) // vertical structure
	{
		for(i=minus;i<height-minus;i++)
		{
			for(j=0;j<width;j++)
			{
				out_data[i*width+j]=1;
				for (n=0;n<stru_size;n++)
				{
					if(tmpdata[i*width+j+(minus-n)*width]==0)
					{
						out_data[i*width+j]=0;
						break;
					}
				}

			}
		}
	}
	else
	{
		for(i=minus;i<height-minus;i++)
		{
			for(j=minus;j<width - minus;j++)
			{
				out_data[i*width+j] = 1;
				for (m=0;m<stru_size;m++)
				{
					for(n=0;n<stru_size;n++)
					{
						if(*((int*)structure + stru_size*m + n)==-1)
							continue;
						if(tmpdata[i*width+j+(minus-m)*width+(n-minus)]==0)
						{
							out_data[i*width+j]=0;
							break;
						}
					}
				}

			}
		}
	}
	free(tmpdata);
	return 0;
}

//int _mae_image_dilation(unsigned char *in_data, int width, int height, unsigned char * out_data)
//{
//	int   mode = 2;
//	int structure[3][3];
//	structure[0][0]=-1;
//	structure[0][1]=0;
//	structure[0][2]=-1;
//	structure[1][0]=0;
//	structure[1][1]=0;
//	structure[1][2]=0;
//	structure[2][0]=-1;
//	structure[2][1]=0;
//	structure[2][2]=-1;
//	
//
//	// µ÷ÓÃDilationDIB()º¯ÊýÅòÕÍDIB
//	if (_mae_dilation(in_data, width, height, mode, structure, out_data))
//	{
//		return 1;
//	}
//	return 0;
//}

int _mae_dilation(unsigned char *in_data,int width, int height, int mode, int * *structure, int stru_size,  unsigned char * out_data)
{
	int i,j,n,m,sum;
	int minus;
	minus = (stru_size + 1)  / 2;

	sum = height * width * sizeof(unsigned char); 
	unsigned char *tmpdata = (unsigned char*)malloc(sum);  
	memcpy((unsigned char*)tmpdata, (unsigned char*)in_data, sum);

	if (mode==0)     // horizontal structure 
	{
		for(i=0;i<height;i++)
		{
			for(j=minus;j<width-minus;j++)
			{
				out_data[i*width+j]=0;
				for (n=0;n<stru_size;n++)
				{
					if(tmpdata[i*width+j+(n-minus)]==1)
					{
						out_data[i*width+j]=1;
						break;
					}
				}

			}
		}
	}
	else if(mode==1) // vertical structure
	{
		for(i=minus;i<height-minus;i++)
		{
			for(j=0;j<width;j++)
			{
				out_data[i*width+j]=0;
				for (n=0;n<stru_size;n++)
				{
					if(tmpdata[i*width+j+(minus-n)*width]==1)
					{
						out_data[i*width+j]=1;
						break;
					}
				}

			}
		}
	}
	else
	{
		for(i=minus;i<height-minus;i++)
		{
			for(j=minus;j<width-minus;j++)
			{
				out_data[i*width+j]=0;
				for (m=0;m<stru_size;m++)
				{
					for(n=0;n<stru_size;n++)
					{
						if(*((int*)structure + stru_size*m + n)==-1)
							continue;
						if(tmpdata[i*width+j+(minus-m)*width+(n-minus)]==1)
						{
							out_data[i*width+j]=1;
							break;
						}
					}
				}

			}
		}
	}
	free(tmpdata);
	return 0;
}


int _mae_save_gray_image(MAE_image *out_image,unsigned char *in_data, int w, int h, MAE_image *in_image )
{
	int error = MAE_NO_ERROR;
	out_image->depth = in_image->depth;
	out_image->w = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = in_image->stride ;
	out_image->type = in_image->type;

	out_image->data  = _mae_mallocate_image(in_image->w,in_image->h,in_image->depth);
	error = _mae_set_gray_to_image(in_data, w, h, out_image);
	return error;
}
int _mae_set_gray_to_image(unsigned char* in_data,int width, int height, MAE_image *mae_image)
{
	unsigned char *mae_data;
	int depth;
	int movedbits;
	long widthbytes;
	int x,y;
	unsigned char *src;
	unsigned char *dst;
	unsigned char *srctmp;
	unsigned char *dsttmp;

	unsigned char gray;
	depth = mae_image->depth;
	mae_data = mae_image->data;

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
		dsttmp = mae_data + widthbytes * y;
		srctmp = in_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			dst = dsttmp + movedbits * x;
			src = srctmp + x;
			//dst = mae_data + widthbytes * y + movedbits * x;
			//src = in_data + y * width + x;
			gray = *src; 
			if( movedbits == 1 )
			{
				*dst = gray;
			}else{
				*(dst + 0) = gray;
				*(dst + 1) = gray;
				*(dst + 2) = gray;
			}
		}
	}

	return 0;
}

void _mae_get_region_image(MAE_image *in_image, int l, int t, int w, int h, MAE_image *out_image)
{
	//int error = MAE_NO_ERROR;
	int left;
	int top;
	int width;
	int height;
	int i,j, k;
	int depth;
	int movedbits;
	long in_widthbytes;
	long out_widthbytes;

	unsigned char *in_data;
	unsigned char *out_data;
	unsigned char *in_temp;
	unsigned char *out_temp;

	out_image->depth = in_image->depth;
	out_image->type = in_image->type;
	out_image->w  = w;
	out_image->h = h;
	out_image->stride = _mae_width_bytes(  out_image->depth, out_image->w );
	out_image->data  = _mae_mallocate_image(out_image->w, out_image->h, out_image->depth);


	depth = in_image->depth;
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

	in_widthbytes = _mae_width_bytes(depth, in_image->w);
	out_widthbytes = _mae_width_bytes(depth, out_image->w);
	left = l;
	top = t;
	width = w ;
	height = h;


	for(j = 0; j < height; j ++ )
	{
		out_data = out_image->data + out_widthbytes * j;
		in_data = in_image->data+ in_widthbytes * (j + top);
		for(i = 0; i < width; i ++ )
		{
			out_temp = out_data + movedbits * i;
			in_temp = in_data + movedbits * (i + left);

			for( k = 0; k < movedbits; k ++ )
			{
				*(out_temp + k) = *(in_temp + k);
			}
		}
		//memcpy(out_data,in_data, out_widthbytes);
	}

	return;
}

void _mae_set_region_to_image(MAE_image *image, MAE_region *region, unsigned char *seg_data)
{
	//int error = MAE_NO_ERROR;
	int left;
	int top;
	int width;
	int height;
	int i,j, k;
	int depth;
	int movedbits;
	long widthbytes;

	unsigned char *src_data;
	unsigned char *dst_data;
	unsigned char *src_temp;
	unsigned char *dst_temp;


	depth = image->depth;
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

	widthbytes = _mae_width_bytes(depth, image->w);

	left = region->l;
	top = region->t;
	width = region->w ;
	height = region->h;

	for(j = 0; j < height; j ++ )
	{
		src_data = seg_data + region->w * j;
		dst_data = image->data+ widthbytes * (j + top);
		for(i = 0; i < width; i ++ )
		{
			src_temp = src_data + i;
			dst_temp = dst_data + movedbits * (i + left);
			if( *src_temp == 0 )
			{
				for( k = 0; k < movedbits; k ++ )
				{
					*(dst_temp + k) = 255;
				}
			}
		}
	}

	return;
}
void _mae_set_region_to_image2(MAE_image *image, MAE_region *region, unsigned char *seg_data, MAE_image * h_image)
{
	int left;
	int top;
	int width;
	int height;
	int i,j, k;
	int depth;
	int movedbits;
	long widthbytes;

	unsigned char *src_data;
	unsigned char *dst_data;
	unsigned char *h_data;
	unsigned char *src_temp;
	unsigned char *dst_temp;
	unsigned char *h_temp;


	depth = image->depth;
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

	widthbytes = _mae_width_bytes(depth, image->w);

	left = region->l;
	top = region->t;
	width = region->w ;
	height = region->h;

	for(j = 0; j < height; j ++ )
	{
		src_data = seg_data + region->w * j;
		dst_data = image->data+ widthbytes * (j + top);
		h_data = h_image->data+ h_image->stride * j;
		for(i = 0; i < width; i ++ )
		{
			src_temp = src_data + i;
			dst_temp = dst_data + movedbits * (i + left);
			h_temp = h_data + movedbits * i;
			if( *src_temp == 0 && 
				(*h_temp != 255 && *(h_temp+1) != 255 && *(h_temp+2) != 255) )
			{
				for( k = 0; k < movedbits; k ++ )
				{
					*(dst_temp + k) = 255;
				}
			}
		}
	}

	return;
}

void _mae_zoom(MAE_image *in_image, MAE_image *out_image, float *xfector, float *yfector)
{
	float xfctr;
	float yfctr;
	unsigned char *indata;
	int iw;
	int ih;
	int depth;
	unsigned char *zmdata;
	int zw;
	int zh;
	depth = in_image->depth;
	indata = in_image->data;
	iw = in_image->w;
	ih = in_image->h;

	xfctr = (float)MAE_NORMAL_X_SIZE /(float)iw;
	//	yfctr = (float)MAE_NORMAL_Y_SIZE /(float)ih;
	yfctr  = xfctr;

	//	xfctr = 0.25;
	//	yfctr = 0.25;

	*xfector = xfctr;
	*yfector = yfctr;

	_mae_zoom(&zmdata, &zw, &zh, 
		indata, iw, ih, depth, 
		xfctr, yfctr);

	out_image->depth = depth;
	out_image->w = zw;
	out_image->h = zh;
	out_image->stride = _mae_width_bytes( depth, zw);
	out_image->type  = in_image->type;
	out_image->data  = zmdata;
	//out_image->data  = _mae_mallocate_image(out_image->w, out_image->h, out_image->depth);
	//memcpy(out_image->data, zmdata, _mae_sizefromdepth(out_image->w, out_image->h, out_image->depth));
	//free(zmdata);

}

// _mae_grayscale_rotate_90 
// rotates a grayscale image exactly 90 degrees
// clockwise by copying pixel rows to pixel columns.
void _mae_grayscale_rotate_90(unsigned char *idata, unsigned char *odata,int w,int h)
{
	int x, y;
	unsigned char *iptr, *siptr, *optr, *soptr;

	siptr = idata;
	soptr = odata + h - 1;
	for(y = 0; y < h; y++)
	{
		iptr = siptr;
		optr = soptr;
		for(x = 0; x < w; x++)
		{
			*optr = *iptr;
			optr += h;
			iptr++;
		}
		siptr += w;
		soptr--;
	}
}
// _mae_grayscale_rotate_n90 
// rotates a grayscale image exactly -90 degrees
// counter-clockwise by copying pixel rows to pixel columns.

void _mae_grayscale_rotate_n90(unsigned char *idata, unsigned char *odata,int w, int h)
{
	int x, y;
	unsigned char *iptr, *siptr, *optr, *soptr;

	siptr = idata;
	soptr = odata + ((w-1) * h);
	for(y = 0; y < h; y++){
		iptr = siptr;
		optr = soptr;
		for(x = 0; x < w; x++){
			*optr = *iptr;
			optr -= h;
			iptr++;
		}
		siptr += w;
		soptr++;
	}
}

void _mae_inverse_image(unsigned char *in_data, int width, int height, int rowsize, unsigned char  *out_data)
{
	int i;
	unsigned char * temp1;
	unsigned char * temp2;

	//_width is not be used
	int _width;
	_width = width;
	//
	for(i = 0; i < height; i ++ )
	{
		temp1 = in_data + (height - i - 1) * rowsize;
		temp2 = out_data + i * rowsize;
		memcpy(temp2, temp1, rowsize );
	}
}



int  ThiningImage(unsigned char *lpDIBBits, long lWidth, long lHeight)
{

	unsigned char *	lpSrc;

	unsigned char *	lpDst;

	unsigned char *lpNewDIBBits;
	unsigned char *	hNewDIBBits;

	int bModified;

	long i;
	long j;
	int  n;
	int  m;

	int  bCondition1;
	int bCondition2;
	int bCondition3;
	int bCondition4;

	unsigned char nCount;

	unsigned char pixel;

	unsigned char neighbour[5][5];

	hNewDIBBits = _mae_mallocate_image(lWidth,lHeight, 8);

	if (hNewDIBBits == NULL)
	{
		return 0;
	}

	lpDst = (unsigned char *)lpNewDIBBits;
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	bModified=1;

	while(bModified)
	{

		bModified = 0;
		lpDst = (unsigned char *)lpNewDIBBits;
		memset(lpDst, (BYTE)0, lWidth * lHeight);

		for(j = 2; j <lHeight-2; j++)
		{
			for(i = 2;i <lWidth-2; i++)
			{

				bCondition1 = 0;
				bCondition2 = 0;
				bCondition3 = 0;
				bCondition4 = 0;

				lpSrc = (unsigned char *)lpDIBBits + lWidth * j + i;

				lpDst = (unsigned char *)lpNewDIBBits + lWidth * j + i;

				pixel = (unsigned char)*lpSrc;

				if(pixel != 1 && *lpSrc != 0)
				{
					continue;
				}
				else if(pixel == 0)
				{
					continue;
				}
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						//neighbour[m][n] =(255 - (unsigned char)*(lpSrc + ((4 - m) - 2)*lWidth + n - 2 )) / 255;
						neighbour[m][n] = *(lpSrc + ((4 - m) - 2)*lWidth + n - 2 );
					}
				}
				//				neighbour[][]
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
					+ neighbour[2][1] + neighbour[2][3] + \
					+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = 1;

				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = 1;

				if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = 1;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = 1;
				}

				if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = 1;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = 1;
				}
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					*lpDst = (unsigned char)0;
					bModified = 1;
				}
				else
				{
					*lpDst = (unsigned char)1;
				}
			}
		}
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);


	}
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	free(hNewDIBBits);
	return TRUE;
}

void _mae_split_image(unsigned char *out_data, long width, long height, int step)
{
	unsigned char *data;
	int i;

	for(i = 0; i <height; i = i+ step)
	{
		data = out_data + i * width;
		memset(data, (unsigned char)0, width);
	}


}

void _mae_v_dilation(unsigned char *data, long width, long height)
{
	int x, y;
	unsigned char *temp1;
	unsigned char *temp;
	unsigned char *temp2;
	unsigned char *temp3;
	unsigned char c1,c2,c, c3;
	unsigned char *cpydata;
	cpydata = _mae_mallocate_image(width,height,8);
	memcpy(cpydata, data, width * height);

	for(y = 1; y < height - 1; y ++ )
	{
		temp1 = cpydata + (y - 1) * width;
		temp  = data + (y) * width;
		temp2 = cpydata + (y + 1) * width;
		//temp3 = cpydata + (y) * width;
		for ( x = 0 ; x < width; x++)
		{
			c1 = *(temp1 + x);
			c2 = *(temp2 + x);
			// c3 = *(temp3 + x);
			// if( (c1> 0 + c2 + c3) > 0 )
			if( c1> 0 && c2 > 0 )
			{
				*(temp + x) = 1;
			}
		}
	}

	free(cpydata);
	return;
}

void _mae_get_image_rgb_e_v2(MAE_image *in_image, int *re,int *ge, int *be)
{
	int depth;
	int width;
	int height;
	long widthbytes;

	int rhist[256];
	int ghist[256];
	int bhist[256];
	depth = in_image->depth;
	width = in_image->w;
	height = in_image->h;
	widthbytes = _mae_width_bytes(depth, width);
	//histogram r g b
	_mae_hist_high_rgb2(in_image->data, width, height, depth, widthbytes,
		rhist, ghist, bhist);

	//feature->mark_pixs = pixs;

	int rtemp;
	int rcol;
	int gtemp;
	int gcol;
	int btemp;
	int bcol;
	int i;
	int k;
	int step = 8;
	int sum;
	rtemp = 0;
	gtemp = 0;
	btemp = 0;
	rcol = 0;
	gcol = 0;
	bcol = 0;

	sum = 0;
	//	for(i = 0; i < 256; i ++ )
	for(i = 0; i < 256 - step; i ++ )
	{
		sum = 0;
		for(k = 0; k < step; k ++ )
		{
			sum = sum + rhist[i+k];
		}
		if(rtemp <  sum )
		{
			rtemp = sum;
			rcol = i;
		}
		sum = 0;
		for(k = 0; k < step; k ++ )
		{
			sum = sum + ghist[i+k];
		}
		if(gtemp <  sum )
		{
			gtemp = sum;
			gcol = i;
		}
		sum = 0;
		for(k = 0; k < step; k ++ )
		{
			sum = sum + bhist[i+k];
		}
		if(btemp <  sum )
		{
			btemp = sum;
			bcol = i;
		}
	}

	*re = rcol+step/2;
	*ge = gcol+step/2;
	*be = bcol+step/2;

}
void _mae_init_image_feature(MAE_image *in_image, MAE_init_feature *init_feature)
{
	int re = 0;
	int ge = 0;
	int be = 0;
	_mae_get_image_rgb_e_v2(in_image, &re, &ge, &be);
	init_feature->r_bk = re;
	init_feature->g_bk = ge;
	init_feature->b_bk = be;
}
