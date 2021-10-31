#include<stdio.h>
#include "inventoryFunctions.h"
#include "OrderFunctions.h"
#include "userFunctions.h"
//The first page that will get displayed as soon as the program starts
int welcome()
{
    int n,a;
    long U;
label:
    printf("Welcome to the *Insert a name* online shopping website \n");
    printf("Enter 1 if you want to create a new account and 2 if you want to login to your existing account \n");
    scanf("%d",&n);
    if(n==1)
    {
        a=CreateUser();
        if(a==1)
        {
            printf("Your account has been succesfully created");
            goto label;
        }
        else
        {
            printf("This ID is already registered. Use some different ID");
            goto label;
        }
    }
    else if(n==2)
    {
        printf("Please enter your UID \n");
        scanf("%ld",&U);
        a=AuthUser(U);
        return a;
    }
}

int main()
{
    welcome();
}
