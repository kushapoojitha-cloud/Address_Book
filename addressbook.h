#ifndef CONTACT_H
#define CONTACT_H
#include<stdio.h>
#include<string.h>


struct Contact{
    char name[50];
    char phone[15];
    char email[50];
};

struct AddressBook {
    struct Contact contacts[100];      //nested structure
    int contactCount;
    int index_record[100];             //it is used in editing and deletion of data
    int ir_size;                       //size of the index record
};

// Prototypes

void pull_file_data(struct AddressBook * , char *);
void add_contact(struct AddressBook *addressBook);
void search_contact(struct AddressBook *addressBook);
void edit_contact(struct AddressBook *addressBook);
void delete_contact(struct AddressBook *addressBook);
void list_contacts(struct AddressBook *addressBook);
void push_data_to_file(struct AddressBook * ,char *);

int phone_unique(struct AddressBook *, char *);            //mobile unique function prototype
int email_unique(struct AddressBook *, char *);            //email unique or not prototype
int search_contact_by_name(struct AddressBook*, char*);    //search contact by name
int search_contact_by_mobile(struct AddressBook*, char*);   //search contact by phone
int search_contact_by_email(struct AddressBook*, char*);   //search contact by email

#endif
