#include "stdafx.h"
#include "load.h"
#include "cluster.h"
#include "imageprocess.h"
#include "imageutil.h"
#include "segment.h"
#include "iomrf.h"

/**
 * yangruiguo 2011-12-21
 * function  : write data to filename, h is the height, w is the width
 * h         : the height of data array
 * w         : the width of data array
 * data      : the data array 
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_2dimension_Text(int h, int w, float** data, const char* filename)
{
	int i = 0;
	int j = 0;
	FILE* pf;
	pf = fopen(filename, "w");
	fprintf(pf, "%d %d\n", h, w);
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			fprintf(pf, "%f ", data[i][j]);
		}
		fputs("\n", pf);
	}
	i = fclose(pf);
	for(i = 0; i < h; i++)
	{
		if(data[i] != NULL)
		{
			free(data[i]);
		}
	}
	if(data != NULL)
	{
		free(data);
	}

	return 0;
}

/**
 * yangruiguo 2011-12-29
 * function  : write middle statistic data to filename, h is the height, w is the width
 * h         : the height of data array
 * w         : the width of data array
 * c         : the count array which saves the appearence number of each yi 
 * data      : the data array 
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_2dimension_Text(int h, int w, int* c, float** data, const char* filename)
{
	int i = 0;
	int j = 0;
	FILE* pf;
	pf = fopen(filename, "w");
	fprintf(pf, "%d %d\n", h, w);
	for(i = 0; i < h; i++)
	{
		fprintf(pf, "[%d] ", c[i]);
		for(j = 0; j < w; j++)
		{
			fprintf(pf, "%d ", (int)data[i][j]);
		}
		fputs("\n", pf);
	}
	i = fclose(pf);
	return 0;
}

/**
 * function  : write data to filename, h is the height, w is the width
 * h         : the height of data array
 * w         : the width of data array
 * data      : the data array 
 * filename  : the filename which is going to write
 * return    : 0 success 
 */

