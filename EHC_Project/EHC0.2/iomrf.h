#pragma once

#include "datahead.h"

/**
 * yangruiguo 2011-12-21
 * h         : the height of data array
 * w         : the width of data array
 * data      : the data array 
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_2dimension_Text(LL h, LL w, float** data, const char* filename);

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
int write_2dimension_Text(LL h, LL w, int* c, float** data, const char* filename);

//Add_Start 20111227 Candy
/**
 * function  : write data to filename, h is the height, w is the width
 * h         : the height of data array
 * w         : the width of data array
 * data      : the data array 
 * filename  : the filename which is going to write
 * return    : 0 success 
 */

int write_twodimension_Text(LL h, LL w, LL** data, const char* filename);
//Add_End 20111227

/**
 * yangruiguo 2011-12-21
 * h         : the height of data array
 * w         : the width of data array
 * data      : the data array 
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_2dimension_Text(LL* h, LL* w, float** data, const char* filename);

/**
 * yangruiguo 2011-12-27
 * data      : the data array 
 * return    : 0 success 
 */
int destroy_2dimension_Text(float* data);

/**
 * yangruiguo 2011-12-21
 * function  : write l_mrf to file
 * num       : the length of l_mrf
 * l_mrf     : the low images' MRF
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_LMRF_Text(int num, IMAGE_L_MRF* l_mrf, const char* filename);

/**
 * yangruiguo 2011-12-22
 * function  : read l_mrf from file
 * num       : the length of l_mrf
 * l_mrf     : the low images' MRF
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_LMRF_Text(int* num, IMAGE_L_MRF** l_mrf, const char* filename);

/**
 * yangruiguo 2011-12-21
 * function  : write h_mrf to file
 * num       : the length of h_mrf
 * h_mrf     : the high images' MRF
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_HMRF_Text(int num, IMAGE_H_MRF* h_mrf, const char* filename);

/**
 * yangruiguo 2011-12-22
 * function  : read h_mrf from file
 * num       : the length of h_mrf
 * h_mrf     : the high images' MRF
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_HMRF_Text(int* num, IMAGE_H_MRF** h_mrf, const char* filename);

/**
 * yangruiguo 2011-12-23
 * function  : write l_cluster to file
 * num       : the length of l_cluster
 * l_cluster : the low images' clusters
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_LCLUSTER_Text(LL num, L_cluster* l_cluster, const char* filename);

/**
 * yangruiguo 2011-12-23
 * function  : read l_cluster from file
 * num       : the length of l_cluster
 * l_cluster : the low images' clusters
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_LCLUSTER_Text(LL* num, L_cluster** l_cluster, const char* filename);

/**
 * yangruiguo 2011-12-23
 * function  : write h_cluster to file
 * num       : the length of h_cluster
 * h_cluster : the high images' clusters
 * filename  : the filename which is going to write
 * return    : 0 success 
 */
int write_HCLUSTER_Text(LL num, H_cluster* h_cluster, const char* filename);

/**
 * yangruiguo 2011-12-23
 * function  : read h_cluster from file
 * num       : the length of h_cluster
 * h_cluster : the high images' clusters
 * filename  : the filename which is going to read
 * return    : 0 success 
 */
int read_HCLUSTER_Text(LL* num, H_cluster** h_cluster, const char* filename);