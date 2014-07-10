/**
 * Para COMPILAR: gcc hola_mundo_hilos.c -o algun_nombre -pthread
 *
 * Para CORRERLO:
 * 1.- Versión con sin semáforos:
 * 	./algun_nombre nombre_prueba sin_semaforo
 *
 * 2.- Versión con semáforos:
 * ./algun_nombre nombre_prueba semaforo
 *
 * NOTA1: se ejecuta de esta forma porque el main recibe parámetros, de hechos
 * son dos: el primero indica un nombre de persona cualquiera, el segundo es 
 * una variable condicional que indica si se crean los hilos con la versión que
 * contiene semáforos o la que no los posee.
 *
 * NOTA2: El orden de la creación de los hilos no indica el orden de ejecución,
 * si se ejecutan en ese orden es pura coincidencia. Para casos donde se requiera
 * mayor tiempo de cómputo se debe tener un control mediante semáforos.
 *
 * EXPLICACIÓN
 * En la versión sin semáforos no hay un control en la impresión de las veces de "hola"
 *  y "mundo", y se imprimen al azar.
 *
 * En cambio en la que tiene semáforo en el hilo "hola" el semaforo está inicializado en 1
 * y al hacer sem_wait pasa sin problemas, pero en el hilo "mundo" como está en cero el semáforo , se bloquea
 * al tratar de hacer sem_wait y puede entrar a impimir sólo en el momento en cuanto en el hilo "hola"
 * le hace sem_post aumenta la variable y el otro al verificar pasa y se ejecuta.
 * 
 *
 * TEORÍA
 * "sem_wait(&sem)": Decrementa el valor del semáforo "sem". Cabe descatar que
 * cuando el valor es <= 0 bloquea el hilo correspondiente
 * 
 * "sem_post"(&sem)": Incrementa el valor del semáforo "sem"
 */
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

#define TRUE 1

/**
 * Declaración de los procedimientos necesarios para los semáforos
 */
void* hola();
void* mundo(void *dato);
/**
 * Versiones que no usan semáforos
 */
void* hola_ss();
void* mundo_ss(void *dato);

//Otra función para otro hilo de prueba
void* otra_prueba(void * dato);

/**
 * Creando los semáforos
 */
sem_t sem_hola, sem_mundo;

int main(int num_args, char **vals){
	
	/**
	 * Inicializando los semáforos
	 *
	 * - El primer valor es la referencia a la  variable semáforo previamente declarada
	 * - El segundo valor indica con quien se va a compartir el semáforo, si "0" lo 
	 * compartimos con todos los hilos, para efectos del proyecto siempre se colocará en cero "0".
	 * 
	 * - El tercer valor indica el valor propio del semaforo. Recordar que cuando se hace
	 * un "sem_wait(&algun_semaforo)" estamos decrementando el valor de dicho semáforo, y cuando 
	 * llega a cero se bloquea el hilo; en el caso de "sem_post(&algun_semaforo)"  sucede lo contrario
	 * aumenta el valor del semáforo, si hay algún semáforo que se encuentre en cero y por ende 
	 * esté bloqueado, entonces al hacer la llamda lo desbloquea.
	 *
	 * NOTA: en Lenguaje C POSIX "sem_post" hace las veces del signal mencionado en la teoría.
	 *
	 * 
	 */
	sem_init(&sem_hola, 0, 1);
	sem_init(&sem_mundo, 0, 0);
	
	/**
	 * Creando identificadores de los hilos
	 *
	 * Cabe destacar que se pueden crearvectores de identificadores como cualquier otro 
	 * tipo de variable de C.
	 */
	pthread_t hilo_hola, hilo_mundo, hilo_otra_prueba;
	
	if (strcasecmp("semaforo",vals[2]) == 0){
		/**
		* "pthread_create(&tid,atrr, func,arg)"
		* 
		* "tid" : identificador del hilo. 
		* 
		* "atrr" : Son los atributos compartidos por los hilos. Siempre lo pasaremos NULL, porque no es
		* necesario configurar este atributo.
		* 
		* "func" : es el nombre de la función de tipo void*. La función debe retornar un void * y recibir 
		* un parámetro void * o no recibir ningún parámetro, en tal caso se debe  pasar NULL
		* 
		* "arg" : Es el argumento que recibe "func" ,los cuales deben ser del
		* del tipo genérico void*, en tal caso no exista dicho argumento debe pasarse NULL.
		* 	--Se debe hacer casting de las variables: (void *) nom_var
		* 	--Para casting devuela:
		* 		-(tipo_básico) var : se convierte en una variable de tipo_básico
		* 		-(tipo_básico*) var : se convierte en un array de tipo básico
		*/
		pthread_create(&hilo_hola, NULL, hola, NULL);
		pthread_create(&hilo_mundo, NULL, mundo, (void *) vals[1]);
	}else{
		pthread_create(&hilo_hola, NULL, hola_ss, NULL);
		pthread_create(&hilo_mundo, NULL, mundo_ss, (void *) vals[1]);
	}
	
	int valor = 12;
	// el hilo no recibe constantes como parámetros de funciones
	pthread_create(&hilo_otra_prueba, NULL, otra_prueba,(void*) &valor);
	
	///Haciendo que el hilo MAIN espere a que terminen los hilos que se han creado
	pthread_join(hilo_hola, NULL);
	pthread_join(hilo_mundo, NULL);
	pthread_join(hilo_otra_prueba, NULL);
	
	///Destruyendo los semáforos
	sem_destroy(&sem_hola);
	sem_destroy(&sem_mundo);
}

/**
 * Prueba de hilo con parámetro entero
 */
void* otra_prueba(void * dato){
	int item;

	item = *((int *)dato);

	int i;
	printf("\n");
	for ( i = 0; i < 3; ++i)
	{
		printf("Imprimiendo item de prueba %d\n",item );
	}
	printf("\n");
}


void* hola(){
	while(TRUE){
		sem_wait(&sem_hola);
			printf("Hola ");
			int i;
			for ( i = 0; i < 10000000; ++i)
			{
				/* Para hacer tiempo */
			}
		sem_post(&sem_mundo);
	}
}




void* mundo(void *dato){
	char *nombre;
	nombre = (char *)dato;
	while(TRUE){
		sem_wait(&sem_mundo);
			printf("mundo, %s.\n ",nombre);
			int i;
			for (i = 0; i < 10000000; ++i)
			{
				/* Para hacer tiempo */
			}
		sem_post(&sem_hola);
	}
}
//Versiones sin semáforos
void* hola_ss(){
	while(TRUE){
			printf("Hola ");
			int i;
			for ( i = 0; i < 10000000; ++i)
			{
				/* Para hacer tiempo */
			}
	}
}


void* mundo_ss(void *dato){
	char *nombre;
	nombre = (char *)dato;
	while(TRUE){
			printf("mundo, %s.\n ",nombre);
			int i;
			for (i = 0; i < 10000000; ++i)
			{
				/* Para hacer tiempo */
			}
	}
}






