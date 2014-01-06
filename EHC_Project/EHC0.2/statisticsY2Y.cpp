#include "stdafx.h"
#include "load.h"
#include "cluster.h"
#include "imageprocess.h"
#include "imageutil.h"
#include "segment.h"
#include "statisticsY2Y.h"
#include "iomrf.h"

/**
 * yangruiguo 2011-12-20
 * num       : the number of pictures
 * h_num     : the number of high quality images' cluster
 * h_mrf     : the high quality images' MRF
 * h_cluster : the high quality images' clusters
 * y2yA      : the calculated result A, a 2 dimensions array, right
 * y2yB      : the calculated result B, a 2 dimensions array, top
 * y2yC      : the calculated result C, a 2 dimensions array, top-right
 * y2yD      : the calculated result D, a 2 dimensions array, bottom-right
 * return    : 0 success 
 */

//Modify_Start 20111223 Candy
//int _statisY2Y(int num, LL h_num, IMAGE_H_MRF* h_mrf, H_cluster* h_cluster, float*** y2yA, float*** y2yB, float*** y2yC, float*** y2yD)
int _statisY2Y(int num, LL h_num, IMAGE_H_MRF* h_mrf, LL*** o_hmrf, H_cluster* h_cluster, float*** y2yA, float*** y2yB, float*** y2yC, float*** y2yD)
//Modify_End 20111223
{
	int i;
	int h = h_mrf->xnum;
	int w = h_mrf->ynum;
	int status = 0;	
	int len = h * w;
	int* ca = NULL;
	int* cb = NULL;
	int* cc = NULL;
	int* cd = NULL;
	float** ya = NULL;
	float** yb = NULL;
	float** yc = NULL;
	float** yd = NULL;
	H_statis* h_statis = (H_statis*)malloc(sizeof(H_statis) * h_num);

	LL** hmrf = (LL**)malloc(sizeof(LL*) * num);


	ca = (int*)malloc(sizeof(int) * h_num);
	cb = (int*)malloc(sizeof(int) * h_num);
	cc = (int*)malloc(sizeof(int) * h_num);
	cd = (int*)malloc(sizeof(int) * h_num);

	ya = (float**)malloc(sizeof(float*) * h_num);
	yb = (float**)malloc(sizeof(float*) * h_num);
	yc = (float**)malloc(sizeof(float*) * h_num);
	yd = (float**)malloc(sizeof(float*) * h_num);

	memset(ca, 0, sizeof(int) * h_num);
	memset(cb, 0, sizeof(int) * h_num);
	memset(cc, 0, sizeof(int) * h_num);
	memset(cd, 0, sizeof(int) * h_num);

	for(i = 0; i < h_num; i++)
	{
		ya[i] = (float*)malloc(sizeof(float) * h_num);
		yb[i] = (float*)malloc(sizeof(float) * h_num);
		yc[i] = (float*)malloc(sizeof(float) * h_num);
		yd[i] = (float*)malloc(sizeof(float) * h_num);

		memset(ya[i], 0, sizeof(float) * h_num);
		memset(yb[i], 0, sizeof(float) * h_num);
		memset(yc[i], 0, sizeof(float) * h_num);
		memset(yd[i], 0, sizeof(float) * h_num);
	}

	status = _projectY2Y(h_num, h_cluster, h_statis);

	//Delete_Start 20111223 Candy
	/* free h_cluster */
	//if(h_cluster != NULL)
	//{
	//	free(h_cluster);
	//}
    //Delete_End 20111223

	/* statistic */
	for(i = 0; i < num; i++)
	{
		hmrf[i] = (LL*)malloc(sizeof(LL) * len);
		status = _replaceY2Y(h_num, h_statis, &h_mrf[i], hmrf[i]);
		status = _statisY2Y_step1(h, w, hmrf[i], ca, cb, cc, cd, ya, yb, yc, yd);
	}

	/* write the statistic middle result */
//20111230 test use start
	status = write_2dimension_Text(h_num, h_num, ca, ya, OUTPUT_DIR_NAME_mA);
	status = write_2dimension_Text(h_num, h_num, cb, yb, OUTPUT_DIR_NAME_mB);
	status = write_2dimension_Text(h_num, h_num, cc, yc, OUTPUT_DIR_NAME_mC);
	status = write_2dimension_Text(h_num, h_num, cd, yd, OUTPUT_DIR_NAME_mD);
//20111230 test use end

	/* the step 2 of statistic */
	status = _statisY2Y_step2(h_num, ca, ya);
	status = _statisY2Y_step2(h_num, cb, yb);
	status = _statisY2Y_step2(h_num, cc, yc);
	status = _statisY2Y_step2(h_num, cd, yd);

	*y2yA = ya;
	*y2yB = yb;
	*y2yC = yc;
	*y2yD = yd;

	//Add_Start 20111223 Candy
	*o_hmrf = hmrf;
	//Add_End 20111223

	if(ca != NULL)
	{
		free(ca);
	}
	if(cb != NULL)
	{
		free(cb);
	}
	if(cc != NULL)
	{
		free(cc);
	}
	if(cd != NULL)
	{
		free(cd);
	}

	//Delete_Start 20111223 Candy
	//if(hmrf != NULL)
	//{
	//	for(i = 0; i < num; i++)
	//	{
	//		if(hmrf[i] != NULL)
	//		{
	//			free(hmrf[i]);
	//		}
	//	}
	//	free(hmrf);
	//}
	//Delete_End 20111223

	if(h_statis != NULL)
	{
		free(h_statis);
	}
	
	return 0;
}

