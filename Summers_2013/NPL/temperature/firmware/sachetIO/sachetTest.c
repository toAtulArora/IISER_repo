#include <stdio.h>
//#include "sachetIO.h"
#include "sachetTest.h"


int main()
{	
	char SendThese[38]="This should get broken into sachets";

	int i=sachetSend(SendThese,36,bam);
	//sendData(SendThese,30);
	//printf("This is a test for sachetIO transmission\n");
	
	getchar();
	return 0;
}

