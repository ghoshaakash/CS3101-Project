#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include<stdlib.h>
#include "searchFunctions.h"

//structures
/*struct people
{
    char name[50];
    char role[6];
    long UID;
    char password[50];
};*/

struct item{
	char name[500];
	char UID[20];
	int quantity;
	float price;
};

struct order
{
    long OrderID;
    char UID;
    int delivery_type;
    int date;
    int slot;
};


//fucntions

/*
prints the entire the inventory on the screen
returns : None
*/
void view_items()
{
	struct item t;
	int i=0;
	//printf("--------------------------------------------------- \n");
	printf("%-14s%-50s%-15s%-10s%-2s\n\n","SERIAL No.","NAME","UID","QUANTITY","PRICE (Rs)");
	FILE * file= fopen("inventory", "rb");
	if(file != NULL) 
	{
    		while(fread(&t, sizeof(struct item), 1, file))
    		{
    			printf("%-14d%-50s%-15s%-10d%-17.2f\n",++i,t.name,t.UID,t.quantity,t.price);
		}
    
    		fclose(file);
	}	
	//printf("--------------------------------------------------- \n\n");
	
}

void add_UID(struct item *t)
{
	struct item t1;
	FILE * file= fopen("inventory", "rb");
	if (file != NULL) 
	{
    
    	fseek(file, 0, SEEK_END);
    	fseek(file,-sizeof(struct item),SEEK_CUR);
    	fread(&t1, sizeof(struct item), 1, file);
    		
    	char uid[30];
    		
    	sprintf(uid,"ITM%d",atoi((t1.UID)+3)+1);
    	strcpy(t->UID,uid);
	}
	else
	{
			
			strcpy(t->UID,"ITM10000001");
	}
	fclose(file);
}	
	
void initialize_item(struct item *t)
{
	printf("Enter item name :\n");
	gets(t->name);
	add_UID(t);
	//printf("Enter item UID :\n");
	//scanf("%s",t->UID);
	printf("Enter item quantity :\n");
	scanf("%d",&(t->quantity));
	printf("Enter item price (in rupees) :\n");
	scanf("%f",&(t->price));
	printf("\n\n");
}


/*
takes in the uid of an item and the reference to a string and copies the name of that item to the string. 
returns 1 if item present else returns 0.
*/
int get_item_name(char* uid,char* name)
{	
	struct item t;
	FILE *file= fopen("inventory", "rb");
	if(file != NULL)
	{
		while(fread(&t, sizeof(struct item), 1, file))
		{
			if(strcmp(t.UID,uid)==0)
    		{
    		fclose(file);
    		strcpy(name,t.name);
    		return 1;
			}
		}
		fclose(file);
	}
	return 0;
	
}

/*
Adds a new item to the inventory 
returns : None
*/
void add_item()
{
	struct item t;
	initialize_item(&t);
	
	FILE * file= fopen("inventory", "ab");
	if (file != NULL) 
	{
    fwrite(&t, sizeof(struct item), 1, file);
    fclose(file);
	}	
	
}

/*
takes the UID of the item and returns the quantity of the item present in stock
*/
int get_quantity(char *uid)
{
	struct item t;
	FILE *file= fopen("inventory", "rb");
	if(file != NULL)
	{
		while(fread(&t, sizeof(struct item), 1, file))
		{
			if(strcmp(t.UID,uid)==0)
    		{
    		fclose(file);
    		return t.quantity;
			}
		}
		fclose(file);
	}
	return -1;
}


/*
takes the UID of the item and its quantity and updates stock with this quantity 
*/
void edit_item_quantity(char *uid,int quantity)
{
	struct item t;
	
	FILE * file1= fopen("inventory", "rb");
	FILE * file2= fopen("inventory_tmp", "wb");
	if(file1 != NULL && file2 != NULL) 
	{
    while(fread(&t, sizeof(struct item), 1, file1))
    {
    	if(strcmp(t.UID,uid)!=0)
    	{
    		fwrite(&t, sizeof(struct item), 1, file2);
		}
		else
		{
			t.quantity=quantity;
			fwrite(&t, sizeof(struct item), 1, file2);
		}
	}
    
    fclose(file1);
    fclose(file2);
    
    remove("inventory");
    rename("inventory_tmp","inventory");
	}	
	
}

