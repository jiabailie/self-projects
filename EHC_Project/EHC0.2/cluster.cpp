#include "stdafx.h"
#include "load.h"
#include "cluster.h"
#include "imageprocess.h"
#include "imageutil.h"
#include "segment.h"

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
int _clusterMRF(int num, int l_dis, int h_dis, LL* l_num, LL* h_num, IMAGE_L_MRF* l_mrf, IMAGE_H_MRF* h_mrf, L_cluster** l_cluster, H_cluster** h_cluster)
{
 int i = 0;
 int status = 0;

 //identify the currently length of h_count
 //if the length of this array is not enough long
 //use realloc to enlarge the array each by MAXSIZE long
 LL hLen = 0;

 LL iCount_l = 0;
 LL iCount_h = 0;
 LL iCluster_l = 0;
 LL iCluster_h = 0;
 L_count* l_count = NULL;
 H_count* h_count = NULL;

 L_cluster* lcluster = NULL;
 H_cluster* hcluster = NULL;

 hLen = MAXSIZE / 100;
 l_count = (L_count*)malloc(sizeof(L_count) * MAXSIZE);
 h_count = (H_count*)malloc(sizeof(H_count) * hLen);

 memset(l_count, 0, sizeof(L_count) * MAXSIZE);
 memset(h_count, 0, sizeof(H_count) * hLen);

 /* count the number of different MRF */
 for(i = 0; i < num; i++)
 {
  status = _countLMRF(&iCount_l, l_count, &l_mrf[i]);
  status = _countHMRF(&iCount_h, &hLen, h_count, &h_mrf[i]);
 }

 /* order the MRF according frequency from high to low */
 status = _orderLMRF(iCount_l, l_count);
 status = _orderHMRF(iCount_h, h_count);

 /* cluster */ 
 lcluster = (L_cluster*)malloc(sizeof(L_cluster) * iCount_l);
 hcluster = (H_cluster*)malloc(sizeof(H_cluster) * iCount_h);

 status = _clusterLMRF(l_dis, iCount_l, &iCluster_l, l_count, lcluster);
 status = _clusterHMRF(h_dis, iCount_h, &iCluster_h, h_count, hcluster);

 /* replace the samples' MRF with their own cluster */
 for(i = 0; i < num; i++)
 {
  status = _replaceLMRF(iCluster_l, lcluster, &l_mrf[i]);
  status = _replaceHMRF(iCluster_h, hcluster, &h_mrf[i]);
 }

 /* give values to the output variables */
 *l_cluster = lcluster;
 *h_cluster = hcluster;
 *l_num = iCluster_l;
 *h_num = iCluster_h;

#ifdef DEBUG_CLUSTER

 char l_mrf_img[1024];
 char h_mrf_img[1024];
 char l_img[1024];
 char h_img[1024];
 sprintf(l_mrf_img, "%s%s", TEST_RESULT, "low_mrf_000.jpg");
 sprintf(h_mrf_img, "%s%s", TEST_RESULT, "hig_mrf_000.jpg");
 sprintf(l_img, "%s%s", TEST_RESULT, "low_cluster_000.jpg");
 sprintf(h_img, "%s%s", TEST_RESULT, "hig_cluster_000.jpg");

 _testLMRF_Cluster(397, 562, iCluster_l, lcluster, &l_mrf[0], l_mrf_img);
 _testHMRF_Cluster(794, 1123, iCluster_h, hcluster, &h_mrf[0], h_mrf_img);

 _testLMRF_Write_Cluster(397, 562, iCluster_l, 20, lcluster, l_img);
 _testHMRF_Write_Cluster(794, 1123, iCluster_h, 20, hcluster, h_img);

#endif

 /* free memory */
 if(l_count != NULL)
 {
  free(l_count);
 }
 if(h_count != NULL)
 {
  free(h_count);
 }

 return 0;
}

