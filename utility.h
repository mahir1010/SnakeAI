#ifndef SNAKE_AI_UTILITY_H
#define SNAKE_AI_UTILITY_H
#include "Structures.h"
#include "export.h"
void insert_command(Command_List**, Command);
void next_command(Command_List**);
Command pop_command(Command_List**);
#ifdef DEBUG
void printDebugInfo(int,int);
#endif
#ifdef NAIVE_PQUEUE
#include "float.h"
#include "stddef.h"
#include "stdlib.h"

typedef struct n {
    int x, y;
    float cost, cost_g;
    Command command;
    struct n* parent;
} Node;

typedef struct l {
    Node* data;
    struct l *next, *prev;
} list;
#ifdef DEBUG
list* debug;
Node* debug_path;
#endif
void insert_node(list**, Node*);
Node* create_node(int x, int y, Node* parent, float cost, float cost_g, float safety_distance, Command c);
Node* remove_node(list**);
void delete_node(list**, int x, int y);
Node* find_node(list*, int, int);
void free_list(list**);
#endif

#endif
