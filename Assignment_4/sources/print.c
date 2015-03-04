#include "../headers/print.h"

int vprintf_(void (*destfunc)(char *), char *string, ...)
{	//subset of vprintf. Only implents integers. Not threadsafe
	
	static char outstring[MAX_LINE_LENGHT]; //Max at this. To bad if not enough.
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
				case 'd': //int, signed
				case 'i':
					instring_index++;
					outstring_index += itoa(va_arg(args, int), outstring+outstring_index, 10);
					break;
					
				case 'u': //int, unsigned
					instring_index++;
					outstring_index += itoa(va_arg(args, unsigned int), outstring+outstring_index, 10);
					break;
				
				/*case 'f': //double.
						instring_index++;
						//FP64 number_float = va_arg(args, double);
					break;*/ //Not done
				
				case 'x': // unsigned int, hexidecimal
					instring_index++;
					outstring_index += itoa(va_arg(args, unsigned int), outstring+outstring_index, 16);
					break;
				
				case 'o': //unsigned int, octal
					instring_index++;
					outstring_index += itoa(va_arg(args, int), outstring+outstring_index, 8);
					break;
					
				case 'b': //unsigned int, binary
					instring_index++;
					outstring_index += itoa(va_arg(args, unsigned int), outstring+outstring_index, 2);
					break;
					
				case 's': //null-terminated string
					instring_index++;
					char *thestring = va_arg(args, char *);
					for(INT16U index = 0; thestring[index]; index++)
					{
						outstring[outstring_index++] = thestring[index];
					}
					break;
				
				case 'c': //char
					instring_index++;
					outstring[outstring_index++] = va_arg(args, int);
					break;
				
				case '%': //print a %
					instring_index++;
					outstring[outstring_index++] = '%';
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

INT8U itoa(INT64S number, char * string, INT8U base)
{ //converts the number to a string. returns lenght of string(without null)
	char tmpstring[65]; //max to hold 64 bit base 2, with
	INT8U index = 65;
	INT64S tmp_number = number;
	if (number < 0) number = -number;
	if(!number) tmpstring[index--] = '0';
	
	while(number)
	{
		INT8U tmp = number % base;
		if(tmp < 10)
			tmpstring[index--] = tmp +'0';
		else
			tmpstring[index--] = tmp-10 +'a';
		number /= base;
	}
	if(tmp_number < 0)
		tmpstring[index] = '-';
	else
		index++;
	
	//copy the generated string into the argument buffer.
	INT8U i = 0;
	while(index <= 65)
	{
		string[i++]=tmpstring[index++];
	}
	string[i] = 0;
	
	return i;
}
