#include "stdafx.h"
#include "statisticsY2Y.h"
#include "iomrf.h"

/**
 * yangruiguo 2012-02-02
 * num       : the number of pictures
 * h         : the hight of each hmrf
 * w         : the width of each hmrf
 * h_num     : the number of high quality images' cluster
 * ph_mrf    : the project result of high quality images
 * return    : 0 success 
 */
int _statisY2Y(int num, int h, int w, int h_num, int** ph_mrf)
{
	int i;
	int status = 0;	
	int* c = NULL;
	float** y = NULL;

	c = (int*)malloc(sizeof(int) * h_num);
	y = (float**)malloc(sizeof(float*) * h_num);

	memset(c, 0, sizeof(int) * h_num);
	for(i = 0; i < h_num; i++)
	{
		y[i] = (float*)malloc(sizeof(float) * h_num);
		memset(y[i], 0, sizeof(float) * h_num);
	}

	/* statistic type A */
	for(i = 0; i < num; i++)
	{
		status = _statisY2Y_step1_A(h, w, ph_mrf[i], c, y);
	}
	status = _statisY2Y_step2(h_num, c, y);
	status = saveArrayAsCRS(h_num, h_num, y, OUTPUT_COL_A, OUTPUT_ROW_A);
	clearDimension(h_num, c, y);

	/* statistic type B */
	for(i = 0; i < num; i++)
	{
		status = _statisY2Y_step1_B(h, w, ph_mrf[i], c, y);
	}
	status = _statisY2Y_step2(h_num, c, y);
	status = saveArrayAsCRS(h_num, h_num, y, OUTPUT_COL_B, OUTPUT_ROW_B);
	clearDimension(h_num, c, y);

	/* statistic type C */
	for(i = 0; i < num; i++)
	{
		status = _statisY2Y_step1_C(h, w, ph_mrf[i], c, y);
	}
	status = _statisY2Y_step2(h_num, c, y);
	status = saveArrayAsCRS(h_num, h_num, y, OUTPUT_COL_C, OUTPUT_ROW_C);
	clearDimension(h_num, c, y);

	/* statistic type D */
	for(i = 0; i < num; i++)
	{
		status = _statisY2Y_step1_D(h, w, ph_mrf[i], c, y);
	}
	status = _statisY2Y_step2(h_num, c, y);
	status = saveArrayAsCRS(h_num, h_num, y, OUTPUT_COL_D, OUTPUT_ROW_D);

	/* release the memory */
	if(c != NULL)
	{
		free(c);
	}
	if(y != NULL)
	{
		for(i = 0; i < h_num; i++)
		{
			if(y[i] != NULL)
			{
				free(y[i]);
			}
		}
		free(y);
	}
	
	return 0;
}

/**
 * yangruiguo 2012-02-02
 * function  : clear the one dimension array
 * len       : the length of data array
 * c         : the count array
 * y         : the statistic result array
 */
void clearDimension(int len, int*& c, float**& y)
{
	int i = 0;
	memset(c, 0, sizeof(int) * len);
	for(i = 0; i < len; i++)
	{
		memset(y[i], 0, sizeof(float) * len);
	}
}

/**
 * yangruiguo 2012-02-02
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * c         : the amount of certain MRF appearance in this type
 * y         : the calculated result, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1_A(int h, int w, int* ph_mrf, int* c, float** y)
{
	int i = 0;
	int j = 0;
	int m = 0;

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			m = i * w + j;
			//A
			if(j + 1 < w)
			{
				c[ph_mrf[m]]++;
				y[ph_mrf[m]][ph_mrf[m + 1]] += 1;
			}
		}
	}
	return 0;
}

/**
 * yangruiguo 2012-02-02
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * c         : the amount of certain MRF appearance in this type
 * y         : the calculated result, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1_B(int h, int w, int* ph_mrf, int* c, float** y)
{
	int i = 0;
	int j = 0;
	int m = 0;

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			m = i * w + j;
			//B
			if(i - 1 >= 0)
			{
				c[ph_mrf[m]]++;
				y[ph_mrf[m]][ph_mrf[m - w]] += 1;
			}
		}
	}
	return 0;
}

/**
 * yangruiguo 2012-02-02
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * c         : the amount of certain MRF appearance in this type
 * y         : the calculated result, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1_C(int h, int w, int* ph_mrf, int* c, float** y)
{
	int i = 0;
	int j = 0;
	int m = 0;

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			m = i * w + j;
			//C
			if(i - 1 >= 0 && j + 1 < w)
			{
				c[ph_mrf[m]]++;
				y[ph_mrf[m]][ph_mrf[m - w + 1]] += 1;
			}
		}
	}
	return 0;
}

/**
 * yangruiguo 2012-02-02
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * c         : the amount of certain MRF appearance in this type
 * y         : the calculated result, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1_D(int h, int w, int* ph_mrf, int* c, float** y)
{
	int i = 0;
	int j = 0;
	int m = 0;

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			m = i * w + j;
			//D
			if(i + 1 < h && j + 1 < w)
			{
				c[ph_mrf[m]]++;
				y[ph_mrf[m]][ph_mrf[m + w + 1]] += 1;
			}
		}
	}
	return 0;
}

/**
 * yangruiguo 2011-12-21
 * h         : the length of ca
 * ca        : the amount of certain MRF appearance in type a
 * ya        : the calculated result A, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step2(int h, int* ca, float** ya)
{
	int i = 0;
	int j = 0;

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < h; j++)
		{
			if(ca[i] < 1 )
			{
				ya[i][j] = 0;
				break;
			}
			else
			{
				ya[i][j] = (float)(ya[i][j] * 1.0 / ca[i]);
			}
		}
	}
	return 0;
}