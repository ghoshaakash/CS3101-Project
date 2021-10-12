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
void initialize_item(struct item *t)
{
	printf("Enter item name :\n");
	scanf("%s",t->name);
	printf("Enter item UID :\n");
	scanf("%s",t->UID);
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



int CreateUser()
{   //calculating max number of user
    long x=0;
    struct people p;
    FILE *fptr;
    if ((fptr = fopen("people.bin","ab+")) == NULL)
    {
    printf("Error! opening file");
    return(0);
    }
    if(fseek(fptr, 0, SEEK_END)!=0)//checking if seeking to end is possible
    {
        fclose(fptr);
        return(0);
    }
    if(ftell(fptr)<sizeof(people))//checking if there is atleast 1 entry
    {
        p.UID=1000000000; 
    }
    else
    {
    if (fseek(fptr, -sizeof(people), SEEK_CUR) != 0)
    {
        fclose(fptr);
        return(0);
    }
    struct people lastAppln;
    fread(&lastAppln, sizeof(people), 1, fptr);
    p.UID=lastAppln.UID+1;
    }
    printf("Enter User Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    printf("Enter User Password: ");
    fgets(p.password, sizeof(p.password), stdin);
    strcpy(p.role,"User");
    fwrite(&p, sizeof(people), 1, fptr);
    puts("User creation succesful. UID is:%lu",p.UID);
    fclose(fptr);
    return(1);
}