/**
 * yangruiguo 2011-12-14
 * num    : currently, the total number of different MRF
 * l_count: the number of low quality images' certain MRF
 * l_mrf  : the low quality images' MRF
 * return : 0 success 
 */
int _countLMRF(LL* num, L_count* l_count, IMAGE_L_MRF* l_mrf)
{
 int i = 0;
 int j = 0;
 int l = l_mrf->xnum * l_mrf->ynum;

 for(i = 0; i < l; i++)
 {
  for(j = 0; j < (*num); j++)
  {
   if(l_count[j].l == l_mrf->label[i])
   {
    l_count[j].c += 1;
    break;
   }
  }
  if(j == (*num))
  {
   l_count[(*num)].l = l_mrf->label[i];
   l_count[(*num)].c = 1;
   (*num)++;
  }
 }

 return 0;
}

/**
 * yangruiguo 2011-12-14
 * num    : currenty, the total number of different MRF
 * len    : currenty, the length of h_count
 * h_count: the number of high quality images' certain MRF
 * h_mrf  : the high quality images' MRF
 * return : 0 success 
 */
int _countHMRF(LL* num, LL* len, H_count* h_count, IMAGE_H_MRF* h_mrf)
{
 int i = 0;
 int l = h_mrf->xnum * h_mrf->ynum;
 H_count* h_tmp = NULL;
 
 LL j = 0;
 
 for(i = 0; i < l; i++)
 {
  for(j = 0; j < (*num); j++)
  {
   if(h_mrf->label[i].lb == h_count[j].lb &&
      h_mrf->label[i].lt == h_count[j].lt &&
      h_mrf->label[i].rb == h_count[j].rb && 
      h_mrf->label[i].rt == h_count[j].rt)
   {
    h_count[j].c += 1;
    break;
   }
  }
  if(j == (*num))
  {
   (*num)++;
   if((*len) - (*num) < (MAXSIZE / 1000))
   {
    (*len) += MAXSIZE / 100;
    h_tmp = (H_count*)realloc(h_count, sizeof(H_count) * (*len));
    if(h_tmp != NULL)
    {
     h_count = h_tmp;
    }
   }

   h_count[(*num) - 1].lb = h_mrf->label[i].lb;
   h_count[(*num) - 1].lt = h_mrf->label[i].lt;
   h_count[(*num) - 1].rb = h_mrf->label[i].rb;
   h_count[(*num) - 1].rt = h_mrf->label[i].rt;
   h_count[(*num) - 1].c = 1;
  }
 }

 return 0;
}

/**
 * yangruiguo 2011-12-14
 * num    : currenty, the total number of different MRF
 * l_count: the number of low quality images' certain MRF
 * return : 0 success 
 */
int _orderLMRF(LL num, L_count* l_count)
{
 LL i = 0;
 LL j = 0;
 LL m = 0;
 LL c = 0;
 int l = 0;

 for(i = 0; i < num - 1; i++)
 {
  for(m = i, j = i + 1; j < num; j++)
  {
   if(l_count[j].c > l_count[m].c)
   {
    m = j;
   }
  }
  if(m != i)
  {
   c = l_count[m].c;
   l = l_count[m].l;
   l_count[m].c = l_count[i].c;
   l_count[m].l = l_count[i].l;
   l_count[i].c = c;
   l_count[i].l = l;
  }
 }

 return 0;
}

/**
 * yangruiguo 2011-12-13
 * num    : currenty, the total number of different MRF
 * h_count: the number of high quality images' certain MRF
 * return : 0 success 
 */
