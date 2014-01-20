
#include "StdAfx.h"
#include <stddef.h>
#include <stdlib.h>
#include "memalloc.h"

void syserr(char *msg1,char *msg2,char *msg3)
{
 char * _msg1;
 char * _msg2;
 char * _msg3;
 _msg1 = msg1;
 _msg2 = msg2;
 _msg3 = msg3;
 return;
}
/*******************************************************************/
//
void *_datadup(void *p, int nbytes, char *s)
{
 void *q;
 char *_s;
 _s = s;

   if ( p == NULL )
   {
      return NULL;
   }
   if ((q = (void *)malloc(nbytes)) == NULL)
   {
      return NULL;
   }
   memcpy(q, p, nbytes);
   return q;
}

/*******************************************************************/
void _malloc_char(char **ptr, int n, char *s)
{
 if(((*ptr) = (char *)malloc(n * sizeof(char))) == NULL)
      syserr("malloc_char","malloc",s);
    memset(*ptr, 0, n*sizeof(char));
}

/*******************************************************************/
void _malloc_uchar(unsigned char **ptr, int n, char *s)
{
   if(((*ptr) = (unsigned char *)malloc(n * sizeof(unsigned char))) == NULL)
     syserr("malloc_uchar","malloc",s);
   memset(*ptr, 0, n*sizeof(unsigned char));
}

/*******************************************************************/
void _malloc_shrt(short **ptr, short n, char *s)
{
   if(((*ptr) = (short *)malloc(n * sizeof(short))) == NULL)
     syserr("malloc_shrt","malloc",s);
}

/*******************************************************************/
void _malloc_int(int **ptr, int n, char *s)
{
   if(((*ptr) = (int *)malloc(n * sizeof(int))) == NULL)
      syserr("malloc_int","malloc",s);
   memset(*ptr, 0, n*sizeof(int));
}
void _malloc_long(long **ptr, int n, char *s)
{
   if(((*ptr) = (long *)malloc(n * sizeof(long))) == NULL)
      syserr("malloc_long","malloc",s);
   memset(*ptr, 0, n*sizeof(long));
}

/*******************************************************************/
void _malloc_flt(float **ptr,int  n, char *s)
{
   if(((*ptr) = (float *)malloc(n * sizeof(float))) == NULL)
      syserr("malloc_float","malloc",s);
   memset(*ptr, 0, n*sizeof(float));
}

/*******************************************************************/
void _calloc_char(char **ptr, int n, char *s)
{
   if(((*ptr) = (char *)calloc(n, sizeof(char))) == NULL)
      syserr("calloc_char","calloc",s);
   memset(*ptr, 0, n*sizeof(char));
}

/*******************************************************************/
void _calloc_uchar(unsigned char **ptr, int n, char *s)
{
   if(((*ptr) = (unsigned char *)calloc(n, sizeof(unsigned char))) == NULL)
      syserr("calloc_uchar","calloc",s);
   memset(*ptr, 0, n*sizeof(unsigned char));
}

/*******************************************************************/
void _calloc_shrt(short **ptr, short n, char *s)
{
   if(((*ptr) = (short *)calloc(n, sizeof(short))) == NULL)
      syserr("calloc_shrt","calloc",s);
   memset(*ptr, 0, n*sizeof(short));
}

/*******************************************************************/
void _calloc_int(int **ptr, int n, char *s)
{
   if(((*ptr) = (int *)calloc(n, sizeof(int))) == NULL)
      syserr("calloc_int","calloc",s);
   memset(*ptr, 0, n*sizeof(int));
}

/*******************************************************************/
void _calloc_flt(float **ptr, int n, char *s)
{
   if(((*ptr) = (float *)calloc(n, sizeof(float))) == NULL)
      syserr("calloc_float","calloc",s);
   memset(*ptr, 0, n*sizeof(float));
}

