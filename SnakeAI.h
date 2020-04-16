#ifndef SNAKEAI_H
#define SNAKEAI_H

#include "stdlib.h"
#include "math.h"

#if !defined(PROB_HEURISTIC) && !defined(SERIAL_A_STAR)
#include "greedy_local_optimum.h"
#elif defined(SERIAL_A_STAR)
#include "serial_a_star.h"
#else
#include "probabilistic_heuristic.h"
#endif
#endif