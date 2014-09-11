#include <iostream> //Well..
#include <conio.h> //For getch()
#include <vector> //For arrays
#include <complex> //For complex numbers
#include <math.h> //For pow etc.
#include <random> //For supporting random numbers
using namespace std;

class QC
{
	int qBits;
	vector <complex <float> > amplitudes;
	string statusPrefix()
	{
		return "QC [" + to_string(msgCount++) + "]: ";
	}
public:
	//Does something like this
	//num=0x110100
	//tBit=0
	//value=1
	//returned=0x1101001
	int insertBit(int num, int tBit, int value)
	{
		int numLHS=num/(int)pow(2,tBit);
		int numRHS=num%(int)pow(2,tBit);
		return 
	}
	int getBit(int num,int tBit)
	{
		return num/(int)pow(2,tBit) - 2*(num/(int)pow(2,tBit+1));
	}
	int setBit(int num, int tBit, int value)
	{
		return 0;
	}
public:
	string status;
	int msgCount;
	string log;
	QC(int init_qBits)
		:log(""),status(""),msgCount(0)
	{
		qBits=init_qBits;
		amplitudes.resize((int)pow(2,qBits));
		amplitudes[0]=(1,0); //Set all qbits to zero
		status=statusPrefix()+"Initialized to simulate " + to_string(qBits) + " qBits. " + to_string((int)pow(2,qBits)) + " complex numbers reserved.";
		log+=status+"\n";
	}
	int init_qBit(int qBit,int value)
	{


	}
	//QC() : QC(10) {};
};


void main()
{
	QC qc(8);
	cout<<qc.status<<endl;

	//cout<<"This works";
	_getch();

}