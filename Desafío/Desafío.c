//incluimos la librerías
#include <stdio.h> //libreria estandar de entrada y salida
#include <stdlib.h>
#include <locale.h> //librería utilizada para tener caracteres españoles
#include <string.h> 
#include <time.h> //libreria utilizada para obtener la hora actual

//definimos punteros de tipo FILE
FILE *fd, *fp, *fd_csv; //fd es una variable de tipo puntero

char archivoPrincipal[]="archivoPrincipal.dat"; //utilizado en los primeros 5 puntos
char archivoTemporal[] = "archivoTemporal.dat"; //utilizado en el punto 6
char archivoBackup[] = "archivoBackup.dat"; //utilizado en el punto 6
char archivoOrdenado[] = "archivoOrdenado.dat"; //utilizado en el punto 7
char archivoImportado[] = "datos.csv"; //utilizado en el punto 11
char archivoExportado[] = "datosExportados.csv"; //utilizado en el punto 12

//Definimos la estructura
typedef struct{
	int anio;
	int mes;
	int dia;
	char fecha[10];
	float tMax; //temperatura maxima
	float tMin; //temperatura minima
	float tMedia; //termperatura media
	char tiempo[20];
	float hRel; //humedad relativa	
}datos;

//añadimos los prototipos de las funciones
//funciones de los puntos 
void crear();
void visualizar();
void buscarMostrar();
void agregar();
void modificar();
void eliminar();
void ordenar();
void promedioTotal();
void promedioMes();
void importar();
void exportar();
//funciones de visualización
void cabeceraTabla();
void datosTabla(datos datosMeteorologicos,int registro);
void cargaDatos(datos *datosMeteorologicos);
void mensajeError();
//funciones de control de datos
int esNumero(const char *s);
int esEntero (const char *cadena);
int errorAnio(const char *tip);
int errorMes(int aniotipeado, const char *tip);
int errorDia(int aniotipeado, int mestipeado, const char *tip);
int bisiesto(int anioentero);
//metodos de ordenamiento
void metodoBurbujaMejorado(datos vector[], int cantRegistros);
void metodoShell(datos vector[], int n);

int main(){
	//definimos las variables
	char opc[3];
	//palabras en español
	setlocale(LC_CTYPE,"spanish");
	//numeros en ingles
	setlocale(LC_NUMERIC, "en-US");
	
	//menu utilizado para manipulación de datos meteorologicos	
	do{
		system("cls"); //limpiamos la pantalla
		//mostramos el menu
		printf("\n-------------------------------------------------MENÚ-------------------------------------------------\n");
		printf("1. Crear un archivo de temperaturas\n");
		printf("2. Agregar mas registros\n");
		printf("3. Visualizar registros existentes\n");
		printf("4. Buscar y mostrar un registro\n");
		printf("5. Modificar un registro\n");
		printf("6. Eliminar un registro\n");
		printf("7. Ordenar el archivo por el campo fecha y almacenarlo en otro archivo\n");
		printf("8. Promedio total de temperaturas y porcentaje de humedad relativa\n");
		printf("9. Promedio mensual de temperaturas y porcentaje de humedad relativa\n");
		printf("10. Importar registros\n");
		printf("11. Exportar registros\n");
		printf("12. Salir\n");
	
		//pedimos al usuario que opción quiere elegir, entre 1 y 5
		do{
			do{
				printf("Opcion: \t");
				fflush(stdin);
				scanf ("%s", &opc);				
			}while(!esEntero(opc));	
		}while(atoi(opc)<1 || atoi(opc)>12);		

		//Dependiendo de la elección del usuario, realizamos diferentes tareas
		switch(atoi(opc)){
			case 1: 
				system("cls"); //limpiamos la pantalla
				crear();
				system("pause"); //esperamos a que el usuario pulse una tecla para que no desaparezca el contenido de la pantalla	
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
				buscarMostrar();
				system("pause");
			break;	
			case 5:
				system("cls");
				modificar();
				system("pause");
			break;
			case 6:
				system("cls");
				eliminar();
				system("pause");
			break;
			case 7:
				system("cls");
				ordenar();	
				system("pause");			
			break;
			case 8:
				system("cls");
				promedioTotal();
				system("pause");
			break;
			case 9:	
				system("cls");			
				promedioMes();
				system("pause");
			break;
			case 10:
				system("cls");
				importar();
				system("pause");
			break;
			case 11:
				system("cls");
				exportar();
				system("pause");
			break;
			default:
				printf("\nGracias por utilizar el programa, adiós.\n");
			break;
		}			
	}while(atoi(opc) != 12);	
	return 0;
}

