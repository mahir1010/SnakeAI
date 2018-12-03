#ifndef PROBABILISTIC_HEURISTIC_H
#define PROBABILISTIC_HEURISTIC_H
#include "export.h"
#include "Heuristic_Calculation.cuh"
#include "utility.h"

void calculate_path();
float calculate_prob(int,int,Command);
float get_cost(int,int);
float get_prob(int,int);
void next_command(Command_List **);

#endif