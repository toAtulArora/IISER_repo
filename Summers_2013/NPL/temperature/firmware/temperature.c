/*	
	filename: temperature
	description: This is the firmware of the hardware that simulates temperature on the dipole lattice	
	
	baby steps(TM):
		1. Bare Minimum Tests: Communication Test with the lattice analyser	[done]
		2. Proof of Concept:
			The target of this would be to keep a dipole continously rotating
			a. Test the kind of currents and voltages required to fire up the magnet
			b. 
	
	communication protocol [clear text]
		fixed length, 10 characters (8 bit each)
		
		xxxxx xxxxx
		\---/ \---/
		  |     |
		  |     +------- Field Intensity
		  +--------------Dipole ID
		for
	Binary Protocol for testing
		fixed length, 10 charactes, 8 it each
		01234 56789
		xxxxx xxxxx
		||||  \---/
		||||    |
		||\|    +---------Undefined for now
		\| |-------------Dipole Intensity (65536 max)
		 |---------------Dipole ID (65536 max)

*/

#include "usbIO.h"

#define DEBUG_STAGE_ZERO


#ifndef DEBUG_STAGE_ZERO
	// U16Bit acknowledge[10]="Ok";
	U16Bit*	dipoleID;
	U16Bit* intensity;
	U8Bit	len;
	U8Bit*	data=0;
#endif

int main(void)
{
	#ifdef DEBUG_STAGE_ZERO
		U8Bit   i;
		U8Bit   ucInDataLen;
		U8Bit   *pucInData = 0;

		usbInit();

		// ---------------- enforce re-enumeration.
		usbDeviceDisconnect();  // enforce re-enumeration, do this while interrupts are disabled!
		i = 255;
		while(--i)					// fake USB disconnect for > 250 ms
			_delay_ms(1);
		usbDeviceConnect();

		sei();
		for(;;)
		{
			usbPoll();
			ucInDataLen = ucGetUSBData( &pucInData );
			if ( ucInDataLen > INBUFFER_LEN )
				ucInDataLen = INBUFFER_LEN;
			if ( ucInDataLen )
			{						// copy indata and send back ascii values incremented by 1
									// also, make the first value, the length of data recieved
				pucOutBuf[0] = ucInDataLen;
				for ( i = 0; i < ucInDataLen; i++ )
					pucOutBuf[i+1] = pucInData[i] + 1;
				vSendUSBData( ucInDataLen+1 );				
			}
		}
		return 0;
	#else
		usbInit();								//Initialize USB		
		usbDeviceDisconnect();					//Disconnect and re-connect
		_delay_ms(255);
		usbDeviceConnect();

		sei();									//Enable Interrupts
		for(;;)
		{
			usbPoll();								//This has to be called frequently enough
			len=ucGetUSBData(&data);				//Get data
			len=(len>INBUFFER_LEN)?INBUFFER_LEN;	//truncate if overflew
			if(len)									//Got data?
			{				
				dipoleID=(U16Bit*)&data[0];			//point the dipoleID to its location in the data 
													//(according to the binary protocol)
				intensity=(U16Bit*)&data[2];		//point the intensity pointer to its location

				if(*dipoleID==0)					//if the id is for the zeroth dipole (testing)
				{
					pucOutBuf[0]=intensity;					
					vSendUSBData(1);				//Acknowledge receiving data

					//TODO: ADD PORT CODE			//Fire up the magnet for a time proportional to the intensity
					_delay_us((U8Bit) intensity/2 );
					_delay_us((U8Bit) intensity/2 );
					//TODO: ADD CODE TO TURN OFF					
				}
				else
				{
					pucOutBuf[0]=':';
					pucOutBuf[1]='(';
					vSendUSBData(2);				//Acknowledge receiving data

				}
			}
		}
		return 0;
	#endif


}