#include <stdio.h>
//#include "sachetIO.h"
#include "sachetTest.h"


int main()
{	
	char SendThese[38]="This should get broken into sachets";
	char* data;
	int i,len;
	len=sachetRecieve(&data,boom,100);
	for(i=0;i<sachetRecieve;i++)
		printf("%c",data[i]);
	printf("\nThat was decoded from sachet data\n");
	i=sachetSend(SendThese,36,bam);
	//sendData(SendThese,30);
	//printf("This is a test for sachetIO transmission\n");
	
	getchar();
	return 0;
}