//procedimiento crear - primera opción
void crear(){
	int opc = 0;
	datos datosMeteorologicos; //definimos una variable de tipo data
	char continuar; //definimos variable de comprobación ingreso de datos (s=Sí n=No) 
	
	printf("\nEsta seguro que desea crear registros, esta opción elimina los registros existentes? (s=Si n=No)\n");
	fflush(stdin); 
	scanf("%c", &opc);
	
	if(opc=='s' || opc =='S'){
		
		fd = fopen(archivoPrincipal,"wb"); //abrimos el archivo como lectura
			
		if(fd==NULL){ //Se cumple cuando no se cumplen los permisos necesarios o no hay memoria suficiente
			mensajeError();
			return; // return 0;	
		}
		else{
			printf("\n>>> Ingreso de datos: \n");		
			do{			
				cargaDatos(&datosMeteorologicos); //función encargada de cargar datos
				fwrite(&datosMeteorologicos, sizeof(datos),1,fd);
			
				//comprobación para saber si el usuario quiere ingresar mas datos meteorológicos
				printf("Desea ingresar otros datos meteorológicos? - (s=Si n=No) \n");
				fflush(stdin);
				scanf("%c", &continuar);
				//condición de comprobación (s=Sí n=No) 
			}while((continuar=='s') || (continuar== 'S'));
		}
		fclose(fd); //cerramos el archivo
		ordenar(); //llamamos al ordenar para poder utilizar las otras opciones con los datos cargados actualmente
	}
}

//procedimiento para agregar datos - segunda opción
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
	ordenar(); //llamamos el procedimiento ordenar
}

