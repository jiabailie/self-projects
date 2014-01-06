#ifndef _DATAHEAD_H_ 

#define _DATAHEAD_H_ 

#define H_STEP 10
#define L_STEP 5

//#define H_DIS	5
#define H_DIS	20
#define L_DIS	3
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

#define OUTPUT_DIR_NAME_mA      "..\\..\\output\\y2y_ma.txt"
#define OUTPUT_DIR_NAME_mB      "..\\..\\output\\y2y_mb.txt"
#define OUTPUT_DIR_NAME_mC      "..\\..\\output\\y2y_mc.txt"
#define OUTPUT_DIR_NAME_mD      "..\\..\\output\\y2y_md.txt"
#define OUTPUT_DIR_NAME_mXY      "..\\..\\output\\x2y_md.txt"

#define OUTPUT_DIR_NAME_A       "..\\..\\output\\y2y_a.txt"
#define OUTPUT_DIR_NAME_B       "..\\..\\output\\y2y_b.txt"
#define OUTPUT_DIR_NAME_C       "..\\..\\output\\y2y_c.txt"
#define OUTPUT_DIR_NAME_D       "..\\..\\output\\y2y_d.txt"
#define OUTPUT_DIR_NAME_XY      "..\\..\\output\\x2y.txt"
#define OUTPUT_DIR_NAME_OrderXY "..\\..\\output\\orderx2y.txt"

#define LOWMRF_NAME        "..\\..\\output\\l_mrf.txt"
#define HIGMRF_NAME        "..\\..\\output\\h_mrf.txt"

#define LOW_CLUSTER_NAME   "..\\..\\output\\l_cluster.txt"
#define HIG_CLUSTER_NAME   "..\\..\\output\\h_cluster.txt"

//#define SAMPLE_DIR         "E:\\project\\EHC_Enhancement\\EHC_Project\\sample\\"
//#define SAMPLE_DIR_NAME    "E:\\project\\EHC_Enhancement\\EHC_Project\\sample\\dir.txt"

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
	LL v;
	H_label l;
}H_statis;

//give each low cluster a continuous integer value
typedef struct st_L_statis{
	LL v;
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