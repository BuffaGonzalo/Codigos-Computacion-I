//Gonzalo Martín Buffa - Recuperatorio Computación I
//incluimos la librerías
#include <stdio.h> //libreria estandar de entrada y salida
#include <stdlib.h>
#include <locale.h> //librería utilizada para tener caracteres españoles
#include <string.h> 
#include <time.h> //libreria utilizada para obtener la hora actual

//punteros archivos
FILE *fd, *fp; 

//nombres de archivos
char archivoPrincipal[]="SMN.dat";
char archivoSecundario[]="misEstaciones.dat";

//estructura
typedef struct{
	char fecha[10];
	int hora;
	float temp;
	float hum;
	float pnm; 
	int dd; 
	int ff; 
	char nombre[50];
}datos;

//funciones principales
void carga();
void visualizar();
void cantidad();
void temperatura();

//funciones secundarias
void cabeceraTabla();
void datosTabla(datos datosMeteorologicos, int registro);
void cargaDatos(datos *datosMeteorologicos);
void mensajeError();

int main(){
	//MIS ESTACIONES BOLIVAR AERO y BUENOS AIRES OBSERVATORIO, TODO EN BASE A MIS REGISTROS
	
	//definimos las variables
	int opc;
	//palabras en español
	setlocale(LC_CTYPE,"spanish");
	//numeros en ingles
	setlocale(LC_NUMERIC, "en-US");
	
	//menu utilizado para manipulación de datos meteorologicos
	do{
		do{
			system("cls"); //limpiamos la pantalla
			//mostramos el menu
			printf("\nMENÚ\n");
			printf("1. Visualizar registros de BOLIVAR AERO y BUENOS AIRES OBSERVATORIO \n");		
			printf("2. Cantidad de registros de BOLIVAR AERO y BUENOS AIRES OBSERVATORIO\n");
			printf("3. Temperaturas Mínima y Máxima de BOLIVAR AERO y BUENOS AIRES OBSERVATORIO\n");			
			printf("4. Salir\n");
		
			//pedimos al usuario que opción quiere elegir, entre 1 y 5
			do{
					printf("Opcion: \t");
					fflush(stdin);
					scanf ("%i", &opc);		
			}while(opc<1 || opc>4);		
	
			//Dependiendo de la elección del usuario, realizamos diferentes tareas
			switch(opc){
				case 1:
					visualizar();
					system("pause");
				break;
				case 2:
					cantidad();					
					system("pause");
				break;
				case 3: 
					temperatura();				
					system("pause");
				break;
				default:
					printf("\nGracias por utilizar el programa, adiós.\n");
				break;
			}			
		}while(opc<1 || opc>4);	
	}while(opc!=4);
	return 0;
}

void carga(){
	datos datosMeteorologicos;
	
	fd=fopen(archivoPrincipal,"rb");
	fp=fopen(archivoSecundario,"wb");	
	
	if((fd == NULL)||(fp == NULL)){
		mensajeError();
		return; // return 0;
	}
	else{				
		fread(&datosMeteorologicos, sizeof(datos),1,fd);				        
	    while (!feof(fd)){
			if(!strcmp(datosMeteorologicos.nombre,"BOLIVAR AERO") || !strcmp(datosMeteorologicos.nombre,"BUENOS AIRES OBSERVATORIO")){
				fwrite(&datosMeteorologicos,sizeof(datos),1,fp);
			}
	       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
		}
	}
	fclose(fd);
	fclose(fp);
}

void visualizar(){
	//MIS ESTACIONES BOLIVAR AERO y BUENOS AIRES OBSERVATORIO
	
	datos datosMeteorologicos; //declaramos una variable de tipo data
	int registro = 0;
	int opc = 0;
	
	do{
		printf("\nSUBMENU - VISUALIZAR\n");
		printf("1. Registros de BOLIVAR AERO y BUENOS AIRES OBSERVATORIO\n"); 
		printf("2. Salir\n");
		scanf("%i", &opc);
	}while(opc<1 || opc>2);	

	switch(opc){
		case 1:
			carga(); //creamos el archivo con mis estaciones
			fd=fopen(archivoSecundario,"rb");
			if(fd == NULL){
				mensajeError();
				return; // return 0;
			}
			else{				
				system("cls"); //limpiamos la pantalla			
				fread(&datosMeteorologicos, sizeof(datos),1,fd);
				printf("\n -------------------------------Mostrando los registros de temperaturas------------------------------- \n");		
				cabeceraTabla(); //funcion que muestra la cabecera de la tabla 
								
				while(!feof(fd)){
					registro++;
					datosTabla(datosMeteorologicos, registro);
					fread(&datosMeteorologicos, sizeof(datos),1,fd);
				}			
			}
			fclose(fd);
		break;			
		default: 
			printf ("Volviendo al menu\n");
			return;
		break;		
	}		
}

void cantidad(){
	//MIS ESTACIONES BOLIVAR AERO y BUENOS AIRES OBSERVATORIO
	
	datos datosMeteorologicos;	
	int cantBolivarAero = 0;
	int cantBsAsObservatorio = 0;
	int totalEstaciones = 0;
	
	carga(); //creamos el archivo con mis estaciones
	fd=fopen(archivoSecundario,"rb");
	
	if(fd==NULL){
		mensajeError();
		return; // return 0;
	}
	else{
		fread(&datosMeteorologicos, sizeof(datos),1,fd);
		while(!feof(fd)){
			if(!strcmp(datosMeteorologicos.nombre,"BOLIVAR AERO") || !strcmp(datosMeteorologicos.nombre,"BUENOS AIRES OBSERVATORIO")){
				if(!strcmp(datosMeteorologicos.nombre,"BOLIVAR AERO")){
					cantBolivarAero++;
				}
				if(!strcmp(datosMeteorologicos.nombre,"BUENOS AIRES OBSERVATORIO")){
					cantBsAsObservatorio++;
				}
				totalEstaciones++;
			}			
			fread(&datosMeteorologicos, sizeof(datos),1,fd);
			}
		printf("Se encontraron %i registros para la estacion BOLIVAR AERO\n",cantBolivarAero);
		printf("Se encontraron %i registros para la estacion BUENOS AIRES OBSERVATORIO\n",cantBsAsObservatorio);
		printf("Hay en total %i registros\n", totalEstaciones);
		}
	fclose(fd); //cerramos el archivo
}

