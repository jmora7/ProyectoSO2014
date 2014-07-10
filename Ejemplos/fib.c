#include <stdio.h>
#include <pthread.h>

unsigned long int  sec[1000];

void* fib(void* n);
int  main(){
	unsigned long int  n ,i;
	
	for (i = 0; i < 1000; i++)
	{
		sec[i] = 0l;
	}
	
	scanf("%ld",&n);
	pthread_attr_t atrr;
	pthread_attr_init(&atrr);
	pthread_t num_id;
	
	
	pthread_create(&num_id, &atrr,fib, (void *)n);
	
	pthread_join(num_id,NULL);
	
	for (i = 0; i < n; i++)
	{
		printf("%ld\n",sec[i]);
	}
	
}

void* fib(void* n){
	unsigned long int  n_ = (unsigned long int )n;
	
	if(n_ < 2){
		if(n ==  0)
			sec[0] = 0;
		
		if(n_ == 1 )
			sec[1] = 1;
		
	}else {
		unsigned long int  n1 = n_-1, n2 = n_-2;
		pthread_attr_t atrr;
		pthread_attr_init(&atrr);/// atributos por defecto
		pthread_t num_id_1,num_id_2;
		
		if(!sec[n1]){// para que no se creen hilos innecesarios
			pthread_create(&num_id_1, &atrr,fib,(void *)n1);
			pthread_join(num_id_1,NULL);
		}
		
		if(!sec[n2]){// para que no se creen hilos innecesarios	
			pthread_create(&num_id_2, &atrr,fib,(void *)n2);
			pthread_join(num_id_2,NULL);
		}
		
		/**Llega a este punto hayan finalizado los hilos creados, sea
		 * la espera o por o por ambos, siempre y cuando los valores no
		 * hayan sido ya calculados. 
		 * */		
		sec[n_] = sec[n1] + sec[n2];
		
	}	
	pthread_exit(0);
}
