#include "stdafx.h"
#include "imageprocess.h"
#include "imageutil.h"
#include "zoom.h"
#include "memalloc.h"
#include "ehcdefs.h"
#include "histogram.h"

#ifdef EHC_USE_GDIPLUS
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

int _open_image_bitmap(unsigned char * data, int width, int height, EHC_image* ehc_image)
{
	int error = EHC_NO_ERROR;
	int depth = 24;
	//int i;
//	unsigned char * temp1;
//	unsigned char * temp2;
	int rowsize;
	ehc_image->depth = depth;
	ehc_image->w  = width;
	ehc_image->h  = height;
	ehc_image->stride  = _width_bytes(depth, width);
	ehc_image->data = _mallocate_image(width,height,depth);
	//memcpy(ehc_image->data, data, 	_sizefromdepth(width, height, depth));
	rowsize = _width_bytes(depth, width );
	//for(i = 0; i < height; i ++ )
	//{
	//	temp1 = ehc_image->data + (height - i - 1) * rowsize;
	//	temp2 = data + i * rowsize;
	//	memcpy(temp1, temp2, rowsize );
	//}
	memcpy(ehc_image->data, data, rowsize * height);
	//_inverse_image(data, width, height, rowsize, ehc_image->data);

	return error;
}


int _open_image(const char* file_name, EHC_image* ehc_image)
{
	int error = EHC_NO_ERROR;
#ifdef EHC_USE_GDIPLUS

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
		return EHC_OPEN_IMAGE_FAIL ;
	}
	//20100901 add start
	ehc_image->xdpi = (int)bitmap->GetHorizontalResolution();
	ehc_image->ydpi = (int)bitmap->GetVerticalResolution();
	if(ehc_image->xdpi == 0 )
	{
		ehc_image->xdpi = 96;
	}
	if(ehc_image->ydpi == 0 )
	{
		ehc_image->ydpi = 96;
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
	error = _open_image_bitmap( pBuf, width, height, ehc_image);
	bitmap->UnlockBits(&bmpData);
	delete(bitmap);
	GdiplusShutdown(gdiplusToken); 
#else
	FILE* hFile;	//file handle to read the image

	if ((hFile=fopen(file_name,"rb"))==NULL){
		return EHC_FILE_NOT_EXIST;
	}

	error = _decode_bmp( hFile, ehc_image);
	fclose(hFile);

#endif
	return error;
}


int _decode_bmp(FILE * fp, EHC_image* ehc_image)
{
	unsigned char * outdata;

	int error = EHC_NO_ERROR;
	if (fp == NULL) {
		return EHC_FILE_NOT_EXIST;
	}

	BITMAPFILEHEADER   bf;
	DWORD off = ftell(fp);


	if(fread(&bf, min(14,sizeof(bf)), 1, fp)==0){
	  return EHC_FILE_UNSUPPORT_FORMAT;
	}
    if (bf.bfType != BFT_BITMAP) { //do we have a RC HEADER?
        bf.bfOffBits = 0L;
        fseek(fp,off,SEEK_SET);
    }

	BITMAPINFOHEADER bmpHeader;
	if (_read_bitmap_info(fp,&bmpHeader) != EHC_NO_ERROR ){
		return EHC_READ_IMAGE_FAIL;
	}
	DWORD dwCompression=bmpHeader.biCompression;
	DWORD dwBitCount=bmpHeader.biBitCount; //preserve for BI_BITFIELDS compression <Thomas Ernst>
	bool bIsOldBmp;
	bIsOldBmp = bmpHeader.biSize == sizeof(BITMAPCOREHEADER);


	bool bTopDownDib = bmpHeader.biHeight<0; //<Flanders> check if it's a top-down bitmap
	if (bTopDownDib) bmpHeader.biHeight=-bmpHeader.biHeight;

	//20100901 add start
	ehc_image->xdpi = bmpHeader.biXPelsPerMeter;
	ehc_image->ydpi = bmpHeader.biYPelsPerMeter;
	if(ehc_image->xdpi == 0 )
	{
		ehc_image->xdpi = 96;
	}
	if(ehc_image->ydpi == 0 )
	{
		ehc_image->ydpi = 96;
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

	outdata = _mallocate_image(bmpHeader.biWidth,bmpHeader.biHeight,bmpHeader.biBitCount);

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
				error = EHC_FILE_UNSUPPORT_FORMAT;
				free(outdata);
				return error;
			}
			break;
		default:
				error = EHC_FILE_UNSUPPORT_FORMAT;
				free(outdata);
				return error;
			break;
	}


	  ehc_image->depth = bmpHeader.biBitCount;
	  ehc_image->w  = bmpHeader.biWidth;
	  ehc_image->h  = bmpHeader.biHeight;
	  ehc_image->stride  = _width_bytes(bmpHeader.biBitCount, bmpHeader.biWidth);
	  ehc_image->data  = outdata;

    return error;
}

