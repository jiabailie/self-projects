#include "stdafx.h"
#include "datahead.h"
#include "load.h"
#include "cluster.h"
#include "statisticsY2Y.h"
#include "statisticsX2Y.h"
#include "iomrf.h"

//#define DEBUG_READ_FROM_IMGS

//#define DEBUG_READ_FROM_TEXTS

int train()
{
 int num = 0;
 int status = 0;
 LL l_num = 0;
 LL h_num = 0;
 char* namefile = SAMPLE_DIR_NAME;
 
 Sample_name* namelist = NULL;
 
 IMAGE_H_MRF *h_mrf = NULL;
 IMAGE_L_MRF *l_mrf = NULL;

 L_cluster* l_cluster = NULL;
 H_cluster* h_cluster = NULL;

 L_statis* l_statis = NULL;
 H_statis* h_statis = NULL;

 float** y2yA = NULL;
 float** y2yB = NULL;
 float** y2yC = NULL;
 float** y2yD = NULL;
 float** x2y  = NULL;
 LL** orderx2y = NULL;

 LL** hmrf = NULL;

#ifdef DEBUG_READ_FROM_IMGS

 status = loadsamplenamelist(namefile, &namelist, &num);

 initimagemrf(&h_mrf, &l_mrf, num);

 status = loadsamples(namelist, num, h_mrf, l_mrf);

 /* write the MRF to files */
 status = write_LMRF_Text(num, l_mrf, LOWMRF_NAME);
 status = write_HMRF_Text(num, h_mrf, HIGMRF_NAME);
#else

 /* initialize the l_mrf and h_mrf using data from text files */

 status = read_LMRF_Text(&num, &l_mrf, LOWMRF_NAME);
 status = read_HMRF_Text(&num, &h_mrf, HIGMRF_NAME);

#endif

#ifdef DEBUG_READ_FROM_TEXTS

 /* initialize the cluster using data from text files */
 status = read_LCLUSTER_Text(&l_num, &l_cluster, LOW_CLUSTER_NAME);
 status = read_HCLUSTER_Text(&h_num, &h_cluster, HIG_CLUSTER_NAME);

 /* replace the samples' MRF with their own cluster */
 for(i = 0; i < num; i++)
 {
  status = _replaceLMRF(l_num, l_cluster, &l_mrf[i]);
  status = _replaceHMRF(h_num, h_cluster, &h_mrf[i]);
 }

#else
 /* cluster */
 status = _clusterMRF(num, L_DIS, H_DIS, &l_num, &h_num, l_mrf, h_mrf, &l_cluster, &h_cluster);

 /* project the discrete flags to continuous numbers */
 l_statis = (L_statis*)malloc(sizeof(L_statis) * l_num);
 h_statis = (H_statis*)malloc(sizeof(H_statis) * h_num);

 /* write cluster to file */
 status = write_LCLUSTER_Text(l_num, l_cluster, LOW_CLUSTER_NAME);
 status = write_HCLUSTER_Text(h_num, h_cluster, HIG_CLUSTER_NAME);
#endif

#ifdef DEBUG_CLUSTER

 char l_mrf_img[1024];
 char h_mrf_img[1024];
 char l_img[1024];
 char h_img[1024];
 sprintf(l_mrf_img, "%s%s", TEST_RESULT, "low_mrf_000.jpg");
 sprintf(h_mrf_img, "%s%s", TEST_RESULT, "hig_mrf_000.jpg");
 sprintf(l_img, "%s%s", TEST_RESULT, "low_cluster_000.jpg");
 sprintf(h_img, "%s%s", TEST_RESULT, "hig_cluster_000.jpg");

 _testLMRF_Cluster(397, 562, l_num, l_cluster, &l_mrf[0], l_mrf_img);
 _testHMRF_Cluster(794, 1123, h_num, h_cluster, &h_mrf[0], h_mrf_img);

 //write a cluster all class
 _testLMRF_Write_Cluster(397, 562, l_num, 20, l_cluster, l_img);
 _testHMRF_Write_Cluster(794, 1123, h_num, 20, h_cluster, h_img);

#endif

 /* statistic the Y2Y table */
 status = _statisY2Y(num, h_num, h_mrf, &hmrf, h_cluster, &y2yA, &y2yB, &y2yC, &y2yD);

 /*statistic the X2Y table */
 status = _statisX2Y(num, h_num, l_num, hmrf, h_mrf, l_mrf, h_cluster,l_cluster, &x2y, &orderx2y);

 status = write_2dimension_Text(h_num, h_num, y2yA, OUTPUT_DIR_NAME_A);
 status = write_2dimension_Text(h_num, h_num, y2yB, OUTPUT_DIR_NAME_B);
 status = write_2dimension_Text(h_num, h_num, y2yC, OUTPUT_DIR_NAME_C);
 status = write_2dimension_Text(h_num, h_num, y2yD, OUTPUT_DIR_NAME_D);
 status = write_2dimension_Text(l_num, h_num, x2y, OUTPUT_DIR_NAME_XY);
 status = write_twodimension_Text(l_num, h_num, orderx2y, OUTPUT_DIR_NAME_OrderXY);
 
 if( namelist != NULL )
 {
  free(namelist);
 }
 
 if(h_cluster != NULL)
 {
  free(h_cluster);
 }

 if(l_cluster != NULL)
 {
  free(l_cluster);
 }

 destroy(h_mrf, l_mrf, num);

 return 0;
}