#pragma once

#include "datahead.h"

/**
 * yangruiguo 2011-12-14
 * num       : the number of pictures
 * l_dis     : the minimum distance of low quality images
 * h_dis     : the minimum distance of high quality images
 * l_num     : the number of low quality images' cluster
 * h_num     : the number of high quality images' cluster
 * l_mrf     : the low quality images' MRF
 * h_mrf     : the high quality images' MRF
 * l_cluster : the low quality images' clusters
 * h_cluster : the high quality images' clusters
 * return    : 0 success 
 */
int _clusterMRF(int num, int l_dis, int h_dis, int* l_num, int* h_num, IMAGE_L_MRF* l_mrf, IMAGE_H_MRF* h_mrf, L_cluster** l_cluster, H_cluster** h_cluster);

/**
 * yangruiguo 2011-12-14
 * num    : currently, the total number of different MRF
 * l_count: the number of low quality images' certain MRF
 * l_mrf  : the low quality images' MRF
 * return : 0 success 
 */
int _countLMRF(int* num, L_count* l_count, IMAGE_L_MRF* l_mrf);

/**
 * yangruiguo 2011-12-14
 * num    : currenty, the total number of different MRF
 * len    : currenty, the length of h_count
 * h_count: the number of high quality images' certain MRF
 * h_mrf  : the high quality images' MRF
 * return : 0 success 
 */
int _countHMRF(int* num, int* len, H_count* h_count, IMAGE_H_MRF* h_mrf);

/**
 * yangruiguo 2011-12-14
 * num    : currenty, the total number of different MRF
 * l_count: the number of low quality images' certain MRF
 * return : 0 success 
 */
int _orderLMRF(int num, L_count* l_count);

/**
 * yangruiguo 2011-12-13
 * num    : currenty, the total number of different MRF
 * h_count: the number of high quality images' certain MRF
 * return : 0 success 
 */
int _orderHMRF(int num, H_count* h_count);

/**
 * yangruiguo 2011-12-13
 * dis       : the max distance away from the center
 * num       : currenty, the total number of different MRF
 * cnum      : the number of cluster after the process
 * l_count   : the number of low quality images'
 * l_cluster : the clusters of low quality images'
 * return    : 0 success 
 */
int _clusterLMRF(int dis, int num, int* cnum, L_count* l_count, L_cluster* l_cluster);

/**
 * yangruiguo 2011-12-13
 * dis       : the max distance away from the center
 * num       : currenty, the total number of different MRF
 * cnum      : the number of cluster after the process
 * h_count   : the number of high quality images'
 * h_cluster : the clusters of high quality images'
 * return    : 0 success 
 */
int _clusterHMRF(int dis, int num, int* cnum, H_count* h_count, H_cluster* h_cluster);

/**
 * yangruiguo 2011-12-13
 * l1 : one of the label
 * l2 : the other label
 * return : the distance between l1 and l2
 */
int countDistance(int l1, int l2);

/**
 * yangruiguo 2011-12-14
 * width  : the images' width
 * height : the images' height
 * num    : the number of cluster
 * l_mrf  : the low images' MRF
 * imgname: the output image's name
 * return : NULL
 */
void _testLMRF_Cluster(int width, int height, LL num, L_cluster* lcluster, IMAGE_L_MRF *l_mrf, const char* imgname);

/**
 * yangruiguo 2011-12-14
 * width  : the images' width
 * height : the images' height
 * num    : the number of cluster
 * h_mrf  : the high images' MRF
 * imgname: the output image's name
 * return : NULL
 */
void _testHMRF_Cluster(int width, int height, LL num, H_cluster* hcluster, IMAGE_H_MRF *h_mrf, const char* imgname);

/**
 * yangruiguo 2011-12-15
 * w       : the images' width
 * h       : the images' height
 * num     : the number of cluster
 * dis     : the number of displayed cluster
 * lcluster: the images' cluster
 * imgname : the output image's name
 * return  : NULL
 */
void _testLMRF_Write_Cluster(int w, int h, LL num, LL dis, L_cluster* lcluster, char* imgname);

/**
 * yangruiguo 2011-12-15
 * w       : the images' width
 * h       : the images' height
 * num     : the number of cluster
 * dis     : the number of displayed cluster
 * hcluster: the images' cluster
 * imgname : the output image's name
 * return  : NULL
 */
void _testHMRF_Write_Cluster(int w, int h, LL num, LL dis, H_cluster* hcluster, char* imgname);

/**
 * yangruiguo 2011-12-15
 * width  : the images' width
 * height : the images' height
 * l_mrf  : the low images' MRF
 * imgname: the output image's name
 * return : NULL
 */
void _testLMRF2img(int width, int height, IMAGE_L_MRF *l_mrf, const char* imgname);

/**
 * yangruiguo 2011-12-15
 * width  : the images' width
 * height : the images' height
 * h_mrf  : the high images' MRF
 * imgname: the output image's name
 * return : NULL
 */
void _testHMRF2img(int width, int height, IMAGE_H_MRF *h_mrf, const char* imgname);

/**
 * yangruiguo 2011-12-20
 * num     : the number of low images' cluster
 * lcluster: the low images' clusters
 * l_mrf   : the low images' MRF
 * return  : 0 success
 */
int _replaceLMRF(int num, L_cluster* lcluster, IMAGE_L_MRF *l_mrf);

/**
 * yangruiguo 2011-12-20
 * num     : the number of high images' cluster
 * hcluster: the high images' clusters
 * h_mrf   : the high images' MRF
 * return  : 0 success
 */
int _replaceHMRF(int num, H_cluster* hcluster, IMAGE_H_MRF *h_mrf);