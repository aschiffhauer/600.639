#ifndef _MINSKETCH_H
#define _MINSKETCH_H

#include <stdbool.h>

typedef struct {
	int w;
	int d;
	int **rows;
} minsketch;

minsketch* minsketch_new(int w, int d);
bool minsketch_add(minsketch *m, const char *str);
int minsketch_get(minsketch *m, const char *str);
void minsketch_free(minsketch *m);
float minsketch_load_factor(minsketch *m);

#endif