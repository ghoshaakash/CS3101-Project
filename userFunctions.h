#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>

//structures
struct people
{
    char name[50];
    char role[10];
    long UID;
    char password[50];
    char add[5][100];
};


//fucntions

int AuthUser(long int UID)//searches for uid and checks password. returns 1 if authorized admin 2 if authorised user 0 otherwise 
{
    struct people i;
    FILE *fptr;
    if ((fptr = fopen("people","rb")) == NULL)
    {
    printf("Error! opening file\n");
    return(0);
    }
    while (fread(&i, sizeof(i), 1, fptr)==1)
    {
        if(i.UID==UID)
        {
            char password[50]={'\0'};
            printf("Enter User Password: ");
            scanf(" %[^\n]",password);
            if(strcmp(password, i.password)==0)
            {
                printf("Hello %s .Your Authentication is successful. Press any key to continue.\n",i.name);
                getch();

                if(strcmp(i.role,"Admin")==0)
                {
                    return(1);
                }
                else
                {
                    return(2);
                }
            }
            else
            {
                printf("Password incorrect\n");
                return(0);
            }

        }
    }
    printf("UID not found \n");
    return(0);    
}



int CreateUser()//creates a user. returns 1 if successful 0 otherwise
{   //calculating max number of user
    long x=0;
    struct people p={.name={'\0'}, .role={'\0'},.UID=0,.password={'\0'}};
    FILE *fptr;
    fptr=fopen("people","ab");
    fseek(fptr, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(fptr);
    if(len==0)
    {
        p.UID=10000;
    }
    else
    {
        p.UID=ftell(fptr)/sizeof(p)+10000;
    }
    printf("Enter User Name : ");

    scanf(" %[^\n]",p.name);
    printf("Enter User Password: ");
    scanf(" %[^\n]",p.password);
    printf("Enter User Role: ");
    char str[50]={'\0'};
    scanf(" %s",p.role);
    if((!strcmp(p.role,"Admin"))||(!strcmp(p.role,"admin")))
    {
        long int UID;
        printf("Existing Admin credentials needed for Adding Admins \n");
        printf("Enter Admin UID : ");
        scanf(" %ld", &UID);
        if(AuthUser(UID)==1)
        {
            strcpy(p.role,"Admin");
        }
        else
        {
            printf("Wrong Credentials\n");
            return 0;
        }
    }
    strcpy(p.role,"User");
    fwrite(&p, sizeof(p), 1, fptr);
    fclose(fptr);
    printf("Hello %s. You have been succesfully registered with UID %ld.\n",p.name,p.UID);
    return 1;
}


void Initialize()//To inititalzie first Admin
{
    long x=0;
    struct people p={.name={'\0'}, .role={'\0'},.UID=0,.password={'\0'}};
    FILE *fptr;
    fptr=fopen("people","ab");
    fseek(fptr, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(fptr);
    if(len==0)
    {
        p.UID=10000;
    }
    else
    {
        p.UID=ftell(fptr)/sizeof(p)+10000;
    }
    printf("Enter User Name : ");
    scanf(" %[^\n]",p.name);
    printf("Enter User Password: ");
    scanf(" %[^\n]",p.password);
    strcpy(p.role,"Admin");
    for(int i=0;i<5;i++)
    {
        p.add[i][0]='\0';
    }
    fwrite(&p, sizeof(p), 1, fptr);
    fclose(fptr);
    printf("Hello %s. You have been succesfully registered with UID %ld.\nPress any key to continue.",p.name,p.UID);
}


int addAddress(long UID)// adds address entry to user record
{
    int flag=0;
    struct people t;
	FILE * file1= fopen("people", "rb");
	FILE * file2= fopen("copy", "wb");
	if(file1 != NULL && file2 != NULL) 
	{
    while(fread(&t, sizeof(struct people), 1, file1))
    {
    	if(t.UID==UID)
    	{
            flag=1;
            int top=0;
            for(;top<=4;top++)
            {
                if(t.add[top][0]=='\0')
                {
                    break;
                }
            }
            if(top==4)
            {
                printf("You have used all 5 address slots. Please deltele some entries and try again\n");
            }
            else
            {
                printf("Enter new Address :\n");
                scanf(" %[^\n]",t.add[top]);
            }


    		fwrite(&t, sizeof(struct people), 1, file2);
		}
		else
		{
			fwrite(&t, sizeof(struct people), 1, file2);
		}
	}
    
    fclose(file1);
    fclose(file2);
    
    remove("people");
    rename("copy","people");
	}	
    if(flag==0)
    {
        printf("UID not found\n");
    }
    return flag;
}

int deleteAddress(long UID)//delete address record
{
    int flag=0;
    struct people t;
	FILE * file1= fopen("people", "rb");
	FILE * file2= fopen("copy", "wb");
	if(file1 != NULL && file2 != NULL) 
	{
    while(fread(&t, sizeof(struct people), 1, file1))
    {
    	if(t.UID==UID)
    	{
            flag=1;
            int top=0;
            for(;top<=4;top++)
            {
                if(t.add[top][0]=='\0')
                {
                    break;
                }
            }
            if(top==0)
            {
                printf("Your address list is empty\n");
            }
            else
            {
                for(int i=0;i<=top-1;i++)
                {
                    printf("%d. %s\n",i+1,t.add[i]);
                }
                int i;
                printf("Enter address number you wish to delete :");
                scanf(" %d",&i);
                while(!(i<=top))
                {
                    printf("Bad input. Try again :");
                    scanf(" %d",&i);
                }
                for(int j=i-1;j<=top-1;j++)
                {
                    strcpy(t.add[j],t.add[j+1]);
                }
                t.add[top][0]='\0';
            }


    		fwrite(&t, sizeof(struct people), 1, file2);
		}
		else
		{
			fwrite(&t, sizeof(struct people), 1, file2);
		}
	}
    
    fclose(file1);
    fclose(file2);
    
    remove("people");
    rename("copy","people");
	}	
    if(flag==0)
    {
        printf("UID not found\n");
    }
    return flag;
}


void getaddress(long UID,char ADD[100])//returns addrress
{
    struct people t;
	FILE * file1= fopen("people", "rb");
	if(file1 != NULL) 
	{
        while(fread(&t, sizeof(struct people), 1, file1))
        {
    	    if(t.UID==UID)
    	    {
                fclose(file1);
                int top=0;
                for(;top<=4;top++)
                {
                    if(t.add[top][0]=='\0')
                    {
                        break;
                    }
                    printf("%d. %s",top+1,t.add[top]);
                }   
                if(top==0)
                {
                    printf("Your address list is empty\n" );
                    return(0);
                }
                printf("Enter address choice: ");
                int i;
                scanf(" %d",&i);
                while (i>top)
                {
                    printf("Bad input. Try again :");
                    scanf(" %d",&i);
                }
                strcpy(ADD,t.add[i-1]);
                fclose(file1);
                return 1;
		    }
	    }   
	}	
    fclose(file1);
    printf("UID not found\n");
    return 0;
}





void listAll()
{
    struct people p;
    FILE *fptr;
    if ((fptr = fopen("people","rb")) == NULL)
    {
    printf("Error! opening file\n");
    return;
    }
    while (fread(&p, sizeof(p), 1, fptr)==1)
    {
        printf("%ld - %s - %s - %s -%s - %s\n",p.UID,p.add[0],p.add[1],p.add[2],p.add[3],p.add[4]);
    }
    fclose(fptr);
}