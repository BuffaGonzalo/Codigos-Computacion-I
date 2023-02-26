//incluimos la librerías
#include <stdio.h> //libreria estandar de entrada y salida
#include <stdlib.h>
#include <locale.h> //librería utilizada para tener caracteres españoles
#include <string.h> 
#include <time.h> //libreria utilizada para obtener la hora actual

FILE *fd, *fp, *fd_csv; //variables de tipo FILE

//definimos los archivos
char archivoPrincipal[]="archivoPrincipal.dat";
char archivoImportado[]="datoshorarios_reducido_SMN.csv";
char archivoEstaciones[]="misEstaciones.dat";
char archivoEstacionesOrdenado[]="misEstacionesOrdenado.dat";
char archivoExportado[] = "registrosExportados.csv";

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

//funciones primarias
void importar();
void agregar();
void visualizar();
void eliminar();
void cantidadEstaciones();
void tempMin();
void tempMax();
void ordenado();
void exportar();

//funcioens secundarias
void cabeceraTabla();
void datosTabla();
void cargaDatos(datos *datosMeteorologicos);
void mensajeError();
void metodoShell(datos vector[], int n);

int main(){
	int opc = 0;	
	
	//palabras en español
	setlocale(LC_CTYPE,"spanish");
	//numeros en ingles
	setlocale(LC_NUMERIC, "en-US");
	
	do{
		system("cls"); //limpiamos la pantalla
		//mostramos el menu
		printf("\nMENÚ\n");
		printf("1. Importar registros\n");
		printf("2. Agregar mas registros\n");
		printf("3. Visualizar registros existentes\n");
		printf("4. Eliminar todos los registros\n");
		printf("5. Cantidad de Estaciones -  Bolivar Aereo\n");
		printf("6. Registro con la mínima temperatura\n");
		printf("7. Registro con la máxima temperatura\n");
		printf("8. Registros ordenados por la hora\n");
		printf("9. Exportar registros ordenados por hora\n");
		printf("10. Salir\n");
	
		//pedimos al usuario que opción quiere elegir, entre 1 y 5
		do{
			printf("Opcion: \t");
			fflush(stdin);
			scanf ("%i", &opc);
		}while(opc<1 || opc>10);

		//Dependiendo de la elección del usuario, realizamos diferentes tareas
		switch(opc){
			case 1: 
				system("cls");
				importar();
				system("pause");
			break;
			case 2:
				system("cls");
				agregar();
				system("pause");
			break;	
			case 3:
				system("cls");
				visualizar();
				system("pause");
			break;
			case 4:
				system("cls");
				eliminar();
				system("pause");
			break;
			case 5:
				system("cls");
				cantidadEstaciones();
				system("pause");			
			break;
			case 6:
				system("cls");
				tempMin();
				system("pause");
			break;
			case 7:	
				system("cls");
				tempMax();
				system("pause");
			break;
			case 8:
				system("cls");
				ordenado();
				system("pause");
			break;
			case 9:
				system("cls");
				exportar();
				system("pause");
			break;
			default:
				printf("\nGracias por utilizar el programa, adiós.\n");
			break;
			}			
		}while(opc != 10);	
	return 0;
}

