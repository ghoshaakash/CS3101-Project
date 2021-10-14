#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>

//structures
struct people
{
    char name[50];
    char role[6];
    long UID;
    char password[50];
};

struct item{
	char name[50];
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
void view_items()
{
	struct item t;
	int i=0;
	//printf("--------------------------------------------------- \n");
	printf("%-14s%-32s%-15s%-10s%-2s\n\n","SERIAL No.","NAME","UID","QUANTITY","PRICE");
	FILE * file= fopen("inventory", "rb");
	if(file != NULL) 
	{
    		while(fread(&t, sizeof(struct item), 1, file))
    		{
    			printf("%-14d%-32s%-15s%-10d%-17.2f\n",++i,t.name,t.UID,t.quantity,t.price);
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

void view_items()
{
	struct item t;
	
	printf("--------------------------------------------------- \n");
	FILE * file= fopen("inventory", "rb");
	if(file != NULL) 
	{
    while(fread(&t, sizeof(struct item), 1, file))
    {
    	printf("item name : %s\nitem UID : %s\nquantity : %d\nprice : Rs %.2f\n\n",t.name,t.UID,t.quantity,t.price);
	}
    
    fclose(file);
	}	
	printf("--------------------------------------------------- \n\n");
	
}

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
