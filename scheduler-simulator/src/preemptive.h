#ifndef PREEMPTIVE_H
#define PREEMPTIVE_H

#include "librerie_dati_funz.h"

//funzioni preemptive:
void print_log (struct log log_core);
void cl0ck(int id);
int job_search(int id);

//preemptive:
void* scheduler(void* arg);
	
#endif