//procedimiento visualizar - tercera opción
void visualizar(){
	datos datosMeteorologicos; //declaramos una variable de tipo data
	int registro = 0;
	int opc = 0;
	do{
		printf("\n-----------------------------------------SUBMENU - VISUALIZAR-----------------------------------------\n");
		printf("1. Registro desordenado\n");
		printf("2. Registro ordenado\n"); 
		printf("3. Salir\n");
		scanf("%i", &opc);
	}while(opc<1 || opc>3);	

	switch(opc){
		case 1: fd = fopen(archivoPrincipal,"rb");
		break;
		case 2: 
		ordenar();	
		fd = fopen(archivoOrdenado,"rb");
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

//procedimiento de busqueda de registros y mostrado por pantalla - cuarta opción
void buscarMostrar(){
	//definimos las variables
	datos datosMeteorologicos;
	int opc = 0;
	
	fd = fopen(archivoPrincipal,"rb");
	
	if(fd==NULL){
		mensajeError();
		return; // return 0;
	}
	else{
		do{
			printf("Ingrese el registro que desea ver, en base al archivo desordenado \t");
			fflush(stdin);
			scanf("%i", &opc); 		
		}while(opc<1);
	
		fseek(fd,(opc-1)*sizeof(datos),0);
		fread(&datosMeteorologicos, sizeof(datos),1,fd);
		
		if(!feof(fd)){
			printf("\n -------------------------------Mostrando el registro de temperaturas--------------------------------- \n");
			cabeceraTabla();
			datosTabla(datosMeteorologicos,opc);
		}
		else{
			printf("\n-----------------------------------------El registro no existe----------------------------------------\n\n");
			return;
		}			
	}
	fclose(fd);	
}

//procedimeinto modificar - quinta opción
void modificar(){	
	//creamos las variables 
	datos datosMeteorologicos;
	int opc = 0; 
	char continuar;
	
	fd = fopen(archivoPrincipal,"rb+");
	
	if(fd==NULL){
		mensajeError();
		return; // return 0;
	}
	else{
		do{
			printf("Ingrese el registro que desea modificar, en base al archivo desordenado \t");
			fflush(stdin);
			scanf("%i", &opc); 
		}while(opc<1);
		
		fseek(fd,(opc-1)*sizeof(datos),0);
		fread(&datosMeteorologicos, sizeof(datos),1,fd);
		
		if(!feof(fd)){
			printf("\n------Mostrando datos del archivo de temperaturas que desea modificar------\n");
       		cabeceraTabla(); //mostramos la cabecera
			datosTabla(datosMeteorologicos,opc); //mostramos la tabla de registros
			
			printf("Esta seguro que desea modificarlo? - (s=Si n=No) \t");
			fflush(stdin);
			scanf ("%c", &continuar);
			
			if(continuar == 's' || continuar == 'S'){
				fseek(fd,(opc-1)*sizeof(datos),0);//Volvemos a la posición del registro que queremos modificar
				cargaDatos(&datosMeteorologicos); //funcion encargada de la carga de datos al registro	
				fwrite(&datosMeteorologicos, sizeof(datos),1,fd); //escribimos los datos
			}				
		}
		else{
			printf("\n-----------------------------------------El registro no existe----------------------------------------\n\n");
			return;
		}			
	}	
	fclose(fd);	
}

//procedimiento de eliminación de un registro - sexta opción
void eliminar(){
	//si no elimina los archivos ingresados la primera vez, volver a compilarlo y correrlo
	//declaramos las variables
	datos datosMeteorologicos;	
	char continuar;
	int opc = 0;
	int contador = 1;
	
	fd = fopen(archivoPrincipal,"rb"); //abrimos el archivo con datos previos
	fp = fopen(archivoTemporal,"wb"); //abrimos el archivo temporal
	
	if(fd==NULL || fp==NULL){
		mensajeError();
		fclose(fd);
		fclose(fp);	
		return;
	}
	else{
		do{
			printf("Ingrese el registro que desea eliminar, en base al archivo desordenado \t");
			fflush(stdin);
			scanf("%i", &opc);	
		}while(opc<1);
		
		fseek(fd,(opc-1)*sizeof(datos),0);
		fread(&datosMeteorologicos,sizeof(datos),1,fd);
		
		if(!feof(fd)){
			printf("\n-------------------------Mostrando registro de temperaturas que desea eliminar------------------------\n");
        	cabeceraTabla();       
       		datosTabla(datosMeteorologicos,opc);
        	
    		printf("Esta seguro que desea eliminarlo? - (s=Si n=No) \t");
			fflush(stdin);
			scanf ("%c", &continuar);
			
			if(continuar == 's' || continuar == 'S'){
				//luego de que el usuario afirme que lo quiere eliminar ingresa aqui dentro, donde se cargan los datos al otro archivo
				//printf("PRUEBA");
				rewind(fd);
				//fseek(fd,0,0);
				fread(&datosMeteorologicos,sizeof(datos),1,fd); 
				while(!feof(fd)){
					//printf("PRUEBA WHILE");
					if (contador!=opc){
						fwrite(&datosMeteorologicos,sizeof(datos),1,fp); //PORQUE???		
						//printf("PRUEBA IF");
        			}    	
        			fread(&datosMeteorologicos,sizeof(datos),1,fd); 
					contador++;
				}
				fclose(fd);
				fclose(fp);	
				
				remove(archivoBackup); //borramos el archivo backup viejo
				rename(archivoPrincipal,archivoBackup); 
				rename(archivoTemporal,archivoPrincipal);
								
				printf("\n-----------------------------------El registro a sido eliminado---------------------------------------\n");
				ordenar(); //ordenamos para podr utilizar otras funciones
			}								
		}
		else{
			printf("\n-----------------------------------------El registro no existe----------------------------------------\n\n");
			fclose(fd);
			return;
		}		
	}	
}

//procedimiento de ordenamiento del registro - septima opción
void ordenar(){
	datos datosMeteorologicos;
	int cantRegistros = 0;
	int i = 0;	
	
	fd = fopen(archivoPrincipal,"rb"); //abrimos el archivo desordenado, como lectura	
	
	if (fd==NULL){ //error asignación embes de comparación devolvia valor incorrecto en ftell()
		mensajeError();
		fclose(fd); //en caso de que falle cerramos el archivo
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
	
	fclose(fd); //cerramos el archivo principal
	//se pueden utilizar cualquiera de los metodos de ordenamiento, ordenan desde el mas viejo al mas nuevo
	//metodoBurbujaMejorado(vector,cantRegistros);//metodo de ordenamiento burbuja mejorado
	metodoShell(vector,cantRegistros); //metodo de ordenamiento shell
	remove(archivoOrdenado); //eliminamos el archivo viejo ordenado	
	fp = fopen(archivoOrdenado, "wb"); //abrimos el archivo que tendra los registros ordenados, como escritura
			
	if(fp==NULL){
		mensajeError();
		return;
	}
	else{
		i=0;
        
        for(i=0; i<cantRegistros; i++){
            fwrite(&vector[i],sizeof(datos),1,fd);
        }    
		printf("\n------------------------------------Se ordenaron los registros----------------------------------------\n");			
	}					
	fclose(fp); //cerramos el archivo ordenado
	}
}

//promedio de temperaturas y porcentaje de humedad - octava opción
void promedioTotal(){
	datos datosMeteorologicos;
	float promTempMax, promTempMin, promTempMed, promHumRel;
	promTempMax = 0;
	promTempMin = 0;
	promTempMed = 0;
	promHumRel = 0;
	int contador = 0;
	
	//abrimos el archivo binario como lectura
	fd = fopen(archivoOrdenado, "rb");
		
	if(fd == NULL){
		mensajeError();
		return; // return 0;
	}
	else{
		fread(&datosMeteorologicos, sizeof(datos),1,fd);
		if(feof(fd)){
			printf("---------------------------------------------No hay registros-----------------------------------------\n");
			return;
		}else{
			while(!feof(fd)){
			promTempMax += datosMeteorologicos.tMax;
			promTempMin += datosMeteorologicos.tMin;
			promTempMed += datosMeteorologicos.tMedia;
			promHumRel += datosMeteorologicos.hRel;
			contador++;
			fread(&datosMeteorologicos,sizeof(datos),1,fd);
			}
		}
	}
	fclose(fd);	
	//imprimimos los promedios
	printf("\nPromedio total de temperaturas y humedad relativa\n");
	printf("Promedio temperatura máxima %f \n", promTempMax/contador);
	printf("Promedio temperatura mínima %f \n", promTempMin/contador);
	printf("Promedio temperatura media %f \n", promTempMed/contador);
	printf("Promedio humedad relativa %f \n", promHumRel/contador);
}

//promedio de temperaturas y porcentaje de humedad con respecto al mes- novena opción 
void promedioMes(){
	datos datosMeteorologicos;
	int anio = 0;
	int mes = 0;
	int contador = 0; 
	float promTempMax,promTempMin, promTempMed, promHumRel = 0; //declaramos e inicializamos la variables de promedios
	
	//abrimos el archivo como escritura
	fd = fopen(archivoOrdenado,"rb");
	
	if(fd == NULL){
		mensajeError();
		return; // return 0;
	}
	else{
		fread(&datosMeteorologicos,sizeof(datos),1,fd);
		printf("\nPromedio mensual de temperaturas y humedad relativa\n");
		while(!feof(fd)){
			//anio,mes=0;
			if(anio==0 && mes==0){ //realiza la primera pasada, las variables estan igualadas a 0
				anio = datosMeteorologicos.anio;
				mes = datosMeteorologicos.mes;
				promTempMax += datosMeteorologicos.tMax;
				promTempMin += datosMeteorologicos.tMin;
				promTempMed += datosMeteorologicos.tMedia;
				promHumRel	+= datosMeteorologicos.hRel;
				contador++;
			}
			else{
				if(datosMeteorologicos.anio==anio && datosMeteorologicos.mes==mes){
					promTempMax += datosMeteorologicos.tMax;
					promTempMin += datosMeteorologicos.tMin;
					promTempMed += datosMeteorologicos.tMedia;
					promHumRel	+= datosMeteorologicos.hRel;
					contador++;
				}
			}
			
			fread(&datosMeteorologicos,sizeof(datos),1,fd);
			
			if(datosMeteorologicos.anio!=anio || datosMeteorologicos.mes!=mes){ //tiene que se || porque puede estar en el mismo año
				printf("\nPromedio del año %i mes %i\n", anio,mes);
				printf("Promedio temperatura máxima %f \n", promTempMax/contador);
				printf("Promedio temperatura mínima %f \n", promTempMin/contador);
				printf("Promedio temperatura media %f \n", promTempMed/contador);
				printf("Promedio humedad relativa %f \n", promHumRel/contador);	
				anio = datosMeteorologicos.anio;
				mes = datosMeteorologicos.mes;
				promTempMax = 0, promTempMin = 0, promTempMed = 0, promHumRel = 0;
				contador = 0;	//igualamos a 0 las variables			
			}			
			if(feof(fd)){
				//imprimimos el promedio del ultimo registro
				printf("\nPromedio del año %i mes %i\n", anio,mes);
				printf("Promedio temperatura máxima %f \n", promTempMax/contador);
				printf("Promedio temperatura mínima %f \n", promTempMin/contador);
				printf("Promedio temperatura media %f \n", promTempMed/contador);
				printf("Promedio humedad relativa %f \n\n", promHumRel/contador);	
			}
		}
	}
	fclose(fd);		
}

//Importar datos de un archivo de texto a un archivo binario - decima opción
void importar(){
	datos dMet; //lo mismo que datos datosMeteorologicos de las otroa funciones, mas corto debido al sscanf
	int registro = 0;
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
					sscanf(cadena,"%i;%i;%i;%[^;];%f;%f;%f;%[^;];%f\n",&dMet.anio,&dMet.mes,&dMet.dia,dMet.fecha,&dMet.tMax,&dMet.tMin,&dMet.tMedia,dMet.tiempo,&dMet.hRel);
					fwrite(&dMet,sizeof(datos),1,fd);
				}
				registro++;	
			}
			printf("\n----------------------------Datos importados correctamente al archivo binario-------------------------\n");
		}
	}
	fclose(fd);
	fclose(fd_csv);
	ordenar(); //llamamos a la función ordenar para poder exportar correctamente estos archivos
}

