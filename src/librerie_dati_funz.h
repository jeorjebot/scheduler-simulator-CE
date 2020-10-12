#ifndef LIBRERIE_DATI_FUNZ_H
#define LIBRERIE_DATI_FUNZ_H

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //per usleep()
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
	
	
FILE *preemptive;
FILE *not_preemptive;
FILE *file_in1;
FILE *file_in2;

int JOB_NUMBER;
long int INSTR_NUMBER;

struct count *s_count; //tutti i count sono da cambiare
struct job *s_job;		
struct instr *s_instr;
struct state current_state;
struct log log_core;

pthread_t core0; //core0
pthread_t core1; //core1
pthread_mutex_t job_queue_mutex;

int qnt; //quanto di tempo --> devo fare un casting e trasformarlo in int
int tot_block0;
int tot_block1;
int current_job;
int last_current_job;
int current_block;
int alive; //ogniqualvolta un job va in exit, alive decrementa
int quantum; //è il quanto di tempo. si attiva in ready, viene decrementato in running
int rc; //per il controllo mutex


long int current_instr;
long int tempp; //assegnare a job.instr_list il puntatore alla prima istruzione è stato un fallimento. faremo alla maniera più lunga

long int cl0ck0;
long int cl0ck1;


//-------------

typedef enum {not_block, block} flag; // 0 not block --- 1 block
typedef enum {new_st,ready_st,running_st,blocked_st,exit_st} nome;

struct instr {
	flag type_flag;
	int length;
	int io_max; 
};

struct state {
	nome name; 
	int core_number;
	int n_clock_block;
};

struct job {
	int id;
	int arrival_time;
	long int instr_list; //numero della prima istruzione
	struct state job_state;
};


struct log {
	nome name_state;
	int id_core;
};


struct count {
	int tot;
	int current;
	int length;
};

void evaluate_input(int argc, char* argv[]);
void evaluate_numbers(FILE* file_in1);
void fill_data_array(struct job *DATA_JOB, struct instr *DATA_INSTR, struct count *COUNT, FILE *file_in2);
void fill_shared_memory(struct job *DATA_JOB, struct instr *DATA_INSTR, struct count *COUNT);
void reset_parameters();

#endif
