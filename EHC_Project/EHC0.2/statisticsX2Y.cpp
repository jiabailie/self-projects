#include "stdafx.h"
#include "load.h"
#include "cluster.h"
#include "imageprocess.h"
#include "imageutil.h"
#include "segment.h"
#include "statisticsX2Y.h"
#include "iomrf.h"

/**
 * Candy 2011-12-21
 * num       : the number of pictures
 * h_num     : the number of high quality images' cluster
 * l_num     : the number of low quality images' cluster
 * hmrf      : the replaced high quality images' MRF
 * h_mrf     : the high quality images' MRF
 * l_mrf  : the low quality images' MRF
 * h_cluster : the high quality images' clusters
 * l_cluster : the low quality images' clusters
 * x2y       : the calculated result , a 2 dimensions array, y-->x
 * orderx2y  : the order result , a 2 dimensions array, y-->x
 * return    : 0 success 
 */
int _statisX2Y(int num, LL h_num, LL l_num, LL** hmrf, IMAGE_H_MRF* h_mrf, IMAGE_L_MRF* l_mrf, H_cluster* h_cluster, L_cluster* l_cluster, float*** x2y, LL*** orderx2y)
{
 int i,j,k;
 int index;
 int temp;
 int hh = h_mrf->xnum;
 int hw = h_mrf->ynum;
 int hnum = (int)h_num;
 int lnum = (int)l_num;
 int status = 0; 
 int hlen = hh * hw;
 int lh = l_mrf->xnum;
 int lw = l_mrf->ynum;
 int llen = lh * lw;
 float** xy = NULL;
    LL** orderxy = NULL;
 float *p;
 //20111230 update
 int *sum;
 sum = (int*)malloc(sizeof(int) * lnum);
 memset(sum,(int)0,lnum);
 //
 H_statis* h_statis = (H_statis*)malloc(sizeof(H_statis) * hnum);
 L_statis* l_statis = (L_statis*)malloc(sizeof(L_statis) * lnum);

 LL** lmrf = (LL**)malloc(sizeof(LL*) * num);


 xy = (float**)malloc(sizeof(float*) * lnum);
 orderxy = (LL**)malloc(sizeof(LL*) * lnum);

 for(i = 0; i < lnum; i++)
 {
  xy[i] = (float*)malloc(sizeof(float) * hnum);
  orderxy[i] = (LL*)malloc(sizeof(LL) * hnum);
 
  memset(xy[i], 0, sizeof(float) * hnum);  
  memset(orderxy[i], 0, sizeof(LL) * hnum);
 }

 status = _projectX2X(l_num, l_cluster, l_statis);

 // 20111230 update   
 //for(i = 0; i < num; i++)
 //{

 // lmrf[i] = (LL*)malloc(sizeof(LL) * llen);
 // status = _replaceX2X(l_num, l_statis, &l_mrf[i], lmrf[i]);
 // for(j = 0; j < lh; j++)
 // {
 //  for(k = 0; k < lw; k++)
 //  {
 //   index = j * lw + k;
 //   xy[lmrf[i][index]][hmrf[i][index]]++;
 //  }
 // }
 //}
    
 for(i = 0; i < num; i++)
 {
  lmrf[i] = (LL*)malloc(sizeof(LL) * llen);
  status = _replaceX2X(l_num, l_statis, &l_mrf[i], lmrf[i]);
 }
 for(i = 0; i < num; i++)
 {
  for(j = 0; j < lh; j++)
  {
   for(k = 0; k < lw; k++)
   {
    index = j * lw + k;
    xy[lmrf[i][index]][hmrf[i][index]]++;
   }
  }
 }
///////////////////////    
 // 20111230 update  
 //for(i = 0; i < lnum; i++)
 //{
 // float sum = 0.0;
 // for(j = 0; j < hnum; j++)
 // {
 //  sum += xy[i][j];
 // }
 // for(k = 0; k < hnum; k++)
 // {
 //  if(sum < 1 )
 //  {
 //   xy[i][k] = 0;
 //  }else{
 //   xy[i][k] = xy[i][k]/sum;
 //  }
 // }
 //}
 for(i = 0; i < lnum; i++)
 {
  sum[i] = 0;
  for(j = 0; j < hnum; j++)
  {
   sum[i] += xy[i][j];
  }
 }
//20111230 test use start
 status = write_2dimension_Text(lnum, hnum, sum, xy, OUTPUT_DIR_NAME_mXY);
//20111230 test use end

 for(i = 0; i < lnum; i++)
 {
  for(k = 0; k < hnum; k++)
  {
   if(sum[i] < 1 )
   {
    xy[i][k] = 0;
   }else{
    xy[i][k] = xy[i][k]/sum[i];
   }
  }
 }
 // 20111230  
 *x2y = xy;
 p = (float*)malloc(sizeof(float) * hnum);

 for(i = 0; i < lnum; i++)
 {
  //copy xy[i] to p
  //init orderxy
  for(k = 0; k < hnum; k ++ )
  {
    p[k] = xy[i][k];
    orderxy[i][k] = k;
  }

  for(j = 0; j < hnum - 1; j++)
   for(k = j + 1; k < hnum; k++)
    if(p[j] < p[k])
    {
     temp = p[j];
     p[j] = p[k];
     p[k] = temp;
     temp = orderxy[i][j];
     orderxy[i][j] = orderxy[i][k];
     orderxy[i][k] = temp;
    }
 }
 
 //free p
 if(p != NULL )
 {
  free(p);
 }

 *orderx2y = orderxy;

 //free h_statis
 if(h_statis != NULL)
 {
  free(h_statis);
 }

 //free l_statis
 if(l_statis != NULL)
 {
  free(l_statis);
 }

 //free hmrf
 if(hmrf != NULL)
 {
  for(i = 0; i < num; i++)
  {
   if(hmrf[i] != NULL)
   {
    free(hmrf[i]);
   }
  }
  free(hmrf);
 }

 //free lmrf
 if(lmrf != NULL)
 {
  for(i = 0; i < num; i++)
  {
   if(lmrf[i] != NULL)
   {
    free(lmrf[i]);
   }
  }
  free(lmrf);
 }
 
 return 0;
}