//Exportar datos del archivo binario a un archivo de texto - onceava opción
void exportar(){
	int contador = 0;
	char opc;
	datos dMet; //definimos una variable de tipo datos
	fd = fopen(archivoOrdenado, "rb"); //abrimos el archivo binario como lectura
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
			fprintf(fd_csv,"Año;Mes;Día;Fecha;Máxima;Mínima;Media;Tiempo;Humedad Relativa %%");
			fread(&dMet,sizeof(datos),1,fd);		
			while(!feof(fd)){
				contador++;
				fprintf(fd_csv,"\n%i;%i;%i;%s;%.2f;%.2f;%.2f;%s;%.2f", dMet.anio,dMet.mes,dMet.dia,dMet.fecha,dMet.tMax,dMet.tMin,dMet.tMedia,dMet.tiempo,dMet.hRel);
				fread(&dMet,sizeof(datos),1,fd);
			}
		}			
	}
	fclose(fd);
	fclose(fd_csv);
	printf("\nExportación finalizada, se exportaron %i archivos\n", contador);
}
//cabecera de la tabla
void cabeceraTabla(){
	printf("------------------------------------------------------------------------------------------------------\n");
	printf("|Reg.| Año  | Mes | Dia |  Fecha  |  Tmax   |  Tmin   |  Tmed   |     Tiempo      | % Humedad Relativa |\n");
	printf("------------------------------------------------------------------------------------------------------\n");
}
//procedimiento de muestra de los registros
void datosTabla(datos datosMeteorologicos, int registro){
	printf("|%4i", registro);
   	printf("| %4i ", datosMeteorologicos.anio );
   	printf("| %2i  ", datosMeteorologicos.mes);
	printf("| %2i  ", datosMeteorologicos.dia);
	printf("| %8s", datosMeteorologicos.fecha);
 	printf("| %7.2f ", datosMeteorologicos.tMax); //el 7 cuenta los tanto la parte entera como los decimales, el 2 nos determina la cantidad de decimales que muestra
	printf("| %7.2f ", datosMeteorologicos.tMin);
	printf("| %7.2f ", datosMeteorologicos.tMedia);	    
	printf("| %15s ", datosMeteorologicos.tiempo); // - valor a izquierda
    printf("|%%%17.2f|", datosMeteorologicos.hRel); //%% es utilizado para imprimir el signo %
	printf("\n------------------------------------------------------------------------------------------------------\n");
}
//procedimiento de carga de registros
void cargaDatos(datos *datosMeteorologicos){ //se pasa la estructura como puntero para poder cargarle los datos
	int opc = 0;
	char valorIngresado[10]; //Variable utilizada para guardar el valor ingresado temporalmente y comprobar si es un entero
	
	do{
		printf("Año\t"); 
		fflush(stdin);
		scanf("%s", &valorIngresado);	
	}while(errorAnio(valorIngresado)); //corroboramos que el año este entre 1901 y el actual, valor ingresado se pasa por defecto por referecia
	datosMeteorologicos->anio = atoi(valorIngresado); //se utiliza el operador flecha debido a que accedemos a los elementos de una estructura que se pasa por referencia
	
	do{
		printf("Mes\t");
		fflush(stdin);
		scanf("%s", &valorIngresado); //operador flecha debido a que se pasa por referencia la variable de tipo estructura
	}while(errorMes(datosMeteorologicos->anio,valorIngresado)); //corroboramos que sea entero
	datosMeteorologicos->mes = atoi(valorIngresado);
	
	do{
		printf("Dia\t");
		fflush(stdin); //limpiamos el buffer
		scanf("%s", &valorIngresado);	//guardamos el valor del usuario en valor ingresado para comprobar si es entero
	}while(errorDia(datosMeteorologicos->anio, datosMeteorologicos->mes, valorIngresado)); //comprobamos que sea un entero
	datosMeteorologicos->dia = atoi(valorIngresado); //cargamos el valor al registro
		
	sprintf(datosMeteorologicos->fecha, "%4i%02i%02i", datosMeteorologicos->anio,datosMeteorologicos->mes, datosMeteorologicos->dia);
	printf("Fecha \t%s\n", datosMeteorologicos->fecha);

	do{
		do{
			printf("Temperatura Maxima\t");
			fflush(stdin);
			scanf("%s", &valorIngresado);	
		}while(!esNumero(valorIngresado));//comprobamos si la temperatura maxima es un numero
		datosMeteorologicos->tMax = atof(valorIngresado); //se convierte de string a float mediante atof	
	}while((datosMeteorologicos->tMax<-100)||(datosMeteorologicos->tMax>100)); //comprobamos que la temperatura este entre -100 y 100 °C
			
	do{
		do{
			printf("Temperatura Minimo\t");
			fflush(stdin);
			scanf("%s", &valorIngresado);	
		}while(!esNumero(valorIngresado));//comprobamos si la temperatura minima es un numero
		datosMeteorologicos->tMin = atof(valorIngresado);  //se convierte de string a float mediante atof	
		//comprobamos que la temperatura este entre -100 y 100 °C, y que sea menor a la temperatura máxima
	}while((datosMeteorologicos->tMin<-100)||(datosMeteorologicos->tMin>100)||(datosMeteorologicos->tMin>datosMeteorologicos->tMax));
	
	do{
		do{
			printf("Temperatura Media\t");
			fflush(stdin);
			scanf("%s",valorIngresado);
		}while(!esNumero(valorIngresado));//comprobamos si la temperatura media es un numero
		datosMeteorologicos->tMedia = atof(valorIngresado); //se convierte de string a float mediante atof
		 //comprobamos que la temperatura este entre -100 y 100 °C, y que este entre la temperatura mínima y máxima
	}while((datosMeteorologicos->tMedia<-100)||(datosMeteorologicos->tMedia>100)||(datosMeteorologicos->tMedia>datosMeteorologicos->tMax)||(datosMeteorologicos->tMedia<datosMeteorologicos->tMin));
	
	//Le mostramos al usuario las opciones a elegir
	printf("Tiempo\n");
	printf("1. Soleado\n");
	printf("2. Despejado\n");
	printf("3. Nublado\n");
	printf("4. Parcial Nublado\n");
	printf("5. Lluvioso\n");
	printf("6. Tormentoso\n");
	printf("7. Nevado\n");

	do{
		fflush(stdin);
		scanf("%i", &opc);
	}while(opc<1 || opc>7); //coroboramos que el valor este entre 1 y 7
	//Dependiendo de la elección del usuario guardamos en datosMeteorologicos->tiempo
	switch(opc){ 
		case 1:
			sprintf(datosMeteorologicos->tiempo, "Soleado"); //escribimos en datosMeteorologicos->tiempo Soleado
		break;
		case 2: 
			sprintf(datosMeteorologicos->tiempo, "Despejado");
		break;
		case 3:
			sprintf(datosMeteorologicos->tiempo, "Nublado");
		break;
		case 4:
			sprintf(datosMeteorologicos->tiempo, "Parcial Nublado");
		break;
		case 5:
			sprintf(datosMeteorologicos->tiempo, "Lluvioso");
		break;
		case 6:
			sprintf(datosMeteorologicos->tiempo, "Tormentoso");
		break;
		case 7:
			sprintf(datosMeteorologicos->tiempo, "Nevado");
		break;
	}		
	//mostramos el tiempo elegido
	printf("Tiempo\t%s\n", datosMeteorologicos->tiempo);
	
	do{
		do{
			printf("Humedad Relativa\t");
			fflush(stdin);		
			scanf("%s", valorIngresado);
		}while(!esNumero(valorIngresado));
		datosMeteorologicos->hRel = atof(valorIngresado); //se convierte de string a float mediante atof
	}while((datosMeteorologicos->hRel>100)||(datosMeteorologicos->hRel<0)); //comprobamos que la humedad ingresada no sea mayor a 100%	
	printf ("\n");
}

