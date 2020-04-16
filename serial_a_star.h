#ifndef SERIAL_A_STAR_H
#define SERIAL_A_STAR_H

#include "export.h"
#include "math.h"
#include "utility.h"

void calculate_path();
float calculate_minimum_distance(int, int, int, int);
list *generate_neighbours(Node *);
int contains_better_node(list *, Node *);
int cleanup(void*);
list *open,*close,*neighbours;
#endif