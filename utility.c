#include "utility.h"
#include "stdlib.h"
#include "stdio.h"

void insert_command(Command_List** command_head, Command c)
{
    Command_List* new = (Command_List*)malloc(sizeof(Command_List));
    if (new == NULL) {
        printf("Error allocating memory at Utility.c\n");
        return;
    }
    new->command = c;
    new->next = NULL;
    if (*command_head == NULL) {
        *command_head = new;
        return;
    }
    new->next = *command_head;
    *command_head = new;
}

Command pop_command(Command_List** command_head)
{
    if (*command_head == NULL) {
        return NOOP;
    }
    Command_List* temp = (*command_head)->next;
    (*command_head)->next = NULL;
    Command nxt = (*command_head)->command;
    free(*command_head);
    *command_head = temp;
    return nxt;
}

#ifdef NAIVE_PQUEUE
void free_list(list** head)
{
    if (head == NULL) {
        return;
    }
    list* temp = *head;
    list* delete;
    while (temp != NULL) {
        delete = temp;
        temp = temp->next;
        free(delete->data);
        free(delete);
    }
}
Node* create_node(int x, int y, Node* parent, float cost, float cost_g,
    float safety_distance, Command c)
{   
    // printf("Child %d %d Safety Distance %f cost_g %f\n",x,y,safety_distance,cost_g);
    
    Node* data = malloc(sizeof(Node));
    if (data == NULL) {
        printf("Stack Overflow");
        return NULL;
    }
    data->parent = parent;
    if (x < 0) {
        x = RES_COL - 1;
    }
    if (x > RES_COL - 1) {
        x = 0;
    }
    if (y < 0) {
        y = RES_ROW - 1;
    }
    if (y > RES_ROW - 1) {
        y = 0;
    }
    data->x = x;
    data->y = y;
    if(safety_distance!=0 && safety_distance>cost_g){
        data->cost=99999;
        data->cost_g=99999;
    }else{
    data->cost=cost+cost_g;
    data->cost_g = cost_g;
    }
    data->command = c;
    return data;
}
void insert_node(list** head, Node* data)
{
    if(data==NULL){
      return;
    }
    list* node = malloc(sizeof(list));
    if (node == NULL) {
        printf("Stack Overflow");
        return;
    }
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    if ((*head) == NULL) {
        (*head) = node;
        return;
    }
    list *temp = (*head), *prev = NULL;
    while (temp != NULL && temp->data->cost < data->cost) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == (*head)) {
        node->next = temp;
        temp->prev = node;
        (*head) = node;
        return;
    }
    node->next = prev->next;
    node->prev = prev;
    if (prev->next != NULL) {
        prev->next->prev = node;
    }
    prev->next = node;
    temp=(*head);
}

Node* remove_node(list** head)
{
    list* temp = (*head);
    Node* removed = temp->data;
    temp->data = NULL;
    (*head) = temp->next;
    if ((*head) != NULL) {
        (*head)->prev = NULL;
    }
    free(temp);
    return removed;
}

Node *find_node(list * head,int x,int y){
    list *temp=head;
    while(temp!=NULL){
        if((temp->data->x==x && temp->data->y==y)){
            return temp->data;
        }
        temp=temp->next;
    }
    return NULL;
}

void delete_node(list **head,int x,int y){
    list *temp=*head;
    while(temp!=NULL){
        if(temp->data->x==x && temp->data->y==y){
            if(temp->next!=NULL){
                temp->next->prev=temp->prev;
            }
            temp->prev->next=temp->next;
            free(temp);
            return;
        }
        temp=temp->next;
    }
}
#endif