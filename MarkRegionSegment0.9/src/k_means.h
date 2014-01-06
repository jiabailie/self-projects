#pragma once

typedef struct point {
	double h;
} point;
typedef struct dataset {
	point p;
	struct dataset * next;
} dataset;
typedef struct cluster {
	point * p;
	struct cluster * next;
} cluster;
typedef struct clusters {
	point center;
	cluster *clp;
	struct clusters * next;
} clusters;

int inputk(void);
void init(dataset**head, int n);
void inputdataset(dataset **head,int *n,int k);
void input(dataset **head,int *n,int *k);
void initclusters(clusters**clhead,int k);
void arbicenter(clusters *clhead,int k);
double distance(point *p1,point *p2);
void printclusters(clusters *clhead);
void clusp(dataset *head,clusters *clhead);
point center(cluster *clp);
int eqpoint(point p1,point p2);
int updatecenter(clusters *clhead);
void kmeans(dataset*head,clusters**clhead,int k);
void output(clusters *clhead);
void free_dataset(dataset *head);
void free_clusters(clusters *clhead);
void freeall(dataset *head,clusters *clhead);
int test_k_mean();