/*
Takes the item UID and deletes it from the stock 
*/
void delete_item(char *uid)
{
	struct item t;
	
	FILE * file1= fopen("inventory", "rb");
	FILE * file2= fopen("inventory_tmp", "wb");
	if(file1 != NULL && file2 != NULL) 
	{
    while(fread(&t, sizeof(struct item), 1, file1))
    {
    	if(strcmp(t.UID,uid)!=0)
    	{
    		fwrite(&t, sizeof(struct item), 1, file2);
		}
	}
    
    fclose(file1);
    fclose(file2);
    
    remove("inventory");
    rename("inventory_tmp","inventory");
	}	
}

/*
Takes the UID of an item and returns 1 if the item is present in the inventory, else returns 0
*/
int search_item(char *uid)
{
	struct item t;
	FILE *file= fopen("inventory", "rb");
	if(file != NULL)
	{
		while(fread(&t, sizeof(struct item), 1, file))
		{
			if(strcmp(t.UID,uid)==0)
    		{
    		fclose(file);
    		return 1;
			}
		}
		fclose(file);
		return 0;
	}
	return -1;
}

/*gives an order of item(uid) from inventory and updates the quantity in the inventory after the order
returns 1 for successful order
returns -1 if the quantity ordered is more than available stock*/
int give_order(char *uid,int quantity)
{
	int Quantity=0;
	if(search_item(uid)==1)
	{
		Quantity=get_quantity(uid);
	}
	
	if(Quantity<quantity)
	{
		return -1;
	}
	else
	{
		edit_item_quantity(uid,Quantity-quantity);
		return 1;
	}
}

/*
takes in the uid of an item and returns its price
returns -1 if item not in inventory
*/
float get_price(char* uid)
{
	struct item t;
	FILE *file= fopen("inventory", "rb");
	if(file != NULL)
	{
		while(fread(&t, sizeof(struct item), 1, file))
		{
			if(strcmp(t.UID,uid)==0)
    		{
    		fclose(file);
    		return t.price;
			}
		}
		fclose(file);
	}
	return -1;
	
}


/*
takes in an array of uid of items and prints their details as present in the inventory
*/
void view_items_by_uid(char (*list)[20],int size)
{
	printf("%-14s%-50s%-15s%-10s%-2s\n\n","SERIAL No.","NAME","UID","QUANTITY","PRICE (Rs)");
	
	int i;
	int j=0;

	struct item t;
	
	FILE * file= fopen("inventory", "rb");
	if(file != NULL) 
	{
    		while(fread(&t, sizeof(struct item), 1, file))
    		{
    			for(i=0;i<size;i++)
    			{
    				if(strcmp(t.UID,list[i])==0)
    					printf("%-14d%-50s%-15s%-10d%-17.2f\n",++j,t.name,t.UID,t.quantity,t.price);
				}
    			
		    }
    
    		fclose(file);
	}	
}

int search_function(char* super, char* sub)
{
	int flag=1;
	char *token=NULL;
   
  	/* get the first token */
   	token = strtok(sub," ");
   
  	/* walk through other tokens */
	while( token != NULL )
	{
      			
      	if(checksubstring(super,token)==0)
      	{
      	flag=0;
		}
    
   		token = strtok(NULL," ");
	}	
	if(flag==1)
	return 1;
	return 0;
}


/*
takes in a string and prints only those items on the screen whose name have the string in common
*/
void view_items_by_string(char *string)
{
	char new_string[100]="\0";
	printf("%-14s%-50s%-15s%-10s%-2s\n\n","SERIAL No.","NAME","UID","QUANTITY","PRICE (Rs)");
	
	int i;
	int j=0;
	char item_name[100]="\0";
	struct item t;
	FILE * file= fopen("inventory", "rb");
	if(file != NULL) 
	{
    	while(fread(&t, sizeof(struct item), 1, file))
    	{
    		strcpy(new_string,string);
    		strcpy(item_name,t.name);
    		if(search_function(item_name,new_string)==1)
			{printf("%-14d%-50s%-15s%-10d%-17.2f\n",++j,t.name,t.UID,t.quantity,t.price);}
   			
		}
    	
    	if(j==0)
    	{
    		printf("\n\n\n\n\n-----------------------------Sorry, No items matched the search------------------------------------\n\n\n\n\n");
		}
    	
    	fclose(file);
	}	
}
