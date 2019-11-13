#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
int i=0;
	FILE *ptr,*qtr;
	char x[1001];
	ptr=fopen("image.jpeg","r");
	qtr=fopen("test.jpeg","w");
	while(fgets(x,1000,ptr)!=NULL){
		for(i=0;i<strlen(x);i++)
		{
			if(x[i]=='\n'){x[i]='\0';break;}
		}
		fprintf(qtr,"%s",x);
	}
	return 0;
}
