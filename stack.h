#include<stdio.h>
#include"inventoryFunctions.h"
#include"OrderFunctions.h"
#include<String.h>




/*
 * main.c
 *
 *  Created on: 23-Oct-2021
 *      Author: hp
 */

/*
Creates a string stack and an int stack ; takes a string array,an integer array, pointers to their respective 'top' variables, and a char and int variable as input.
Pushes the char and int variables to the respective stacks. Increments top. The string stack stores UIDs of items to be purchased, and the int stack stores their respective quantities (taken as input from user)
*/
void push(char item_arr[50][20], int quantity_arr[50], int* p,int* q,char UID[20],int quantity)
{
	if(*p==50-1)
	{
		printf("Cart is full!\n");
		return;
	}
	*p+=1;
	*q+=1;
	strcpy(item_arr[*p],UID);
	quantity_arr[*q]=quantity;
}

/*
Removes the top element from a string stack and copies the element to a char variable and returns the char variable;
takes a string array and pointer to its top variable, and a char variable as input
*/
char* pop_string(char item_arr[50][20],int* p, char top_string[20])
{
	if(*p==-1)
	{
		return 0;
	}

	strcpy(top_string,item_arr[*p]);
	*p-=1;
	return top_string;
}

/*
Removes the top element of an int stack and returns the element. Takes an int array and pointer to its top variable as input
*/
int pop_int(int quantity_arr[50], int* q) //
{
	int quantity;

	if(*q==-1)
	{
		return 0;
	}
	else
	{
	quantity=quantity_arr[*q];
	*q-=1;
	return quantity;
	}
}

/*
Displays contents of cart along with quantity.
Takes a string array and int array, and the 'top' variable (top itself, and not pointer to top) of the string array as input.
*/
void display(char item_arr[50][20], int quantity_arr[50], int top)
{
	int i;

	if(top==-1)
	{
		printf("Cart empty!\n");
	}
	for(i=top;i>-1;i--)
	{
		char name[20];
		get_item_name(item_arr[i],name);
		printf("UID: %s \t Item name: %s \t Quantity: %d\n",item_arr[i],name,quantity_arr[i]);
	}
}

/*
Takes user input (char UID and int quantity) and adds to respective stacks. Also edits item quantity in inventory.
Takes string array and int array and pointers to their respective top variables as input.
*/
void add_to_cart(char item_arr[50][20], int quantity_arr[50], int* p, int* q)
{
	char UID[20];
	printf("Enter UID of the desired item:\n");
	scanf("%s",UID);
	if(!search_item(UID))
	{
		printf("Invalid UID! Please try again.\n");
		return;
	}
	int quantity;
	printf("Enter the desired quantity:\n");
	scanf("%d",&quantity);

	if(give_order(UID,quantity)==-1)
		printf("The required quantity is not in stock.\n");
	else
		push(item_arr,quantity_arr,p,q,UID,quantity);
}

/*
Takes user input and removes the desired number of an item at an arbitrary position in the stack.
Takes string array, int array and pointers to their respective top variables, and a char variable (needed for the pop_string function) as input.
*/
void remove_from_cart(char item_arr[50][20], int quantity_arr[50],int* p, int* q, char top_string[20])
{
	char UID[20];
	printf("Enter UID of item to be removed:\n");
	scanf("%s",UID);

	int quantity;
	printf("Enter new quantity (enter 0 if item is to be removed):\n");
	scanf("%d",&quantity);

	int key,i;
	for(i=*p;i>-1;i--)
		{
			if(!(strcmp(UID,item_arr[i])))
			{
				key=i;
				break;
			}
		}

	if(quantity)
	{
		int temp;
		temp=quantity_arr[key];
		quantity_arr[key]=quantity;
		give_order(UID,-(temp-quantity));
	}
	else
	{
		char temp_1[20];
		strcpy(temp_1,item_arr[*p]);
		strcpy(item_arr[*p],item_arr[key]);
		strcpy(item_arr[key],temp_1);

		int temp_2;
		temp_2=quantity_arr[*q];
		quantity_arr[*q]=quantity_arr[key];
		quantity_arr[key]=temp_2;

		give_order(UID,-quantity_arr[*q]);
		pop_string(item_arr,p,top_string);
		pop_int(quantity_arr,q);
	}
}

/*
Prints bill and resets both top variables to -1. Takes string array, int array and pointers to their respective top variables, and a char variable (needed for the pop_string function) as input.
*/
void print_bill(long PeopleUID,char item_arr[50][20], int quantity_arr[50], int* p,int* q, char top_string[20])
{
	float total=0,amount;
	printf("%-50s %-8s%-12s%-8s\n","ITEM","RATE","QUANTITY","AMOUNT");
	printf("================================================================================\n");
	while(*p!=-1)
	{
		char name[50];
		get_item_name(pop_string(item_arr,p,top_string),name);
		char UID[20];
		*p+=1;
		strcpy(UID,pop_string(item_arr,p,top_string));
		int quantity;
		quantity=pop_int(quantity_arr,q);

		amount=get_price(UID)*quantity;
		total+=amount;
		printf("%-50s%-6.2f    %-11d%-6.2f\n",name,get_price(UID),quantity,amount);
	}
	printf("================================================================================\n");
	printf("TOTAL = Rs. %.2f/-\n",total);
	int OrderID=AssignSlots(PeopleUID,total);
	
}


int Cart(long PeopleUID)
{
	char item_arr[50][20], top_string[20];
	int quantity_arr[50];
	int top_1=-1,top_2=-1, *p, *q;
	p=&top_1;
	q=&top_2;
	int a,b,i;
	char c,d, s[100];
	int key;

	while(1)
	{
		printf("Choose operation:\n");
		printf("0 to go to the previous set of commands \n1 to add an item to the cart \n2 to view cart \n3 to remove an item \n4 to proceed to checkout\n5 to search items by string.\n");
		printf("===========================================\n");
		scanf("%d",&key);

	switch(key)
		{
			case 0:	exit(0);
					break;
			case 1: add_to_cart(item_arr,quantity_arr,p,q);
					break;
			case 2: display(item_arr,quantity_arr,*p);
					break;
			case 3: remove_from_cart(item_arr,quantity_arr,p,q, top_string);
					break;
			case 4: print_bill(PeopleUID,item_arr,quantity_arr,p,q, top_string);
					exit(0);
					break;
			case 5: 
					{
						printf("%s","Enter search phrase :\n");
						char searchPhrase[100]={'\0'};
						scanf("%s",searchPhrase);
						a=strlen(searchPhrase);
						for(i=0;i<=a;i++)
						{
							if(searchPhrase[i]>='a' && searchPhrase[i]<='z')
							{
								searchPhrase[i]=searchPhrase[i]-32;
							}	
						}
						view_items_by_string(searchPhrase);
						break;
					}
			default: printf("Invalid operation!\n");
		}
	}

	return 0;
}