int _orderHMRF(LL num, H_count* h_count)
{
 LL c = 0;
 LL i = 0, j = 0, m = 0;
 int lt = 0, lb = 0;
 int rt = 0, rb = 0;

 for(i = 0; i < num - 1; i++)
 {
  for(m = i, j = i + 1; j < num; j++)
  {
   if(h_count[j].c > h_count[m].c)
   {
    m = j;
   }
  }
  if(m != i)
  {
   c  = h_count[m].c;
   lt = h_count[m].lt;
   lb = h_count[m].lb;
   rt = h_count[m].rt;
   rb = h_count[m].rb;
   h_count[m].c  = h_count[i].c;
   h_count[m].lt = h_count[i].lt;
   h_count[m].lb = h_count[i].lb;
   h_count[m].rt = h_count[i].rt;
   h_count[m].rb = h_count[i].rb;
   h_count[i].c  = c;
   h_count[i].lt = lt;
   h_count[i].lb = lb;
   h_count[i].rt = rt;
   h_count[i].rb = rb;
  }
 }
 return 0;
}

/**
 * yangruiguo 2011-12-13
 * dis       : the max distance away from the center
 * num       : currenty, the total number of different MRF
 * cnum      : the number of cluster after the process
 * l_count   : the number of low quality images'
 * l_cluster : the clusters of low quality images'
 * return    : 0 success 
 */
int _clusterLMRF(int dis, LL num, LL* cnum, L_count* l_count, L_cluster* l_cluster)
{
 LL i = 0;
 LL j = 0;
 LL k = 0;
 int d = 0;
 int status = 0;
 
 L_label* tmp = NULL;
 bool* used = (bool*)malloc(sizeof(bool) * num);
 memset(used, (bool)true, num);

 for(i = 0; i < num; i++)
 {
  if(used[i] && l_count[i].l == 0)
  {
   used[i] = false;
   l_cluster[(*cnum)].l = 0;
   l_cluster[(*cnum)].sub = (L_label*)malloc(sizeof(L_label) * 1);
   l_cluster[(*cnum)].subNum = 1;
   l_cluster[(*cnum)].sub[0] = 0;
   (*cnum)++;
  }
  if(used[i])
  {
   k = 10;
   l_cluster[(*cnum)].l = l_count[i].l;
   l_cluster[(*cnum)].sub = (L_label*)malloc(sizeof(L_label) * k);
   l_cluster[(*cnum)].subNum = 0;
   if(i + 1 < num)
   {
    for(j = i + 1; j < num; j++)
    {
     if(used[j])
     {
      d = countDistance(l_count[i].l, l_count[j].l);
      if(d <= dis)
      {
       used[j] = false;
       l_cluster[(*cnum)].sub[l_cluster[(*cnum)].subNum] = l_count[j].l;
       l_cluster[(*cnum)].subNum++;
       if(l_cluster[(*cnum)].subNum == k)
       {
        k += 10;
        tmp = (L_label*)realloc(l_cluster[(*cnum)].sub, sizeof(L_label) * k);
        if(tmp != NULL)
        {
         l_cluster[(*cnum)].sub = tmp;
        }
       }
      }
     }
    }
   }
   used[i] = false;
   (*cnum)++;
  }
 }

 if(tmp != NULL)
 {
  free(tmp);
 }
 return 0;
}

/**
 * yangruiguo 2011-12-13
 * dis       : the max distance away from the center
 * num       : currenty, the total number of different MRF
 * cnum      : the number of cluster after the process
 * h_count   : the number of high quality images'
 * h_cluster : the clusters of high quality images'
 * return    : 0 success 
 */