//punto 1
void importar(){
	datos dMet; //lo mismo que datos datosMeteorologicos de las otroa funciones, mas corto debido al sscanf
	int registro = 0;
	int contador = 0;
	char cadena[100],opc;
	
	fd = fopen(archivoPrincipal,"ab+"); //abrimos el archivo binario
	fd_csv = fopen(archivoImportado,"rt"); //abrimos el archivo de texto
	
	if((fd == NULL) || (fd_csv == NULL)){
		mensajeError();
		return;
	}
	else{
		printf("\nEsta seguro que desea importar los registros desde el archivo datos.csv? (s=Si n=No)\n");
		fflush(stdin); 
		scanf("%c", &opc);
		
		if(opc=='s' || opc =='S'){
			while(fgets(cadena,100,fd_csv)!= NULL){ //fgets copia el registro en cadena
				if(registro != 0){ //hacemos esta comparación para evitar el encabezado 
					//guardamos los valores de cadena en dMet
					sscanf(cadena,"%[^;];%i;%f;%f;%f;%i;%i;%[^\n]",dMet.fecha,&dMet.hora,&dMet.temp,&dMet.hum,&dMet.pnm,&dMet.dd,&dMet.ff,dMet.nombre);
					fwrite(&dMet,sizeof(datos),1,fd);
					contador++;
				}
				registro++;	
			}
			printf("\nDatos importados correctamente, se importaron %i registros\n", contador);
		}
	}
	fclose(fd);
	fclose(fd_csv);
}
//Punto 2
void agregar(){
	datos datosMeteorologicos;
	char continuar;
	int opc = 0;
	
	fd = fopen(archivoPrincipal,"ab");
	
	if(fd==NULL){
		mensajeError();
		return; // return 0;
	}
	else{
		printf("\nEsta seguro que desea agregar registros? (s=Si n=No)\n");
		fflush(stdin); 
		scanf("%c", &opc);
		
		if(opc=='s' || opc =='S'){
			
			do{
				cargaDatos(&datosMeteorologicos); //función encargada de cargar datos	
				fwrite(&datosMeteorologicos, sizeof(datos),1,fd);
		
				printf("Desea ingresar otros datos meteorológicos? - (s=Si n=No) \n");
				fflush(stdin);
				scanf("%c", &continuar); 
			}while((continuar=='s') || (continuar== 'S')); //comprobación para para añadir mas  registros
		}
	}	
	fclose(fd); //cerramos el archivo
}





//funciones secundarias
void cabeceraTabla(){
	printf("------------------------------------------------------------------------------------------------------\n");
	printf("|Reg.|  fecha   | hora |  temp  |   hum  |   pnm   |  dd  |  ff  |               nombre               |\n");
	printf("------------------------------------------------------------------------------------------------------\n");
}
//procedimiento de muestra de los registros
void datosTabla(datos datosMeteorologicos, int registro){
	printf("|%4i", registro);
   	printf("| %8s ", datosMeteorologicos.fecha );
   	printf("| %4i ", datosMeteorologicos.hora);
	printf("| %7.2f", datosMeteorologicos.temp);
	printf("| %7.2f", datosMeteorologicos.hum);
 	printf("| %7.2f ", datosMeteorologicos.pnm); 
	printf("| %4i ", datosMeteorologicos.dd);
	printf("| %4i ", datosMeteorologicos.ff);	    
	printf("| %34s |", datosMeteorologicos.nombre); 
	printf("\n------------------------------------------------------------------------------------------------------\n");
}
//procedimiento de carga de registros
void cargaDatos(datos *datosMeteorologicos){ //se pasa la estructura como puntero para poder cargarle los datos
	int opc = 0;
	char valorIngresado[10]; //Variable utilizada para guardar el valor ingresado temporalmente y comprobar si es un entero
	
	
	printf("fecha\t"); 
	fflush(stdin);
	gets(datosMeteorologicos->fecha);	
	
	printf("hora\t");
	fflush(stdin);
	scanf("%i", &datosMeteorologicos->hora); //operador flecha debido a que se pasa por referencia la variable de tipo estructura
	
	printf("temp\t");
	fflush(stdin); //limpiamos el buffer
	scanf("%f", &datosMeteorologicos->temp );	//guardamos el valor del usuario en valor ingresado para comprobar si es entero

	printf("hum\t");
	fflush(stdin);
	scanf("%f", &datosMeteorologicos->hum);	
	
	printf("pnm\t");
	fflush(stdin);
	scanf("%f", &datosMeteorologicos->pnm);	

	printf("dd\t");
	fflush(stdin);
	scanf("%i",&datosMeteorologicos->dd);

	printf("ff\t");
	fflush(stdin);
	scanf("%i",&datosMeteorologicos->ff);
	
	printf("nombre\t"); 
	fflush(stdin);
	gets(datosMeteorologicos->nombre);
}

void mensajeError(){
	printf("\n----Error al tratar de abrir los archivos----\n");
}