/**
 * Candy 2011-12-26
 * l_num     : the number of low quality images' cluster
 * l_statis  : the low quality images' project
 * l_mrf     : the low images' MRF
 * lmrf      : the replaced low images' MRF
 * return    : 0 success 
 */
int _replaceX2X(LL l_num, L_statis* l_statis, IMAGE_L_MRF *l_mrf, LL* lmrf)
{
 int i = 0;
 int j = 0;
 LL k = 0;
 bool flg = false;
 
 for(i = 0; i < l_mrf->xnum; i++)
 {
  for(j = 0; j < l_mrf->ynum; j++)
  {
   flg = false;
   for(k = 0; k < l_num && !flg; k++)
   {
    if(l_mrf->label[i * l_mrf->ynum + j]  == l_statis[k].l)
    {
     lmrf[i * l_mrf->ynum + j] = l_statis[k].v;
     flg = true;
    }
   }
   if(!flg)
   {
    lmrf[i * l_mrf->ynum + j] = 0;
   }
  }
 }
 return 0;
}

/**
 * Candy 2011-12-26
 * l_num     : the number of low quality images' cluster
 * l_cluster : the low quality images' clusters
 * l_statis  : the projected result
 * return    : 0 success 
 */
int _projectX2X(LL l_num, L_cluster* l_cluster, L_statis* l_statis)
{
 LL i = 0;
 for(i = 0; i < l_num; i++)
 {
  l_statis[i].v = i;
  l_statis[i].l = l_cluster[i].l ;
 }

 return 0;
}
