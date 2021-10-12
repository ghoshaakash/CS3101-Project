#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>

struct people
{
    char name[50];
    char role[6];
    long UID;
    char password[50];
};

struct items
{
    char name[50];
    long UID;
    int quantity;
};

struct order
{
    long OrderID;
    char UID;
    int delivery_type;
    int date;
    int slot;
};


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