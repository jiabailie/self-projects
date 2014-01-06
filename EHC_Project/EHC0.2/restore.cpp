// EHC_Train.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "load.h"
#include "cluster.h"
#include "statisticsY2Y.h"
#include "statisticsX2Y.h"
#include "iomrf.h"
#include "restore.h"


int restore(char* infilename, char *outfilename)
{
	int status = 0;

	LL ah, aw;
	LL bh, bw;
	LL ch, cw;
	LL dh, dw;
	LL oh, ow;
	LL xyh, xyw;

	LL l_num = 0;
	LL h_num = 0;

	L_cluster* l_cluster = NULL;
	H_cluster* h_cluster = NULL;

	float* y2yA = NULL;
	float* y2yB = NULL;
	float* y2yC = NULL;
	float* y2yD = NULL;
	float* orderx2y = NULL;
	float* x2y  = NULL;

	IMAGE_L_MRF l_mrf;
	IMAGE_H_MRF h_mrf;

	//load clusters
	status = read_LCLUSTER_Text(&l_num, &l_cluster, LOW_CLUSTER_NAME);
	status = read_HCLUSTER_Text(&h_num, &h_cluster, HIG_CLUSTER_NAME);

	//load tables
	status = read_2dimension_Text(&ah, &aw, &y2yA, OUTPUT_DIR_NAME_A);
	status = read_2dimension_Text(&bh, &bw, &y2yB, OUTPUT_DIR_NAME_B);
	status = read_2dimension_Text(&ch, &cw, &y2yC, OUTPUT_DIR_NAME_C);
	status = read_2dimension_Text(&dh, &dw, &y2yD, OUTPUT_DIR_NAME_D);
	status = read_2dimension_Text(&xyh, &xyw, &x2y, OUTPUT_DIR_NAME_XY);
	status = read_2dimension_Text(&oh, &ow, &orderx2y, OUTPUT_DIR_NAME_OrderXY);

	//load low image
	loadlowimage(infilename, &l_mrf);

	//replace l_mrf to low cluster
	status = replaceLMRF_Store(l_num, l_cluster, &l_mrf);
	
	status = makeFinalMRF(&l_mrf, &h_mrf, 
		l_num, l_cluster,
		h_num, h_cluster,
		y2yA, ah, aw,
		y2yB, bh, bw,
		y2yC, ch, cw,
		y2yD, dh, dw,
		x2y, xyh, xyw,
		orderx2y, oh, ow
		);
	
	//write restored image
	_testHMRF2img(h_mrf.ynum * H_DIS, h_mrf.xnum * H_DIS, &h_mrf, outfilename);

	//destroy mrf
	status = destroy_lmrf(&l_mrf, 1);
	status = destroy_hmrf(&h_mrf, 1);

	//destroy arrays
	status = destroy_2dimension_Text(y2yA);
	status = destroy_2dimension_Text(y2yB);
	status = destroy_2dimension_Text(y2yC);
	status = destroy_2dimension_Text(y2yD);
	status = destroy_2dimension_Text(orderx2y);
	status = destroy_2dimension_Text(x2y);

	return 0;
}

/**
 * yangruiguo 2011-12-27
 * num     : the number of low images' cluster for restore
 * lcluster: the low images' clusters
 * l_mrf   : the low images' MRF
 * return  : 0 success
 */
int replaceLMRF_Store(LL num, L_cluster* lcluster, IMAGE_L_MRF *l_mrf)
{
	LL i = 0;
	int r = 0;
	int t = 0;
	int j = 0;
	int m = 0;
	int n = 0;
	int min = 0;

	for(m = 0; m < l_mrf->xnum; m++)
	{
		for(n = 0; n < l_mrf->ynum; n++)
		{
			r = lcluster[0].l;
			min = countDistance(l_mrf->label[m * l_mrf->ynum + n], lcluster[0].l);
			for(i = 1; i < num; i++)
			{
				t = countDistance(l_mrf->label[m * l_mrf->ynum + n], lcluster[j].l);
				if(t < min)
				{
					r = lcluster[j].l;
					min = t;
				}
				if(min == 0)
				{
					break;
				}
			}
		}
	}
	return 0;
}

/**
 * Candy 2011-12-27
 * return  : 0 success
 */
//********************************//
//y2ya
//************************
//*   y      *     y1    *
//*          *           *
//************************
//y2yb
//************
//*   y1     *
//************
//*   y      *
//************
//y2yc
//           ************
//           *   y1     *
//***********************
//*   y      *
//************
//y2yd
//************
//*   y1     *
//***********************
//           *   y      *
//           ************
//********************************//