int  _read_bitmap_info(FILE* fp, BITMAPINFOHEADER *bmpinfo)
{
	int error = EHC_NO_ERROR;

	if ((fp == NULL)||(bmpinfo==NULL)){
		return EHC_FILE_NOT_EXIST;
	}

	if (fread(bmpinfo,sizeof(BITMAPINFOHEADER),1,fp)==0){
		return EHC_FILE_NOT_EXIST;
	}

    switch (bmpinfo->biSize) // what type of bitmap info is this?
    {
        case sizeof(BITMAPINFOHEADER):
            break;

		case 64: //sizeof(OS2_BMP_HEADER):
			fseek(fp, (long)(64 - sizeof(BITMAPINFOHEADER)), SEEK_CUR);
			break;
        default:
			return EHC_FILE_NOT_EXIST;
    }
	bmpinfo->biSizeImage = _sizefromdepth(bmpinfo->biWidth,bmpinfo->biHeight,bmpinfo->biBitCount);
    return error;
}


int _save_image(const char* file_name, const EHC_image* src)
{
	int error = EHC_NO_ERROR;
#ifdef EHC_USE_GDIPLUS

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
	//_inverse_image(src->data, src->w, src->h, src->stride, pBuf);


	CLSID clsid;
	if( GetEncoderClsid(L"image/jpeg", &clsid) == -1 )
	{
		error = EHC_SAVE_IMAGE_FAIL;
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
		return EHC_OPEN_IMAGE_FAIL;
	}

	error = _encode_bmp(hFile, src);
	fclose(hFile);
#endif
	return error;
}

int _encode_bmp(FILE * fp, const EHC_image * ehc_image)
{
	int error = EHC_NO_ERROR;
	int bits_size;
	if (fp==NULL) {
		return EHC_FILE_NOT_EXIST;
	}

	if (ehc_image->data ==NULL){
		return EHC_ERROR_NUMS;
	}
	BITMAPFILEHEADER	hdr;

	bits_size = ehc_image->stride * ehc_image->h;

	hdr.bfType = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
	hdr.bfSize = sizeof(BITMAPINFOHEADER)+ bits_size + 14 /*sizeof(BITMAPFILEHEADER)*/;
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = 14 /*sizeof(BITMAPFILEHEADER)*/ + sizeof(BITMAPINFOHEADER) + _get_palette_size();

	BITMAPINFOHEADER  bmpinfoheader;
	_set_bmp_header(&bmpinfoheader,ehc_image );
    // Write the file header
	fwrite(&hdr,min(14,sizeof(BITMAPFILEHEADER)),1, fp);
    // Write the bitmap info header
	fwrite(&bmpinfoheader,min(40,sizeof(bmpinfoheader)),1, fp);
    // Write the DIB header and the pixels
	fwrite(ehc_image->data,bits_size,1, fp);
	return error;
}
void _set_bmp_header(BITMAPINFOHEADER *bmpinfoheader,const EHC_image* ehc_image )
{
  
  bmpinfoheader->biSize = 40;
    
  bmpinfoheader->biWidth = ehc_image->w;
    
  bmpinfoheader->biHeight = ehc_image->h;
    
  bmpinfoheader->biPlanes = 1;
    
  bmpinfoheader->biBitCount = 24;

  bmpinfoheader->biCompression = 0;
    
  bmpinfoheader->biSizeImage = 0;
    
  //bmpinfoheader->biXPelsPerMeter = 3790;
  if( ehc_image->xdpi != 0 )
  {
	  bmpinfoheader->biXPelsPerMeter = ehc_image->xdpi;
  }else{
	  bmpinfoheader->biXPelsPerMeter = 3790;
  }
    
  //bmpinfoheader->biYPelsPerMeter = 3790;
  if( ehc_image->ydpi != 0 )
  {
	  bmpinfoheader->biYPelsPerMeter = ehc_image->ydpi;
  }else{
	  bmpinfoheader->biYPelsPerMeter = 3790;
  }
    
  bmpinfoheader->biClrUsed = 0;
    
  bmpinfoheader->biClrImportant = 0;

}
  

