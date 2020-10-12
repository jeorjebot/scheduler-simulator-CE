#include "librerie_dati_funz.h"

void print_help(FILE* stream, int exit_code, char* program_name) {
	fprintf(stream, "Usage: %s [options] [inputfile ...]\n", program_name);
	fprintf(stream, 
			" -op --output-preemption	Scrive i risultati dello scheduler con preemption su file.\n"
			" -on --output-not-preemption	Scrive i risultati dello scheduler senza preemption su file.\n"
			" -i --input			Il file di input contentente la lista dei job.\n"
			" -q --quantum			La durata del quanto di tempo.\n");
	exit(exit_code);
}

void evaluate_input(int argc, char* argv[]) {
	const char* program_name = argv[0];
	int control = 0; //--> mi serve per vedere la seconda lettera di o, op, on
		
		
	const struct option long_options[] = {
		{ "output-preemption",       required_argument, NULL, '1' },
		{ "output-no-preemption",     required_argument, NULL, '2' },
		{ "input",    required_argument, NULL, 'i' },
		{ "quantum",     required_argument, NULL, 'q' },
		{ NULL,         no_argument, NULL, 0   }
	};

	int next_option; 

	const char* const short_options = "op:on:i:q:"; //lista delle short opt con relativi parametri

	if (argc == 1) { //ovvero se c'è solo il nome del programma e basta
		print_help(stderr, 1, program_name);
	}
    
	do{
	next_option = getopt_long(argc, argv, short_options, long_options, NULL);

		switch(next_option){
		    case '1':
						preemptive = fopen(optarg, "w"); 
						if(preemptive == NULL) {
							printf("\nstream log preemptive  errore apertura");
						}
					printf("\nstream log preemptive aperto");
			break;

		    case '2':
					not_preemptive = fopen(optarg, "w"); 
						if(not_preemptive == NULL) {
							printf("\nstream log not preemptive errore apertura");
						}
					printf("\nstream log not preemptive aperto");
			break;

		    case 'o':
		//case short opt:
	 		control = getopt_long(argc, argv, short_options, long_options, NULL); //int perchè la getopt da come output un int!!!
	
				if (control=='p') {
					//fai le cose di op
						preemptive = fopen(optarg, "w"); 
						if(preemptive == NULL) {
							printf("\nstream log preemptive  errore apertura");
						}
					printf("\nstream log preemptive aperto");
					break;
				}

				if (control=='n') {
					not_preemptive = fopen(optarg, "w"); 
						if(not_preemptive == NULL) {
							printf("\nstream log not preemptive errore apertura");
							exit(1);
						}
					printf("\nstream log not preemptive aperto");
					break;
				}
				else {
					print_help(stderr, 1, program_name);
					break;
				}
			break;

		    case 'i':
			file_in1 = fopen(optarg, "r");
				if(file_in1 == NULL) {
					printf("\nstream input 1 errore apertura");
					exit(1);
				}
					printf("\nstream input 1 aperto");
			file_in2 = fopen(optarg, "r");
				if(file_in2 == NULL) {
					printf("\nstream input 2 errore apertura");
					exit(1);
				}
					printf("\nstream input 2 aperto");
			break;
		    
		    case 'q':
			qnt=atoi(optarg); //converte la stringa in un numero intero
			break;

		    case '?':
			print_help(stderr, 1, program_name);
			break;

		    case -1:
			break;

		    default:
			abort();
		}
    }while(next_option != -1);
		
}

void evaluate_numbers(FILE* file_in1) {
	char c;
	do{
		c=fgetc(file_in1); //la funzione giusta è fgetc()
			if(c == 'j') {
				JOB_NUMBER++;
			}
			if(c == 'i') {
				INSTR_NUMBER++;
			}
	}while(c != EOF);
	
	fclose(file_in1);
}


void fill_data_array(struct job *DATA_JOB, struct instr *DATA_INSTR, struct count *COUNT, FILE *file_in2) {
	flag temp;	
	long int v=0; //contatore instr
	int j=0; //contatore job
	int var=0; 				
	int eof;//fscanf ritorna eof se lo legge
	int var_count=0;
	int length_max =0; //per il not preemptive, per la somma di instr di ogni job
	char c;
	
	//questo mega ciclo mi alloca l'array di job, di istruzioni, e l'array con il conto di istruzioni		
	fscanf(file_in2, "%c", &c); 
	do{  
		if(c=='j'){
			 
			var_count=0; //ad ogni job viene rinizializzato il contatore delle instr
			length_max =0; //e il contatore della somma lunghezze instr (per il not preemptive)

			fscanf(file_in2, "%c", &c); 
			fscanf(file_in2, "%d", &var); 

			DATA_JOB[j].id=var; 

			fscanf(file_in2, "%c", &c); 
			fscanf(file_in2, "%d", &var); 

			DATA_JOB[j].arrival_time=var; 

			DATA_JOB[j].job_state.name = new_st;
			j++;
		}
						 
		if(c=='i'){
		
			var_count++;
			COUNT[j-1].tot=var_count;
			COUNT[j-1].current=var_count;

			fscanf(file_in2, "%c", &c); 
			fscanf(file_in2, "%c", &c); 
				if (c=='0') {

					temp=not_block;
					DATA_INSTR[v].type_flag=temp;
				}
	
				if(c=='1'){

					temp=block;
					DATA_INSTR[v].type_flag=temp;
				}

			fscanf(file_in2, "%c", &c); 
			fscanf(file_in2, "%d", &var); 
							  
			DATA_INSTR[v].length=var; 

			length_max += var; //per not preemptive
			COUNT[j-1].length=length_max;   //somma la lunghezza di ogni istruzione, per sapere quanto ''costa'' un job
			
			fscanf(file_in2, "%c", &c); 
			fscanf(file_in2, "%d", &var); 
											  
			DATA_INSTR[v].io_max=var;  

			v++; 
		} 
		
		eof = fscanf(file_in2, "%c", &c);                     
							
	}while(eof != EOF);

	fclose(file_in2);	
}	


void fill_shared_memory(struct job *DATA_JOB, struct instr *DATA_INSTR, struct count *COUNT) {

	for (int i = 0; i < JOB_NUMBER; i++) {
		s_job[i] = DATA_JOB[i]; 		
	}

	for (long int i = 0; i < INSTR_NUMBER; i++) {
		s_instr[i] = DATA_INSTR[i]; 	 
	}

	for (int i = 0; i < JOB_NUMBER; i++) {
		s_count[i] = COUNT[i]; 		
	}

	//assegno la posizione della prima istruzione nell'array instr a ogni job (INSTR_LIST)
	for(int i = 0; i < JOB_NUMBER; i++) {

		if(i==0) { //la prima;
			tempp=0;
			s_job[i].instr_list=tempp;
		}
		
		if(i>0) {
			tempp+=s_count[i-1].tot;
			s_job[i].instr_list=tempp;
		}
	}
}

void reset_parameters() {
	current_instr=0;		
	current_job=0;
	cl0ck0=0;
	cl0ck1=0;
	tot_block0 = 0;
	tot_block1 = 0;
	current_block = 0;
	last_current_job = 0;
	alive=JOB_NUMBER;

}