int _clusterHMRF(int dis, LL num, LL* cnum, H_count* h_count, H_cluster* h_cluster)
{
 LL i = 0;
 LL j = 0;
 LL k = 0;
 int d = 0;
 int status = 0;
 
 H_label* tmp = NULL;
 bool* used = (bool*)malloc(sizeof(bool) * num); 
 memset(used, (bool)true, num);

 for(i = 0; i < num; i++)
 {
  if(used[i] && h_count[i].lb == 0 && h_count[i].lt == 0 && h_count[i].rb == 0 && h_count[i].rt == 0)
  {
   used[i] = false;
   h_cluster[(*cnum)].l.lb = 0;
   h_cluster[(*cnum)].l.lt = 0;
   h_cluster[(*cnum)].l.rb = 0;
   h_cluster[(*cnum)].l.rt = 0;
   h_cluster[(*cnum)].sub = (H_label*)malloc(sizeof(H_label) * 1);
   h_cluster[(*cnum)].subNum = 1;
   h_cluster[(*cnum)].sub[0].lb = 0;
   h_cluster[(*cnum)].sub[0].lt = 0;
   h_cluster[(*cnum)].sub[0].rb = 0;
   h_cluster[(*cnum)].sub[0].rt = 0;
   (*cnum)++;
  }
  if(used[i])
  {
   k = 10;
   h_cluster[(*cnum)].l.lb = h_count[i].lb;
   h_cluster[(*cnum)].l.lt = h_count[i].lt;
   h_cluster[(*cnum)].l.rb = h_count[i].rb;
   h_cluster[(*cnum)].l.rt = h_count[i].rt;
   h_cluster[(*cnum)].sub = (H_label*)malloc(sizeof(H_label) * k);
   h_cluster[(*cnum)].subNum = 0;
   if(i + 1 < num)
   {
    for(j = i + 1; j < num; j++)
    {
     if(used[j])
     {
      d = countDistance(h_count[i].lb, h_count[j].lb) +
       countDistance(h_count[i].lt, h_count[j].lt) +
       countDistance(h_count[i].rb, h_count[j].rb) +
       countDistance(h_count[i].rt, h_count[j].rt);

      if(d <= dis)
      {
       used[j] = false;
       h_cluster[(*cnum)].sub[h_cluster[(*cnum)].subNum].lb = h_count[j].lb;
       h_cluster[(*cnum)].sub[h_cluster[(*cnum)].subNum].lt = h_count[j].lt;
       h_cluster[(*cnum)].sub[h_cluster[(*cnum)].subNum].rb = h_count[j].rb;
       h_cluster[(*cnum)].sub[h_cluster[(*cnum)].subNum].rt = h_count[j].rt;

       h_cluster[(*cnum)].subNum++;
       if(h_cluster[(*cnum)].subNum == k)
       {
        k += 10;
        tmp = (H_label*)realloc(h_cluster[(*cnum)].sub, sizeof(H_label) * k);
        if(tmp != NULL)
        {
         h_cluster[(*cnum)].sub = tmp;
        }
       }
      }
     }
    }
   }
   used[i] = false;
   (*cnum)++;
  }
 }

 if(tmp != NULL)
 {
  free(tmp);
 }
 return 0;
}

/**
 * yangruiguo 2011-12-13
 * l1 : one of the label
 * l2 : the other label
 * return : the distance between l1 and l2
 */
int countDistance(int l1, int l2)
{
 int d = 0;
 int x = l1 ^ l2;
 while(x > 0)
 {
  d += x % 2;
  x >>= 1;
 }
 return d;
}

/**
 * yangruiguo 2011-12-14
 * width  : the images' width
 * height : the images' height
 * num    : the number of cluster
 * l_mrf  : the low images' MRF
 * imgname: the output image's name
 * return : NULL
 */
void _testLMRF_Cluster(int width, int height, LL num, L_cluster* lcluster, IMAGE_L_MRF *l_mrf, const char* imgname)
{
 LL i = 0;
 int j = 0;
 int m = 0;
 int n = 0;
 bool find = false;

 for(m = 0; m < l_mrf->xnum; m++)
 {
  for(n = 0; n < l_mrf->ynum; n++)
  {
   find = false;
   for(i = 0; i < num && !find; i++)
   {
    if(l_mrf->label[m * l_mrf->ynum + n] == lcluster[i].l)
    {
     find = true;
    }
    for(j = 0; j < lcluster[i].subNum && !find; j++)
    {
     if(l_mrf->label[m * l_mrf->ynum + n] == lcluster[i].sub[j])
     {
      find = true;
      l_mrf->label[m * l_mrf->ynum + n] = lcluster[i].l;
     }
    }
   }
  }
 }
 _testLMRF(width, height, l_mrf, imgname);
}