void visualizar(){
	datos datosMeteorologicos; //declaramos una variable de tipo data
	int registro = 0;
	int opc = 0;
	do{
		printf("\nSUBMENU - VISUALIZAR\n");
		printf("1. Registro desordenado\n");
		printf("2. Estaciones Ordendas por hora\n"); 
		printf("3. Salir\n");
		scanf("%i", &opc);
	}while(opc<1 || opc>3);	

	switch(opc){
		case 1: fd = fopen(archivoPrincipal,"rb");
		break;
		case 2:	
			fd = fopen(archivoEstacionesOrdenado,"rb");
		break;
		default: 
			printf ("Volviendo al menu\n");
			return;
		break;		
	}		
		
	if(fd == NULL){
		mensajeError();
		return; // return 0;
	}
	else{				
		system("cls"); //limpiamos la pantalla
		fread(&datosMeteorologicos, sizeof(datos),1,fd);
		printf("\n -------------------------------Mostrando los registros de temperaturas------------------------------- \n");		
		cabeceraTabla(); //funcion que muestra la cabecera de la tabla 
			        
	    while (!feof(fd)){	
			registro++;   	
	       	datosTabla(datosMeteorologicos,registro); //funcion que muestra los datos de la tabla
	       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
		}
	}
	fclose(fd);
}

void eliminar(){
	char opc;
		
	printf("\nEsta seguro que desea eliminar todos los registros existentes? (s=Si n=No)\n");
	fflush(stdin); 
	scanf("%c", &opc);
	
	if(opc=='s' || opc =='S'){
		
		fd = fopen(archivoPrincipal,"wb"); //abrimos el archivo como lectura
			
		if(fd==NULL){ //Se cumple cuando no se cumplen los permisos necesarios o no hay memoria suficiente
			mensajeError();
			return; // return 0;	
		}
		printf("Se eliminaron todos los registros extosamente\n");
		fclose(fd); //cerramos el archivo
	}else{
		printf("\nVolviendo al menu\n");
	}
}

void cantidadEstaciones(){
	datos datosMeteorologicos; //definimos una variable de tipo data
	int comparacion = 0; //definimos variable de comprobación ingreso de datos (s=Sí n=No) 
	int contador = 0;
	
	fd = fopen(archivoPrincipal,"rb");  //abrimos el archivo como lectura
	fp = fopen(archivoEstaciones, "wb");
		
	if(fd==NULL || fp==NULL){ //Se cumple cuando no se cumplen los permisos necesarios o no hay memoria suficiente
		mensajeError();
		return; // return 0;	
	}else{		
		fread(&datosMeteorologicos, sizeof(datos),1,fd); 
	    while (!feof(fd)){	
	    		comparacion = strcmp(datosMeteorologicos.nombre,"BOLIVAR AERO"); //debe de estar dentro para que tome los distintos nombres
				if(comparacion == 0){
					//printf("prueba");
					fwrite(&datosMeteorologicos, sizeof(datos),1,fp);
					contador++;
				}			
	       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
		}
		printf("Hay %i estaciones Bolivar Aero\n", contador);
	}
	fclose(fd); //cerramos el archivo
	fclose(fp);
}

void tempMin(){
	datos datosMeteorologicos; //declaramos una variable de tipo data
	float tempMin = 0;
	int contador = 0;
		
	fd = fopen(archivoEstaciones,"rb");
		
	if(fd == NULL){
		mensajeError();
		return; // return 0;
	}
	else{			
		fread(&datosMeteorologicos, sizeof(datos),1,fd);			     
		tempMin = datosMeteorologicos.temp;
		  
	    while (!feof(fd)){	
			if(datosMeteorologicos.temp<tempMin){
				tempMin=datosMeteorologicos.temp;
			}
	       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
		}
		
		rewind(fd);
		
		while (!feof(fd)){	
			contador++;
			if(datosMeteorologicos.temp == tempMin){
				cabeceraTabla();
				datosTabla(datosMeteorologicos, contador);
				return;
			}
	       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
		}		
	}
	fclose(fd);
}

