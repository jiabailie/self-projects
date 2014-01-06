#include "StdAfx.h"
#include "k_means.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>

int inputk(void)
{
	int k;
	while(1)
	{
	   printf("Please input the number of clusters k: ");
	   scanf("%d",&k);
	   if(k<=0) continue;
	   return k;
	}
}
void init(dataset**head,int n)
{
	int i;
	dataset* dp;
	*head=(dataset*)malloc(sizeof(dataset));
	if(*head==NULL) exit(1);
	(*head)->next=NULL;
	for(dp=*head,i=1; i<n; dp=dp->next,i++)
	{
	   dp->next=(dataset*)malloc(sizeof(dataset));
	   if(dp->next==NULL) exit(1);
	   dp->next->next=NULL;
	}
}
void inputdataset(dataset **head,int *n,int k)
{
	//int i;
	//dataset *dp;
	//while(1)
	//{
	//   printf("Please input the number of points n: ");
	//   scanf("%d",n);
	//   if(*n<k)
	//   {
	//	printf("Your input n<k.\n");
	//	continue;
	//   }
	//   else break;
	//}
	//init(head,*n);
	//for(dp=*head,i=0;i<*n && dp!=NULL;dp=dp->next,i++)
	//{
	//   printf("The %dth point: ",i+1);
	//   scanf("%f%f",&(dp->p.x),&(dp->p.y));
	//}
}
void input(dataset **head,int *n,int *k)
{
	*k=inputk();
	inputdataset(head,n,*k);
}
void initclusters(clusters**clhead,int k)
{
	int i;
	clusters *tmp;
	*clhead=(clusters*)malloc(sizeof(clusters));
	if(*clhead==NULL) exit(1);
	(*clhead)->next=NULL;
	(*clhead)->clp=NULL;
	for(tmp=*clhead,i=1;i<k;tmp=tmp->next,i++)
	{
	   tmp->next=(clusters*)malloc(sizeof(clusters));
	   if(tmp->next==NULL) exit(1);
	   tmp->next->clp=NULL;
	   tmp->next->next=NULL;
	}
}

void arbicenter(clusters *clhead,int k)
{
	//int i;
	//point pcen;
	//clusters *clsp;
	//printf("Please arbitrarily choose k objects from the dataset as the initial cluster centers.\n");
	//for(clsp=clhead,i=0;i<k && clsp!=NULL;clsp=clsp->next,i++)
	//{
	//   printf("Please choose the %dth center: ",i+1);
	//   scanf("%f%f",&(pcen.x),&(pcen.y));
	//   clsp->center=pcen;
	//}
}

