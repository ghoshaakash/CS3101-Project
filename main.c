#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>

#include"userFunctions.h"

struct people
{
    char name[50];
    char role[6];
    long UID;
    char password[50];
};


struct order
{
    long OrderID;
    char UID;
    int delivery_type;
    int date;
    int slot;
};