/*******************************************************************/
void _malloc_dbl_char_l1(char ***ptr, int ndbl, char *s)
{
   if(((*ptr) = (char **)malloc(ndbl * sizeof(char *))) == NULL)
      syserr("malloc_dbl_char_l1","malloc",s);
   memset(*ptr, 0, ndbl*sizeof(char*));
}

/*******************************************************************/
void _malloc_dbl_uchar_l1(unsigned char ***ptr, int ndbl, char *s)
{
   if(((*ptr) = (unsigned char **)malloc(ndbl * sizeof(unsigned char *))) == NULL)
      syserr("malloc_dbl_uchar_l1","malloc",s);
   memset(*ptr, 0, ndbl*sizeof(unsigned char*));
}

/*******************************************************************/
void _malloc_dbl_shrt_l1(short ***ptr, short ndbl, char *s)
{
   if(((*ptr) = (short **)malloc(ndbl * sizeof(short *))) == NULL)
      syserr("malloc_dbl_shrt_l1","malloc",s);
   memset(*ptr, 0, ndbl*sizeof(short*));
}

/*******************************************************************/
void _malloc_dbl_int_l1(int ***ptr, int ndbl, char *s)
{
   if(((*ptr) = (int **)malloc(ndbl * sizeof(int *))) == NULL)
      syserr("malloc_dbl_int_l1","malloc",s);
   memset(*ptr, 0, ndbl*sizeof(int*));
}

/*******************************************************************/
void _malloc_dbl_flt_l1(float ***ptr, int ndbl, char *s)
{
   if(((*ptr) = (float **)malloc(ndbl * sizeof(float *))) == NULL)
      syserr("malloc_dbl_flt_l1","malloc",s);
   memset(*ptr, 0, ndbl*sizeof(float*));
}

/*******************************************************************/
void _realloc_char(char **ptr, int n, char *s)
{
   if((*ptr = (char *)realloc(*ptr, n * sizeof(char))) == NULL)
      syserr("realloc_char","realloc",s);
}

/*******************************************************************/
void _realloc_uchar(unsigned char **ptr, int n, char *s)
{
   if((*ptr = (unsigned char *)realloc(*ptr, n * sizeof(unsigned char))) == NULL)
      syserr("realloc_uchar","realloc",s);
}

/*******************************************************************/
void _realloc_shrt(short **ptr, short n, char *s)
{
   if((*ptr = (short *)realloc(*ptr, n * sizeof(short))) == NULL)
      syserr("realloc_shrt","realloc",s);
}

/*******************************************************************/
void _realloc_int(int **ptr, int n, char *s)
{
   if((*ptr = (int *)realloc(*ptr, n * sizeof(int))) == NULL)
      syserr("realloc_int","realloc",s);
}

/*******************************************************************/
void _realloc_flt(float **ptr, int n, char *s)
{
   if((*ptr = (float *)realloc(*ptr, n * sizeof(float))) == NULL)
      syserr("realloc_flt","realloc",s);
}

/*******************************************************************/
void _realloc_dbl_int_l1(int ***ptr, int ndbl, char *s)
{
   if((*ptr = (int **)realloc(*ptr, ndbl * sizeof(int *))) == NULL)
      syserr("realloc_dbl_int_l1","realloc",s);
}

/*******************************************************************/
void _realloc_dbl_char_l1(char ***ptr, int ndbl, char *s)
{
   if((*ptr = (char **)realloc(*ptr, ndbl * sizeof(char *))) == NULL)
      syserr("realloc_dbl_char_l1","realloc",s);
}

/*******************************************************************/
void _realloc_dbl_uchar_l1(unsigned char ***ptr, int ndbl, char *s)
{
   if((*ptr = (unsigned char **)realloc(*ptr, ndbl * sizeof(unsigned char *))) == NULL)
      syserr("realloc_dbl_uchar_l1","realloc",s);
}

/*******************************************************************/
void _realloc_dbl_flt_l1( float ***ptr, int ndbl, char *s)
{
   if((*ptr = (float **)realloc(*ptr, ndbl * sizeof(float *))) == NULL)
      syserr("realloc_dbl_flt_l1","realloc",s);
}
