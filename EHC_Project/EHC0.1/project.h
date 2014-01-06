#pragma once
#include "datahead.h"

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
int project(int l_num, int h_num, L_cluster* l_cluster, H_cluster* h_cluster, L_statis* l_statis, H_statis* h_statis);

/**
 * yangruiguo 2012-01-14
 * function: project the low images' discrete flags to continuous numbers
 * l_num   : the number of low quality images' cluster 
 * l_cluster: the low quality images' cluster
 * l_statis : the low quality images' project struct
 * return : 0
 */
int project_low(int l_num, L_cluster* l_cluster, L_statis* l_statis);

/**
 * yangruiguo 2012-01-14
 * function: project the high images' discrete flags to continuous numbers
 * h_num   : the number of high quality images' cluster 
 * h_cluster: the high quality images' cluster
 * h_statis : the high quality images' project struct
 * return : 0
 */
int project_hig(int h_num, H_cluster* h_cluster, H_statis* h_statis);

/**
 * yangruiguo 2012-01-15
 * function: replace the struct array of high mrf and low mrf to two dimensions integer array
 * num     : the length of l_mrf or h_mrf
 * llen    : the length of l_statis
 * hlen    : the length of h_statis
 * l_mrf   : the struct of low quality images' mrf
 * h_mrf   : the struct of high quality images' mrf
 * l_statis: the project relationship of low quality images
 * h_statis: the project relationship of high quality images 
 * pl_mrf  : the project result of low quality images
 * ph_mrf  : the project result of high quality images 
 * return  : 0
 */
int project_mrf(int num, int l_num, int h_num, IMAGE_L_MRF* l_mrf, IMAGE_H_MRF* h_mrf, L_statis* l_statis, H_statis* h_statis, int** pl_mrf, int** ph_mrf);