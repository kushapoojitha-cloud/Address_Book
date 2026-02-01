/* Name: K.Poojitha
   Date: 06/01/2026
   Description: This project is a Address Book application in C that allows users to add, search, edit, delete, and display contacts.
                It loads contact data from a CSV file at startup and saves uppdates back to the file beffore exiting. */


#include "addressbook.h"

//FOR PULLING THE DATA
void pull_file_data(struct AddressBook* addressBook, char* file_name)   //load existing contacts from csv file to structure array
{                     
    FILE* fp = fopen(file_name,"r");                                    // Open CSV file in read mode
    int i=0;
    addressBook->contactCount=0;
    fseek(fp,0,SEEK_END);                                              // Move file pointer to end of file
    if(ftell(fp)!=0)                                                   // Check if file is not empty
    {
        rewind(fp);                                                    // Move file pointer back to start
        while(!feof(fp))                                               // Read until end of file
        {
            fscanf(fp,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
            i++;
        }
    } 
    addressBook->contactCount=i;                                        // Store total number of contacts
    fclose(fp);                                                         // Close file
}

//PUSHING THE DATA
void push_data_to_file(struct AddressBook * addressBook, char * file_name)
{   
    //save data from structure array to csv
    FILE *fp = fopen(file_name,"w");                                   // Open CSV file in write mode
    for(int i=0;i< addressBook->contactCount;i++)
    {
        fprintf(fp,"%s,",addressBook->contacts[i].name);
        fprintf(fp,"%s,",addressBook->contacts[i].phone);
        fprintf(fp,"%s\n",addressBook->contacts[i].email);
    }
    fclose(fp);
}

int mobile_validation(char*);                                          // Function prototype for mobile validation
char* email_validation(char*);                                         // Function prototype for email validation

//cheking mobile number is unique or not
int phone_unique(struct AddressBook *addressBook, char *phone)         
{
    for(int i = 0; i < addressBook->contactCount; i++)                  
    {
        if(strcmp(addressBook->contacts[i].phone, phone) == 0)          // Compare stored phone with input phone
        {
            return 1;                                                   // Return 1 if phone is not unique
        }
    }
    return 0;                                                           // Return 0 if phone is unique
}

//cheking email is unique or not
int email_unique(struct AddressBook *addressBook, char *email)          // Function to check email uniqueness
{
    for(int i = 0; i < addressBook->contactCount; i++)                  
    {
        if(strcmp(addressBook->contacts[i].email, email) == 0)          // Compare stored email with input email
        {
            return 1;                                                   // Return 1 if email is not unique
        }
    }
    return 0;                                                           // Return 0 if email is unique
}

//ADDING CONTACT MAIN FUNCTION
void add_contact(struct AddressBook *addressBook)
{
    char name[50];
    printf("enter the User_Name: ");
    scanf(" %[^\n]",name);

    int res=0;
    char phone[15]; 
    do
    {
        printf("enter mobile number: ");
        scanf(" %s",phone);
        res=mobile_validation(phone);                                  //mbl validation function call
        if(res == 0 && phone_unique(addressBook, phone))               // Check uniqueness only if number is valid
            res = 1; 
        if(res)
            printf("Error: enter valid mobile number\n");  
    }
    while(res);

    char* res1;
    char email[50];
    do
    {
        printf("enter the email: ");
        scanf("%s",email);
        res1=email_validation(email);                  //email validation function call
        if(res1 && email_unique(addressBook, email))   // Check email uniqueness
            res1 = NULL;
        if(res1==NULL)
            printf("Error: enter valid email\n");
    }
    while(res1==NULL);

    strcpy(addressBook->contacts[addressBook->contactCount].name,name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone,phone);
    strcpy(addressBook->contacts[addressBook->contactCount].email,email);

    addressBook->contactCount++;                       //gives the count of total data present in the data base
}

//MOBILE VALIDATION
int mobile_validation(char* phone)
{
    if(strlen(phone)!=10)                              //10 digits or not
        return 1;
    if((phone[0]<'6') || (phone[0]>'9'))               // First digit must be 6–9
        return 1;
    int i=0;
    while(phone[i])
    {
        if((phone[i]>='0') && (phone[i]<='9'))         //digits or not
            i++;
        else
            return 1;
    }
    return 0;                                          // Valid phone number
}

//EMAIL VALIDATION
char* email_validation(char*email)
{
    if((email[0]==' ')||(email[0]=='\t')||(email[0]=='\n')||(email[0]=='@'))   // Check invalid starting characters
        return NULL;
    char*str="@gmail.com";
    char*res1=strstr(email,str);                         // Find "@gmail.com" in email    
    if(res1==NULL)
        return NULL;                                     // Not found
    if(strcmp(res1,str)!=0)                              // Must end exactly with "@gmail.com"
        return NULL;
    return res1;                                          // Valid email
}

//SEARCH CONTACT MAIN FUNCTION
void search_contact(struct AddressBook *addressBook)
{
    int choice;
    do 
    {
    addressBook->ir_size=0;                               // Reset index record size
    int result;                                           // Store search result count
	printf("1. Search contact by Name\n");
	printf("2. Search contact by Mobile\n");
    printf("3. Search contact by Email\n");
	printf("4. Exit\n");
	printf("Enter your choice:\n");

	scanf("%d", &choice);

	switch (choice) 
	{
	    case 1:
        char name[50];
        printf("enter the User_Name: ");
        scanf(" %[^\n]",name);
		result = search_contact_by_name(addressBook,name);
		break;

        case 2:
	    int res=0;
        char phone[15]; 
        do
        {
            printf("enter mobile number: ");
            scanf(" %s",phone);
            res=mobile_validation(phone);          //mobile validation
            if(res)
            {
                printf("Error: enter valid mobile number\n");  
            }
        }
        while(res);
        result = search_contact_by_mobile(addressBook,phone);
        break;

        case 3:
        char* res1;
        char email[50];
        do
        {
            printf("enter the email: ");
            scanf("%s",email);
            res1=email_validation(email);
            if(res1==NULL)
                printf("Error: enter valid email\n");
        }
        while(res1==NULL);
        result = search_contact_by_email(addressBook,email);    //email validation
        break;                                                 

        case 4:
        return;                                                 // Exit function
        
	    default:
		printf("Invalid choice. Please try again.\n");
	}

    if(result==0)
    {
        printf("data not found\n");
    }
    else{
    for(int i=0;i<addressBook->ir_size;i++)
    {
        int k=addressBook->index_record[i];
        printf("%d. %s %s %s\n",(i+1),addressBook->contacts[k].name,addressBook->contacts[k].phone,addressBook->contacts[k].email);
    }} return;
    } while (choice != 4);
}

//SEARCH CONTACT BY NAME
int search_contact_by_name(struct AddressBook*addressBook,char*name)
{
    int i;
    for(i=0;i<addressBook->contactCount;i++)
    {
        int res=strcmp(addressBook->contacts[i].name,name);
        if(res==0)
        {
            addressBook->index_record[addressBook->ir_size]=i;
            addressBook->ir_size++;
        }
    }
    return addressBook->ir_size;
}

//SEARCH CONTACT BY MOBILE
int search_contact_by_mobile(struct AddressBook*addressBook,char*phone)
{
    int i;
    for(i=0;i<addressBook->contactCount;i++)
    {
        int res=strcmp(addressBook->contacts[i].phone,phone);
        if(res==0)
        {
            addressBook->index_record[addressBook->ir_size]=i;
            addressBook->ir_size++;
            break;
        }
    }
    return addressBook->ir_size;
}

//SEARCH CONTACT BY EMAIL 
int search_contact_by_email(struct AddressBook*addressBook,char*email)
{
    int i;
    for(i=0;i<addressBook->contactCount;i++)
    {
        int res=strcmp(addressBook->contacts[i].email,email);
        if(res==0)
        {
            addressBook->index_record[addressBook->ir_size]=i;
            addressBook->ir_size++;
            break;
        }
    }
    return addressBook->ir_size;
}

//EDIT CONTACT MAIN FUNCTION
void edit_contact(struct AddressBook *addressBook)
{
    int choice;
    do
    {
        search_contact(addressBook);
        if(addressBook->ir_size==0)
        return;
        printf("\nPlease select one option\n");
	    scanf("%d", &choice);
        if(choice>addressBook->ir_size)
            printf("Invalid choice");
    }
    while(choice>addressBook->ir_size);
    int i=addressBook->index_record[choice-1];
    printf("%s %s %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    do
    {
        printf("\nplease choose option you want to edit\n");
        printf("1. Edit name\n");
        printf("2. Edit mobile no.\n");
        printf("3. Edit Email\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);
        getchar();

        switch(choice)
        {
            case 1:
            char new_name[50];
            printf("Enter new Name: ");         
            scanf(" %[^\n]",new_name);                           //for editing we copying the new data in to it
            strcpy(addressBook->contacts[i].name,new_name);      //editing the name
            break;

            case 2:
            int res=0;
            char new_phone[15]; 
            do
            {
            printf("Enter new_mobile number: ");
            scanf(" %s",new_phone);
            res=mobile_validation(new_phone);                    //calling mbl validation function to check new no. is valid or not
            if(res == 0 && phone_unique(addressBook, new_phone))
                res = 1;
            if(res)
                printf("Error: enter valid mobile number\n");  
            }
            while(res);
            strcpy(addressBook->contacts[i].phone,new_phone);     //editing the mobile number
            break;

            case 3:
            char* res1;
            char new_email[50];
            do
            {
            printf("enter the email: ");
            scanf("%s",new_email);
            res1=email_validation(new_email);                   //calling email validation function to check the new email is valid or not
            if(res1 && email_unique(addressBook, new_email))
                res1 = NULL;
            if(res1==NULL)
            printf("Error: enter valid email\n");
            }
            while(res1==NULL);
            strcpy(addressBook->contacts[i].email,new_email);    //editing the email
            break;

            case 4:
            return;

        } 
    }while (choice != 4);
}
    
//DELETE CONTACT MAIN FUNCTION
void delete_contact(struct AddressBook *addressBook)
{
    int choice;
    do
    {
        search_contact(addressBook);
        if(addressBook->ir_size==0)
        return;
        printf("\nPlease select one option\n");
	    scanf("%d", &choice);
        if(choice>addressBook->ir_size)
            printf("Invalid choice");
    }
    while(choice>addressBook->ir_size);
    int i=addressBook->index_record[choice-1];
    printf("%s %s %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);

    printf("Are you sure, you want to delete\n1. Y\n2. N\n");
    printf("Enter your choice:\n");
    int option;
    scanf(" %d",&option);
    getchar();

    switch(option)
    {
        case 1:
        for(int i=addressBook->index_record[choice-1];i<addressBook->contactCount-1;i++)
        {
            addressBook->contacts[i] = addressBook->contacts[i+1];     //squeezing the data
        }
        addressBook->contactCount--;
        break;

        case 2:
        return;
        break;

        default:
        printf("Invalid entry try again\n");
    }
}

//LISTING THE CONTACTS
void list_contacts(struct AddressBook *addressBook)
{
    if(addressBook->contactCount==0)         // Check if no contacts match found
        printf("no data available");
    else
    for(int i=0;i<addressBook->contactCount;i++)
        printf("%s %s %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        // Display contact
}