void tempMax(){
	datos datosMeteorologicos; //declaramos una variable de tipo data
	float tempMax = 0;
	int contador = 0;
		
	fd = fopen(archivoEstaciones,"rb");
		
	if(fd == NULL){
		mensajeError();
		return; // return 0;
	}
	else{			
		fread(&datosMeteorologicos, sizeof(datos),1,fd);			     
		tempMax = datosMeteorologicos.temp;
		  
	    while (!feof(fd)){	
			if(datosMeteorologicos.temp>tempMax){
				tempMax=datosMeteorologicos.temp;
			}
	       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
		}
		
		rewind(fd);
		
		while (!feof(fd)){	
			contador++;
			if(datosMeteorologicos.temp == tempMax){
				cabeceraTabla();
				datosTabla(datosMeteorologicos, contador);
				return;
			}
	       	fread(&datosMeteorologicos,sizeof(datos),1,fd); //necesito un read dentro y otro fuera del while para corroborar si no hay mas de un registro
		}		
	}
	fclose(fd);
}


void ordenado(){
	datos datosMeteorologicos; //declaramos una variable de tipo data
	float tempMax = 0;
	int contador, cantRegistros, i = 0;
	datos vector[100];
		
	fd = fopen(archivoEstaciones,"rb");
	fp = fopen(archivoEstacionesOrdenado,"wb");
	
	if(fd == NULL){
		mensajeError();
		return; // return 0;
	}
	else{		
		fseek(fd,0,SEEK_END); //nos posicionamos al final
		cantRegistros = ftell(fd)/sizeof(datos); //determinamos el tamaño del vector	
		datos vector[cantRegistros]; //definimos luego debido a que sino falla porque no esta definido cantRegistros	
		
		rewind(fd);	
			
		fread(&datosMeteorologicos,sizeof(datos),1,fd);
		while((!feof(fd)) && (i<cantRegistros)){ //comprobamos que no sea el final del archivo y 
			vector[i]=datosMeteorologicos;
			fread(&datosMeteorologicos,sizeof(datos),1,fd); //movemos el cursor
			i++;
		}	
		
		metodoShell(vector,cantRegistros);
		
		i=0;		
		for(i=0;i<cantRegistros;i++){
			fwrite(&vector[i],sizeof(datos),1,fp);		
		}	
	}
	fclose(fd);
	fclose(fp);	
}

void exportar(){
	int contador = 0;
	char opc;
	datos dMet; //definimos una variable de tipo datos
	fd = fopen(archivoEstacionesOrdenado, "rb"); //abrimos el archivo binario como lectura
	fd_csv = fopen(archivoExportado,"wt"); //abrimos el archivo de texto en modo escritura, se borra el archivo previo
	
	if((fd == NULL) || (fd_csv == NULL)){
		mensajeError();
		return;
	}
	else{
		printf("\nEsta seguro que desea exportar los registros? (s=Si n=No)\n");
		fflush(stdin);
		scanf("%c", &opc);
		if(opc=='s' || opc =='S'){
			fprintf(fd_csv,"fecha;hora;temp;hum;pnm;dd;ff;nombre");
			fread(&dMet,sizeof(datos),1,fd);		
			while(!feof(fd)){
				contador++;
				fprintf(fd_csv,"\n%s;%i;%.1f;%.1f;%.1f;%i;%i;%s", dMet.fecha,dMet.hora,dMet.temp,dMet.hum,dMet.pnm,dMet.dd,dMet.ff,dMet.nombre);
				fread(&dMet,sizeof(datos),1,fd);
			}
		}
		printf("\nExportación finalizada, se exportaron %i registros\n", contador);			
	}
	fclose(fd);
	fclose(fd_csv);	
}





void metodoShell(datos vector[], int n){ //int a[]
    int intervalo,comparacion, i, j, k;
    intervalo = n / 2;
    while (intervalo > 0){
      for (i = intervalo; i < n; i++){
        j = i - intervalo;
        while (j >= 0){
          k = j + intervalo;
          comparacion = (vector[j].hora<vector[k].hora);
          if (comparacion)
            j = -1; /* asÃ­ termina el bucle, par ordenado */
          else{
            datos temp;
            temp = vector[j];
            vector[j] = vector[k];
            vector[k] = temp;
            j -= intervalo;
            }
          }
      }
      intervalo = intervalo / 2;
    }
}
