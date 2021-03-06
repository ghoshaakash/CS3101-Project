#include <stdio.h>
#include <string.h>
#include <math.h>
#include <windows.h> 
#include "userFunctions.h"


#define DeliveryPeople 10
#define Slots 5



void clrscr()
{
	printf("\e[1;1H\e[2J");
}

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
		char str[50]="Free Slots are: \n";
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
				printf(" %d : (%d PM - %d PM) \n", i+1, i+1, i+2);
				NewRecordFlag=0;
			}
			
			printf("\n");
			fclose(fptr);
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
					printf(" %d : (%d PM - %d PM) \n", i+1, i+1, i+2);
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
				printf(" %d : (%d PM - %d PM) \n", i+1, i+1, i+2);
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
		char str[50]="Free Slots are: \n";
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
			printf(" %d : (%d PM - %d PM) \n", i+1, i+1, i+2);
			NewRecordFlag=0;
		}
		printf("\n");
	}
	*d=s.d;
	*m=s.m;
	*y=s.y;
	 fclose(fptr);

}






void LogOrderDelivery(long OID,long UID,int d,int m,int y,int slot,float Price, char ADD[1000], char Det[1000])//Appends log order to CSV file
{
	FILE *fptr = fopen("Delivery.csv","a");
	fprintf(fptr,"%ld,2,%ld,%d,%d,%d,%d,%f,%s,%s\n",OID,UID,d,m,y,slot,Price,ADD,Det);
	fclose(fptr);
}

void LogOrderPickUp(long OID,long UID, char Det[1000])//Appends pickup order to CSV file
{
	FILE *fptr = fopen("PickUp.csv","a");
	fprintf(fptr,"%ld,%ld,%s\n",OID,UID,Det);
	fclose(fptr);
}



long addOrderDelivery(int slot)//Increments the slot in last order where order was placed and returns orderID
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

long addOrderPickup()
{
	FILE* fptr = fopen("LatestScheduler.ord","rb+");
	struct scheduler s;
	fread(&s, sizeof(s), 1, fptr);
	fseek(fptr,0,SEEK_SET);
	s.OrderID=s.OrderID+1;
	long ord=s.OrderID;
	fwrite(&s,1,sizeof(s),fptr);
	fclose(fptr);
	return ord;
}









int addAddress(long UID)// adds address entry to user record
{
    int flag=0;
    struct people t;
	FILE * file1= fopen("people", "rb");
	FILE * file2= fopen("lol", "wb");
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
            if(top==5)
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
    rename("lol","people");
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
	FILE * file2= fopen("lol", "wb");
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
    rename("lol","people");
	}	
    if(flag==0)
    {
        printf("UID not found\n");
    }
    return flag;
}


int getaddress(long UID,char ADD[100])//returns addrress
{
    struct people t;
	int i;
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
					printf("%d. %s\n",top+1,t.add[top]);
                }   
                if(top==0)
                {
                    printf("Your address list is empty\n");
                    fclose(file1);
                    return(0);
                }
                printf("Enter address choice: \n");
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





void listAll(long UID)
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
		if(p.UID==UID)
		{
			printf("1. %s \n2. %s \n3. %s \n4. %s \n5. %s\n",p.add[0],p.add[1],p.add[2],p.add[3],p.add[4]);
		}
    }
    fclose(fptr);
}


long AssignSlots(long UID, float price,char OrderDet[1000])//Assigns slot and logs order, returns ORDER ID
{
	int PickUpFlag;
	printf("Enter 1 for pickup and 2 for delivery option: ");
	scanf(" %d",&PickUpFlag);
	while(1)
	{
		if(PickUpFlag==1)
		{
			long OID=addOrderPickup();
			LogOrderPickUp(OID,UID,OrderDet);
			printf("Your order has been placed with order ID %ld. Press any key to return to previous menu.\n",OID);
			printf("-----------------------------------------------------------------------------------\n");
			getchar();
			return OID;
		}
		else if(PickUpFlag==2)
		{
			break;
		}
		printf("Bad Input. Plase Try Again: ");
		scanf(" %d",&PickUpFlag);
	}
	int d=0,m=0,y=0;
	int flag=1;
	int AvailableSlots[Slots]={0};
	MakeSlot(AvailableSlots,&d,&m,&y);
	//printf("%s","\n\nSlot 1 is from 9 to 10 am. Slot 2 is from 10 to 11 am. Slot 3 is from 11am to 12 noon. Slot 4 is from 1 to 2 pm. Slot 4 is from 2 to 3 pm. Slot 5 is from 3 to 4 pm. Enter Desired Slot : \n");
	int slot;
	while(flag)
	{
		scanf(" %d",&slot);
		if(AvailableSlots[slot-1]==1)
		{
			flag=0;
			break;
		}
		printf("Chosen slot is full/Bad input. Please input valid slot number again:\n\n");
	}
	char Add[1000]={'\0'};
	printf("\n\nAddresses: \n");
	int flag1=getaddress(UID,Add);
	
	if(flag1==0)
	{
		addAddress(UID);
		
		printf("\n\nYour Address list :\n");
		getaddress(UID,Add);
	}
	
	
	long OID=addOrderDelivery(slot);
	printf("Order Booked for %d/%d/%d ,slot no: %d priced at %.2f. Your order ID is %ld\nDelivery will be done at %s\n",d,m,y,slot,price,OID,Add);
	printf("----------------------------------------------------------------------------------------------------------");
	LogOrderDelivery(OID,UID,d,m,y,slot,price,Add,OrderDet);
}