//mensaje de error de la comprobación de si el archivo se abre correctamente
void mensajeError(){
	printf("\n----Error al tratar de abrir los archivos----\n");
}
//función de comprobación de si es un numero
int esNumero (const char *s){
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char *p;
    strtod (s, &p);
    return (*p == '\0');
}
//funcion de comprobación de si es un numero entero
int esEntero (const char *cadena){
    int numEntero;
    float numFloat;
    if(esNumero(cadena)){
        numEntero=atoi(cadena);
        numFloat=atof(cadena);
        if((numFloat-numEntero) == 0){
            return 1;
        }else{
            return 0;
        }
    } else{
        return 0;
    }
}

//función para calcular cuando ingresan un año incorrecto
int errorAnio(const char *tip){
    // Esto se utiliza para obtener la hora actual de la computadora
    time_t t;
    struct tm *tm;
    t=time(NULL);
    tm=localtime(&t);
    
	int anioentero, anioactual;
     
    /* Esta linea no deberia mostrarse, solo esta para mostrar que es lo que hace
    printf ("\nHoy es: %02d/%02d/%d\n", tm->tm_mday, 1+tm->tm_mon, 1900+tm->tm_year); */
    anioactual = 1900+tm->tm_year;
    
	if (esEntero(tip)){
        anioentero=atoi(tip);
        if ((anioentero > 1900) && (anioentero <= anioactual)){
            return 0;
        }else {
            return 1;
        }
    }else{
        return 1;
    }
}

