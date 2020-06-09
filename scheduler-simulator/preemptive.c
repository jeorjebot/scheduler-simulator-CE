#include "preemptive.h"

void print_log (struct log log_core) {
//preemptive
	
	long int cl0ck;
	int id = log_core.id_core;
	
		if(id==0) {
			cl0ck=cl0ck0;
		}
		if(id==1) {
			cl0ck=cl0ck1;
		}
	char *state;

		switch(log_core.name_state) {
			case new_st :
				printf("\nerrore print log, caso new");
				break;
			case ready_st :
				state="ready";
				break;
			case running_st :
				state="running";
				break;
			case blocked_st :
				state="blocked";
				break;
			case exit_st :
				state="exit";
				break;
			default:
				printf("\nerrore print log, caso default");
				break;

		}
	fprintf(preemptive, "\ncore%d, %ld, %d, %s", id,cl0ck, s_job[current_job].id,state);
}

void cl0ck(int id) {
//preemptive

	if(id == 0) {
	//printf("\nclock0, blocked = %d",tot_block);	
		cl0ck0++;
		for(int i=0; i < JOB_NUMBER; i++) {

			if(tot_block0 == 0) {
				break;
			}
	
			if((s_job[i].job_state.name == blocked_st) && (s_job[i].job_state.core_number == 0)) {//ovvero se è bloccato e ha core0
				//printf("\nBLOCKED: job = %d, core= %d, cicli di clock bloccati = %d",s_job[i].id, s_job[i].job_state.core_number, s_job[i].job_state.n_clock_block);				
				s_job[i].job_state.n_clock_block--; 
				if((s_job[i]).job_state.n_clock_block == 0) {
					s_job[i].job_state.name = ready_st; //qua c'era un [current_state] che poteva dare noie
					
					tot_block0--;

					fprintf(preemptive, "\ncore%d, %ld, %d, ready", id,cl0ck0, s_job[i].id);
					//printf("\njob %d sbloccato da core 0", s_job[i].id);
				}
			}
		}

	}	
	if(id == 1) {
	//printf("\nclock1, blocked = %d",tot_block);
		cl0ck1++;
		for(int i=0; i < JOB_NUMBER; i++) {

			if(tot_block1 ==0) {
				break;
			}

			if((s_job[i].job_state.name == blocked_st) && (s_job[i].job_state.core_number == 1)) {//ovvero se è bloccato e ha core1
				//printf("\nBLOCKED: job = %d, core= %d, cicli di clock bloccati = %d",s_job[i].id, s_job[i].job_state.core_number, s_job[i].job_state.n_clock_block);
				s_job[i].job_state.n_clock_block--;

				if((s_job[i]).job_state.n_clock_block == 0) {
					s_job[i].job_state.name = ready_st;
					
					tot_block1--;				

					fprintf(preemptive, "\ncore%d, %ld, %d, ready", id,cl0ck1, s_job[i].id);
					//printf("\njob %d sbloccato da core 1",s_job[i].id);
				}
			}
		}

	}


}

int job_search(int id) {
//preemptive
	if(id == 0) {
		if(s_job[current_job].id == 0) { //è il primo 
			do{
				cl0ck(id);
			}while(s_job[current_job].arrival_time > cl0ck0);
			
			return current_job;

		} 
		else
		
			do{
				for(int i=0; i<=current_job; i++) { //cerca il primo new o eseguibile
					//printf("\njob search job=%d, i=%d, clock0=%ld",s_job[i].id,i, cl0ck0);
					if(((s_job[i].job_state.name == new_st) || (s_job[i].job_state.name == ready_st)) && (s_job[i].arrival_time <= cl0ck0)){
					//ovvero se il job è in new o ready ed è eseguibile
					return i;
					}
				
				}
				cl0ck(id);
			
			}while(1); //continua a iterare e aumenta il cl0ck: prima o poi troverà un'struzione che arriva!				
	}
		


	if(id == 1) {
		if(s_job[current_job].id == 0) { //è il primo 
			do{
				cl0ck(id);
			}while(s_job[current_job].arrival_time > cl0ck1);
			
			return current_job;

		} 
		else
		
			do{
				for(int i=0; i<=current_job; i++) { //cerca il primo new o eseguibile
					if(((s_job[i].job_state.name == new_st) || (s_job[i].job_state.name == ready_st)) && (s_job[i].arrival_time <= cl0ck1)){
					//ovvero se il job è in new o ready ed è eseguibile
					return i;
					}
				}

				cl0ck(id);

			}while(1); //continua a iterare e aumenta il cl0ck: prima o poi troverà un'struzione che arriva!
	}

printf("\nerrore nella job_search");
return -1;
}



