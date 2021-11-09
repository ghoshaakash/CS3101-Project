#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <windows.h> 

#define DeliveryPeople 10
#define Slots 3

struct scheduler//struct to store data in LatestScheduler.ord
{
    int d;
    int m;
    int y;
    int slot[Slots];
    long OrderID;
};

void nextDate(int *d,int *m,int *y)//changes date to next day
{
    int DaysInMonth[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if((*y)%100==0)
    {
        if((*y)%400==0)
        {
            DaysInMonth[1]=29;
        }
    }
    else if ((*y)%4==0)
    {
        DaysInMonth[1]=29;
    }
    if((*d)!=DaysInMonth[(*m)-1])
    {
        *d=*d+1;
    }
    else
    {
        *d=1;
        if((*m)!=12)
        {
            *m=*m+1;
        }
        else
        {
            *m=1;
            *y=*y+1;
        }
    }
    
}




//Creates aset of available slot and passes to AvailableSlots[]. Will first check if next available date has passed or not. 
//If yes then will assign next date according to system date 
//if no then will check if all slots are finished in next the available date
//if no slots are full will assign the next date and set all slots to be free 
//changes int pointers to d,m,y values
//changes AvailableSlot[i] to 1 if slot i+1 is free else it stays at 0

void MakeSlot(int AvailableSlots[],int *d,int *m, int *y)
{
    FILE* fptr = fopen("LatestScheduler.ord","rb+");
    struct scheduler s; 
    int NewRecordFlag=0;
    int i=fread(&s, sizeof(s), 1, fptr);
    if(i==1)
    {
        char str[50]="Free Slots are:";
        printf("%s",str);
        SYSTEMTIME t;
        GetLocalTime(&t);
        if(t.wYear<s.y)
        {
            NewRecordFlag=0;
        }
        else if(t.wYear>s.y)
        {
            NewRecordFlag=1;
        }
        else
        {
            if(t.wMonth<s.m)
            {
                NewRecordFlag=0;
            }
            else if(t.wMonth>s.m)
            {
                NewRecordFlag=1;
            }
            else
            {
                if(t.wDay>s.d)
                {
                    NewRecordFlag=1;
                }
                else
                {
                    NewRecordFlag=0;
                }
            }
        }
         if(NewRecordFlag==1)
        {
            fseek(fptr,0,SEEK_SET);
            s.y=t.wYear;
            s.m=t.wMonth;
            s.d=t.wDay;
            nextDate(&s.d,&s.m,&s.y);
            *d=s.d;
            *m=s.m;
            *y=s.y;
            for(int i=0;i<Slots;i++)
            {
                s.slot[i]=0;
            }
            fwrite(&s,1,sizeof(s),fptr);
            for(int i=0;i<Slots;i++)
            {
                AvailableSlots[i]=1;
                printf(" %d", i+1);
                NewRecordFlag=0;
            }
            
            printf("\n");
            return;
        }    


        if(NewRecordFlag==0)
        {
            NewRecordFlag=1;
            for(int i=0;i<Slots;i++)
            {
                if(s.slot[i]<DeliveryPeople)
                {
                    AvailableSlots[i]=1;
                    printf(" %d", i+1);
                    NewRecordFlag=0;
                }
            }
        }
        if(NewRecordFlag==1)
        {
            fseek(fptr,0,SEEK_SET);
            nextDate(&s.d,&s.m,&s.y);
            for(int i=0;i<Slots;i++)
            {
                s.slot[i]=0;
            }
            fwrite(&s,1,sizeof(s),fptr);
            for(int i=0;i<Slots;i++)
            {
                AvailableSlots[i]=1;
                printf(" %d", i+1);
                NewRecordFlag=0;
            }
        }    
        printf("\n");
    }
    else
    {
        fclose(fptr);
        FILE* fptr = fopen("LatestScheduler.ord","wb");
        SYSTEMTIME t;
        GetLocalTime(&t);
        fseek(fptr,0,SEEK_SET);
        char str[50]="Free Slots are:";
        printf("%s",str);
        s.y=t.wYear;
        s.m=t.wMonth;
        s.d=t.wDay;
        s.OrderID=10000;
        nextDate(&(s.d),&(s.m),&(s.y));
        for(int i=0;i<Slots;i++)
        {
            s.slot[i]=0;
        }
        fwrite(&s,1,sizeof(s),fptr);
        for(int i=0;i<Slots;i++)
        {
            AvailableSlots[i]=1;
            printf(" %d", i+1);
            NewRecordFlag=0;
        }
        printf("\n");
    }
    *d=s.d;
    *m=s.m;
    *y=s.y;
     fclose(fptr);

}






void LogOrder(long OID,long UID,int d,int m,int y,int slot,float Price)//Appends order to CSV file
{
    FILE *fptr = fopen("Log.csv","a");
   fprintf(fptr,"%ld,%ld,%d,%d,%d,%d,%f\n",OID,UID,d,m,y,slot,Price);
   fclose(fptr);
}

long addorder(int slot)//Incements the slot in last order where order was placed and returns orderID
{
    FILE* fptr = fopen("LatestScheduler.ord","rb+");
    struct scheduler s;
    fread(&s, sizeof(s), 1, fptr);
    s.slot[slot-1]=s.slot[slot-1]+1;
    fseek(fptr,0,SEEK_SET);
    s.OrderID=s.OrderID+1;
    long ord=s.OrderID;
    fwrite(&s,1,sizeof(s),fptr);
    fclose(fptr);
    return ord;
}


long AssignSlots(long UID, float price)//Assigns slot and logs order, returns ORDER ID
{
    int d=0,m=0,y=0;
    int flag=1;
    int AvailableSlots[Slots]={0};
    MakeSlot(AvailableSlots,&d,&m,&y);
    printf("%s","Enter Desired Slot :");
    int slot;
    while(flag)
    {
        scanf(" %d",&slot);
        if(AvailableSlots[slot-1]==1)
        {
            flag=0;
            break;
        }
        printf("Chosen slot is full/Bad input. Please input valid slot number again:");
    }
    long OID=addorder(slot);
    printf("Order Booked for %d/%d/%d ,slot no: %d priced at %.2f. Your order ID is %ld\n",d,m,y,slot,price,OID);
    LogOrder(OID,UID,d,m,y,slot,price);
}