/**
 * yangruiguo 2011-12-14
 * width  : the images' width
 * height : the images' height
 * num    : the number of cluster
 * h_mrf  : the high images' MRF
 * imgname: the output image's name
 * return : NULL
 */
void _testHMRF_Cluster(int width, int height, LL num, H_cluster* hcluster, IMAGE_H_MRF *h_mrf, const char* imgname)
{
 LL i = 0;
 int j = 0;
 int m = 0;
 int n = 0;
 bool find = false;

 for(m = 0; m < h_mrf->xnum; m++)
 {
  for(n = 0; n < h_mrf->ynum; n++)
  {
   find = false;
   for(i = 0; i < num && !find; i++)
   {
    if(h_mrf->label[m * h_mrf->ynum + n].lb == hcluster[i].l.lb &&
       h_mrf->label[m * h_mrf->ynum + n].lt == hcluster[i].l.lt &&
       h_mrf->label[m * h_mrf->ynum + n].rb == hcluster[i].l.rb &&
       h_mrf->label[m * h_mrf->ynum + n].rt == hcluster[i].l.rt)
    {
     find = true;
    }
    for(j = 0; j < hcluster[i].subNum && !find; j++)
    {
     if(h_mrf->label[m * h_mrf->ynum + n].lb == hcluster[i].sub[j].lb &&
        h_mrf->label[m * h_mrf->ynum + n].lt == hcluster[i].sub[j].lt &&
        h_mrf->label[m * h_mrf->ynum + n].rb == hcluster[i].sub[j].rb &&
        h_mrf->label[m * h_mrf->ynum + n].rt == hcluster[i].sub[j].rt)
     {
      find = true;
      h_mrf->label[m * h_mrf->ynum + n].lb = hcluster[i].l.lb;
      h_mrf->label[m * h_mrf->ynum + n].lt = hcluster[i].l.lt;
      h_mrf->label[m * h_mrf->ynum + n].rb = hcluster[i].l.rb;
      h_mrf->label[m * h_mrf->ynum + n].rt = hcluster[i].l.rt;
     }
    }
   }
  }
 }
 
 _testHMRF(width, height, h_mrf, imgname);
}

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
void _testLMRF_Write_Cluster(int w, int h, LL num, LL dis, L_cluster* lcluster, char* imgname)
{
 int i = 0;
 int j = 0;
 int l = 0;
 LL m = num > dis ? dis : num;
 LL n = 0;
 IMAGE_L_MRF* l_mrf;

 l_mrf = (IMAGE_L_MRF*)malloc(sizeof(IMAGE_L_MRF));

 l_mrf->xnum = h / L_STEP;
 l_mrf->ynum = w / L_STEP;
 l_mrf->label = (L_label*)malloc(sizeof(L_label) * l_mrf->xnum * l_mrf->ynum);
 
 l = l_mrf->xnum * l_mrf->ynum;

 for(n = 0; n < m; n++)
 {
  for(i = 0; i < l; i++)
  {
   l_mrf->label[i] = 0;
  }

  l_mrf->label[0] = lcluster[n].l;
  //l_mrf->label[1] = 8659214;
  //for(i = 0, j = 2; i < lcluster[n].subNum && j < l; i++, j += 2)
  for(i = 0, j = 1; i < lcluster[n].subNum && j < l; i++, j++)
  {
   l_mrf->label[j] = lcluster[n].sub[i];
  }
  imgname[30] = (char)((n / 10) + '0');
  imgname[31] = (char)((n % 10) + '0');
  //_testLMRF2img(w, h, l_mrf, imgname);
  _testLMRF(w, h, l_mrf, imgname);
 }
}

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
void _testHMRF_Write_Cluster(int w, int h, LL num, LL dis, H_cluster* hcluster, char* imgname)
{
 int i = 0;
 int j = 0;
 int l = 0;
 LL m = num > dis ? dis : num;
 LL n = 0;
 IMAGE_H_MRF *h_mrf;

 h_mrf = (IMAGE_H_MRF*)malloc(sizeof(IMAGE_H_MRF));

 h_mrf->xnum = h / H_STEP;
 h_mrf->ynum = w / H_STEP;
 h_mrf->label = (H_label*)malloc(sizeof(H_label) * h_mrf->xnum * h_mrf->ynum);
 
 l = h_mrf->xnum * h_mrf->ynum;

 for(n = 0; n < m; n++)
 {
  for(i = 0; i < l; i++)
  {
   h_mrf->label[i].lb = 0;
   h_mrf->label[i].lt = 0;
   h_mrf->label[i].rb = 0;
   h_mrf->label[i].rt = 0;
  }

  h_mrf->label[0].lb = hcluster[n].l.lb;
  h_mrf->label[0].lt = hcluster[n].l.lt;
  h_mrf->label[0].rb = hcluster[n].l.rb;
  h_mrf->label[0].rt = hcluster[n].l.rt;

  //h_mrf->label[1].lb = 8659214;
  //h_mrf->label[1].lt = 8659214;
  //h_mrf->label[1].rb = 8659214;
  //h_mrf->label[1].rt = 8659214;

  //for(i = 0, j = 2; i < hcluster[n].subNum && j < l; i++, j += 2)
  for(i = 0, j = 1; i < hcluster[n].subNum && j < l; i++, j++)
  {
   h_mrf->label[j].lb = hcluster[n].sub[i].lb;
   h_mrf->label[j].lt = hcluster[n].sub[i].lt;
   h_mrf->label[j].rb = hcluster[n].sub[i].rb;
   h_mrf->label[j].rt = hcluster[n].sub[i].rt;
  }
  imgname[30] = (char)((n / 10) + '0');
  imgname[31] = (char)((n % 10) + '0');
  //_testHMRF2img(w, h, h_mrf, imgname);
  _testHMRF(w, h, h_mrf, imgname);
 }
}

