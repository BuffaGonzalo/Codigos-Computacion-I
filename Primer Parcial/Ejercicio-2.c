//BUFFA GONZALO - PUNTO 2

//INCLUIMOS LAS LIBRERIAS A UTILIZAR
#include <stdio.h>
#include <math.h>
#define MAX 5

//PROTOTIPOS DE LAS FUNCIONES
void Cargar_Vector(float vector[MAX], int size);
void Generar_B(float vectorA[MAX], float vectorB[MAX],int size);
void Mostrar_Vector(float vector[MAX],int size);

int main(){
	//DEFINIMOS E INICIALIZAMOS LAS VARIABLES
	float vectorA[MAX], vectorB[MAX] = {};
	int size = 0;
	
	printf("------------------------- \n");
	printf("*** B[i] = A[i] ^ 3 ***\n");
	printf("------------------------- \n");
	
	do{
		printf("Ingrese la cantidad de numeros a procesar (Nro. Entero entre 1 y 5) \n");
		scanf("%i", &size);
	}while(size<=0 || size>5);
	
	Cargar_Vector(vectorA, size);
	
	//SECCION UTILIADA COMO TESTEO DE CARGA DE DATOS EN EL VECTOR
	/*
	int i=0;
	for(i;i<size;i++){
		printf(" VECTOR[%i] -> %f \n",i+1, vectorA[i]);
	}
	*/
	
	//GENERAMOS EL VECTOR B - CONTENIENDO EL CUBO DE LOS VALORES DE A
	Generar_B(vectorA,vectorB,size);
	
	//MOSTRAMOS LOS DOS VECTORES INVOCANDO LA FUNCIÓN DOS VECES
	printf("Vector A:");
	Mostrar_Vector(vectorA,size);
	printf("\n");
	printf("Vector B:");
	Mostrar_Vector(vectorB,size);
	
	return 0;
}

//PROCEDIMIENTO - CARGAR VECTOR (CARGA EL VECTOR A)
void Cargar_Vector(float vector[MAX], int size){
	int i = 0;
	printf("Cargar con datos el vector A \n");
	printf(">>> Ingreso de datos \n");
	for(i=0;i<size;i++){
		printf("Posicion %i:", i+1);
		scanf("%f", &vector[i]);
	}
}

//PROCEDIMEINTO - GENERAR B (CARGA EL VECTOR B CON EL CUBO DE LOS ELEMENTOS DE A)
void Generar_B(float vectorA[MAX], float vectorB[MAX],int size){
	int i = 0;
	for(i;i<size;i++){
		vectorB[i] = pow(vectorA[i],3);
	}
}

//PROCEDIMIENTO - MOSTRAR VECTOR (MUESTRA LOS VECTORES)
void Mostrar_Vector(float vector[MAX],int size){
	int i = 0;
	printf("[ \t");
	for(i=0;i<size;i++){
		printf("%f \t", vector[i]);
	}	
	printf("]");
}

