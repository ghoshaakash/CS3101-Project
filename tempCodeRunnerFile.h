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
    scanf("%s",p.name);
    printf("Enter User Password: ");
    scanf("%s",p.password);
    strcpy(p.role,"Admin");
    fwrite(&p, sizeof(p), 1, fptr);
    fclose(fptr);
    printf("Registered with UID %ld \n",p.UID);
}