/**
 * yangruiguo 2011-12-15
 * width  : the images' width
 * height : the images' height
 * l_mrf  : the low images' MRF
 * imgname: the output image's name
 * return : NULL
 */

void _testLMRF2img(int width, int height, IMAGE_L_MRF *l_mrf, const char* imgname)
{
 EHC_image image;
 char x[34];
 int k = 0;
 int len = l_mrf->xnum * l_mrf->ynum * L_STEP * L_STEP;
 unsigned char* d_data;
 int i, j;
 
 image.depth = 24;
 image.w  = width;
 image.h  = height;
 image.stride  = _width_bytes(image.depth, width);
 image.data = _mallocate_image(width,height,image.depth);

 d_data = (unsigned char*)malloc(width * height);
 memset(d_data, 0, width * height);
 for(i = 0; i < l_mrf->xnum; i++)
 {
  for(j = 0; j < l_mrf->ynum; j++)
  {
   _itoa(l_mrf->label[i * l_mrf->ynum + j], x, 2);
   k = (int)strlen(x) - 1;
   for(int m = L_STEP * (i + 1) - 1; m >= L_STEP * i && k >= 0; m--)
   {
    for(int n = L_STEP * (j + 1) - 1; n >= L_STEP * j && k >= 0; n--)
    {
     d_data[m * width + n] = (unsigned char)(x[k] - '0');
     k--;
    }
   }
  }
 }

 init_image_background(L_STEP, width, height, &image);
 _set_segment_to_backimage(d_data, width, height, &image);
 _save_image(imgname, &image);
 free(image.data);
 free(d_data);
}

/**
 * yangruiguo 2011-12-15
 * width  : the images' width
 * height : the images' height
 * h_mrf  : the high images' MRF
 * imgname: the output image's name
 * return : NULL
 */
