// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

// The one id generator of the program. Its struct is defined in the .c file,
// so other files can hold a pointer to it but cannot make another one.
struct id_generator;

struct id_generator *id_generator_instance(void);
long id_generator_next(struct id_generator *g);
int id_generator_constructions(void);

// singleton_report gets the instance calls times, takes one id each time, and
// writes [last id - first id, constructions, distinct instances] to out.
void singleton_report(int calls, long out[3]);

#endif
