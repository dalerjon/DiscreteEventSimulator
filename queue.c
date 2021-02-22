#include <stdio.h> 
#include <stdlib.h> 
#include "queue.h"

Node* newNode(int d, int p)
{
    Node* temp = (Node*)malloc(sizeof(Node)); 
    temp->data = d; 
    temp->priority = p;
    temp->next = NULL;
 
    return temp; 
} 
 
int peek(Node** head)
{ 
    return (*head)->data; 
}

int getPriority(Node** head) {
    return (*head)->priority;
}

Node* getNode(Node** head)
{
    return (*head);
}

void pop(Node** head) 
{
    Node* temp = *head;
    (*head) = (*head)->next;
    free(temp);
}
 
void push(Node** head, int d, int p) 
{
    Node* start = (*head);
    Node* temp = newNode(d, p);
    if(*head != NULL) {
        if ((*head)->priority > p) {
            temp->next = *head;
            (*head) = temp;
        }
        else {
            while (start->next != NULL &&
                start->next->priority < p) {
                start = start->next;
            }
            temp->next = start->next;
            start->next = temp;
        }
    }
    else {
        (*head) = temp;
    }
}

void pushFifo(Node** head, int d, int p)
{
    Node* start = (*head);
    Node* temp = newNode(d, p);
    if(*head != NULL) {
        while (start->next != NULL) {
            start = start->next;
        }
        temp->next = start->next;
        start->next = temp;
    }
    else {
        (*head) = temp;
    }
}
 
int isEmpty(Node** head)
{ 
    return (*head) == NULL; 
}

int queueSize(Node** head)
{
    int size = 0;
    struct node *temp;
    if(head == NULL)
    {
        return size;
    }
    temp = *head;
    while(temp != NULL)
    {
        size += 1;
        temp = temp -> next;
    }
    return size;
}
 