/**
 * yangruiguo 2011-12-20
 * h_num     : the number of high quality images' cluster
 * h_statis  : the high quality images' project
 * h_mrf     : the high images' MRF
 * hmrf      : the replaced high images' MRF
 * return    : 0 success 
 */
int _replaceY2Y(LL h_num, H_statis* h_statis, IMAGE_H_MRF *h_mrf, LL* hmrf)
{
	int i = 0;
	int j = 0;
	LL k = 0;
	bool flg = false;
	
	for(i = 0; i < h_mrf->xnum; i++)
	{
		for(j = 0; j < h_mrf->ynum; j++)
		{
			flg = false;
			for(k = 0; k < h_num && !flg; k++)
			{
				if(h_mrf->label[i * h_mrf->ynum + j].lb == h_statis[k].l.lb &&
				   h_mrf->label[i * h_mrf->ynum + j].lt == h_statis[k].l.lt &&
				   h_mrf->label[i * h_mrf->ynum + j].rb == h_statis[k].l.rb &&
				   h_mrf->label[i * h_mrf->ynum + j].rt == h_statis[k].l.rt)
				{
					hmrf[i * h_mrf->ynum + j] = h_statis[k].v;
					flg = true;
				}
			}
			if(!flg)
			{
				hmrf[i * h_mrf->ynum + j] = 0;
			}
		}
	}
	return 0;
}

/**
 * yangruiguo 2011-12-20
 * h_num     : the number of high quality images' cluster
 * h_cluster : the high quality images' clusters
 * h_statis  : the projected result
 * return    : 0 success 
 */
int _projectY2Y(LL h_num, H_cluster* h_cluster, H_statis* h_statis)
{
	LL i = 0;
	for(i = 0; i < h_num; i++)
	{
		h_statis[i].v = i;
		h_statis[i].l.lb = h_cluster[i].l.lb;
		h_statis[i].l.lt = h_cluster[i].l.lt;
		h_statis[i].l.rb = h_cluster[i].l.rb;
		h_statis[i].l.rt = h_cluster[i].l.rt;
	}

	return 0;
}

/**
 * yangruiguo 2011-12-21
 * h         : the height of hmrf
 * w         : the width of hmrf
 * h_mrf     : the high quality images' MRF
 * ca        : the amount of certain MRF appearance in type a
 * cb        : the amount of certain MRF appearance in type b
 * cc        : the amount of certain MRF appearance in type c
 * cd        : the amount of certain MRF appearance in type d
 * ya        : the calculated result A, a 2 dimensions array
 * yb        : the calculated result B, a 2 dimensions array
 * yc        : the calculated result C, a 2 dimensions array
 * yd        : the calculated result D, a 2 dimensions array
 * return    : 0 success 
 */
int _statisY2Y_step1(int h, int w, LL* hmrf, int* ca, int* cb, int* cc, int* cd, float** ya, float** yb, float** yc, float** yd)
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
				ya[hmrf[m]][hmrf[m + 1]]++;
				ca[hmrf[m]]++;
			}
			//B
			if(i - 1 >= 0)
			{
				yb[hmrf[m]][hmrf[m - w]]++;
				cb[hmrf[m]]++;
			}
			//C
			if(i - 1 >= 0 && j + 1 < w)
			{
				yc[hmrf[m]][hmrf[m - w + 1]]++;
				cc[hmrf[m]]++;
			}
			//D
			if(i + 1 < h && j + 1 < w)
			{
				yd[hmrf[m]][hmrf[m + w + 1]]++;
				cd[hmrf[m]]++;
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
int _statisY2Y_step2(LL h, int* ca, float** ya)
{
	LL i = 0;
	LL j = 0;

	for(i = 0; i < h; i++)
	{
		for(j = 0; j < h; j++)
		{
			if(ca[i] < 1 )
			{
				ya[i][j] = 0;
			}
			else
			{
				ya[i][j] = (float)(ya[i][j] * 1.0 / ca[i]);
			}
		}
	}
	return 0;
}