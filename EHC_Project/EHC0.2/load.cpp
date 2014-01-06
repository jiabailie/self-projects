#include "stdafx.h"
#include "load.h"
#include "imageprocess.h"
#include "imageutil.h"
#include "segment.h"

#ifdef DEBUG_LABEL
bool is_first_label_low = true;
bool is_first_label_hig = true;
#endif

void initimagemrf(IMAGE_H_MRF **h_mrf, IMAGE_L_MRF **l_mrf, int num)
{
	IMAGE_H_MRF * hmrf;
	IMAGE_L_MRF * lmrf;
	hmrf  = (IMAGE_H_MRF *)malloc(sizeof(IMAGE_H_MRF) * num);
	lmrf  = (IMAGE_L_MRF *)malloc(sizeof(IMAGE_L_MRF) * num);
	*h_mrf = hmrf;
	*l_mrf = lmrf;
}

int destroy(IMAGE_H_MRF *h_mrf, IMAGE_L_MRF *l_mrf, int num)
{
	int i = 0;

	for(i = 0; i < num; i ++ )
	{
		if( h_mrf[i].label != NULL ) free(h_mrf[i].label);
		if( l_mrf[i].label != NULL ) free(l_mrf[i].label);
	}
	free(h_mrf);
	free(l_mrf);
	return 0;
}
int destroy_lmrf(IMAGE_L_MRF *l_mrf, int num)
{
	int i = 0;

	for(i = 0; i < num; i ++ )
	{
		if( l_mrf[i].label != NULL ) free(l_mrf[i].label);
	}
	return 0;
}
int destroy_hmrf(IMAGE_H_MRF *h_mrf, int num)
{
	int i = 0;

	for(i = 0; i < num; i ++ )
	{
		if( h_mrf[i].label != NULL ) free(h_mrf[i].label);
	}
	return 0;
}

/**
 * yangruiguo 2011-12-06
 * namefile: the file which includes all of the needing file name list
 * namelist: array saves the pics' name
 * return  : 0 success 
 */
int loadsamplenamelist(const char* namefile, Sample_name ** namelist, int * num)
{
	char buf[MAXLEN];
	int i = 0;
	int m = 0;
	int n = 0;
	FILE* fp;
	char tmp[2048];
	int ret = SUC_RETURN;
	Sample_name *list; 

	if((fp = fopen(namefile, "r")) == NULL)
	{
		ret = FAIL_OPEN_FILE;
	}

	while(!feof(fp)) 
	{
	   if( fgets( buf, MAXLEN, fp ) == NULL )
	   {		
		  break;
       }
	  i++;
	}
    
	*num = i;

	list = (Sample_name*)malloc(sizeof(Sample_name) * (*num));
	*namelist = list;

	fseek(fp, 0L, SEEK_SET); //fp in the front of file

	i = 0;
	while(fgets(tmp, 2048, fp) != NULL && i < (*num))
	{
		for(m = 0; m < (int)strlen(tmp) && tmp[m] != ','; list[i].h_name[m] = tmp[m], m++);
		list[i].h_name[m++] = '\0';
		for(n = 0; m < (int)strlen(tmp); list[i].l_name[n] = tmp[m], n++, m++);
		list[i].l_name[n - 1] = '\0';
#ifdef DEBUG
			printf("%s,%s", namelist[i].h_name, namelist[i].l_name);
#endif

		i++;
	}
	fclose(fp);

	return ret;
}

int loadsamples(Sample_name* namelist, int num, IMAGE_H_MRF *h_mrf, IMAGE_L_MRF *l_mrf)
{
	int i;
	char name[1024];
	for(i = 0; i < num; i ++ )
	{
		sprintf(name,"%s%s",SAMPLE_DIR, namelist[i].h_name);
		loadhighimage(name, &h_mrf[i] );

		sprintf(name,"%s%s",SAMPLE_DIR, namelist[i].l_name);
		loadlowimage(name, &l_mrf[i]);
	}
	return 0;
}

int	loadhighimage(char *name, IMAGE_H_MRF *h_mrf)
{
	EHC_image h_image;
	int width;
	int height;
	unsigned char * bi_data;
	unsigned char * gray_data;
	//EHC_image testimage;
	//EHC_image testimage2;

	_open_image(name, &h_image);

	width = h_image.w;
	height = h_image.h;
	bi_data = _mallocate_image(width,height,8);
	gray_data = _mallocate_image(width,height,8);

	_convert_rgb2gray2(gray_data, &h_image);


	_segment_gray(gray_data, width, height, bi_data);

#ifdef DEBUG
	_save_gray_image(&testimage2,gray_data, width, height, &h_image );
	_save_image("test_gray.jpg", &testimage2);
	_save_image("test_himage.jpg", &h_image);
	_save_segment_image(&testimage, bi_data, width, height, &h_image);
	_save_image("test_binary.jpg", &testimage);
	free(testimage.data);
	free(testimage2.data);
#endif

	_createHMRF(bi_data, width, height, h_mrf);

	free(h_image.data);
	free(gray_data);
	free(bi_data);
	return 0;
}

