#include "utility.h"
#include "stdlib.h"
#include "stdio.h"

void insert_command(Command_List **command_head,Command c){
  Command_List *new=(Command_List*)malloc(sizeof(Command_List));
  if(new==NULL){
    printf("Error allocating memory at Utility.c:6\n");
    return;
  }
  new->command=c;
  new->next=NULL;
  if(*command_head==NULL){
    *command_head=new;
    return;
  }
  new->next=*command_head;
  *command_head=new;
}

Command pop_command(Command_List **command_head){
  if(*command_head==NULL){
    return NOOP;
  }
  Command_List *temp=(*command_head)->next;
  (*command_head)->next=NULL;
  Command nxt=(*command_head)->command;
  free(*command_head);
  *command_head=temp;
  return nxt;
}