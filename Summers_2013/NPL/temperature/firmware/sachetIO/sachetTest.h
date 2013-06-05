
int bam(char* data, int length);

int bam(char* data,int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		printf("%c",data[i]);
	}
	//printf("\n\nI was called");
	return 10;
}
int boom(char** data);

int boom(char** data)
{
	static char testString[30]="ART12345678901234567890SOP";
	static int start=0;
	*data=testString+start;
	start+=10;
	return 10;
}
