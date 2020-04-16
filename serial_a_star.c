#include "serial_a_star.h"
#include "SDL2/SDL_thread.h"

#include "stdio.h"
void next_command(Command_List** head)
{
    calculate_path(head);
    // Command_List* temp = *head;
    // while (temp != NULL) {
    //     switch (temp->command) {
    //     case LEFT:
    //         printf("LEFT ");
    //         break;
    //     case RIGHT:
    //         printf("RIGHT ");
    //         break;
    //     case UP:
    //         printf("UP ");
    //         break;
    //     case DOWN:
    //         printf("DOWN ");
    //         break;
    //     }
    //     temp = temp->next;
    // }
    // printf("\n");
}

int contains_better_node(list* head, Node* n)
{
    list* temp = head;
    while (temp != NULL) {
        if (temp->data->x == n->x && temp->data->y == n->y) {
            if (temp->data->cost > n->cost) {
                temp->data->parent = n->parent;
                n->parent = NULL;
                temp->data->cost = n->cost;
                temp->data->cost_g = n->cost_g;
                temp->data->command = n->command;
            }
            return 1;
        }
        temp = temp->next;
    }
    return -1;
}

float calculate_safety_distance(int x, int y)
{
    RenderingLines* temp = head;
    float cost = 0;
    while (temp != NULL) {
        float cost1 = calculate_distance(temp->attr->x1, temp->attr->y1, x, y);
        float cost2 = calculate_distance(temp->attr->x2, temp->attr->y2, x, y);
        if (cost1 + cost2 == calculate_distance(temp->attr->x1, temp->attr->y1, temp->attr->x2, temp->attr->y2)) {
            cost = cost2 + 1;
            temp = temp->next;
            while (temp != NULL) {
                cost += calculate_distance(temp->attr->x1, temp->attr->y1,
                    temp->attr->x2, temp->attr->y2);
                temp = temp->next;
            }
            break;
        }
        temp = temp->next;
    }
    return cost;
}

float calculate_minimum_distance(int x, int y, int x1, int y1)
{
    return fminf(fabsf(x1 - x), ((float)RES_COL) - fabsf(x1 - x)) + fminf(fabsf(y1 - y), ((float)RES_ROW) - fabsf(y1 - y));
}

void calculate_path(Command_List** command_list)
{
    int target_x = f->x1;
    int target_y = f->y1;
    // printf("Source %d %d target %d %d\n",head->attr->x1,head->attr->y1,target_x,target_y);
    open = NULL;
    close = NULL;
    // printf("-----------------------------\n");
    insert_node(&open, create_node(head->attr->x1, head->attr->y1, NULL, calculate_minimum_distance(head->attr->x1, head->attr->y1, target_x, target_y), 0.0f, 0.0f, currentCommand));
    while (open != NULL) {
        // printf("Start Position %d %d end Position %d %d\n",head->attr->x1,head->attr->y1,target_x,target_y);
        Node* min_cost = remove_node(&open);
        insert_node(&close, min_cost);
        neighbours = generate_neighbours(min_cost);
        // printf("Expanding %d %d with cost %f cost_g %f\n",min_cost->x,min_cost->y,min_cost->cost,min_cost->cost_g);
        while (neighbours != NULL) {
            Node* neighbour = remove_node(&neighbours);
            if ((neighbour->x <= target_x + 2 && neighbour->x >= target_x) && (neighbour->y <= target_y + 2 && neighbour->y >= target_y)) {
                Node* temp = neighbour;
                while (temp->parent != NULL) {
                    insert_command(command_list, temp->command);
                    temp = temp->parent;
                }
                #ifdef DEBUG
                if (debug_path != NULL) {
                    free(debug_path);
                    debug_path = NULL;
                }
                debug_path = neighbour;
                #else
                free(neighbour);
                #endif
                SDL_Thread* cleanup_t=SDL_CreateThread(cleanup,"cleanup",(void*)NULL);
                return;
            }
            Node* temp = find_node(close, neighbour->x, neighbour->y);
            if (!temp && contains_better_node(open, neighbour) == -1) {
                insert_node(&open, neighbour);
            } else {
                free(neighbour);
            }
        }
        free_list(&neighbours);
    }
    free_list(&open);
    free_list(&close);
}

