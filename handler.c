/* 
 ***********************************************************
 * handler.c contiene las funcionalidades necesarias para  *
 * gestionar la ejecución de comandos con pipe, sin pipe o *
 * con direccionamiento de archivos.                       *
 *********************************************************** 
 */ 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "parser.h"

//Método para gestionar los comandos que contienen pipe o direccionamiento:
void ejecutarPipe(char ** args1, char ** args2, int direccionar, char * archivo){
	
	//Creación de las variables a utilizar:
	int tuberia[2];
	pid_t pid;
	
	//Ejecución del pipe y del fork:
	pipe(tuberia);
	pid = fork();
	
	//Validación de error al ejecutar el fork:
	if(pid < 0){
		printf("Fallo en el fork: No se pudo crear el proceso.\n");
		exit(-1);
		
	//Validación del proceso hijo:
	}else if(pid == 0){   
		
			//Generación del duplicado del descriptor [1]:
      		dup2(tuberia[1],1); 
      		
      		//Cierre del lado de lectura del pipe:
			close(tuberia[0]);	
			
			//Cierre del descriptor de fichero que sobra tras el dup2():
			close(tuberia[1]);			
			
			//Ejecución del primer comando: 
			execvp(args1[0],args1);	
		
		//Validación del proceso padre:	
		}else{
			
			//Generación del duplicado del descriptor [2]:
			dup2(tuberia[0],0);	
			
			//Cierre del lado de lectura del pipe:
			close(tuberia[1]);
			
			//Cierre del descriptor de fichero que sobra tras el dup2():				
			close(tuberia[0]);				
			
			//Validación del direccionamiento:
			if(direccionar == 1){
				close(STDOUT_FILENO);
				open(archivo,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
			}
			
			//Ejecución del segundo comando:
			execvp(args2[0],args2);	
					  
		}	
			
}

//Método para validar si el archivo donde se va a direccionar existe:
int existe(char * archivo){   
	
	//Creación de las variables a utilizar:
	int aux = 0;
	
	//Concatenación de la extensión del archivo:
	archivo = strcat(archivo,".output");  
	
	//Intento de abrir el archivo:
	int ar = open(archivo, O_RDONLY);  
	
	//Validación de la existencia del archivo:	
	if(ar < 0){	
		aux = 0;
	}else{	
		
		//Cierre del archivo:							
		close(ar);  					
		aux = 1; 
		
	}
	
	return aux;
	
}	

//Método para ejecutar los comandos:	
int ejecutar(char * linea, char * archivo, int direccionar){
	
	//Creación de las variables a utilizar:
	int cantidad, pipe = 0, posible = 0, exis = 0; 
	
	//Conteo de los comandos:
	cantidad = contarComandos('|',linea,cantidad);
	
	//Creación del arreglo para almacenar los comandos:
	char ** comandos = malloc(cantidad * sizeof(char *));
	for(int x=0; x<cantidad; ++x){
		comandos[x] = malloc(strlen(linea) * sizeof(char));
	}
	
	//Almacenamiento de los comandos:
	separarComandos("|",linea,comandos);
	
	//Validación del pipe:
	if(cantidad == 2){
			pipe = 1;
	}
	
	//Validación del direccionamiento:
	if(direccionar == 1){  
		
		//Validación de la existencia del archivo:
		exis = existe(archivo);
		if(exis == 1){
			printf("Error: El archvio ya existe.\n");
			
			//Indicación del tipo de error: archivo existente:
			return 1; 	
								
		}else{
			
			//Indicación de que es posible crear el archivo:
			posible = 1; 	
									
		}	
			
	}
	
	//Conteo de la cantidad de argumentos del primer comando:	
	cantidad = contarComandos(' ',comandos[0],cantidad);
	
	//Creación del arreglo para almacenar los argumentos del primer comando:
	char ** primero = malloc(cantidad * sizeof(char *));
	for(int x=0; x<cantidad; ++x){
		primero[x] = malloc(strlen(comandos[0]) * sizeof(char));
	}
	separarComandos(" ",comandos[0],primero);
	
	//Preparación del primer arreglo a ejecutar:	
	char * args[cantidad + 1];
	for(int x=0; x<cantidad; x++){
		args[x] = strdup(primero[x]);
	}
	args[cantidad] = NULL;

	//Validación de la existencia del archivo:
	if(exis != 1){
		
		//Validación del pipe:						
		if(pipe){	
			
			//Conteo de la cantidad de argumentos del segundo comando:			
			cantidad = contarComandos(' ',comandos[1],cantidad);
			
			//Creación del arreglo para almacenar los argumentos del segundo comando:
			char ** segundo = malloc(cantidad * sizeof(char *));
			for(int x=0; x<cantidad; ++x){
				segundo[x] = malloc(strlen(comandos[1]) * sizeof(char));
			}
			separarComandos(" ",comandos[1],segundo);
			
			//Preparación del segundo arreglo a ejecutar:
			char * args2[cantidad + 1];
			for(int x=0; x<cantidad; x++){
				args2[x] = strdup(segundo[x]);
			}
			args2[cantidad] = NULL;	
			
			//Ejecución del comando con pipe:
			ejecutarPipe(args,args2,posible,archivo);	
			
		}else{
			
			//Validación de posibilidad de crear el archivo:
			if(posible == 1){ 	
				
				//Creación del archivo:
				close(STDOUT_FILENO);
				open(archivo,O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);  
				
			}
			
			//Ejecución del comando sin pipe:
			execvp(args[0],args); 	
							
		}
		
	}	
	
}
