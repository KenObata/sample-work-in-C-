#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "ics.h"

typedef struct node_t {
    event_t        *val;
    struct node_t  *next;
    struct node_t  *previous;
} node_t;

node_t *new_node(event_t *val);

void extract(char *dt, int *yy, int *mo, int *dd, int *hh, int *min,char *ampm);
void dashes(char *output);
int getDiff(char *dt_end,char *repeat_until);
void dt_increment(char *after, char *before, int num_days);
void apply(node_t *list, void (*fn)(node_t *list, void *, 
           char *yyyymmdd_from), void *arg, char *yyyymmdd_from, char *yyyymmdd_to);
node_t *insert(node_t *list, node_t *temp, node_t *previous, node_t *current);
void freeList(node_t *head);
node_t *insert(node_t *temp_node, node_t *current, node_t *previous);
#endif
