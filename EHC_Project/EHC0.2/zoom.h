/*
# proc: zoom - scales a binary bitmap up or down depending on the x & y factors
# proc:        passed.
# proc: zoom_x - scales a binary bitmap up or down based on the x factor
# proc:          passed.
# proc: zoom_y - scales a binary bitmap up or down based on the y factor
# proc:          passed.
# proc: enlarge_x - expand a binary bitmap's columns by the x factor passed.
# proc:
# proc: enlarge_y - expand a binary bitmap's rows by the y factor passed.
# proc:
# proc: shrink_x - down sample a binary bitmap's columns by the x factor
# proc:            passed.
# proc: shrink_y - down sample a binary bitmap's rows by the y factor passed.
# proc:
# proc: copy_cols - copies a single column of pixels to a range of column
# proc:             locations in a destination image.
# proc: copy_col - copies a column of pixels to a column location in a
# proc:            destination image.
# proc: copy_rows - copies a single row of pixels to a range of row locations
# proc:             in a destination image.
# proc: copy_row - copies a row of pixels to a row location in a destination
# proc:            image.
*/
#pragma once
/************************************************************************/
void _zoom(unsigned char **zmdata, int *zw, int *zh, 
    unsigned char *bindata, int iw, int ih, int depth,
    float xfctr, float yfctr);

/************************************************************************/
void _zoom_x(unsigned char **xzmdata, int *zw, int *zh, 
   unsigned char *chardata, int iw, int ih, int depth,float xfctr);

/************************************************************************/
void _zoom_y(unsigned char **yzmdata, int *zw, int *zh, 
   unsigned char *chardata, int iw, int ih, int depth,float yfctr);

/************************************************************************/
void _enlarge_x(unsigned char **xzmdata, int *zw, int *zh, 
      unsigned char *chardata, int iw, int ih, int depth,float xfctr);


/************************************************************************/
void _enlarge_y(unsigned char **, int *zw, int *zh, 
      unsigned char *chardata, int iw, int ih, int depth,float yfctr);


/************************************************************************/
void _shrink_x(unsigned char **xzmdata, int *zw, int *zh, 
     unsigned char *chardata, int iw, int ih, int depth,float xfctr);

/************************************************************************/
void _shrink_y(unsigned char **yzmdata, int *zw, int *zh, 
     unsigned char *chardata, int iw, int ih, int depth,float yfctr);

/************************************************************************/
void _copy_cols(unsigned char *xzmdata, int zw, int zh, 
     int to1, int to2, 
     unsigned char *chardata, int iw, int ih, int depth, int from);

/************************************************************************/
void _copy_col(unsigned char *xzmdata, int zw, int zh, int to, 
     unsigned char *chardata,int  iw, int ih, int depth, int from);

/************************************************************************/
void _copy_rows(unsigned char *yzmdata, int zw, int zh, int to1, int to2, 
      unsigned char *chardata, int iw, int ih, int depth, int from);

/************************************************************************/
void _copy_row(unsigned char *yzmdata, int zw, int zh, int to, 
     unsigned char *chardata, int iw, int ih, int depth, int from);
