#include "stdafx.h"
#include "project.h"

/**
 * yangruiguo 2012-01-14
 * function: project the discrete flags to continuous numbers
 * l_num   : the number of low quality images' cluster 
 * h_num   : the number of high quality images' cluster 
 * l_cluster: the low quality images' cluster
 * h_cluster: the high quality images' cluster
 * l_statis : the low quality images' project struct
 * h_statis : the high quality images' project struct
 * return : 0
 */
int project(int l_num, int h_num, L_cluster* l_cluster, H_cluster* h_cluster, L_statis* l_statis, H_statis* h_statis)
{
	int status = 0;

	status = project_low(l_num, l_cluster, l_statis);
	status = project_hig(h_num, h_cluster, h_statis);
	return 0;
}

/**
 * yangruiguo 2012-01-14
 * function: project the low images' discrete flags to continuous numbers
 * l_num   : the number of low quality images' cluster 
 * l_cluster: the low quality images' cluster
 * l_statis : the low quality images' project struct
 * return : 0
 */
int project_low(int l_num, L_cluster* l_cluster, L_statis* l_statis)
{
	int i = 0;
	for(i = 0; i < l_num; i++)
	{
		l_statis[i].l = l_cluster[i].l;
		l_statis[i].v = i;
	}

	return 0;
}

/**
 * yangruiguo 2012-01-14
 * function: project the high images' discrete flags to continuous numbers
 * h_num   : the number of high quality images' cluster 
 * h_cluster: the high quality images' cluster
 * h_statis : the high quality images' project struct
 * return : 0
 */
int project_hig(int h_num, H_cluster* h_cluster, H_statis* h_statis)
{
	int i = 0;
	for(i = 0; i < h_num; i++)
	{
		h_statis[i].l.lb = h_cluster[i].l.lb;
		h_statis[i].l.lt = h_cluster[i].l.lt;
		h_statis[i].l.rb = h_cluster[i].l.rb;
		h_statis[i].l.rt = h_cluster[i].l.rt;
		h_statis[i].v = i;
	}

	return 0;
}

/**
 * yangruiguo 2012-01-15
 * function: replace the struct array of high mrf and low mrf to two dimensions integer array
 * num     : the length of l_mrf or h_mrf
 * l_num   : the length of l_statis
 * h_num   : the length of h_statis
 * l_mrf   : the struct of low quality images' mrf
 * h_mrf   : the struct of high quality images' mrf
 * l_statis: the project relationship of low quality images
 * h_statis: the project relationship of high quality images 
 * pl_mrf  : the project result of low quality images
 * ph_mrf  : the project result of high quality images 
 * return  : 0
 */
int project_mrf(int num, int l_num, int h_num, IMAGE_L_MRF* l_mrf, IMAGE_H_MRF* h_mrf, L_statis* l_statis, H_statis* h_statis, int** pl_mrf, int** ph_mrf)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	int t = 0;

	for(i = 0; i < num; i++)
	{
		/* low quality images */
		for(j = 0; j < l_mrf[i].xnum; j++)
		{
			for(k = 0; k < l_mrf[i].ynum; k++)
			{
				t = j * l_mrf[i].ynum + k;
				for(m = 0; m < l_num; m++)
				{
					if(l_mrf[i].label[t] == l_statis[m].l)
					{
						pl_mrf[i][t] = l_statis[m].v;
						break;
					}
				}
			}
		}

		/* high quality images */
		for(j = 0; j < h_mrf[i].xnum; j++)
		{
			for(k = 0; k < h_mrf[i].ynum; k++)
			{
				t = j * h_mrf[i].ynum + k;
				for(m = 0; m < h_num; m++)
				{
					if(h_mrf[i].label[t].lb == h_statis[m].l.lb &&
					   h_mrf[i].label[t].lt == h_statis[m].l.lt &&
					   h_mrf[i].label[t].rb == h_statis[m].l.rb &&
					   h_mrf[i].label[t].rt == h_statis[m].l.rt)
					{
						ph_mrf[i][t] = h_statis[m].v;
						break;
					}
				}
			}
		}
	}
	return 0;
}
