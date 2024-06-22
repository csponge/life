#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

struct node {
    Command value;
    struct node *next;
};

struct queue {
    size_t len;
    struct node *head;
    struct node *tail;
};

static struct queue *comqueue = NULL;

void command_queue_init() {
    comqueue = calloc(1, sizeof(struct queue));
    if (comqueue) {
        comqueue->len = 0;
    }
}

int command_enqueue(Command cmd) {
    struct node *cn = calloc(1, sizeof(struct node));
    if (cn == NULL) {
        return 1;
    }

    cn->value = cmd;
    cn->next = NULL;

    if (comqueue->len == 0) { // first command in queue
        cn->value = cmd;
        cn->next = comqueue->tail;
        comqueue->head = cn;
        comqueue->tail = comqueue->head;
        comqueue->len++;
        return 0;
    }

    comqueue->tail->next = cn;
    comqueue->tail = cn;
    comqueue->len++;
    return 0;
}

Command command_dequeue(void) {
    if (comqueue->len == 0)
        return Nop;

    Command cmd = comqueue->head->value;

    struct node *tmp = comqueue->head;
    comqueue->head = comqueue->head->next;
    free(tmp);
    tmp = NULL;

    comqueue->len--;
    return cmd;
}

void command_queue_free() {
    if (comqueue->len > 0) {
        struct node *curr = comqueue->head;
        while(curr != NULL) {
            struct node *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }

    free(comqueue);
}