void* scheduler(void* arg) { 
//preemptive
	do{
	
		rc = pthread_mutex_lock (&job_queue_mutex);
			if(rc!=0) {//controllo lock
				printf( "pthread_mutex_lock failed %d - Quit\n", rc); 
				exit(-1);
			}

		int id = *((int*) arg); //lo ha fatto anche aonzo in ese5 array_sum --> casting da void* a int* e dereference
		
		log_core.id_core=id;

		if(id == 0) { //siamo nel core0

		
			if(current_job == 2048) { //per REINIZIARE
				

				for(int i=0; i<current_job; i++) {//i<current_job perchè current_job == 2048
					if(s_job[i].job_state.name == ready_st) { //reinizia il ciclo dal primo che trova ib ready
						current_job=i;
						break;
					}
				}
			}


			if(s_job[current_job].arrival_time > cl0ck0) { //se non è disponibile, ne cerca uno disponibile o lo rende tale
				current_job=job_search(id); //ora è valido, è un new o un ready					
			}
		}

		if(id == 1) { //siamo nel core1
			
			if(current_job == 2048) { //per REINIZIARE
			

				for(int i=0; i<current_job; i++) {//i<current_job perchè current_job == 2048
					if(s_job[i].job_state.name == ready_st) { //reinizia il ciclo dal primo che trova ib ready
						current_job=i;
						break;
					}
				}
			}


			if(s_job[current_job].arrival_time > cl0ck1) { //se non è disponibile, ne cerca uno disponibile o lo rende tale
				current_job=job_search(id); //ora è valido, è un new o un ready					
			}
		}

			
		do{
			current_state.name = s_job[current_job].job_state.name;			

			switch(current_state.name) {

				case new_st :
					//printf("\nsono in new");
					s_job[current_job].job_state.name = ready_st;
	
					cl0ck(log_core.id_core);
	
					log_core.name_state = ready_st; 		//cambia da new a ready 
					print_log(log_core);				//cl0ck++

					quantum = qnt;//inizializza quanto di tempo, per non farlo uscire dal while							

					break;


				case ready_st :

					if(alive==1) { //last job
						last_current_job = current_job;
					}

					//printf("\nsono in ready");
					s_job[current_job].job_state.name = running_st;	//cambia da ready a running	
											//cl0ck++
					cl0ck(log_core.id_core);			//parte il quanto di tempo

					log_core.name_state = running_st;
					print_log(log_core); 
			
					quantum = qnt;//inizializza quanto di tempo;
					break;

				case running_st :
					//hanno già la posizione della prima istruzione dentro s_job[current_job].instr_list (è un long int);
					//quello che devono fare è diminuire la s_count[current_job].current
					//in fondo controllare ovviamente che l'ultima instr abbia length != 0

					//istruzione corrente --> job[...].instr + (tot - current);
					
					current_instr = (s_job[current_job].instr_list + (s_count[current_job].tot - s_count[current_job].current));
					
					if(s_instr[current_instr].length < 0) {
						printf("job = %d, instr = %ld, count tot =%d err=%d", current_job, current_instr,s_count[current_job].tot, s_instr[current_instr].length);
					
						exit(-1);
					}

					if (s_instr[current_instr].type_flag == block) { //istruzione bloccante

						if(log_core.id_core == 0) {
						tot_block0++;
						}

						if(log_core.id_core == 1) {
						tot_block1++;
						}

						srand(time(NULL));								

						s_job[current_job].job_state.name = blocked_st;
						s_instr[current_instr].type_flag = not_block; //cambio tipo all'istruzione, per quando si sbloccherà
			
				
						current_block = (rand() % s_instr[current_instr].io_max) + 1; //calcolo cicli di blocco
						
						s_job[current_job].job_state.core_number = log_core.id_core; //indica che è stato bloccato dal core0
						s_job[current_job].job_state.n_clock_block = current_block; //indica la quantità di cicli di blocco
					
						cl0ck(log_core.id_core);			//posiziona qua perchè è più corretto, si presta meno ad errori					
					
						log_core.name_state = blocked_st; 		//running --> blocked		
						print_log(log_core);

						current_job++;

							if(alive==1) { //last job
								current_job = last_current_job;
								do{
							cl0ck(log_core.id_core); //clocca per sbloccarlo

								}while(s_job[last_current_job].job_state.name != ready_st); 
							}
						quantum = 0; //quanto di tempo = 0 così esce dal ciclo
										
						break;
					}	
				
					s_instr[current_instr].length--;
					
					if(s_instr[current_instr].length == 0) { //decrementa di 1 le istruzioni "vive" del job
						s_count[current_job].current--;
					}
					
					if(s_count[current_job].current == 0) {//allora significa che il job è terminato --> lo sposto in exit	
														
						s_job[current_job].job_state.name = exit_st;
			
						cl0ck(log_core.id_core);

						log_core.name_state = exit_st; 		//running --> exit	
						print_log(log_core);


						alive--; //ogni volta che un job va in exit, decremento alive;
							
						
						current_job++;
						quantum=0;

						break;

					}
				
					quantum--; //decrementa il quanto di tempo

					if(quantum == 0 && (s_job[current_job].job_state.name == running_st)) { //timeout, accade quando il quanto è a 0
													//e il job ancora in running
						
						s_job[current_job].job_state.name = ready_st;
	
						cl0ck(log_core.id_core);

						log_core.name_state = ready_st; 		//running --> ready
						print_log(log_core);

						current_job++;
						
							if(alive==1) { //last job
							current_job = last_current_job;
							}		
						break;
					}
			
					cl0ck(id);

					break;

				case blocked_st :
					if(alive < 5 && (tot_block0>0 || tot_block1>0)) { 
						cl0ck(log_core.id_core);
					}
					current_job++;
					break;
				case exit_st :
					if(alive < 5 && (tot_block0>0 || tot_block1>0)) {
						cl0ck(log_core.id_core);
					}
					current_job++;
					break;
				default :
					perror("error switch scheduler core 0");
					break;	
			}

		}while(quantum != 0); 
			
		rc = pthread_mutex_unlock (&job_queue_mutex);
			if(rc!=0) {//controllo unlock
				printf( "pthread_mutex_unlock failed %d - Quit\n", rc); 
				exit(-1);
			}
	}while(alive!=0);
	
	return 0;
}

