
#include "sp.h"

typedef struct{
	SP *dr_0;
	SP *dr_1;
	int curr_dr;
	int new_dr;
}DR;

void dr_config(DR* dr, SP *a, SP* b);
void dr_set( DR* dr,  int set_to );


