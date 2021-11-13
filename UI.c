#include<stdio.h>
#include<conio.h>
#include "stack.h"
/*We are using cmd x64 bit systems for the display
The first page that will get displayed as soon as the program starts*/
int UI()
{
    int n,a,b,d,f,g,h;
    char c[100],e[100];
    long U;
label://A pointer to return to the first page after the first choice or after logging out from the account 
//The first choice to create an account or to login to an existing account
    clrscr();
    printf("Welcome to the DreamCart official shopping website \n");
    printf("1 if you want to create a new account \n2 if you want to login to your existing account \n0 to close the webiste \n");
    printf("=============================================================\nEnter Your Choice:");
    scanf(" %d",&n);
    clrscr();
    if(n==1)
    {
        a=CreateUser();
        if(a==1)
        {
            goto label;
        }
        else
        {
            printf("This ID is already registered. Use some different ID \n");
            goto label;
        }
    }
    else if(n==2)
    {
        printf("Please enter your UID \n");
        scanf(" %ld",&U);
        a=AuthUser(U);
        if(a==0)
        {
            printf("Please try again \n");
            goto label;
        }
        else if(a==1)
        {
label2://A pointer to return to the point when an admin is logged in and the display page after logging in or before logging out
//The second choice for the admin
            clrscr();
            printf("1 to check the full inventory \n2 to add a new item to the inventory \n3 to check the quantity of a particular item in the inventory \n4 to change the quantity of an existing item in the inventory \n5 to delete an item from the inventory \n6 to check if an item is present in the inventory \n0 to logout from your account \n");
            printf("===================================\nEnter Your Choice: ");
            scanf(" %d",&b);
            clrscr();
            if(b==1)
            {
                view_items();
                goto label2;
            }
            else if(b==2)
            {
            	char name[100];
            	printf("Enter item name: \n");
            	scanf(" %[^\n]",name);
                add_item(name);
                goto label2;
            }
            else if(b==3)
            {
                printf("Enter the UID of the item whose quantity you want to check in the inventory: \n");
                scanf(" %s",&c);
                int q=get_quantity(c);
                printf("The quantity of the item having UID %s in the inventory is %d \n", c, q);
                goto label2;
            }
            else if(b==4)
            {
                printf("Enter the UID of the item whose quantity is to be changed in the inventory: \n");
                scanf(" %s",&c);
                printf("Enter the quantity of the item to be changed to in the inventory \n");
                scanf(" %d",&d);
                edit_item_quantity(c,d);
                goto label2;
            }
            else if(b==5)
            {
                printf("Enter the UID of the item which is to be deleted from the inventory: \n");
                scanf(" %s",&c);
                delete_item(c);
                goto label2;
            }
            else if(b==6)
            {
                printf("Enter the UID of the item which is to be checked for in the inventory: \n");
                scanf(" %s",&c);
                int e=search_item(c);
                if(e==1)
                {
                    printf("The item is present in the inventory \n");
                }
                else
                {
                    printf("The item is not present in the inventory \n");
                }
                goto label2;
            }
            else if(b==0) 
            {
                goto label;
            }
            else
            {
                printf("Enter the correct command. \n");
                goto label2;
            }
        }
        else if(a==2)
        {
label3://A pointer to return to the point when an user is logged in and the display page after logging in or before logging out
//The second choice for the user
            clrscr();
            printf("1 to continue with the shopping experience \n2 to enter and  update your account details \n0 to logout from your account \n");
            scanf(" %d",&b);
            clrscr();
            if(b==0)
            {
                goto label;
            }
            else if(b==1)
            {
                d=Cart(U);
                if(d==0)
                {
                    goto label3;
                }
            }
            else if(b==2)
            {
label4: //A pointer to return back to the User Account Credentials display page                
//The third choice for the user    
                printf("Your UID is %ld \n",U);
                printf("Your Addresses are: \n");
                listAll(U);            
                printf("1 to add an address to your account credentials \n2 to delete an address from your account \n0 to return back to the main menu");
                scanf(" %d",&f);
                if(f==1)
                {
                    g=addAddress(U);
                    printf("%d \n",g);
                    goto label4;
                }
                else if(f==2)
                {
                    h=deleteAddress(U);
                    printf("%d \n",h);
                    goto label4;
                }
                else
                {
                    goto label3;
                }
            }
            else
            {
            	printf("Invalid input!!!\n");
            	goto label3;
            } 
        }
    }
}

int main()
{
    UI();
}
