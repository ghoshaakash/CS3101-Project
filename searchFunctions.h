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


int checksubstring(char* str1,char* str2)//str 1 is parent, str2 is substring. Return 0 if not present, 1 if present
{
int len1 = strlen(str1);
int len2= strlen(str2);
if(len1<len2)
	return 0;

int i,j;
int flag;
for(i=0;i<=len1-len2;i++)//this loop checks if the substring is in the string
{
	flag=0;
	for(j=0;j<len2;j++)
	{
		if(str1[i+j]==str2[j])
		{
			flag+=1;
		}
	}
	if(flag==len2)

	{
		return 1;
	}
}

return 0;
}


	
