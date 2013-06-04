/*	
	filename: temperature
	description: This is the firmware of the hardware that simulates temperature on the dipole lattice	
	
	baby steps(TM):
		1. Communication Test with the 
*/

#include "usbIO.h"

int main(void)
{
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
			pucOutBuf[0] = ucInDataLen;
			for ( i = 0; i < ucInDataLen; i++ )
				pucOutBuf[i+1] = pucInData[i] + 1;
			vSendUSBData( ucInDataLen+1 );
		}
	}
	return 0;
}