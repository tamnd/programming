// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

#include "id_generator.h"

#include <stdatomic.h>
#include <threads.h>

struct id_generator {
    atomic_long next;
};

static struct id_generator instance;
static once_flag instance_once = ONCE_FLAG_INIT;
static int constructions = 0;

static void make_instance(void) {
    atomic_init(&instance.next, 1);
    constructions++;
}

struct id_generator *id_generator_instance(void) {
    call_once(&instance_once, make_instance); // runs make_instance once
    return &instance;
}

long id_generator_next(struct id_generator *g) {
    return atomic_fetch_add(&g->next, 1); // safe when threads share g
}

int id_generator_constructions(void) {
    return constructions;
}

void singleton_report(int calls, long out[3]) {
    struct id_generator *first_g = NULL;
    long distinct = 0;
    long first = 0;
    long last = 0;
    for (int i = 0; i < calls; i++) {
        struct id_generator *g = id_generator_instance();
        if (g != first_g) { // counts each change of the pointer
            distinct++;
            first_g = g;
        }
        last = id_generator_next(g);
        if (i == 0) {
            first = last;
        }
    }
    out[0] = last - first;
    out[1] = constructions;
    out[2] = distinct;
}
