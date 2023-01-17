#include<stdio.h>
#include <stdlib.h>
 
#define MAX 30
 
typedef struct edge{
	int u,v,w;
}edge;
 
typedef struct edgelist{
	edge data[MAX];
	int n;
}edgelist;
 
edgelist elist;
edgelist spanlist;

void readFile(char dizi[]);
void graphCheck();
int find(int belongs[],int vertexno);
void union1(int belongs[],int c1,int c2);
void sort();
void print();
void getNodeandCheck();
 
int main(){
	char fileName[100];

	printf("Enter the filename: ");
	scanf("%s",fileName);

	readFile(fileName);
	
	print();
	
	getNodeandCheck();
	
	print();
	
	return 0;
}

void readFile(char fname[]){
	elist.n=0;
	
	FILE *file;

	if ((file = fopen(fname, "r")) == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
  
	while(!feof(file)){
		fscanf(file,"%d %d %d",&(elist.data[elist.n].w),&(elist.data[elist.n].u),&(elist.data[elist.n].v));
		elist.n++;
	}
 	
 	graphCheck();
}

 
void graphCheck(){
	int belongs[MAX],i,j,cno1,cno2;

	sort();
	for(i=0;i<elist.n;i++)
	belongs[i]=i;
	spanlist.n=0;
	for(i=0;i<elist.n;i++){
		cno1=find(belongs,elist.data[i].u);
		cno2=find(belongs,elist.data[i].v);
		if(cno1!=cno2){
			spanlist.data[spanlist.n]=elist.data[i];
			spanlist.n=spanlist.n+1;
			union1(belongs,cno1,cno2);
		}
	}
}

 
int find(int belongs[],int vertexno){
	return(belongs[vertexno]);
}
 
void union1(int belongs[],int c1,int c2){
	int i;
	for(i=0;i<elist.n;i++)
		if(belongs[i]==c2)
			belongs[i]=c1;
}
 
void sort(){
	int i,j;
	edge temp;
	for(i=1;i<elist.n;i++)
		for(j=0;j<elist.n-1;j++)
			if(elist.data[j].w>elist.data[j+1].w){
				temp=elist.data[j];
				elist.data[j]=elist.data[j+1];
				elist.data[j+1]=temp;
			}
}
 
void print(){
	int i,cost=0;
	printf("\n\nLink values of the graph are: \n");
	for(i=0;i<spanlist.n;i++){
		printf("\n%d\t%d\t%d",spanlist.data[i].u,spanlist.data[i].v,spanlist.data[i].w);
		cost=cost+spanlist.data[i].w;
	}
 
	printf("\n\nCost of the spanning tree=%d",cost);
}

void getNodeandCheck(){
	
	printf("\n\nEnter the weight of the node: ");
	scanf("%d",&(elist.data[elist.n].w));
	printf("\nEnter the first node: ");
	scanf("%d",&(elist.data[elist.n].u));
	printf("\nEnter the second node: ");
	scanf("%d",&(elist.data[elist.n].v));
	elist.n++;
	
	graphCheck();
}


