#include "librerie_dati_funz.h"
#include "preemptive.h"
#include "not_preemptive.h"

FILE *preemptive;
FILE *not_preemptive;
FILE *file_in1;
FILE *file_in2;

int JOB_NUMBER=0;
long int INSTR_NUMBER=0;

struct count *s_count=NULL; //tutti i count sono da cambiare
struct job *s_job=NULL;		
struct instr *s_instr=NULL;
struct state current_state;
struct log log_core;

pthread_t core0; //core0
pthread_t core1; //core1
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

int qnt=0; //quanto di tempo --> devo fare un casting e trasformarlo in int
int tot_block0=0;
int tot_block1=0;
int current_job=0;
int last_current_job=0;
int current_block=0;
int alive =0; //ogniqualvolta un job va in exit, alive decrementa
int quantum=0; //è il quanto di tempo. si attiva in ready, viene decrementato in running
int rc=0; //per il controllo mutex


long int current_instr=0;
long int tempp=0; //assegnare a job.instr_list il puntatore alla prima istruzione è stato un fallimento. faremo alla maniera più lunga

long int cl0ck0=0;
long int cl0ck1=0;


//------------------------------------- MAIN ----------------------------------------------------------------------

int main(int argc, char* argv[]) {

	pid_t pid_master = 0;
	pid_t pid_preemptive = 0;	
	pid_t pid_not_preemptive = 0;
	
	int status;

	key_t KEY_job = 1234; //mem job
	key_t KEY_instr = 5678; //mem instr
	key_t KEY_count = 9753; //mem count
	
	evaluate_input(argc, argv);

	evaluate_numbers(file_in1); //calcola JOB_NUMBER && INSTR_NUMBER
	
	//qua deve calcolare le due SHMSZ 
	int SHMSZ_job = JOB_NUMBER*sizeof(struct job);
	int SHMSZ_count = JOB_NUMBER*sizeof(struct count);					
	long int SHMSZ_instr = INSTR_NUMBER*sizeof(struct instr);


	printf("\nMAIN: master fork");
	printf("\npid = %d", getpid());
	fflush(stdout);
	
	pid_master = fork();

	if(pid_master == 0){
		
//-------------- MASTER -----------------------------------------------------------------------------------------------------------
		struct job *DATA_JOB; 
		struct instr *DATA_INSTR;
		struct count *COUNT;

		DATA_JOB=(struct job*)malloc(JOB_NUMBER*sizeof(struct job));
		DATA_INSTR=(struct instr*)malloc(INSTR_NUMBER*sizeof(struct instr)); 
		COUNT=(struct count*)malloc(JOB_NUMBER*sizeof(struct count)); 
		
		int shmid_job; //MEMORIA JOB
		int shmid_instr; //MEMORIA ISTRUZIONI
		int shmid_count; //MEMORIA couNT
		
		struct job *shm_job; 
		struct instr *shm_instr;
		struct count *shm_count;

		fill_data_array(DATA_JOB, DATA_INSTR, COUNT, file_in2); //in librerie.c


		//MEMORIA CONDIVISA MASTER
		if ((shmid_job = shmget(KEY_job, SHMSZ_job, IPC_CREAT | 0666)) < 0) { 
				perror("shmget error job mem");
			exit(1);
		}
		shm_job = shmat(shmid_job, 0, 0);

		if (shm_job == (struct job *)(-1)) {
			perror("shmat error job mem");
		}
		s_job = shm_job;

		if ((shmid_instr = shmget(KEY_instr, SHMSZ_instr, IPC_CREAT | 0666)) < 0) {
				perror("shmget error instr mem");
			exit(1);
		}
		shm_instr = shmat(shmid_instr, 0, 0);

		if (shm_instr == (struct instr *)(-1)) {
			perror("shmat error instr mem");
		}
		s_instr = shm_instr;

		if ((shmid_count = shmget(KEY_count, SHMSZ_count, IPC_CREAT | 0666)) < 0) {
				perror("shmget error count mem");
			exit(1);
		}
		shm_count = shmat(shmid_count, 0, 0);

		if (shm_count == (struct count *)(-1)) {
			perror("shmat error count mem");
		}
		s_count = shm_count; 

		//riempimento memoria condivisa preemptive
		fill_shared_memory(DATA_JOB, DATA_INSTR, COUNT);

		while(s_job[JOB_NUMBER-1].id != 666) {			
			sleep(1);					
		}	

		//riempimento memoria condivisa  not preemptive
		fill_shared_memory(DATA_JOB, DATA_INSTR, COUNT);


		while(s_job[JOB_NUMBER-1].id != 666) {			
		sleep(1);			
		}
	
		free(DATA_JOB); //deallocazione array dinamici
		free(DATA_INSTR);
		free(COUNT);
	
		shmctl(shmid_instr, IPC_RMID, 0);
		shmctl(shmid_job, IPC_RMID, 0); 
		shmctl(shmid_count, IPC_RMID, 0);

		
		printf("\nMASTER: memoria condivisa deallocata");
			exit(0);

	}

	if(pid_master > 0) {
		printf("\n\nMAIN: fork preemptive");
		fflush(stdout);

		pid_preemptive=fork();
	}

//-------------------------PREEMPTIVE-------------------------------------------
	if(pid_preemptive == 0) {

		sleep(4);
		
		int shmid_job; //MEMORIA JOB
		int shmid_instr; //MEMORIA ISTRUZIONI
		int shmid_count; //MEMORIA couNT
		
		struct job *shm_job; 
		struct instr *shm_instr;		
		struct count *shm_count;
			
		int params0=0;
		int params1=1;
		int err;

		alive=JOB_NUMBER;
		
		//MEMORIA CONDIVISA PREEMPTIVE
		if ((shmid_job = shmget(KEY_job, SHMSZ_job, 0666)) < 0) { 
				perror("shmget error job mem pre");
			exit(1);
		}
		shm_job = shmat(shmid_job, 0, 0);

		if (shm_job == (struct job *)(-1)) {
			perror("shmat error job mem pre");
		}
		s_job = shm_job; 

		if ((shmid_instr = shmget(KEY_instr, SHMSZ_instr, 0666)) < 0) {
				perror("shmget error instr mem pre");
			exit(1);
		}
		shm_instr = shmat(shmid_instr, 0, 0);

		if (shm_instr == (struct instr *)(-1)) {
			perror("shmat error instr mem pre");
		}
		s_instr = shm_instr; 

		if ((shmid_count = shmget(KEY_count, SHMSZ_count, 0666)) < 0) {
				perror("shmget error count mem pre");
			exit(1);
		}	
		shm_count = shmat(shmid_count, 0, 0);

		if (shm_count == (struct count *)(-1)) {
			perror("shmat error count mem pre");
		}
		s_count = shm_count; 

		//chiamo i thread
		do{ 
			if(s_job[JOB_NUMBER-1].id == JOB_NUMBER-1) { //attende che la memoria sia caricata
				break;
			}

		}while(1);

		//core 0 --> thread
		printf("\nPREEMPTIVE: thread %d partito ",params0); fflush(stdout);
		err = pthread_create(&(core0), NULL, scheduler, (void*)&(params0));
		

			if (err != 0) {
					perror("\n\nerrore creazione thread core 0");
				exit(1);
			}

		//core 1 --> thread
		printf("\nPREEMPTIVE: thread %d partito ",params1); fflush(stdout);
		err = pthread_create(&(core1), NULL, scheduler, (void*)&(params1));


			if (err != 0) {
					perror("\n\nerrore creazione thread core 1");
				exit(1);
			}
					
		pthread_join (core0, NULL);
		printf("\nPREEMPTIVE: thread 0 terminato");
		pthread_join (core1, NULL);		
		printf("\nPREEMPTIVE: thread 1 terminato"); fflush(stdout);

		
		fclose(preemptive);

		s_job[JOB_NUMBER-1].id=666;

			do{ 
				sleep(1);
				if(s_job[JOB_NUMBER-1].id != 666) { //attende che la memoria sia caricata
					break;
				}

			}while(1);


		shmdt(shm_job);
		shmdt(shm_instr);
		shmdt(shm_count);
			
		exit(0);
	}
	
	if (pid_preemptive > 0) {
		printf("\n\nMAIN:attendo preemptive");
				
		waitpid(pid_preemptive, &status, 0);
		printf("\n\nMAIN: uscita del preemptive con stato: %d", status);
		printf("\n\nMAIN: fork not preemptive");
		fflush( stdout );

		pid_not_preemptive = fork();		
	}
	
//-------------------------NOT PREEMPTIVE-------------------------------------------
	if(pid_not_preemptive == 0) {

		reset_parameters();

		sleep(4);

		int shmid_job=0; //MEMORIA JOB
		int shmid_instr=0; //MEMORIA ISTRUZIONI
		int shmid_count=0; //MEMORIA couNT
		
		int params0=0;
		int params1=1;
		int err;

		struct job *shm_job;
		struct instr *shm_instr;
		struct count *shm_count;
		

		//MEMORIA CONDIVISA NOT PREEMPTIVE
		if ((shmid_job = shmget(KEY_job, SHMSZ_job, 0666)) < 0) { 
				perror("shmget error job mem not pre");
			exit(-1);
		}
		shm_job = shmat(shmid_job, 0, 0);

		if (shm_job == (struct job *)(-1)) {
			perror("shmat error job mem not pre");
		}
		s_job = shm_job; 

		if ((shmid_instr = shmget(KEY_instr, SHMSZ_instr, 0666)) < 0) {
				perror("shmget error instr mem  not pre");
			exit(1);
		}
		shm_instr = shmat(shmid_instr, 0, 0);

		if (shm_instr == (struct instr *)(-1)) {
			perror("shmat error instr mem not pre");
		}
		s_instr = shm_instr; 

		if ((shmid_count = shmget(KEY_count, SHMSZ_count, 0666)) < 0) {
				perror("shmget error count mem not pre");
			exit(1);
		}	
		shm_count = shmat(shmid_count, 0, 0);

		if (shm_count == (struct count *)(-1)) {
			perror("shmat error count mem not pre");
		}
		s_count = shm_count; 
			
			
		//thread		
		do{ 
			if(s_job[JOB_NUMBER-1].id == JOB_NUMBER-1) { //attende che la memoria sia caricata
				break;
			}

		}while(1);

		//core 0 --> thread
		printf("\nNOT PREEMPTIVE: thread %d partito ",params0); fflush(stdout);
		err = pthread_create(&(core0), NULL, scheduler_np, (void*)&(params0));
		

			if (err != 0) {
					perror("\n\nerrore creazione thread core 0");
				exit(1);
			}


		//core 1 --> thread
		printf("\nNOT PREEMPTIVE: thread %d partito ",params1); fflush(stdout);
		err = pthread_create(&(core1), NULL, scheduler_np, (void*)&(params1));


			if (err != 0) {
					perror("\n\nerrore creazione thread core 1");
				exit(1);
			}
						
		pthread_join (core0, NULL);
		printf("\nNOT PREEMPTIVE: thread 0 terminato");
		pthread_join (core1, NULL);		
		printf("\nNOT PREEMPTIVE: thread 1 terminato"); fflush(stdout);
		
		fclose(not_preemptive); 

		s_job[JOB_NUMBER-1].id=666;

		exit(0);
	}
//----------------------------------------------------------------------------------

	if(pid_not_preemptive > 0) {
		fflush(stdout);
		printf("\n\nMAIN:attendo not preemptive");
		waitpid(pid_not_preemptive, &status, 0);
		printf("\nMAIN:not preemptive terminato con status %d", status);

		fflush(stdout);
		printf("\n\nMAIN:attendo master");	
		waitpid(pid_master, &status, 0);
		printf("\nMAIN:master terminato con status %d", status);
		
		printf("\n\n\nproudly powered by Giorgio & Camilla\n");

	}
  
	return 0;
}