int	loadlowimage(char *name, IMAGE_L_MRF *l_mrf)
{
	EHC_image l_image;
	int width;
	int height;
	unsigned char * bi_data;
	unsigned char * gray_data;
	//EHC_image testimage;
	//EHC_image testimage2;

	_open_image(name, &l_image);

	width = l_image.w;
	height = l_image.h;
	bi_data = _mallocate_image(width,height,8);
	gray_data = _mallocate_image(width,height,8);

	_convert_rgb2gray2(gray_data, &l_image);


	_segment_gray(gray_data, width, height, bi_data);

#ifdef DEBUG
	_save_gray_image(&testimage2,gray_data, width, height, &h_image );
	_save_image("test_gray.jpg", &testimage2);
	_save_image("test_himage.jpg", &h_image);
	_save_segment_image(&testimage, bi_data, width, height, &h_image);
	_save_image("test_binary.jpg", &testimage);
	free(testimage.data);
	free(testimage2.data);
#endif

	_createLMRF(bi_data, width, height, l_mrf);

	free(l_image.data);;
	free(gray_data);
	free(bi_data);
	return 0;
}

/**
 * yangruiguo 2011-12-08
 * bi_data: the data of processed pictures
 * width  : the width of processed pictures
 * height : the height of processed pictures
 * h_mrf  : the high quality pictures' MRF
 * return : 0 success 
 */
int _createHMRF(unsigned char *bi_data, int width, int height, IMAGE_H_MRF *h_mrf)
{
	int i = 0, j = 0;
	int m = 0, n = 0;
	int rt = 0, cur = 0;
	int xs = 0, xm = 0, xe = 0;
	int ys = 0, ym = 0, ye = 0;

	h_mrf->xnum = height / H_STEP;
	h_mrf->ynum = width / H_STEP;
	h_mrf->label = (H_label*)malloc(sizeof(H_label) * h_mrf->xnum * h_mrf->ynum);

	for(i = 0; i < h_mrf->xnum; i++)
	{
		for(j = 0; j < h_mrf->ynum; j++)
		{
			cur = i * h_mrf->ynum + j;
			xs = H_STEP * i;
			xm = (int)(H_STEP * (i + 0.5) - 1);
			xe = H_STEP * (i + 1) - 1;

			ys = H_STEP * j;
			ym = (int)(H_STEP * (j + 0.5) - 1);
			ye = H_STEP * (j + 1) - 1;

			h_mrf->label[cur].lt = _createSUB_MRF(bi_data, width, xs,     ys,     xm, ym);
			h_mrf->label[cur].rt = _createSUB_MRF(bi_data, width, xs,     ym + 1, xm, ye);
			h_mrf->label[cur].lb = _createSUB_MRF(bi_data, width, xm + 1, ys,     xe, ym);
			h_mrf->label[cur].rb = _createSUB_MRF(bi_data, width, xm + 1, ym + 1, xe, ye);
		}
	}

#ifdef DEBUG_LABEL
	if(is_first_label_hig)
	{
		_testHMRF(width, height, h_mrf, "test_mrf2img_hig.jpg");
		is_first_label_hig = false;
	}
#endif
	return 0;
}

/**
 * yangruiguo 2011-12-08
 * bi_data: the data of processed pictures
 * width  : the width of processed pictures
 * height : the height of processed pictures
 * h_mrf  : the low quality pictures' MRF
 * return : 0 success 
 */
int _createLMRF(unsigned char *bi_data, int width, int height, IMAGE_L_MRF *l_mrf)
{
	int i = 0;
	int j = 0;
	l_mrf->xnum = height / L_STEP;
	l_mrf->ynum = width / L_STEP;
	l_mrf->label = (L_label*)malloc(sizeof(L_label) * l_mrf->xnum * l_mrf->ynum);
	for(i = 0; i < l_mrf->xnum; i++)
	{
		for(j = 0; j < l_mrf->ynum; j++)
		{
			l_mrf->label[i * l_mrf->ynum + j] = _createSUB_MRF(bi_data, width, L_STEP * i, L_STEP * j, L_STEP * (i + 1) - 1, L_STEP * (j + 1) - 1); 
		}
	}

#ifdef DEBUG_LABEL
	if(is_first_label_low)
	{
		_testLMRF(width, height, l_mrf, "test_mrf2img_low.jpg");
		is_first_label_low = false;
	}
#endif
	return 0;
}

