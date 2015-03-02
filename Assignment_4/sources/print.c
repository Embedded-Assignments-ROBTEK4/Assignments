#include "../headers/print.h"

int vprintf_(void (*destfunc)(unsigned char *), unsigned char *string, ...)
{	//subset of vprintf. Only implents type.
	static unsigned char outstring[MAX_LINE_LENGHT]; //Max at 4K. To bad if not enough.
	INT16U outstring_index = 0;
	INT16U instring_index = 0;
	va_list args;
	va_start(args, string);
	
	while(string[instring_index]) //loop through whole string
	{
		if( string[instring_index] == '%' ) //If we reach an %, switch on next char
		{
			switch(string[instring_index+1])
			{
				case 'd': //32 bit int, signed
				case 'i':
						
					break;
				case 'u': //32 bit int, unsigned
				
					break;
				
				case 'f': //64 bit double, fixed point notation
					break;
				
				case 'e': //64 bit double, fixed point notation
				
					break;
				
				case 'x': //32 bit unsigned int, hexidecimal
				
					break;
				
				case 'o': //32 bit unsigned int, octal
				
					break;
				
				case 's': //null-terminated string
				
					break;
				
				case 'c': //char
				
					break;
				
				
				case '%': //print a %
				
					break;
				
				default:
					break;
			}
		}
		else
		{
			outstring[outstring_index++] = string[instring_index];
			if(outstring_index == MAX_LINE_LENGHT) break;
		}
		
		instring_index++;
	}
	
	outstring[outstring_index] = 0;
	destfunc(outstring);
	
	return 0;
}
