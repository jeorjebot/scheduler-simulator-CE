#ifndef NOT_PREEMPTIVE_H
#define NOT_PREEMPTIVE_H

#include "librerie_dati_funz.h"

//funzioni not preemptive
void print_log_np (struct log log_core);
void cl0ck_np(int id);
int job_search_np(int id);

//not preemptive
void* scheduler_np(void* arg);
	
#endif
