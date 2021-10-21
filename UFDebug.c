#include <stdio.h>
#include "inventoryFunctions.h"


void main()
{
 char str[50];
 strcpy(str,get_item_name("ITM10000004"));
 printf("%s",str);
}

