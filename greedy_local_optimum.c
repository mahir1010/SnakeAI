#include "greedy_local_optimum.h"
#include "limits.h"
void init()
{
	double length = sqrt(pow(INIT_SNAKE_X1 - INIT_SNAKE_X2, 2)
			     + pow(INIT_SNAKE_Y1 - INIT_SNAKE_Y2, 2));
	for (int i = 0; i < RES_COL; i++) {
		for (int j = 0; j < RES_ROW; j++) {
			if (sqrt(pow(INIT_SNAKE_X1 - i, 2)
				 + pow(INIT_SNAKE_Y1 - j, 2))
				    + sqrt(pow(INIT_SNAKE_X2 - i, 2)
					   + pow(INIT_SNAKE_Y2 - j, 2))
				    - length
			    == 0) {
				map[i][j] = 1;
			} else {
				map[i][j] = 0;
			}
		}
	}
}

int calculate_cost(int x,int y){
  return fmax(INT_MAX*map[x][y],fmin(fabs(f->x1-x),RES_COL-fabs(f->x1-x))+fmin(fabs(f->y1-y),RES_ROW-fabs(f->y1-y)));
}

Command next_command(){
  int x=head->attr->x1,y=head->attr->y1;
  int minCost=INT_MAX,cost=0;
  int X,Y;
  Command next=NOOP;
  X=x==0?RES_COL-1:x-1;
  cost=calculate_cost(X,y);
  if(cost<minCost){
    minCost=cost;
    next=LEFT;
  }
  X=x==RES_COL-1?0:x+1;
  cost=calculate_cost(X,y);
  if(cost<minCost){
    minCost=cost;
    next=RIGHT;
  }
  Y=y==0?RES_ROW-1:y-1;
  cost=calculate_cost(x,Y);
  if(cost<minCost){
    minCost=cost;
    next=UP;
  }
  Y=y==RES_ROW-1?0:y+1;
  cost=calculate_cost(x,Y);
  if(cost<minCost){
    minCost=cost;
    next=DOWN;
  }
  return next;
}

void update_map(){
  int x,y;
  map[head->attr->x1][head->attr->y1]=1;
  x=last->attr->x2-last->attr->x2_increment;
  y=last->attr->y2-last->attr->y2_increment;
  map[x][y]=0;
}