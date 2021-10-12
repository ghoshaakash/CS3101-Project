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


//fucntions



int CreateUser()//creates a user. returns 1 if successful 0 otherwise
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
    if(ftell(fptr)<sizeof(p))//checking if there is atleast 1 entry
    {
        p.UID=1000000000; 
    }
    else
    {
    if (fseek(fptr, -sizeof(p), SEEK_CUR) != 0)
    {
        fclose(fptr);
        return(0);
    }
    struct people lastAppln;
    fread(&lastAppln, sizeof(p), 1, fptr);
    p.UID=lastAppln.UID+1;
    }
    printf("Enter User Name: ");
    fgets(p.name, sizeof(p.name), stdin);
    printf("Enter User Password: ");
    fgets(p.password, sizeof(p.password), stdin);
    printf("Enter Role: ");
    char role[50]={'\0'};
    fgets(role, sizeof(role), stdin);
    if(strcmp(role,"Admin")==0)
    {
        puts("Admin role creastion requires existing admin credentials. Please input existing admin UID/password");
        puts("Enter Admin UID: ");
        long UID;
        scanf("%lu", &UID);
        if(AuthUser(UID)==1)
        {
            puts("Authorised Admin login succesful");
            strcpy(p.role,"Admin");
        }
        else
        {
            puts("Admin login unscuccessful");
            return(0);
        }
    }
    strcpy(p.role,"User");
    fwrite(&p, sizeof(p), 1, fptr);
    puts("User creation succesful. UID is:%lu",p.UID);
    fclose(fptr);
    return(1);
}

int AuthUser(long UID)//searches for uid and checks password. returns 1 if authorized admin 2 if authorised user 0 otherwise 
{
    struct people i;
    FILE *fptr;
    if ((fptr = fopen("people.bin","r")) == NULL)
    {
    printf("Error! opening file");
    return(0);
    }
    while (fread(&i, sizeof(i), 1, fptr)!=sizeof(i))
    {
        if(i.UID==UID)
        {
            char password[50]={'\0'};
            printf("Enter User Password: ");
            fgets(password, sizeof(password), stdin);
            if(strcmp(password, i.password))
            {
                puts("Auth successful");
                if(strcmp(i.role,"Admin"))
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
                puts("Password incorrect");
                return(0);
            }

        }
    }
    puts("UID not found");
    return(0);    
}