void _testHMRF2img(int width, int height, IMAGE_H_MRF *h_mrf, const char* imgname)
{
 EHC_image image;
 char x[34];
 int k = 0;
 int len = h_mrf->xnum * h_mrf->ynum * H_STEP * H_STEP;
 unsigned char* d_data;
 int i, j;
 int xs = 0, xm = 0, xe = 0;
 int ys = 0, ym = 0, ye = 0;
 
 image.depth = 24;
 image.w  = width;
 image.h  = height;
 image.stride  = _width_bytes(image.depth, width);
 image.data = _mallocate_image(width,height,image.depth);

 d_data = (unsigned char*)malloc(width * height);
 memset(d_data, 0, width * height);
 for(i = 0; i < h_mrf->xnum; i++)
 {
  for(j = 0; j < h_mrf->ynum; j++)
  {
   xs = H_STEP * i;
   xm = (int)(H_STEP * (i + 0.5) - 1);
   xe = H_STEP * (i + 1) - 1;

   ys = H_STEP * j;
   ym = (int)(H_STEP * (j + 0.5) - 1);
   ye = H_STEP * (j + 1) - 1;

   //lt
   _itoa(h_mrf->label[i * h_mrf->ynum + j].lt, x, 2);
   k = (int)strlen(x) - 1;
   for(int m = xm; m >= xs && k >= 0; m--)
   {
    for(int n = ym; n >= ys && k >= 0; n--)
    {
     d_data[m * width + n] = (unsigned char)(x[k] - '0');
     k--;
    }
   }

   //rt
   _itoa(h_mrf->label[i * h_mrf->ynum + j].rt, x, 2);
   k = (int)strlen(x) - 1;
   for(int m = xm; m >= xs && k >= 0; m--)
   {
    for(int n = ye; n >= ym + 1 && k >= 0; n--)
    {
     d_data[m * width + n] = (unsigned char)(x[k] - '0');
     k--;
    }
   }

   //lb
   _itoa(h_mrf->label[i * h_mrf->ynum + j].lb, x, 2);
   k = (int)strlen(x) - 1;
   for(int m = xe; m >= xm + 1 && k >= 0; m--)
   {
    for(int n = ym; n >= ys && k >= 0; n--)
    {
     d_data[m * width + n] = (unsigned char)(x[k] - '0');
     k--;
    }
   }

   //rb
   _itoa(h_mrf->label[i * h_mrf->ynum + j].rb, x, 2);
   k = (int)strlen(x) - 1;
   for(int m = xe; m >= xm + 1 && k >= 0; m--)
   {
    for(int n = ye; n >= ym + 1 && k >= 0; n--)
    {
     d_data[m * width + n] = (unsigned char)(x[k] - '0');
     k--;
    }
   }
  }
 }

 init_image_background(H_STEP, width, height, &image);

 _set_segment_to_backimage(d_data, width, height, &image);
 _save_image(imgname, &image);
 free(image.data);
 free(d_data);
}


void _testLMRF2img_color(int width, int height, IMAGE_L_MRF *l_mrf, const char* imgname)
{
 EHC_image image;
 char x[34];
 int k = 0;
 int len = l_mrf->xnum * l_mrf->ynum * L_STEP * L_STEP;
 unsigned char* d_data;
 int i, j;
 
 image.depth = 24;
 image.w  = width;
 image.h  = height;
 image.stride  = _width_bytes(image.depth, width);
 image.data = _mallocate_image(width,height,image.depth);

 d_data = (unsigned char*)malloc(width * height);
 memset(d_data, 0, width * height);
 for(i = 0; i < l_mrf->xnum; i++)
 {
  for(j = 0; j < l_mrf->ynum; j++)
  {
   _itoa(l_mrf->label[i * l_mrf->ynum + j], x, 2);
   k = (int)strlen(x) - 1;
   for(int m = L_STEP * (i + 1) - 1; m >= L_STEP * i && k >= 0; m--)
   {
    for(int n = L_STEP * (j + 1) - 1; n >= L_STEP * j && k >= 0; n--)
    {
     d_data[m * width + n] = (unsigned char)(x[k] - '0');
     k--;
    }
   }
  }
 }

 _set_segment_to_image(d_data, width, height, &image);
 _save_image(imgname, &image);
 free(image.data);
 free(d_data);
}