int _get_palette_size()
{
	return 0;
}

int _set_segment_to_image(unsigned char* in_data,int width, int height, EHC_image *ehc_image)
{
	unsigned char *ehc_data;
	int depth;
	int movedbits;
	long widthbytes;
	int x,y;
	unsigned char *src;
	unsigned char *dst;
	unsigned char *srctmp;
	unsigned char *dsttmp;

	unsigned char gray;
	depth = ehc_image->depth;
	ehc_data = ehc_image->data;
	
	movedbits = 1;
	widthbytes = _width_bytes(depth, width);
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
		dsttmp = ehc_data + widthbytes * y;
		srctmp = in_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			dst = dsttmp + movedbits * x;
			src = srctmp + x;
			//dst = ehc_data + widthbytes * y + movedbits * x;
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

int _save_segment_image(EHC_image *out_image,unsigned char *in_data, int w, int h, EHC_image *in_image )
{
	int error = EHC_NO_ERROR;
	out_image->depth = in_image->depth;
	out_image->w = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = in_image->stride ;
	out_image->type = in_image->type;
	
	out_image->data  = _mallocate_image(in_image->w,in_image->h,in_image->depth);
	error = _set_segment_to_image(in_data, w, h, out_image);
	return error;
}
int _save_h_runs_image(EHC_image *out_image,int *rx, int *ry, int *rlen, int nruns )
{
	int error = EHC_NO_ERROR;
	int x;
	int y;
	int len;
	int i,j;
	int depth;
	int movedbits;
	long widthbytes;
	unsigned char *data;
	unsigned char *temp;

	memset(out_image->data,(BYTE)255,_sizefromdepth(out_image->w,out_image->h,out_image->depth));

	depth = out_image->depth;
	data = out_image->data;
	movedbits = 1;
	widthbytes = _width_bytes(depth, out_image->w);
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

int _save_v_runs_image(EHC_image *out_image,int *rx, int *ry, int *rlen, int nruns )
{
	int error = EHC_NO_ERROR;
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

	memset(out_image->data,(BYTE)255,_sizefromdepth(out_image->w,out_image->h,out_image->depth));

	depth = out_image->depth;
	data = out_image->data;
	movedbits = 1;
	widthbytes = _width_bytes(depth, out_image->w);
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


int _erosion(unsigned char *in_data, int width, int height, int mode, int **structure, int stru_size, unsigned char * out_data)
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

int _dilation(unsigned char *in_data,int width, int height, int mode, int * *structure, int stru_size,  unsigned char * out_data)
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


int _save_gray_image(EHC_image *out_image,unsigned char *in_data, int w, int h, EHC_image *in_image )
{
	int error = EHC_NO_ERROR;
	out_image->depth = in_image->depth;
	out_image->w = in_image->w;
	out_image->h = in_image->h;
	out_image->stride = in_image->stride ;
	out_image->type = in_image->type;
	
	out_image->data  = _mallocate_image(in_image->w,in_image->h,in_image->depth);
	error = _set_gray_to_image(in_data, w, h, out_image);
	return error;
}
int _set_gray_to_image(unsigned char* in_data,int width, int height, EHC_image *ehc_image)
{
	unsigned char *ehc_data;
	int depth;
	int movedbits;
	long widthbytes;
	int x,y;
	unsigned char *src;
	unsigned char *dst;
	unsigned char *srctmp;
	unsigned char *dsttmp;

	unsigned char gray;
	depth = ehc_image->depth;
	ehc_data = ehc_image->data;
	
	movedbits = 1;
	widthbytes = _width_bytes(depth, width);
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
		dsttmp = ehc_data + widthbytes * y;
		srctmp = in_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			dst = dsttmp + movedbits * x;
			src = srctmp + x;
			//dst = ehc_data + widthbytes * y + movedbits * x;
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

void _zoom(EHC_image *in_image, EHC_image *out_image, float *xfector, float *yfector)
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

	//xfctr = (float)EHC_NORMAL_X_SIZE /(float)iw;
	xfctr = *xfector;
	yfctr  = *yfector;

//	xfctr = 0.25;
//	yfctr = 0.25;

	*xfector = xfctr;
	*yfector = yfctr;
	
	_zoom(&zmdata, &zw, &zh, 
		 indata, iw, ih, depth, 
		  xfctr, yfctr);
	
	out_image->depth = depth;
	out_image->w = zw;
	out_image->h = zh;
	out_image->stride = _width_bytes( depth, zw);
	out_image->type  = in_image->type;
	out_image->data  = zmdata;
	//out_image->data  = _mallocate_image(out_image->w, out_image->h, out_image->depth);
	//memcpy(out_image->data, zmdata, _sizefromdepth(out_image->w, out_image->h, out_image->depth));
	//free(zmdata);

}

// _grayscale_rotate_90 
// rotates a grayscale image exactly 90 degrees
// clockwise by copying pixel rows to pixel columns.
void _grayscale_rotate_90(unsigned char *idata, unsigned char *odata,int w,int h)
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
// _grayscale_rotate_n90 
// rotates a grayscale image exactly -90 degrees
// counter-clockwise by copying pixel rows to pixel columns.

void _grayscale_rotate_n90(unsigned char *idata, unsigned char *odata,int w, int h)
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

void _inverse_image(unsigned char *in_data, int width, int height, int rowsize, unsigned char  *out_data)
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

	unsigned char* lpSrc = NULL;	
	unsigned char* lpDst = NULL;	
	unsigned char* lpNewDIBBits = NULL;
	unsigned char* hNewDIBBits = NULL;

	hNewDIBBits = _mallocate_image(lWidth,lHeight, 8);

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

void _split_image(unsigned char *out_data, long width, long height, int step)
{
	unsigned char *data;
	int i;
	int j;
	//20110321 update start
	//for(i = 0; i <height; i = i+ step)
	//{
	//	data = out_data + i * width;
	//	memset(data, (unsigned char)0, width);
	//}
	for(i = 0; i <width; i = i + step)
	for(j = 0; j <height; j++)
	{
		data = out_data + j * width + i;
		*data = 0;
	}
	//20110321 update end


}

void _v_dilation(unsigned char *data, long width, long height)
{
	int x, y;
	unsigned char *temp1;
	unsigned char *temp;
	unsigned char *temp2;
	unsigned char c1,c2;
	unsigned char *cpydata;
	cpydata = _mallocate_image(width,height,8);
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
//
//void _get_image_rgb_e_v2(EHC_image *in_image, int *re,int *ge, int *be)
//{
//	int depth;
//	int width;
//	int height;
//	long widthbytes;
//
//	int rhist[256];
//	int ghist[256];
//	int bhist[256];
//	depth = in_image->depth;
//	width = in_image->w;
//	height = in_image->h;
//	widthbytes = _width_bytes(depth, width);
//	//histogram r g b
//	_hist_high_rgb2(in_image->data, width, height, depth, widthbytes,
//				  rhist, ghist, bhist);
//
//    //feature->mark_pixs = pixs;
//
//	int rtemp;
//	int rcol;
//	int gtemp;
//	int gcol;
//	int btemp;
//	int bcol;
//	int i;
//	int k;
//	int step = 8;
//	int sum;
//	rtemp = 0;
//	gtemp = 0;
//	btemp = 0;
//	rcol = 0;
//	gcol = 0;
//	bcol = 0;
//
//	sum = 0;
////	for(i = 0; i < 256; i ++ )
//	for(i = 0; i < 256 - step; i ++ )
//	{
//		sum = 0;
//		for(k = 0; k < step; k ++ )
//		{
//			sum = sum + rhist[i+k];
//		}
//		if(rtemp <  sum )
//		{
//			rtemp = sum;
//			rcol = i;
//		}
//		sum = 0;
//		for(k = 0; k < step; k ++ )
//		{
//			sum = sum + ghist[i+k];
//		}
//		if(gtemp <  sum )
//		{
//			gtemp = sum;
//			gcol = i;
//		}
//		sum = 0;
//		for(k = 0; k < step; k ++ )
//		{
//			sum = sum + bhist[i+k];
//		}
//		if(btemp <  sum )
//		{
//			btemp = sum;
//			bcol = i;
//		}
//	}
//
//	*re = rcol+step/2;
//	*ge = gcol+step/2;
//	*be = bcol+step/2;
//	
//}
//void _init_image_feature(EHC_image *in_image, EHC_init_feature *init_feature)
//{
//	int re = 0;
//	int ge = 0;
//	int be = 0;
//	_get_image_rgb_e_v2(in_image, &re, &ge, &be);
//	init_feature->r_bk = re;
//	init_feature->g_bk = ge;
//	init_feature->b_bk = be;
//}


int init_image_background(int step,int width, int height, EHC_image *ehc_image)
{
	unsigned char *ehc_data;
	int depth;
	int movedbits;
	long widthbytes;
	int x,y;
	//unsigned char *src;
	unsigned char *dst;
	//unsigned char *srctmp;
	unsigned char *dsttmp;
	
	unsigned char r1 = 200;
	unsigned char g1 = 200;
	unsigned char b1 = 200;
	unsigned char r2 = 255;
	unsigned char g2 = 255;
	unsigned char b2 = 255;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	int flag = 0;
	int flag2 = 0;
//	unsigned char gray;
	depth = ehc_image->depth;
	ehc_data = ehc_image->data;
	
	movedbits = 1;
	widthbytes = _width_bytes(depth, width);
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
		if( y % step == 0 )
		{
			if(flag2 == 0 )
			{
				flag2 = 1;
			}else{
				flag2 = 0;
			}
		}

		dsttmp = ehc_data + widthbytes * y;
		//srctmp = in_data + y * width;
		for(x = 0; x < width; x ++ )
		{

			dst = dsttmp + movedbits * x;
			//src = srctmp + x;
			//dst = ehc_data + widthbytes * y + movedbits * x;
			//src = in_data + y * width + x;
			//gray = (*src)?0:255; 
			//if( movedbits == 1 )
			//{
			//	*dst = gray;
			//}else{
			//	*(dst + 0) = gray;
			//	*(dst + 1) = gray;
			//	*(dst + 2) = gray;
			//}
			if( x % step == 0 )
			{
				if(flag == 0 )
				{
					flag = 1;
				}else{
					flag = 0;
				}
			}
			if(flag)
			{
				if( flag2) 
				{
					r = r1;
					g = g1;
					b = b1;
				}else{
					r = r2;
					g = g2;
					b = b2;
				}
			}else{
				if( flag2) 
				{
					r = r2;
					g = g2;
					b = b2;
				}else{
					r = r1;
					g = g1;
					b = b1;
				}
			}
			*(dst + 0) = r;
			*(dst + 1) = g;
			*(dst + 2) = b;

		}
	}

	return 0;
}


int _set_segment_to_backimage(unsigned char* in_data,int width, int height, EHC_image *ehc_image)
{
	unsigned char *ehc_data;
	int depth;
	int movedbits;
	long widthbytes;
	int x,y;
	unsigned char *src;
	unsigned char *dst;
	unsigned char *srctmp;
	unsigned char *dsttmp;

	unsigned char gray;
	depth = ehc_image->depth;
	ehc_data = ehc_image->data;
	
	movedbits = 1;
	widthbytes = _width_bytes(depth, width);
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
		dsttmp = ehc_data + widthbytes * y;
		srctmp = in_data + y * width;
		for(x = 0; x < width; x ++ )
		{
			dst = dsttmp + movedbits * x;
			src = srctmp + x;
			//dst = ehc_data + widthbytes * y + movedbits * x;
			//src = in_data + y * width + x;
			gray = (*src)?0:255;
			if( gray == 255 )
			{
				continue;
			}
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