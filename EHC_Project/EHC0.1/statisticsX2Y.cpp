#include "stdafx.h"
#include "statisticsX2Y.h"
#include "iomrf.h"

/**
 * yangruiguo 2012-02-02
 * function  : statistic the table x to y
 * num       : the number of image pairs
 * h         : the height of each image
 * w         : the width of each image
 * l_num     : the number of low images' cluster
 * h_num     : the number of high images' cluster
 * pl_mrf    : the array which saves the projected low quality images 
 * ph_mrf    : the array which saves the projected high quality images
 * return    : 0 success 
 */
int _statisX2Y(int num, int h, int w, int l_num, int h_num, int** pl_mrf, int** ph_mrf)
{	
	int i = 0;
	int j = 0;
	int k = 0;
	int t = 0;
	int status = 0;
	int* c = (int*)malloc(sizeof(int) * l_num);
	float** p = (float**)malloc(sizeof(float*) * l_num);

	memset(c, 0, sizeof(int) * l_num);
	for(i = 0; i < l_num; i++)
	{
		p[i] = (float*)malloc(sizeof(float) * h_num);
		memset(p[i], 0, sizeof(float) * h_num);
	}

	for(i = 0; i < num; i++)
	{
		for(j = 0; j < h; j++)
		{
			for(k = 0; k < w; k++)
			{
				t = j * w + k;
				c[pl_mrf[i][t]] += 1;
				p[pl_mrf[i][t]][ph_mrf[i][t]] += 1;
			}
		}
	}

	for(i = 0; i < l_num; i++)
	{
		for(j = 0; j < h_num; j++)
		{
			p[i][j] = (float)(p[i][j] * 1.0 / c[i]);
		}
	}

	status = saveArrayAsCRS(l_num, h_num, p, OUTPUT_COL_XY, OUTPUT_ROW_XY);

	/* release the memory */
	if(c != NULL)
	{
		free(c);
	}
	if(p != NULL)
	{
		for(i = 0; i < l_num; i++)
		{
			if(p[i] != NULL)
			{
				free(p[i]);
			}
		}
		free(p);
	}

	return 0;
}

/**
 * yangruiguo 2012-02-02
 * function  : order each p[i], and save the order in certain file
 * l_num     : the number of low images' cluster, which eaqles the length of p
 * h_num     : the number of high images' cluster, which eaqles the length of p[i]
 * return    : 0 success 
 */
int orderX2Y(int l_num, int h_num, float** p)
{
	return 0;
}