//funcion para calcular cuando ingresan un mes incorrecto
int errorMes(int aniotipeado, const char *tip){
    // Esto se utiliza para obtener la hora actual de la computadora
    time_t t;
    struct tm *tm;
    t=time(NULL);
    tm=localtime(&t);
     
    int anioactual;
    int mesentero, mesactual;     
    /*
    printf ("\nHoy es: %02d/%02d/%d\n", tm->tm_mday, 1+tm->tm_mon,
    1900+tm->tm_year); */
    anioactual = 1900+tm->tm_year;
    mesactual = 1+tm->tm_mon;
    
    if (esEntero(tip)){
        mesentero=atoi(tip);
        if (aniotipeado != anioactual){
            if ((mesentero >=1 ) && (mesentero <= 12)){
                return 0;
            }else {
                return 1;
            }
        }else{
            if ((mesentero >=1 ) && (mesentero <= mesactual)){
                return 0;
            }else{
                return 1;
            }
        }
    }else{
        return 1;
    }
}

//funcion para calcular cuando ingresan un mes incorrecto
int errorDia(int aniotipeado, int mestipeado, const char *tip){
	time_t t;
	struct tm *tm;
    t=time(NULL);
    tm=localtime(&t);
     
    int anioactual;
    int mesactual;
    int diaentero, diaactual;
    
    anioactual = 1900+tm->tm_year;
    mesactual = 1+tm->tm_mon;
    diaactual = tm->tm_mday;
    
    //return 1 = bucle, return 0 = salida
    diaentero=atoi(tip); //convertimos a entero la cadena de caracteres
    if (esEntero(tip)){ //corroboramos que el dia sea un entero
        if(aniotipeado != anioactual){
        	//comprobamos si es febrero
        	if(mestipeado==2){
        		if(bisiesto(aniotipeado)){ //ingresa acá si el año es bisiesto
        			if(diaentero<1 || diaentero>29){ 
        				return 1;
					}else{
						return 0;
					}					
				}else{ //ingresa acá si el año no es bisiesto
					if(diaentero<1 || diaentero>28){ 
        				return 1;
					}else{
						return 0;
					}
				}				
			}
			//comprobamos si el mes tiene 31 dias
			if(mestipeado == 1 || mestipeado == 3 || mestipeado == 5 || mestipeado == 7 || mestipeado == 8 || mestipeado == 10 || mestipeado == 12 ){
				if(diaentero<1 || diaentero>31){
        			return 1;
				}else{
					return 0;
				}
			}
			//comprobamos si el mes tiene 30 dias
			if(mestipeado == 4 || mestipeado == 6 || mestipeado == 9 || mestipeado == 11){
				if(diaentero<1 || diaentero>30){
        			return 1;
				}else{
					return 0;
				}
			}        	
		}
		else{ //si el año ingresado es igual al actual ingresa acá (aniotipeado == anioactual)
			if (mestipeado>mesactual){
				return 1;
			}else{ //ingresa aquí si el mes tipeado no es mayor que el actual
				if(mestipeado == mesactual){ //comprobamos si el mes ingresado es el actual 
					if((diaentero <= diaactual) && (diaentero > 0)){
						return 0;
					}else{
						return 1;
					}
				}else{ //si el mes ingresado no es el actual ingresa acá
					if(mestipeado == 2){
						if(bisiesto(aniotipeado)){ //si es bisiesto ingresa aquí
							if(diaentero<1 || diaentero>29){
	        					return 1;
							}else{
								return 0;
							}
						}else{ //si no es bisiesto ingresa aquí
							if(diaentero<1 || diaentero>28){
	        					return 1;
							}else{
								return 0;
							}
						}
					}
					//comprobamos si el mes tiene 31 dias
					if(mestipeado == 1 || mestipeado == 3 || mestipeado == 5 || mestipeado == 7 || mestipeado == 8 || mestipeado == 10 || mestipeado == 12 ){
						if(diaentero<1 || diaentero>31){
		        			return 1;
						}else{
							return 0;
						}
					}
					//comprobamos si el mes tiene 30 dias
					if(mestipeado == 4 || mestipeado == 6 || mestipeado == 9 || mestipeado == 11){
						if(diaentero<1 || diaentero>30){
		        			return 1;
						}else{
							return 0;
						}
					}				
				}					
			}
		}
    }else{
        return 1;
    }
}
//función encargada de coprobar si un año es bisiesto
int bisiesto(int anioentero){
    return (((anioentero % 4 == 0) && (anioentero % 100 != 0)) || (anioentero % 400 == 0));
    /* Asi es otro modo:
    return ((anioentero % 4 == 0) && ((anioentero % 100 != 0) || (anioentero % 400 == 0))); */
}

