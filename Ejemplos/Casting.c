

#include <stdio.h>

void func(void *dato);

void*   func_r_func(char *cadena[]);


int main(int argc, char **argv)
{
	int  *a,a_, ve[2] = {2,4};
	
	int (*f_i)(int);//funcion f_i que retorna un entero y recibe un entero
	
	void (*f)(void *);//funcion f que retorna un void* y recibe un void*
	
	void *ap_void;// Apuntador a void*, a cualquier cosa
	a_ = 30;
	a = &a_;
	
	func(ve);

	f = func;
	
	int cc[3] = {33,44,55};
	f(cc);
	
	f_i = func_r_func(NULL);	
	ap_void = f_i; // magia de c, lo puede recibir,ya que es apuntador a cualquier cosa
	int result = f_i(3);///llamda a la func recibida

	printf("\nResult %d \n",result);
	return 0;
}

void func(void *dato){
	int *vec = (int*)dato,i;
		
	
	for (i = 0; i < 2; i++)
	{
		printf("valor %d \n",vec[i]);
	}
	
	int f2(){
		printf("Soy func interna");
		return 22;
		
		int f3(){
			printf("Tercer nivel");
			return 33;
		}
	}
	
}

void*  func_r_func(char *cadena[]){
	
	printf ("Entre a la funcion que retorna una funcion \n");
	int va2 = 5;
	
	//declarando variable que va a almacenar la funcion
	
	int f3(int val){
		int va3 = val+ va2 + 1;
		printf("Tercer nivel con este valor %d",va3);
		return 33;
	}
	
	return &f3;
	
}