void _testLMRF(int width, int height, IMAGE_L_MRF *l_mrf, const char* fname)
{
	EHC_image image;
	char x[34];
	int k = 0;
	int len = l_mrf->xnum * l_mrf->ynum * L_STEP * L_STEP;
	unsigned char* d_data;
	int i, j;
	
	image.depth = 24;
	image.w  = width;
	image.h  = height;
	image.stride  = _width_bytes(image.depth, width);
	image.data = _mallocate_image(width,height,image.depth);

	d_data = (unsigned char*)malloc(width * height);
	memset(d_data, 0, width * height);
	for(i = 0; i < l_mrf->xnum; i++)
	{
		for(j = 0; j < l_mrf->ynum; j++)
		{
			_itoa(l_mrf->label[i * l_mrf->ynum + j], x, 2);
			k = (int)strlen(x) - 1;
			for(int m = L_STEP * (i + 1) - 1; m >= L_STEP * i && k >= 0; m--)
			{
				for(int n = L_STEP * (j + 1) - 1; n >= L_STEP * j && k >= 0; n--)
				{
					d_data[m * width + n] = (unsigned char)(x[k] - '0');
					k--;
				}
			}
		}
	}

	_set_segment_to_image(d_data, width, height, &image);
	_save_image(fname, &image);
	if(image.data != NULL)
	{
		free(image.data);
	}
	if(d_data != NULL)
	{
		free(d_data);
	}
}

void _testHMRF(int width, int height, IMAGE_H_MRF *h_mrf, const char* fname)
{
	EHC_image image;
	char x[34];
	int k = 0;
	int len = h_mrf->xnum * h_mrf->ynum * H_STEP * H_STEP;
	unsigned char* d_data;
	int i, j;
	int xs = 0, xm = 0, xe = 0;
	int ys = 0, ym = 0, ye = 0;
	
	image.depth = 24;
	image.w  = width;
	image.h  = height;
	image.stride  = _width_bytes(image.depth, width);
	image.data = _mallocate_image(width,height,image.depth);

	d_data = (unsigned char*)malloc(width * height);
	memset(d_data, 0, width * height);
	for(i = 0; i < h_mrf->xnum; i++)
	{
		for(j = 0; j < h_mrf->ynum; j++)
		{
			xs = H_STEP * i;
			xm = (int)(H_STEP * (i + 0.5) - 1);
			xe = H_STEP * (i + 1) - 1;

			ys = H_STEP * j;
			ym = (int)(H_STEP * (j + 0.5) - 1);
			ye = H_STEP * (j + 1) - 1;

			//lt
			_itoa(h_mrf->label[i * h_mrf->ynum + j].lt, x, 2);
			k = (int)strlen(x) - 1;
			for(int m = xm; m >= xs && k >= 0; m--)
			{
				for(int n = ym; n >= ys && k >= 0; n--)
				{
					d_data[m * width + n] = (unsigned char)(x[k] - '0');
					k--;
				}
			}

			//rt
			_itoa(h_mrf->label[i * h_mrf->ynum + j].rt, x, 2);
			k = (int)strlen(x) - 1;
			for(int m = xm; m >= xs && k >= 0; m--)
			{
				for(int n = ye; n >= ym + 1 && k >= 0; n--)
				{
					d_data[m * width + n] = (unsigned char)(x[k] - '0');
					k--;
				}
			}

			//lb
			_itoa(h_mrf->label[i * h_mrf->ynum + j].lb, x, 2);
			k = (int)strlen(x) - 1;
			for(int m = xe; m >= xm + 1 && k >= 0; m--)
			{
				for(int n = ym; n >= ys && k >= 0; n--)
				{
					d_data[m * width + n] = (unsigned char)(x[k] - '0');
					k--;
				}
			}

			//rb
			_itoa(h_mrf->label[i * h_mrf->ynum + j].rb, x, 2);
			k = (int)strlen(x) - 1;
			for(int m = xe; m >= xm + 1 && k >= 0; m--)
			{
				for(int n = ye; n >= ym + 1 && k >= 0; n--)
				{
					d_data[m * width + n] = (unsigned char)(x[k] - '0');
					k--;
				}
			}
		}
	}

	_set_segment_to_image(d_data, width, height, &image);
	_save_image(fname, &image);
	if(image.data != NULL)
	{
		free(image.data);
	}
	if(d_data != NULL)
	{
		free(d_data);
	}
}

/**
 * yangruiguo 2011-12-08
 * bi_data: the data of processed pictures
 * w      : the width of processed pictures
 * xs     : the start position of x axis
 * ys     : the start position of y axis
 * xe     : the end position of x axis
 * ye     : the end position of y axis
 * return : the processed result
 */
int _createSUB_MRF(unsigned char* bi_data, int w, int xs, int ys, int xe, int ye)
{
	int i = 0;
	int j = 0;
	int k = 24;
	int res = 0;

	for(i = xs; i <= xe; i++)
	{
		for(j = ys; j <= ye; j++)
		{
			if(bi_data[i * w + j] == 1)
			{
				res |= (1 << k);
			}
			k--;
		}
	}

	return res;
}