list* generate_neighbours(Node* head)
{
    list** neighbours = malloc(sizeof(list*));
    *neighbours = NULL;
    switch (head->command) {
    case LEFT:
        insert_node(
            neighbours,
            create_node(
                head->x - 1, head->y, head,
                calculate_minimum_distance(head->x - 1, head->y, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x - 1, head->y),
                LEFT));
        insert_node(
            neighbours,
            create_node(
                head->x, head->y - 1, head,
                calculate_minimum_distance(head->x, head->y - 1, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x, head->y - 1),
                UP));
        insert_node(
            neighbours,
            create_node(
                head->x, head->y + 1, head,
                calculate_minimum_distance(head->x, head->y + 1, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x, head->y + 1),
                DOWN));
        break;
    case RIGHT:
        insert_node(
            neighbours,
            create_node(
                head->x + 1, head->y, head,
                calculate_minimum_distance(head->x + 1, head->y, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x + 1, head->y),
                RIGHT));
        insert_node(
            neighbours,
            create_node(
                head->x, head->y - 1, head,
                calculate_minimum_distance(head->x, head->y - 1, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x, head->y - 1),
                UP));
        insert_node(
            neighbours,
            create_node(
                head->x, head->y + 1, head,
                calculate_minimum_distance(head->x, head->y + 1, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x, head->y + 1),
                DOWN));
        break;
    case UP:
        insert_node(
            neighbours,
            create_node(
                head->x - 1, head->y, head,
                calculate_minimum_distance(head->x - 1, head->y, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x - 1, head->y),
                LEFT));
        insert_node(
            neighbours,
            create_node(
                head->x, head->y - 1, head,
                calculate_minimum_distance(head->x, head->y - 1, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x, head->y - 1),
                UP));
        insert_node(
            neighbours,
            create_node(
                head->x + 1, head->y, head,
                calculate_minimum_distance(head->x + 1, head->y, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x + 1, head->y),
                RIGHT));
        break;
    case DOWN:
        insert_node(
            neighbours,
            create_node(
                head->x - 1, head->y, head,
                calculate_minimum_distance(head->x - 1, head->y, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x - 1, head->y),
                LEFT));
        insert_node(
            neighbours,
            create_node(
                head->x + 1, head->y, head,
                calculate_minimum_distance(head->x + 1, head->y, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x + 1, head->y),
                RIGHT));
        insert_node(
            neighbours,
            create_node(
                head->x, head->y + 1, head,
                calculate_minimum_distance(head->x, head->y + 1, f->x1, f->y1),
                head->cost_g + 1, calculate_safety_distance(head->x, head->y + 1),
                DOWN));
        break;
    }
    return *neighbours;
}
int cleanup(void *data){
                free_list(&neighbours);
                free_list(&open);
#ifdef DEBUG
                if (debug != NULL) {
                    free_list(&debug);
                    debug = NULL;
                }
                debug = close;
#else
                free_list(&close);
                

#endif
}
#ifdef DEBUG
void printDebugInfo(int x, int y)
{
    FILE* f = fopen("debug.txt", "w");
    if (f == NULL) {
        printf("Error while openning file\n");
        return;
    }
    fprintf(f, "Body:\n");
    RenderingLines* l = head;
    while (l != NULL) {
        fprintf(f, "(%3d %3d - %3d %3d)\n", l->attr->x1, l->attr->y1, l->attr->x2, l->attr->y2);
        l = l->next;
    }
    fprintf(f, "Target %3d %3d\n Path:\n", x, y);
    Node* temp = debug_path;
    int count = 0;
    while (temp != NULL) {
        fprintf(f, "(%3d %3d) c_g:%3.0f ->", temp->x, temp->y, temp->cost_g);
        temp = temp->parent;
        count++;
        if (count == 5) {
            count = 0;
            fprintf(f, "\n");
        }
    }
    fprintf(f, "\nClosed Nodes:\n");
    list* t = debug;
    count = 0;
    while (t != NULL) {
        fprintf(f, "(%3d %3d),", t->data->x, t->data->y);
        t = t->next;
        if (count == 5) {
            count = 0;
            fprintf(f, "\n");
        }
    }
}
#endif