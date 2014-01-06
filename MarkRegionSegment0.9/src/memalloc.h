#pragma once

void syserr(char *msg1,char *msg2,char *msg3);
/*******************************************************************/
void* _mae_datadup(void *p, int nbytes, char *s);

/*******************************************************************/
void _mae_malloc_char(char **ptr, int n, char *s);

/*******************************************************************/
void _mae_malloc_uchar(unsigned char **ptr, int n, char *s);

/*******************************************************************/
void _mae_malloc_shrt(short **ptr, short n, char *s);
 
/*******************************************************************/
void _mae_malloc_int(int **ptr, int n, char *s);


/*******************************************************************/
void _mae_malloc_flt(float **ptr,int  n, char *s);

/*******************************************************************/
void _mae_malloc_long(long **ptr, int n, char *s);

/*******************************************************************/
void _mae_calloc_char(char **ptr, int n, char *s);


/*******************************************************************/
void _mae_calloc_uchar(unsigned char **ptr, int n, char *s);
 

/*******************************************************************/
void _mae_calloc_shrt(short **ptr, short n, char *s);
 

/*******************************************************************/
void _mae_calloc_int(int **ptr, int n, char *s);
  
/*******************************************************************/
void _mae_calloc_flt(float **ptr, int n, char *s);
 

/*******************************************************************/
void _mae_malloc_dbl_char_l1(char ***ptr, int ndbl, char *s);
  
/*******************************************************************/
void _mae_malloc_dbl_uchar_l1(unsigned char ***ptr, int ndbl, char *s);
  

/*******************************************************************/
void _mae_malloc_dbl_shrt_l1(short ***ptr, short ndbl, char *s);
 

/*******************************************************************/
void _mae_malloc_dbl_int_l1(int ***ptr, int ndbl, char *s);
  
/*******************************************************************/
void _mae_malloc_dbl_flt_l1(float ***ptr, int ndbl, char *s);
  

/*******************************************************************/
void _mae_realloc_char(char **ptr, int n, char *s);
  
/*******************************************************************/
void _mae_realloc_uchar(unsigned char **ptr, int n, char *s);
  

/*******************************************************************/
void _mae_realloc_shrt(short **ptr, short n, char *s);
  

/*******************************************************************/
void _mae_realloc_int(int **ptr, int n, char *s);
  

/*******************************************************************/
void _mae_realloc_flt(float **ptr, int n, char *s);
  
/*******************************************************************/
void _mae_realloc_dbl_int_l1(int ***ptr, int ndbl, char *s);
  
/*******************************************************************/
void _mae_realloc_dbl_char_l1(char ***ptr, int ndbl, char *s);
   

/*******************************************************************/
void _mae_realloc_dbl_uchar_l1(unsigned char ***ptr, int ndbl, char *s);
   

/*******************************************************************/
void _mae_realloc_dbl_flt_l1( float ***ptr, int ndbl, char *s);
   
