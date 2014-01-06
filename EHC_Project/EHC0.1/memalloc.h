#pragma once

void syserr(char *msg1,char *msg2,char *msg3);
/*******************************************************************/
void* _datadup(void *p, int nbytes, char *s);

/*******************************************************************/
void _malloc_char(char **ptr, int n, char *s);

/*******************************************************************/
void _malloc_uchar(unsigned char **ptr, int n, char *s);

/*******************************************************************/
void _malloc_shrt(short **ptr, short n, char *s);
 
/*******************************************************************/
void _malloc_int(int **ptr, int n, char *s);


/*******************************************************************/
void _malloc_flt(float **ptr,int  n, char *s);

/*******************************************************************/
void _malloc_long(long **ptr, int n, char *s);

/*******************************************************************/
void _calloc_char(char **ptr, int n, char *s);


/*******************************************************************/
void _calloc_uchar(unsigned char **ptr, int n, char *s);
 

/*******************************************************************/
void _calloc_shrt(short **ptr, short n, char *s);
 

/*******************************************************************/
void _calloc_int(int **ptr, int n, char *s);
  
/*******************************************************************/
void _calloc_flt(float **ptr, int n, char *s);
 

/*******************************************************************/
void _malloc_dbl_char_l1(char ***ptr, int ndbl, char *s);
  
/*******************************************************************/
void _malloc_dbl_uchar_l1(unsigned char ***ptr, int ndbl, char *s);
  

/*******************************************************************/
void _malloc_dbl_shrt_l1(short ***ptr, short ndbl, char *s);
 

/*******************************************************************/
void _malloc_dbl_int_l1(int ***ptr, int ndbl, char *s);
  
/*******************************************************************/
void _malloc_dbl_flt_l1(float ***ptr, int ndbl, char *s);
  

/*******************************************************************/
void _realloc_char(char **ptr, int n, char *s);
  
/*******************************************************************/
void _realloc_uchar(unsigned char **ptr, int n, char *s);
  

/*******************************************************************/
void _realloc_shrt(short **ptr, short n, char *s);
  

/*******************************************************************/
void _realloc_int(int **ptr, int n, char *s);
  

/*******************************************************************/
void _realloc_flt(float **ptr, int n, char *s);
  
/*******************************************************************/
void _realloc_dbl_int_l1(int ***ptr, int ndbl, char *s);
  
/*******************************************************************/
void _realloc_dbl_char_l1(char ***ptr, int ndbl, char *s);
   

/*******************************************************************/
void _realloc_dbl_uchar_l1(unsigned char ***ptr, int ndbl, char *s);
   

/*******************************************************************/
void _realloc_dbl_flt_l1( float ***ptr, int ndbl, char *s);
   
