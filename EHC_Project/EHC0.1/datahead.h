#ifndef _DATAHEAD_H_ 

#define _DATAHEAD_H_ 

#define H_STEP 10
#define L_STEP 5

#define H_DIS	7
#define L_DIS	1
#define L_H_MATCH_OPTION  2

#define SAMPLE_NUM 60

#define MAX 512
#define MAXLEN 128
#define MAXSIZE 33554432
#define FAIL_OPEN_FILE -1
#define FAIL_FIND_FILE -2
#define SUC_RETURN		0

#define TEST_RESULT         "..\\..\\testresult\\"

/* the folder which holds the images waitting to be restored */
#define TEST_RESTORE        "..\\..\\restore\\"

#define SAMPLE_DIR         "..\\..\\sampleimages\\"
#define SAMPLE_DIR_NAME    "..\\..\\sampleimages\\dir.txt"

//#define OUTPUT_DIR_NAME_mA      "..\\..\\output\\y2y_ma.txt"
//#define OUTPUT_DIR_NAME_mB      "..\\..\\output\\y2y_mb.txt"
//#define OUTPUT_DIR_NAME_mC      "..\\..\\output\\y2y_mc.txt"
//#define OUTPUT_DIR_NAME_mD      "..\\..\\output\\y2y_md.txt"
//#define OUTPUT_DIR_NAME_mXY      "..\\..\\output\\x2y_md.txt"

/* the txt files which save the col data and row index */
#define OUTPUT_COL_A         "..\\..\\output\\cola.txt"
#define OUTPUT_COL_B         "..\\..\\output\\colb.txt"
#define OUTPUT_COL_C         "..\\..\\output\\colc.txt"
#define OUTPUT_COL_D         "..\\..\\output\\cold.txt"
#define OUTPUT_COL_XY        "..\\..\\output\\colxy.txt"

#define OUTPUT_ROW_A         "..\\..\\output\\rowa.txt"
#define OUTPUT_ROW_B         "..\\..\\output\\rowb.txt"
#define OUTPUT_ROW_C         "..\\..\\output\\rowc.txt"
#define OUTPUT_ROW_D         "..\\..\\output\\rowd.txt"
#define OUTPUT_ROW_XY        "..\\..\\output\\rowxy.txt"

#define OUTPUT_ORDER_XY      "..\\..\\output\\orderxy.txt"

#define LOWMRF_NAME        "..\\..\\output\\l_mrf.txt"
#define HIGMRF_NAME        "..\\..\\output\\h_mrf.txt"

#define P_LOWMRF_NAME        "..\\..\\output\\pl_mrf.txt"
#define P_HIGMRF_NAME        "..\\..\\output\\ph_mrf.txt"

#define LOW_CLUSTER_NAME   "..\\..\\output\\l_cluster.txt"
#define HIG_CLUSTER_NAME   "..\\..\\output\\h_cluster.txt"

#define DEBUG_CLUSTER

typedef _int64 LL;

//low quality images' label
typedef int L_label;

//high quality images' label
typedef struct st_H_LABEL{
	int lt;
	int rt;
	int lb;
	int rb;
}H_label;

//low quality images' MRF count
typedef struct st_L_count{
	int l;
	LL c;
}L_count;

//high quality images' MRF count
typedef struct st_H_count{
	int lt; 
	int rt;
	int lb;
	int rb;
	LL c;
}H_count;

//the cluster of low quality images
typedef struct st_L_cluster{
	int subNum;
	L_label l;
	L_label* sub;
}L_cluster;

//the cluster of high quality images
typedef struct st_H_cluster{
	int subNum;
	H_label l;
	H_label* sub;
}H_cluster;

//give each cluster a integer value
typedef struct st_H_statis{
	int v;
	H_label l;
}H_statis;

//give each low cluster a continuous integer value
typedef struct st_L_statis{
	int v;
	L_label l;
}L_statis;

//the struct of each low quality image
typedef struct st_IMAGE_L_MRF{
	L_label * label;
	int xnum;	
	int ynum;	
}IMAGE_L_MRF;

//the struct of each high quality image
typedef struct st_IMAGE_H_MRF{
	H_label * label;
	int xnum;	
	int ynum;	
}IMAGE_H_MRF;

typedef struct st_sample_name{
	char h_name[500];
	char l_name[500];
}Sample_name;

#endif 