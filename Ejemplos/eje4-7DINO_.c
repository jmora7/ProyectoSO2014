#include <stdio.h>
#include <pthread.h>

int value = 0;

void* runner(void* p);
int main(int argc, char **argv){	
	int pid;
	pthread_t tid;
	pthread_attr_t at;
	printf("Antes del fork!!\n");
	
	pid = fork();// creción de nuevo proceso
	 
	printf("Despues del fork!!\n"); 
	
	if(pid == 0){
		pthread_attr_init(&at);
		pthread_create(&tid, & at, runner, NULL);
		pthread_join(tid,NULL);
		printf("HIJO : valor = %d \n",value);///linea A
		return -1;
	}else{
		if( pid > 0){
			wait(NULL); // el padre espera a que el hijo termine 
			printf("PADRE : valor = %d \n",value);///Linea B	
		}
	}
	printf("\nen el medio!!!\n\n");
	
	int pp = fork();
	if(!pp)
	printf("Soy el hijo, id: %d \n",pp);
		else{
		wait(NULL);
		printf("Soy el padre id, %d\n",pp);
		}
		
	return 0;
}

void* runner(void *p){
	value = 5;
	pthread_exit(0);

}