//metodo de ordenamientop burbuja mejorado
void metodoBurbujaMejorado(datos vector[], int cantidad){
    int interruptor = 1;
    int pasada, j, comparacion = 0;
    for (pasada = 0; pasada < (cantidad-1) && interruptor; pasada++){
    // bucle externo controla la cantidad de pasadas 
    interruptor = 0;
    for (j = 0; j < (cantidad-pasada-1); j++){
    	comparacion = strcmp(vector[j].fecha,vector[j+1].fecha); //devuelve -1,0,1, luego debemos compararlo con un valor en el if
     	if(comparacion>0){ //lo comparamos con 0 en este caso
    // elementos desordenados, es necesario intercambio 
    		//printf("\nDENTRO ORDENAMIENTO BUBBLE SORT\n");
       		datos aux;
        	interruptor = 1;
        	aux = vector[j];
        	vector[j] = vector[j+1];
        	vector[j+1] = aux;
        }        
      }
    }
}

//Metodo de ordenamiento shell
void metodoShell(datos vector[], int n){ //int a[]
    int intervalo,comparacion, i, j, k;
    intervalo = n / 2;
    while (intervalo > 0){
      for (i = intervalo; i < n; i++){
        j = i - intervalo;
        while (j >= 0){
          k = j + intervalo;
          comparacion = strcmp(vector[j].fecha,vector[k].fecha);
          if (comparacion<0)
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


