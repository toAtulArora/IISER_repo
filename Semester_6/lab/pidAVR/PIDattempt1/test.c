#include <avr/io.h>
#include "lcd.h"

char lcdBuffer[32];

int main()
{
	lcd_init();
	lcd_clear();
	lcd_string("Testing");
	while(1);
	
}