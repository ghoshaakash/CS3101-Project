#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/*
takes a string and a destination string, converts the string to its lowercase equivalent and copies it to the destination string
*/
void lowercase(char upper[50],char* chr)
{
	int i=0;    //for the loop
 	char l;

   
    while (upper[i])   //THE LOOP TO CONVERT EACH LETTER OF upper to lowercase
		{
			if ((upper[i]>='a' && upper[i]<='z') || (upper[i]>='A' && upper[i]<='Z'))
				{
					l=tolower(upper[i]);
					strncat(chr, &l, 1);//appends l to chr
					i++; 
					
				
    
				}
			else
				{
				l=upper[i];
				strncat(chr, &l, 1);
				i++;
				} 
		}
		
} 

