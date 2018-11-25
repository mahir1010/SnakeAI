#ifndef GREEDY_LOCAL_OPTIMUM_H
#define GREEDY_LOCAL_OPTIMUM_H
#include "export.h"
#include "math.h"

extern int map[RES_COL][RES_ROW];
void init();
int calculate_cost(int,int);
Command next_command();
void update_map();
#endif