/**
 * yangruiguo 2011-12-20
 * num     : the number of low images' cluster
 * lcluster: the low images' clusters
 * l_mrf   : the low images' MRF
 * return  : 0 success
 */
int _replaceLMRF(LL num, L_cluster* lcluster, IMAGE_L_MRF *l_mrf)
{
 LL i = 0;
 int j = 0;
 int m = 0;
 int n = 0;
 bool find = false;

 for(m = 0; m < l_mrf->xnum; m++)
 {
  for(n = 0; n < l_mrf->ynum; n++)
  {
   find = false;
   for(i = 0; i < num && !find; i++)
   {
    if(l_mrf->label[m * l_mrf->ynum + n] == lcluster[i].l)
    {
     find = true;
    }
    for(j = 0; j < lcluster[i].subNum && !find; j++)
    {
     if(l_mrf->label[m * l_mrf->ynum + n] == lcluster[i].sub[j])
     {
      find = true;
      l_mrf->label[m * l_mrf->ynum + n] = lcluster[i].l;
     }
    }
   }
   if(!find)
   {
    l_mrf->label[m * l_mrf->ynum + n] = lcluster[0].l;
   }
  }
 }
 return 0;
}

/**
 * yangruiguo 2011-12-20
 * num     : the number of high images' cluster
 * hcluster: the high images' clusters
 * h_mrf   : the high images' MRF
 * return  : 0 success
 */
int _replaceHMRF(LL num, H_cluster* hcluster, IMAGE_H_MRF *h_mrf)
{
 LL i = 0;
 int j = 0;
 int m = 0;
 int n = 0;
 bool find = false;

 for(m = 0; m < h_mrf->xnum; m++)
 {
  for(n = 0; n < h_mrf->ynum; n++)
  {
   find = false;
   for(i = 0; i < num && !find; i++)
   {
    if(h_mrf->label[m * h_mrf->ynum + n].lb == hcluster[i].l.lb &&
       h_mrf->label[m * h_mrf->ynum + n].lt == hcluster[i].l.lt &&
       h_mrf->label[m * h_mrf->ynum + n].rb == hcluster[i].l.rb &&
       h_mrf->label[m * h_mrf->ynum + n].rt == hcluster[i].l.rt)
    {
     find = true;
    }
    for(j = 0; j < hcluster[i].subNum && !find; j++)
    {
     if(h_mrf->label[m * h_mrf->ynum + n].lb == hcluster[i].sub[j].lb &&
        h_mrf->label[m * h_mrf->ynum + n].lt == hcluster[i].sub[j].lt &&
        h_mrf->label[m * h_mrf->ynum + n].rb == hcluster[i].sub[j].rb &&
        h_mrf->label[m * h_mrf->ynum + n].rt == hcluster[i].sub[j].rt)
     {
      find = true;
      h_mrf->label[m * h_mrf->ynum + n].lb = hcluster[i].l.lb;
      h_mrf->label[m * h_mrf->ynum + n].lt = hcluster[i].l.lt;
      h_mrf->label[m * h_mrf->ynum + n].rb = hcluster[i].l.rb;
      h_mrf->label[m * h_mrf->ynum + n].rt = hcluster[i].l.rt;
     }
    }
   }
   if(!find)
   {
    h_mrf->label[m * h_mrf->ynum + n].lb = hcluster[0].l.lb;
    h_mrf->label[m * h_mrf->ynum + n].lt = hcluster[0].l.lt;
    h_mrf->label[m * h_mrf->ynum + n].rb = hcluster[0].l.rb;
    h_mrf->label[m * h_mrf->ynum + n].rt = hcluster[0].l.rt;
   }
  }
 }
 return 0;
}