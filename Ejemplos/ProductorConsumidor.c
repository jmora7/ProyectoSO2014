/**
 * Problema clásico del próductor-consumidor
 * Ejercicio tomado del libro de. Sistemas Operativos
 * Una Visión Aplicada. Autor : Carretero y cols.
 * */
 
 
 
 /**
  * GESTIÓN DE LOS HILOS
  * 
  *  int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
  *                        void *(*start_routine) (void *), void *arg);
  * 	
  * "pthread_t"
  * Crea el identificador del hilo que posteriormente
  * sera pasado a la instrución "pthread_create"
  * 
  * 
  * 
  * "pthread_create(&tid,atrr, func,arg)"
  * 
  * "tid" : identificador del hilo
  * 
  * "atrr" : Son los atributos compartidos por los hilos**** BUSCAR
  * 
  * "func" : es el nombre de la función de tipo void*, si se desea pasar
  * una función normal se debe hacer el casting: (void *) &func
  * 
  * "arg" : son los argumentos que recibe "func" ,los cuales deben ser del
  * del tipo genérico void*.
  * 	--Se debe hacer casting de las variables: (void *) nom_var
  * 	--Para casting devuela:
  * 		-(tipo_básico) var : se convierte en una variable de tipo_básico
  * 		-(tipo_básico*) var : se convierte en un array de tipo básico
  * 
  * GESTIÓN DE SEMÁFOROS
  * 
  * "sem_t" : es el tipo de los semáforos
  * 
  * Signatura de sem_init, que inicializa un semáforo
  * 
  * "int sem_init(sem_t  *sem, int pshared, unsigned int value)"
  * 	--sem_t : es la variable tipo semáforo previamente declarada
  * 	
  * 	--pshared: si le pasa cero "0" el semáforo se comparte entre todos
  * 	
  * 	los hilos del proceso, un valor mayor cero es compartido por proceso.
  * 	En este caso se usará con el valor de cero.
  * 	
  * 	--value : Valor inicial del semáforo
  *
  * "sem_wait(&sem)": Decrementa el valor del semáforo "sem". Cabe descatar que
  * cuando el valor es <= 0 bloquea el hilo correspondiente
  * 
  * "sem_post"(&sem)": Incrementa el valor del semáforo "sem"
  * */ 
 
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#define long_buffer 5
#define true 1

/** Definición de las cabeceras de cada una de las funciones y
 * procedimientos
 * */
int generar_dato();
void *consumidor(void *nombre);
void *productor();


///PROGRAMA PRINCIPAL...
sem_t huecos, elementos, exclusion_mutua;

int buffer[long_buffer],num=0;
int main(){
	printf("Inicia ejecucion del programa \n____________\n");
	//Inicialización de los semáforos
	sem_init(&elementos, 0, 0);//número de elementos en el buffer
	sem_init(&huecos, 0, long_buffer);//número de espacios vacíos
	sem_init(&exclusion_mutua, 0, 1);// semaforo binario
	
	//Creación e inicialización de los hilos	 
	pthread_t hilo_consumidor, hilo_productor, otro_cons; 
	pthread_create(&hilo_productor, NULL, productor, NULL);
	pthread_create(&hilo_consumidor, NULL, consumidor, (void *) "gregori");
	pthread_create(&otro_cons, NULL, consumidor, (void *) "milexis");
	
	
	pthread_join(hilo_consumidor, NULL);
	pthread_join(hilo_productor, NULL);
	pthread_join(otro_cons, NULL);
	
	sem_destroy(&elementos);	
	sem_destroy(&huecos);	
	sem_destroy(&exclusion_mutua);
	
	return 0;
}

///DESARROLLO DE LAS FUNCIONES Y PROCEDIMIENTOS
/**
 * Generar Dato: Función que genera un número de forma aleatoria
 * */
 int generar_dato(){
	return random()%1024;
}

/**
 * Definición Productor
 * */
void *productor(){
	int item,pos_cons=0;
	while(true){
		item = generar_dato();	
		sem_wait(&huecos);
		sem_wait(&exclusion_mutua);
			printf("::Se ha producido el item : %d :: ",item);
			buffer[pos_cons] = item;
			pos_cons = (pos_cons+1)%long_buffer;
			num++;
			printf("---> %d \n",num);
			if(num == long_buffer){
				pos_cons = 0;
				printf("Productor bloqueado, el buffer esta lleno\n\n");
				num = 0;
			}
			//simular retardo
			system("sleep 0.002");
		sem_post(&exclusion_mutua);
		sem_post(&elementos);
	}
}

/**
 * Definición del consumidor
 * */
 void *consumidor(void *nombre){
	int item, pos_prod=0, cont=0;
	char *nom;
	nom = (char*)nombre;
	while(true){
		sem_wait(&elementos);
		sem_wait(&exclusion_mutua);			
			item = buffer[pos_prod];
			pos_prod = (pos_prod+1)%long_buffer;
			printf("\t<<%s::: Se ha consumido el item : %d >>\n",nom,item);
			
			cont++;
			if(cont == long_buffer){
				pos_prod = 0;
				printf("%s:: Consumidor bloqueado, no hay elementos en el buffer\n\n",nom);			
				cont = 0;
			}
			//simular retardo
			system("sleep 0.3");
		sem_post(&exclusion_mutua);
		sem_post(&huecos);
	}
}

 
 