void estimate_center(dataset *head, clusters *clhead,int k)
{
	int i;
	point pcen;
	clusters *clsp;
	double h[4];
	double minh;
	double maxh;
	long count;

	dataset *dp;

	h[0] = 0;
	h[1] = 0;
	h[2] = 0;
	h[3] = 0;
	minh = 256;
	maxh = 0;
	count = 0;
	for(dp=head; dp!=NULL; dp=dp->next)
	{
		h[0] += dp->p.h;
		count++;
		if( minh >  dp->p.h )
		{
			minh = dp->p.h;
		}
		if( maxh <  dp->p.h )
		{
			maxh = dp->p.h;
		}
	}
	h[0] = h[0] / count;

	
	switch(k)
	{
	case 1:
		break;
	case 2:
		h[0] = (h[0] + minh) / 2;
		h[1] = (h[0] + maxh) / 2;
		break;
	case 3:
		h[0] = h[0];
		h[1] = (h[0] + minh) / 2;
		h[2] = (h[0] + maxh) / 2;
		break;
	case 4:
		h[0] = h[0];
		h[1] = (h[0] + minh) / 2;
		h[2] = (h[0] + maxh) / 2;
		h[3] = minh;
		break;
	}

	for(clsp=clhead,i=0;i<k && clsp!=NULL;clsp=clsp->next,i++)
	{
	   clsp->center.h = h[k];
	}
}
double distance(point *p1,point *p2)
{
	//float tmpx,tmpy;
	//tmpx=(p1->x-p2->x);
	//tmpy=(p1->y-p2->y);
	//return tmpx*tmpx+tmpy*tmpy;
	double tmph;
	tmph = p1->h - p2->h;
	return (tmph * tmph); 
}
void printclusters(clusters *clhead)
{
	//clusters *clsp;
	//cluster *clu;
	//int i;
	//for(clsp=clhead,i=0;clsp!=NULL;clsp=clsp->next,i++)
	//{
	//   printf("--->>>The %dth cluster:\n",i+1);
	//   printf("\tcenter: (%.2f,%.2f)\n",clsp->center.x,clsp->center.y);
	//   for(clu=clsp->clp;clu!=NULL;clu=clu->next)
	//   {
	//	printf("\t(%.2f,%.2f)\n",clu->p->x,clu->p->y);
	//   }
	//}
}
void clusp(dataset *head,clusters *clhead)
{
	static int count=0;
	float ndis,mindis;
	dataset *dp;
	clusters *clsp,*cp;
	cluster *tmp;
	for(clsp=clhead;clsp!=NULL;clsp=clsp->next)
	{
	   if(clsp->clp!=NULL)
	   {
			cluster *clup,*cluf;
			cluf=clsp->clp;
			for(clup=cluf->next;cluf!=NULL;)
			{
				 free(cluf);
				 cluf=clup;
				 if(clup!=NULL)
				  clup=clup->next;
			}
			clsp->clp=NULL;
	   }
	}
	for(dp=head;dp!=NULL;dp=dp->next)
	{
	   clsp=clhead;
	   cp=clhead;
	   if(clhead->next!=NULL)
	   {
			mindis=distance(&(dp->p),&(clsp->center));
			for(clsp=clhead->next;clsp!=NULL;clsp=clsp->next)
			{
				ndis=distance(&(dp->p),&(clsp->center));
				if(ndis<mindis)
				{
					cp=clsp;
					mindis=ndis;
				}
			}
	   }
	   tmp=(cluster*)malloc(sizeof(cluster));
	   tmp->next=cp->clp;
	   cp->clp=tmp;
	   tmp->p=&(dp->p);
	}
	count++;
	printf("\n-->>The %dth time cluster:\n",count);
	printclusters(clhead);
	printf("-->>The %dth time cluster over\n",count);
}
point center(cluster *clp)
{
	double h;
	long num;
	point p;

	num = 0;
	h = 0;
	while(clp != NULL )
	{
		num ++;
		h = h + clp->p->h;
		clp = clp->next;
	}
	p.h= h/(double)num;
	//float x,y;
	//int i;
	//point p;
	//for(x=0.0,y=0.0,i=0;clp!=NULL;clp=clp->next,i++)
	//{
	//   x=x+clp->p->x;
	//   y=y+clp->p->y;
	//}
	//p.x=x/(float)i;
	//p.y=y/(float)i;
	return p;
}
int eqpoint(point p1,point p2)
{
	double hd;
	hd=fabs(p1.h-p2.h);
	if( hd<1 )
	   return 1;
	//if( hd<0.001 )
	//   return 1;
	//double xd,yd;
	//xd=fabs((double)p1.x-(double)p2.x);
	//yd=fabs((double)p1.y-(double)p2.y);
	//if(xd<0.001 && yd<0.001)
	//   return 1;
	return 0;
}
int updatecenter(clusters *clhead)
{
	int changed;
	point newcenter;
	clusters *clsp;
	changed=0;
	for(clsp=clhead;clsp!=NULL;clsp=clsp->next)
	{
	   newcenter=center(clsp->clp);
	   if(eqpoint(newcenter,clsp->center)==0)
	   {
		changed=1;
		clsp->center=newcenter;
	   }
	}
	return changed;
}
void kmeans(dataset*head,clusters**clhead,int k)
{
	initclusters(clhead,k);
	//arbicenter(*clhead,k);
	estimate_center(head, *clhead, k);
	while(1)
	{
	   clusp(head,*clhead);
	   if(updatecenter(*clhead)==0)
		break;
	}
}
void output(clusters *clhead)
{
	printf("\n********Output*********:\n\n");
	printclusters(clhead);
	printf("\n******output over******\n");
}

void free_dataset(dataset *head)
{
	dataset *dp;
	for(dp=head->next;head!=NULL;)
	{
	   free(head);
	   head=dp;
	   if(dp!=NULL)
		dp=dp->next;
	}
}

void free_clusters(clusters *clhead)
{
	clusters *clsp;
	cluster *cp;
	for(cp=clhead->clp->next;clhead->clp!=NULL;)
	{
	   free(clhead->clp);
	   clhead->clp=cp;
	   if(cp!=NULL)
		cp=cp->next;
	}
	for(clsp=clhead->next;clhead!=NULL;)
	{
	   free(clhead);
	   clhead=clsp;
	   if(clsp!=NULL)
		clsp=clsp->next;
	}
}

void freeall(dataset *head,clusters *clhead)
{
	dataset *dp;
	clusters *clsp;
	cluster *cp;
	for(dp=head->next;head!=NULL;)
	{
	   free(head);
	   head=dp;
	   if(dp!=NULL)
		dp=dp->next;
	}
	for(cp=clhead->clp->next;clhead->clp!=NULL;)
	{
	   free(clhead->clp);
	   clhead->clp=cp;
	   if(cp!=NULL)
		cp=cp->next;
	}
	for(clsp=clhead->next;clhead!=NULL;)
	{
	   free(clhead);
	   clhead=clsp;
	   if(clsp!=NULL)
		clsp=clsp->next;
	}
}
int test_k_mean()
{
	int k,n;
	dataset *head;
	clusters *clhead;
	input(&head,&n,&k);
	kmeans(head,&clhead,k);
	output(clhead);
	freeall(head,clhead);
	return 0;
}