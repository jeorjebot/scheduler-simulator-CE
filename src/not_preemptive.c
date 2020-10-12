#include "not_preemptive.h"

void print_log_np (struct log log_core) {
//not preemptive
	
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
	
	fprintf(not_preemptive, "\ncore%d, %ld, %d, %s", id,cl0ck, s_job[current_job].id,state);
}

void cl0ck_np(int id) {
//not preemptive

	if(id == 0) {
	//printf("\nclock0, blocked = %d",tot_block);	
		cl0ck0++;
		for(int i=0; i < JOB_NUMBER; i++) {

			if(tot_block0 == 0) {
				break;
			}

	
			if((s_job[i].job_state.name == blocked_st) && (s_job[i].job_state.core_number == 0)) {//ovvero se è bloccato e ha core0
								
				s_job[i].job_state.n_clock_block--; 

				if((s_job[i]).job_state.n_clock_block == 0) {
					s_job[i].job_state.name = ready_st; 
					
					tot_block0--;

					fprintf(not_preemptive, "\ncore%d, %ld, %d, ready", id,cl0ck0, s_job[i].id);
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
				
				s_job[i].job_state.n_clock_block--;

				if((s_job[i]).job_state.n_clock_block == 0) {
					s_job[i].job_state.name = ready_st;
					
					tot_block1--;				

					fprintf(not_preemptive, "\ncore%d, %ld, %d, ready", id,cl0ck1, s_job[i].id);
				}
			}
		}

	}


}


int job_search_np(int id) {	
	
	int pos=0;
	int min=0;
	flag js;
	
	if(id==0) {
		
		do {

			for(int i=0;((s_job[i].arrival_time <= cl0ck0) && (i < JOB_NUMBER)); i++) {
				//printf("\n entrato i=%d",i);		
				js = block;


				if(s_job[i].job_state.name == new_st) {
					//printf("\nprimo min new");
					pos = i;
					min = s_count[i].length;
					js = not_block;
				}

		
				if(s_job[i].job_state.name == ready_st) {
					//printf("\nprimo min ready");
					pos = i;
					min = s_count[i].length;
					js = not_block;
				}

				if(js == not_block){ //quindi se c'è un primo minore da cui partire
					for(int j=i; ((s_job[j].arrival_time <= cl0ck0) && (j < JOB_NUMBER)); j++) {


						if((s_job[j].job_state.name == new_st) && (s_count[j].length < min)){
							//printf("\naltro min new");
							pos = j;
							min = s_count[j].length;
						}
		
						if((s_job[j].job_state.name == ready_st) && (s_count[j].length < min)) {
							//printf("\naltro min ready");
							pos = j;
							min = s_count[j].length;
						}
					}
					
					return pos;

				}
			}
		
			cl0ck_np(id); //continua a mandare clock finchè non entra nel ciclo sopra

		}while(1);
	}				


	if(id==1){
		
		do {
			for(int i=0;((s_job[i].arrival_time <= cl0ck1) && (i < JOB_NUMBER)); i++) {
						//printf("\n entrato i=%d",i);		
				js = block;


				if(s_job[i].job_state.name == new_st) {
					//printf("\nprimo min new");
					pos = i;
					min = s_count[i].length;
					js = not_block;
				}

		
				if(s_job[i].job_state.name == ready_st) {
					//printf("\nprimo min ready");
					pos = i;
					min = s_count[i].length;
					js = not_block;
				}

				if(js == not_block){ //quindi se c'è un primo minore da cui partire
					for(int j=i; ((s_job[j].arrival_time <= cl0ck1) && (j < JOB_NUMBER)); j++) {


						if((s_job[j].job_state.name == new_st) && (s_count[j].length < min)){
							//printf("\naltro min new");
							pos = j;
							min = s_count[j].length;
						}
		
						if((s_job[j].job_state.name == ready_st) && (s_count[j].length < min)) {
							//printf("\naltro min ready");
							pos = j;
							min = s_count[j].length;
						}
					}
					
					return pos;
				}
			}
			
			cl0ck_np(id); //continua a mandare clock finchè non entra nel ciclo sopra

		}while(1);
	}				

		
	
}
	


int check_blocked(int id) {
//controlla che gli scheduler possano sempre eseguire job	
//"se è il turno di core0, ma cè solo un job, ed è bloccato da core1, core0 non può lavorare" --> scene di vita vissuta	
	if(alive==0) { //fine scheduler
		return 2;
	}
	
	if(id==0) {
		if((alive-tot_block1)==0) { 
			return 1;
		}
	}
	
	if(id==1) {
		if((alive-tot_block0)==0) { 
			return 1;
		}	
	}
	
	return 0;
}
	
	
	
void* scheduler_np(void* arg) { 
//not preemptive

	int id=0;
	int check=0;

	do {
		rc = pthread_mutex_lock (&job_queue_mutex);
			if(rc!=0) {//controllo lock
				printf( "pthread_mutex_lock failed %d - Quit\n", rc); 
				exit(-1);
			}
		
		id = *((int*) arg); //casting da void* a int* e dereference	
		log_core.id_core=id;	
		check = check_blocked(id);	
		
		do {
		
			
			if(check == 2) {
				return 0;
			}
			if(check == 1) { //se check è a 1 devo bloccare questo core e sbloccare l'altro, perchè solo l'altro può lavorare
				rc = pthread_mutex_unlock (&job_queue_mutex);
					if(rc!=0) {
						printf( "pthread_mutex_unlock failed %d - Quit\n", rc); 
						exit(-1);
					}
					
				usleep(100000); //0.1 sec
				
				rc = pthread_mutex_lock (&job_queue_mutex);
					if(rc!=0) {
						printf( "pthread_mutex_lock failed %d - Quit\n", rc); 
						exit(-1);
					}
			}
			
			id = *((int*) arg); //ricalcolo variabili dopo uscita dal lock
			log_core.id_core=id;	
			check = check_blocked(id);
			
		}while(check!=0);
		
		//controllo alive==0;

		current_job=job_search_np(id); //ricerca nuovo job da schedulare
		current_state.name = s_job[current_job].job_state.name;			

		switch(current_state.name) {

			case new_st :
			//	printf("\nsono in new");
				s_job[current_job].job_state.name = ready_st;

				cl0ck_np(log_core.id_core);

				log_core.name_state = ready_st; 		//cambia da new a ready 
				print_log_np(log_core);				//cl0ck++
				break;


			case ready_st :


			//	printf("\nsono in ready");
				s_job[current_job].job_state.name = running_st;	//cambia da ready a running	
										//cl0ck++
				cl0ck_np(log_core.id_core);			//parte il quanto di tempo

				log_core.name_state = running_st;
				print_log_np(log_core); 
				//break;

			case running_st :

				do {
					//printf("\nsono in running");
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
						//printf("\ninstr bloccante");
						if(log_core.id_core == 0){
						tot_block0++;
						}

						if(log_core.id_core == 1){
						tot_block1++;
						}

						srand(time(NULL));								

						s_job[current_job].job_state.name = blocked_st;
						s_instr[current_instr].type_flag = not_block; //cambio tipo all'istruzione, per quando si sbloccherà
		
			
						current_block = (rand() % s_instr[current_instr].io_max) + 1; //calcolo cicli di blocco
						
						s_job[current_job].job_state.core_number = log_core.id_core; //indica che è stato bloccato dal core0
						s_job[current_job].job_state.n_clock_block = current_block; //indica la quantità di cicli di blocco
				
						cl0ck_np(log_core.id_core);			//posiziona qua perchè è più corretto, si presta meno ad errori					
				
						log_core.name_state = blocked_st; 		//running --> blocked		
						print_log_np(log_core);										
						break;
					}	
			
					s_instr[current_instr].length--;
					s_count[current_job].length--; //serve per job_search
					

					if(s_instr[current_instr].length == 0) { //decrementa di 1 le istruzioni "vive" del job
						s_count[current_job].current--;
					}
				
					if(s_count[current_job].current == 0) {//allora significa che il job è terminato --> lo sposto in exit	
											
						s_job[current_job].job_state.name = exit_st;
		
						cl0ck_np(log_core.id_core);

						log_core.name_state = exit_st; 		//running --> exit	
						print_log_np(log_core);

						alive--; //ogni volta che un job va in exit, decremento alive;							
						break;

					}
				
					cl0ck_np(id);
	
				}while(1); //continua finchè non interviene un break a interrompere l'idillio

				break;

			case blocked_st :
				printf("\nsono in blocked, ERRORE");
				exit(-1);
				break;
			case exit_st :
				printf("\nsono in exit, ERRORE");
				exit(-1);
				break;
			default :
				perror("error switch scheduler core 0");
				break;	
		
		}//fine dello switch

			
		rc = pthread_mutex_unlock (&job_queue_mutex);
			if(rc!=0) {//controllo unlock
				printf( "pthread_mutex_unlock failed %d - Quit\n", rc); 
				exit(-1);
			}
		//pthread_exit(0);
	}while(alive!=0);
		
	printf("\n sono dal return 0, alive = %d", alive);
	return 0;
}

