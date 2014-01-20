#include "stdafx.h"
#include "datahead.h"
#include "load.h"
#include "iomrf.h"
#include "cluster.h"
#include "project.h"
#include "statisticsY2Y.h"
#include "statisticsX2Y.h"

#define READ_FROM_IMGS

int train()
{	
	/************************************* define the variables *************************************/

	int i = 0;
	int num = 0;	/* the number of sample pictures */	
	int llen = 0;   /* the length of each low quality image mrf */
	int hlen = 0;   /* the length of each high quality image mrf */
	int status = 0;	/* the status of function return */	
	int l_num = 0;	/* the number of low quality images' cluster */	
	int h_num = 0;	/* the number of high quality images' cluster */

	float** y2yA = NULL; /* the calculated result A, a 2 dimensions array, right */
	float** y2yB = NULL; /* the calculated result B, a 2 dimensions array, top */
	float** y2yC = NULL; /* the calculated result C, a 2 dimensions array, top-right */
	float** y2yD = NULL; /* the calculated result D, a 2 dimensions array, bottom-right */

	char* namefile = SAMPLE_DIR_NAME; /* the file's name which saves the name list of sample images' */

	IMAGE_L_MRF* l_mrf = NULL; /* the partitioned low quality images  */
	IMAGE_H_MRF* h_mrf = NULL; /* the partitioned high quality images  */

	int** pl_mrf = NULL; /* the projected low quality images */
	int** ph_mrf = NULL; /* the projected high quality images */

	L_cluster* l_cluster = NULL; /* the cluster of low quality images' */
	H_cluster* h_cluster = NULL; /* the cluster of high quality images' */

	L_statis* l_statis = NULL; /* the projected cluster of low quality images' */
	H_statis* h_statis = NULL; /* the projected cluster of high quality images' */

	Sample_name* namelist = NULL; /* the name list read from the SAMPLE_DIR_NAME */

	/************************************* processing *************************************/

#ifdef READ_FROM_IMGS

	/* load the sample images' name list */
	status = loadsamplenamelist(namefile, &namelist, &num);

	/* init the struct mrf */
	status = initimagemrf(&h_mrf, &l_mrf, num);

    /* load the images */
	status = loadsamples(namelist, num, h_mrf, l_mrf);

#else

	/* initialize the l_mrf and h_mrf using data from text files */
	status = read_LMRF_Text(&num, &l_mrf, LOWMRF_NAME);
	status = read_HMRF_Text(&num, &h_mrf, HIGMRF_NAME);

#endif

	llen = l_mrf[0].xnum * l_mrf[0].ynum;
	hlen = h_mrf[0].xnum * h_mrf[0].ynum;

#ifdef READ_FROM_IMGS

	/* cluster */
	status = _clusterMRF(num, L_DIS, H_DIS, &l_num, &h_num, l_mrf, h_mrf, &l_cluster, &h_cluster);

#else

	/* initialize the cluster using data from text files */
	status = read_LCLUSTER_Text(&l_num, &l_cluster, LOW_CLUSTER_NAME);
	status = read_HCLUSTER_Text(&h_num, &h_cluster, HIG_CLUSTER_NAME);

#endif

	/* replace the samples' MRF with their own cluster */
	for(i = 0; i < num; i++)
	{
		status = _replaceLMRF(l_num, l_cluster, &l_mrf[i]);
		status = _replaceHMRF(h_num, h_cluster, &h_mrf[i]);
	}

	/* project the discrete flags to continuous numbers */
	l_statis = (L_statis*)malloc(sizeof(L_statis) * l_num);
	h_statis = (H_statis*)malloc(sizeof(H_statis) * h_num);
	status = project(l_num, h_num, l_cluster, h_cluster, l_statis, h_statis);

	/* replace the mrf to projected number */
	pl_mrf = (int**)malloc(sizeof(int*) * num);
	ph_mrf = (int**)malloc(sizeof(int*) * num);
	for(i = 0; i < num; i++)
	{
		pl_mrf[i] = (int*)malloc(sizeof(int) * llen);
		ph_mrf[i] = (int*)malloc(sizeof(int) * hlen);
		memset(pl_mrf[i], 0, sizeof(int) * llen);
		memset(ph_mrf[i], 0, sizeof(int) * hlen);
	}

	/* project the struct array to integer array */
	status = project_mrf(num, l_num, h_num, l_mrf, h_mrf, l_statis, h_statis, pl_mrf, ph_mrf);

	/* statistic the y to y table */
	status = _statisY2Y(num, h_mrf[0].xnum, h_mrf[0].ynum, h_num, ph_mrf);

	/* statistic the x to y table */
	status = _statisX2Y(num, h_mrf[0].xnum, h_mrf[0].ynum, l_num, h_num, pl_mrf, ph_mrf);

	/************************************* writing the content to the text files *************************************/

#ifdef READ_FROM_IMGS

	/* write the MRF to files */
	status = write_LMRF_Text(num, l_mrf, LOWMRF_NAME);
	status = write_HMRF_Text(num, h_mrf, HIGMRF_NAME);

	/* write cluster to file */
	status = write_LCLUSTER_Text(l_num, l_cluster, LOW_CLUSTER_NAME);
	status = write_HCLUSTER_Text(h_num, h_cluster, HIG_CLUSTER_NAME);

#endif

	/* write project array to file */
	status = write_PMRF_Text(num, l_mrf[0].xnum, l_mrf[0].ynum, pl_mrf, P_LOWMRF_NAME);
	status = write_PMRF_Text(num, h_mrf[0].xnum, h_mrf[0].ynum, ph_mrf, P_HIGMRF_NAME);

	/************************************* free memory *************************************/

	status = destroy(h_mrf, l_mrf, num);
	status = destroy_lcluster(l_num, l_cluster);
	status = destroy_hcluster(h_num, h_cluster);
	status = destroy_lstatis(l_statis);
	status = destroy_hstatis(h_statis);
	status = destroy_namelist(namelist);
	status = destroy_2d_array(num, pl_mrf);
	status = destroy_2d_array(num, ph_mrf);

	return 0;
}