int makeFinalMRF(IMAGE_L_MRF* l_mrf, IMAGE_H_MRF* h_mrf, 
		LL l_num, L_cluster* l_cluster,
		LL h_num, H_cluster* h_cluster,
		float* y2yA, LL ah, LL aw,
		float* y2yB, LL bh, LL bw,
		float* y2yC, LL ch, LL cw,
		float* y2yD, LL dh, LL dw,
		float* x2y, LL xyh, LL xyw,
		float* orderx2y, LL oh, LL ow
		)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int n0 = 0;
	int n1 = 0;
	int n2 = 0;
	int n3 = 0;
	int n4 = 0;
	int n5 = 0;
	int n6 = 0;
	int n7 = 0;
	int n8 = 0;
	int index;
	float maxprob = 0.0;
	float jointprob = 0.0;


	int xidx[9];
	int* yidx[9];
	int* result;
	float* prob[9];
	
	int lx;
	int hy[9];
	int lidx[9];
	int hidx[9];

	L_label tmplabel;

	result = (int *)malloc(sizeof(int) * l_num);

	for(i = 0; i < 9; i ++ )
	{
		yidx[i] = (int *)malloc(sizeof(int)*L_H_MATCH_OPTION);
		//prob[i] = (float *)malloc(sizeof(float)*L_H_MATCH_OPTION);
		//yltidx = (int *)malloc(sizeof(int)*L_H_MATCH_OPTION);
		//yttidx = (int *)malloc(sizeof(int)*L_H_MATCH_OPTION);
		//yrtidx = (int *)malloc(sizeof(int)*L_H_MATCH_OPTION);
		//yllidx = (int *)malloc(sizeof(int)*L_H_MATCH_OPTION);
		//yrridx = (int *)malloc(sizeof(int)*L_H_MATCH_OPTION);
		//ylbidx = (int *)malloc(sizeof(int)*L_H_MATCH_OPTION);
		//yrbidx = (int *)malloc(sizeof(int)*L_H_MATCH_OPTION);
	}

	IMAGE_H_MRF* hmrf = (IMAGE_H_MRF*)malloc(sizeof(IMAGE_H_MRF));

	hmrf->xnum = l_mrf->xnum;
	hmrf->ynum = l_mrf->ynum;

	hmrf->label = (H_label*)malloc(sizeof(H_label) * l_mrf->xnum * l_mrf->ynum);

	for(i = 0; i < hmrf->xnum; i++)
	{
		for(j = 0; j < hmrf->ynum; j++)
		{
			//hmrf->label[i * hmrf->ynum + j].lb = h_cluster[0].l.lb;
			//hmrf->label[i * hmrf->ynum + j].rb = h_cluster[0].l.rb;
			//hmrf->label[i * hmrf->ynum + j].lt = h_cluster[0].l.lt;
			//hmrf->label[i * hmrf->ynum + j].rt = h_cluster[0].l.rt;
			hmrf->label[i * hmrf->ynum + j].lb = 0;
			hmrf->label[i * hmrf->ynum + j].rb = 0;
			hmrf->label[i * hmrf->ynum + j].lt = 0;
			hmrf->label[i * hmrf->ynum + j].rt = 0;
		}
	}
	//visit all low mrf
	for(i = 0; i < l_mrf->xnum; i ++ )
	{
		for(j = 0; j < l_mrf->ynum; j ++ )
		{
			//search dest low mrf index 
			//l_num
			//l_mrf
			// x_idx;
			for( m = 0; m < 9 ; m ++ )
			{
				switch(m)
				{
				case 0:
					if((i-1) < 0 || (j-1) < 0)
					{
						tmplabel = 0;	//edge pixel
					}
					else
					{
						index = (i-1) * l_mrf->ynum + j - 1;
						tmplabel = l_mrf->label[index];
					}
					break;
				case 1:
					if((i-1) < 0)
					{
						tmplabel = 0;	//edge pixel  
					}
					else
					{
						index = (i-1) * l_mrf->ynum + j;
						tmplabel = l_mrf->label[index];
					}	
					break;
				case 2:
					if((i-1) < 0)
					{
						tmplabel = 0;	//edge pixel
					}
					else
					{
						index = (i-1) * l_mrf->ynum + j + 1;
						tmplabel = l_mrf->label[index];
					}
					break;
				case 3:
					if((j-1) < 0 )
					{
						tmplabel = 0;	//edge pixel
					}
					else
					{
						index = i * l_mrf->ynum + j - 1;
						tmplabel = l_mrf->label[index];
					}
					break;
				case 4:
						index = i * l_mrf->ynum + j;
						tmplabel = l_mrf->label[index];
					break;
				case 5:
					if((j+1) > l_mrf->ynum)
					{
						tmplabel = 0;
					}
					else
					{
						index = i * l_mrf->ynum + j + 1;
						tmplabel = l_mrf->label[index];
					}	
					break;
				case 6:
					if((j-1) < 0 || (i+1) > l_mrf->xnum)
					{
						tmplabel = 0;	//edge pixel
					}
					else
					{
						index = (i+1) * l_mrf->ynum + j - 1;
						tmplabel = l_mrf->label[index];
					}
					break;
				case 7:
					if((i+1) > l_mrf->xnum)
					{
						tmplabel = 0;
					}
					else
					{
						index = (i+1) * l_mrf->ynum + j;
						tmplabel = l_mrf->label[index];
					}		
					break;
				case 8:
					if((i+1) > l_mrf->xnum || (j+1) > l_mrf->ynum)
					{
						tmplabel = 0;
					}
					else
					{
						index = (i+1) * l_mrf->ynum + j + 1;
						tmplabel = l_mrf->label[index];
					}	
					break;
				}
				for(k = 0; k < l_num; k ++ )
				{

					if( tmplabel == l_cluster[k].l )
					{
						break;
					}
				}
				xidx[m] = k;
				index = xidx[m] * h_num;	//convert 2dimension table orderx2y
				for(k = 0; k < L_H_MATCH_OPTION; k ++ )
				{
					yidx[m][k] = orderx2y[index + k];
				}
			}
			// calculate prob
			for( n4 = 0; n4 < L_H_MATCH_OPTION ; n4 ++ )
			{
				for( n0 = 0; n0 < L_H_MATCH_OPTION ; n0 ++ )
				for( n1 = 0; n1 < L_H_MATCH_OPTION ; n1 ++ )
				for( n2 = 0; n2 < L_H_MATCH_OPTION ; n2 ++ )
				for( n3 = 0; n3 < L_H_MATCH_OPTION ; n3 ++ )
				for( n5 = 0; n5 < L_H_MATCH_OPTION ; n5 ++ )
				for( n6 = 0; n6 < L_H_MATCH_OPTION ; n6 ++ )
				for( n7 = 0; n7 < L_H_MATCH_OPTION ; n7 ++ )
				for( n8 = 0; n8 < L_H_MATCH_OPTION ; n8 ++ )
				{

					//lx = xidx[n4];
					hy[0] = yidx[0][n0];
					hy[1] = yidx[1][n1];
					hy[2] = yidx[2][n2];
					hy[3] = yidx[3][n3];
					hy[4] = yidx[4][n4];
					hy[5] = yidx[5][n5];
					hy[6] = yidx[6][n6];
					hy[7] = yidx[7][n7];
					hy[8] = yidx[8][n8];
					
					//calculate the index in table x2y
					lidx[0] = xidx[n0] * h_num + hy[0];
					lidx[1] = xidx[n1] * h_num + hy[1];
					lidx[2] = xidx[n2] * h_num + hy[2];
					lidx[3] = xidx[n3] * h_num + hy[3];
					lidx[4] = xidx[n4] * h_num + hy[4];
					lidx[5] = xidx[n5] * h_num + hy[5];
					lidx[6] = xidx[n6] * h_num + hy[6];
					lidx[7] = xidx[n7] * h_num + hy[7];
					lidx[8] = xidx[n8] * h_num + hy[8];

					//calculate the index in table y2yA,y2yB,y2yC,y2yD
					hidx[0] = hy[4] * h_num + hy[0];
					hidx[1] = hy[4] * h_num + hy[1];
					hidx[2] = hy[4] * h_num + hy[2];
					hidx[3] = hy[4] * h_num + hy[5];
					hidx[4] = hy[3] * h_num + hy[4];
					hidx[5] = hy[6] * h_num + hy[4];
					hidx[6] = hy[7] * h_num + hy[4];
					hidx[7] = hy[8] * h_num + hy[4];
					
				    jointprob = x2y[lidx[0]] + x2y[lidx[1]] + x2y[lidx[2]] +
												 x2y[lidx[3]] + x2y[lidx[4]] + x2y[lidx[5]] +
												 x2y[lidx[6]] + x2y[lidx[7]] + x2y[lidx[8]] +
												 y2yD[hidx[0]] + y2yB[hidx[1]] + y2yC[hidx[2]] +
												 y2yA[hidx[3]] + y2yA[hidx[4]] + y2yC[hidx[5]] +
												 y2yB[hidx[6]] + y2yD[hidx[7]];
					if(maxprob < jointprob)
					{
						maxprob = jointprob;
						result[xidx[4]] = yidx[4][n4];	//write down the result y;
					}
				}
			}
		}
	}

	return 0;
}