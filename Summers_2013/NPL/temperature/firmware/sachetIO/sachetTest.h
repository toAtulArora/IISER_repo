
int bam(char* data, int length);

int bam(char* data,int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		printf("%c",data[i]);
	}
	printf("\n\nI was called");
	return 10;
}