void temperatura(){
	int registro = 0;
	int opc = 0;
	float tempMin = 0;
	float tempMax = 0;
	datos datosMeteorologicos;
	
	do{
		printf("\nSUBMENU - VISUALIZAR REGISTROS MINIMO/MAXIMO\n");
		printf("1. Registro menor temperatura entre ambas estaciones\n");
		printf("2. Registro mayor temperatura entre ambas estaciones\n"); 
		printf("3. Salir\n");
		scanf("%i", &opc);
	}while(opc<1 || opc>3);
	
	carga(); //creamos el archivo con mis estaciones
	fd=fopen(archivoSecundario,"rb");	
	
	if(fd == NULL){
		mensajeError();
		return; // return 0;
	}
	else{						
		switch(opc){
			case 1:		
				system("cls"); //limpiamos la pantalla, para quitar el submenu			
				fread(&datosMeteorologicos, sizeof(datos),1,fd);
				tempMin = datosMeteorologicos.temp;		        
			    while (!feof(fd)){	
			    	if(datosMeteorologicos.temp < tempMin){
			    		tempMin = datosMeteorologicos.temp;
					}
			       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
				}
				rewind(fd);  //volvemos al inicio del programa
				
				fread(&datosMeteorologicos,sizeof(datos),1,fd);
				
				while (!feof(fd)){
					registro++;	
			    	if(tempMin == datosMeteorologicos.temp){
			    		cabeceraTabla();
			    		datosTabla(datosMeteorologicos,registro);
			    		return;
					}
			       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
				}
			break;
			case 2: 
				system("cls"); //limpiamos la pantalla, para quitar el submenu
				fread(&datosMeteorologicos, sizeof(datos),1,fd);
				tempMax = datosMeteorologicos.temp;			        
			    while (!feof(fd)){	
			    	if(datosMeteorologicos.temp > tempMax){
			    		tempMax = datosMeteorologicos.temp;
					}
			       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
				}
				
				rewind(fd);  //volvemos al inicio del programa				
				fread(&datosMeteorologicos,sizeof(datos),1,fd);
				
				while (!feof(fd)){
					registro++;	
			    	if(tempMax == datosMeteorologicos.temp){
			    		cabeceraTabla();
			    		datosTabla(datosMeteorologicos,registro);
			    		return;
					}
			       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
				}
			break;
			default: 
				printf ("Volviendo al menu\n");
				return;
			break;		
		}	
	}
	fclose(fd);
}

//funciones secundarias 
void cabeceraTabla(){
	printf("\n------------------------------------------------------------------------------------------------------\n");
	printf("|REG.|  FECHA   |  HORA |   TEMP  |   HUM  |   PNM   |  DD  |  FF  |              NOMBRE              |");
	printf("\n------------------------------------------------------------------------------------------------------\n");
}

//procedimiento de muestra de los registros
void datosTabla(datos datosMeteorologicos, int registro){
	printf("|%4i", registro);
   	printf("| %8s ", datosMeteorologicos.fecha);
   	printf("| %4i  ", datosMeteorologicos.hora);
	printf("| %7.2f ", datosMeteorologicos.temp);
	printf("| %7.2f", datosMeteorologicos.hum);
 	printf("| %7.2f ", datosMeteorologicos.pnm); //el 7 cuenta los tanto la parte entera como los decimales, el 2 nos determina la cantidad de decimales que muestra
	printf("| %4i ", datosMeteorologicos.dd);
	printf("| %4i ", datosMeteorologicos.ff);	    
	printf("| %32s |", datosMeteorologicos.nombre); // - valor a izquierda
	printf("\n------------------------------------------------------------------------------------------------------\n");
}

void cargaDatos(datos *datosMeteorologicos){ //se pasa la estructura como puntero para poder cargarle los datos 
	
	printf("FECHA\t"); 
	fflush(stdin);
	gets(datosMeteorologicos->fecha);	
	
	printf("HORA\t");
	fflush(stdin);
	scanf("%i", &datosMeteorologicos->hora); //operador flecha debido a que se pasa por referencia la variable de tipo estructura

	printf("TEMP\t");
	fflush(stdin); //limpiamos el buffer
	scanf("%f", &datosMeteorologicos->temp);	//guardamos el valor del usuario en valor ingresado para comprobar si es entero

	printf("HUM\t");
	fflush(stdin);
	scanf("%f", &datosMeteorologicos->hum);	

	printf("PNM\t");
	fflush(stdin);
	scanf("%f", &datosMeteorologicos->pnm);	
	
	printf("DD\t");
	fflush(stdin);
	scanf("%i",&datosMeteorologicos->dd);

	printf("FF\t");
	fflush(stdin);
	scanf("%i",&datosMeteorologicos->ff);
	
	printf("NOMBRE\t");
	fflush(stdin);		
	gets(datosMeteorologicos->nombre);
	
}

void mensajeError(){
	printf("\n----Error al tratar de abrir los archivos----\n");
}




