#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
    Nop,
    Play,
} Command;

void command_queue_init();
int command_enqueue(Command cmd);
Command command_dequeue(void);
void command_queue_free();

#endif
