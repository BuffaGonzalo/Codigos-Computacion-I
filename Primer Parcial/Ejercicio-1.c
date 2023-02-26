//BUFFA GONZALO - PUNTO 1

//INCLUIMOS LAS LIBRERIAS A UTILIZAR
#include <stdio.h>
#include <math.h>

//PROTOTIPOS DE LAS FUNCIONES
float Perimetro(float x1, float y1,float x2, float y2, float x3, float y3);

int main(){
	//DEFINIMOS E INICIALIZAMOS LAS VARIABLES
	float x1,y1,x2,y2,x3,y3 = 0;
	int i = 0;
	
	//ESTRUCTURA REPETITIVA LA CUAL PIDE LAS COORDENADAS DEL TRIANGULO
	printf("CALCULO DEL PERIMETRO DE UN TRIANGULO DADOS 3 PUNTOS \n");
	for (i=0;i<3;i++){
		printf("*** %i \n", i+1);
		//PRIMER COORDENADA
		printf("Digite el valor de X1: \n");
		scanf("%f", &x1);
		printf("Digite el valor de Y1: \n");
		scanf("%f", &y1);
		printf("\n");
		//SEGUNDA COORDENADA
		printf("Digite el valor de X2: \n");
		scanf("%f", &x2);
		printf("Digite el valor de Y2: \n");
		scanf("%f", &y2);
		printf("\n");
		//TERCER COORDENADA
		printf("Digite el valor de X3: \n");
		scanf("%f", &x3);
		printf("Digite el valor de Y3: \n");
		scanf("%f", &y3);
		printf("\n");
		//IMPRIMIMOS LAS RESPECIVAS COORDENADAS Y EL PERIMETRO DEL TRIANGULO
		printf("Triangulo: a(%f,%f) b(%f,%f), c(%f,%f) - PERIMETRO = %f \n", x1,y1,x2,y2,x3,y3,Perimetro(x1,y1,x2,y2,x3,y3));
		printf("\n");
	}	
	return 0;
}

//FUNCIÓN - PERIMETRO (CALCULA EL PERIMETRO EN BASE A LAS COORDENADAS)
float Perimetro(float x1, float y1,float x2, float y2, float x3, float y3){
	float perimetro = 0;
	perimetro = sqrt(pow(x2-x1,2)+pow(y2-y1,2))+sqrt(pow(x3-x2,2)+pow(y3-y2,2))+sqrt(pow(x3-x1,2)+pow(y3-y1,2));	
	return perimetro;
}