int write_twodimension_Text(int h, int w, int** data, const char* filename)
{
	int i = 0;
	int j = 0;
	FILE* pf;
	pf = fopen(filename, "w");
	fprintf(pf, "%d %d\n", h, w);
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			fprintf(pf, "%d ", data[i][j]);
		}
		fputs("\n", pf);
	}

	for(i = 0; i < h; i++)
	{
		if(data[i] != NULL)
		{
			free(data[i]);
		}
	}
	if(data != NULL)
	{
		free(data);
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-21
 * function  : read data from filename to data
 * h         : the height of data array
 * w         : the width of data array
 * data      : the data array 
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_2dimension_Text(int* h, int* w, float** data, const char* filename)
{
	int i = 0;
	int j = 0;
	float* d = NULL;
	FILE* pf;

	pf = fopen(filename, "r");
	if(pf != NULL)
	{
		fscanf(pf, "%d %d", h, w);
		d = (float*)malloc(sizeof(float) * (*h) * (*w));
		for(i = 0; i < (*h); i++)
		{
			for(j = 0; j < (*w); j++)
			{
				fscanf(pf, "%f", &(d[i * (*w) + j]));
			}
		}
		*data = d;
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-27
 * data      : the data array 
 * return    : 0 success 
 */
int destroy_2dimension_Text(float* data)
{
	if(data != NULL)
	{
		free(data);
	}
	
	return 0;
}

/**
 * yangruiguo 2011-12-21
 * function  : write l_mrf to file
 * num       : the length of l_mrf
 * l_mrf     : the low images' MRF
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_LMRF_Text(int num, IMAGE_L_MRF* l_mrf, const char* filename)
{
	int i = 0;
	int j = 0;
	int k = 0;

	FILE* pf;
	pf = fopen(filename, "w");

	fprintf(pf, "%d\n", num);
	for(i = 0; i < num; i++)
	{
		fprintf(pf, "%d %d\n", l_mrf[i].xnum, l_mrf[i].ynum);
		for(j = 0; j < l_mrf[i].xnum; j++)
		{
			for(k = 0; k < l_mrf[i].ynum; k++)
			{
				fprintf(pf, "%d ", l_mrf[i].label[j * l_mrf[i].ynum + k]);
			}
			fputs("\n", pf);
		}
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-22
 * function  : read l_mrf from file
 * num       : the length of l_mrf
 * l_mrf     : the low images' MRF
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_LMRF_Text(int* num, IMAGE_L_MRF** l_mrf, const char* filename)
{
	int i = 0;
	int j = 0;
	int x = 0;
	int y = 0;

	FILE* pf;
	IMAGE_L_MRF* mrf = NULL;

	pf = fopen(filename, "r");
	if(pf != NULL)
	{
		fscanf(pf, "%d", num);
		mrf = (IMAGE_L_MRF*)malloc(sizeof(IMAGE_L_MRF) * (*num));
		for(i = 0; i < (*num); i++)
		{
			fscanf(pf, "%d %d", &x, &y);
			mrf[i].xnum = x;
			mrf[i].ynum = y;
			mrf[i].label = (L_label*)malloc(sizeof(L_label) * x * y);

			for(j = 0; j < x * y; j++)
			{
				fscanf(pf, "%d", &(mrf[i].label[j]));
			}
		}
		*l_mrf = mrf;
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-21
 * function  : write h_mrf to file
 * num       : the length of h_mrf
 * h_mrf     : the high images' MRF
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int write_HMRF_Text(int num, IMAGE_H_MRF* h_mrf, const char* filename)
{
	int i = 0;
	int j = 0;
	int k = 0;

	FILE* pf;
	pf = fopen(filename, "w");

	fprintf(pf, "%d\n", num);
	for(i = 0; i < num; i++)
	{
		fprintf(pf, "%d %d\n", h_mrf[i].xnum, h_mrf[i].ynum);
		for(j = 0; j < h_mrf[i].xnum; j++)
		{
			for(k = 0; k < h_mrf[i].ynum; k++)
			{
				fprintf(pf, "%d %d %d %d ", 
					h_mrf[i].label[j * h_mrf[i].ynum + k].lb, 
					h_mrf[i].label[j * h_mrf[i].ynum + k].lt, 
					h_mrf[i].label[j * h_mrf[i].ynum + k].rb, 
					h_mrf[i].label[j * h_mrf[i].ynum + k].rt);
			}
			fputs("\n", pf);
		}
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-22
 * function  : read h_mrf from file
 * num       : the length of h_mrf
 * h_mrf     : the high images' MRF
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int read_HMRF_Text(int* num, IMAGE_H_MRF** h_mrf, const char* filename)
{
	int i = 0;
	int j = 0;
	int x = 0;
	int y = 0;

	FILE* pf;
	IMAGE_H_MRF* mrf = NULL;

	pf = fopen(filename, "r");
	if(pf != NULL)
	{
		fscanf(pf, "%d", num);
		mrf = (IMAGE_H_MRF*)malloc(sizeof(IMAGE_H_MRF) * (*num));
		for(i = 0; i < (*num); i++)
		{
			fscanf(pf, "%d %d", &x, &y);
			mrf[i].xnum = x;
			mrf[i].ynum = y;
			mrf[i].label = (H_label*)malloc(sizeof(H_label) * x * y);

			for(j = 0; j < x * y; j++)
			{
				fscanf(pf, "%d", &(mrf[i].label[j].lb));
				fscanf(pf, "%d", &(mrf[i].label[j].lt));
				fscanf(pf, "%d", &(mrf[i].label[j].rb));
				fscanf(pf, "%d", &(mrf[i].label[j].rt));
			}
		}
		*h_mrf = mrf;
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-23
 * function  : write l_cluster to file
 * num       : the length of l_cluster
 * l_cluster : the low images' clusters
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_LCLUSTER_Text(int num, L_cluster* l_cluster, const char* filename)
{
	int i = 0;
	int j = 0;
	int k = 0;

	FILE* pf;
	pf = fopen(filename, "w");

	fprintf(pf, "%d\n", num);
	for(i = 0; i < num; i++)
	{
		fprintf(pf, "%d %d\n", l_cluster[i].l, l_cluster[i].subNum);
		
		for(j = 0; j < l_cluster[i].subNum; j++)
		{
			fprintf(pf, "%d ", l_cluster[i].sub[j]);
		}
		fputs("\n", pf);
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-23
 * function  : read l_cluster from file
 * num       : the length of l_cluster
 * l_cluster : the low images' clusters
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_LCLUSTER_Text(int* num, L_cluster** l_cluster, const char* filename)
{
	int i = 0;
	int j = 0;
	int x = 0;
	int y = 0;

	FILE* pf;
	L_cluster* cluster = NULL;

	pf = fopen(filename, "r");
	if(pf != NULL)
	{
		fscanf(pf, "%d", num);
		cluster = (L_cluster*)malloc(sizeof(L_cluster) * (*num));
		for(i = 0; i < (*num); i++)
		{
			fscanf(pf, "%d %d", &x, &y);
			cluster[i].l = x;
			cluster[i].subNum = y;
			cluster[i].sub = (L_label*)malloc(sizeof(L_label) * y);

			if(y > 0)
			{
				for(j = 0; j < y; j++)
				{
					fscanf(pf, "%d", &(cluster[i].sub[j]));
				}
			}
		}
		*l_cluster = cluster;
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-23
 * function  : write h_cluster to file
 * num       : the length of h_cluster
 * h_cluster : the high images' clusters
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_HCLUSTER_Text(int num, H_cluster* h_cluster, const char* filename)
{
	int i = 0;
	int j = 0;
	int k = 0;

	FILE* pf;
	pf = fopen(filename, "w");

	fprintf(pf, "%d\n", num);
	for(i = 0; i < num; i++)
	{
		fprintf(pf, "%d %d %d %d %d\n", h_cluster[i].l.lb, 
										h_cluster[i].l.lt, 
										h_cluster[i].l.rb, 
										h_cluster[i].l.rt, 
										h_cluster[i].subNum);

		for(j = 0; j < h_cluster[i].subNum; j++)
		{
			fprintf(pf, "%d %d %d %d ", h_cluster[i].sub[j].lb,
				                        h_cluster[i].sub[j].lt,
										h_cluster[i].sub[j].rb,
										h_cluster[i].sub[j].rt);
		}
		fputs("\n", pf);
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2011-12-23
 * function  : read h_cluster from file
 * num       : the length of h_cluster
 * h_cluster : the high images' clusters
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_HCLUSTER_Text(int* num, H_cluster** h_cluster, const char* filename)
{
	int i = 0;
	int j = 0;

	FILE* pf;
	H_cluster* cluster = NULL;

	pf = fopen(filename, "r");
	if(pf != NULL)
	{
		fscanf(pf, "%d", num);
		cluster = (H_cluster*)malloc(sizeof(H_cluster) * (*num));
		for(i = 0; i < (*num); i++)
		{
			fscanf(pf, "%d %d %d %d %d", &cluster[i].l.lb, 
										 &cluster[i].l.lt, 
										 &cluster[i].l.rb, 
										 &cluster[i].l.rt, 
										 &cluster[i].subNum);
			cluster[i].sub = (H_label*)malloc(sizeof(H_label) * cluster[i].subNum);

			if(cluster[i].subNum > 0)
			{
				for(j = 0; j < cluster[i].subNum; j++)
				{
					fscanf(pf, "%d %d %d %d", &cluster[i].sub[j].lb,
				                              &cluster[i].sub[j].lt,
										      &cluster[i].sub[j].rb,
										      &cluster[i].sub[j].rt);
				}
			}
		}
		*h_cluster = cluster;
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2012-01-15
 * function  : write projected mrf to text files
 * num       : the length of data
 * xlen      : the x direction length of data[i]
 * ylen      : the y direction length of data[i]
 * data      : the projected data array
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_PMRF_Text(int num, int xlen, int ylen, int** data, const char* filename)
{
	int i = 0;
	int j = 0;
	int k = 0;

	FILE* pf;
	pf = fopen(filename, "w");

	fprintf(pf, "%d\n", num);
	for(i = 0; i < num; i++)
	{
		fprintf(pf, "%d %d\n", xlen, ylen);
		for(j = 0; j < xlen; j++)
		{
			for(k = 0; k < ylen; k++)
			{
				fprintf(pf, "%d ", data[i][j * ylen + k]);
			}
			fputs("\n", pf);
		}
	}
	i = fclose(pf);
	return 0;
}

/**
 * yangruiguo 2012-02-02
 * function  : save a two dimension array use crs method
 * h         : the height of data
 * w         : the width of data
 * data      : the processed data array
 * colname   : the file's name which saves the col array
 * rowname   : the file's name which saves the row array
 * return    : 0 success 
 */
int saveArrayAsCRS(int h, int w, float** data, const char* colname, const char* rowname)
{
	int i = 0;
	int j = 0;
	int n = 0;
	float* tmp = NULL;

	float* col = (float*)malloc(sizeof(float) * n);
	int* row = (int*)malloc(sizeof(int) * h);
	
	memset(row, -1, sizeof(int) * h);

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			if(data[i][j] - 0 != 0)
			{
				tmp = (float*)realloc(col, sizeof(float) * (n + 1));
				if(tmp != NULL)
				{
					col = tmp;
				}

				col[n++] = data[i][j];
				if(row[i] == -1)
				{
					row[i] = n - 1;
				}
			}
		}
	}

	i = saveCRS(n, h, col, row, colname, rowname);

	return 0;
}

/**
 * yangruiguo 2012-02-02
 * function  : save crs table to text files
 * lenCol    : the length of col array
 * lenRow    : the length of row array
 * col       : the col data
 * row       : the row data
 * colname   : the file's name which saves the col array
 * rowname   : the file's name which saves the row array
 * return    : 0 success 
 */
int saveCRS(int lenCol, int lenRow, float* col, int* row, const char* colname, const char* rowname)
{
	int i = 0;

	FILE* pf;

	pf = fopen(colname, "w");
	fprintf(pf, "%d\n", lenCol);
	for(i = 0; i < lenCol; fprintf(pf, "%f ", col[i++]));
	i = fclose(pf);

	pf = fopen(rowname, "w");
	fprintf(pf, "%d\n", lenRow);
	for(i = 0; i < lenRow; fprintf(pf, "%d ", row[i++]));
	i = fclose(pf);

	return 0;
}