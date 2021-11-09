#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>

//structures
struct people
{
    char name[50];
    char role[10];
    long UID;
    char password[50];
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
                printf("Auth successful\n");
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
    if(!strcmp(p.role,"Admin"))
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
    printf("Registered with UID %ld  \n",p.UID);
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
    fwrite(&p, sizeof(p), 1, fptr);
    fclose(fptr);
    printf("Registered with UID %ld \n",p.UID);
}

