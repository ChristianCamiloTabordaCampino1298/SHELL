/* 
 ***********************************************************
 * shell.c contiene el main del programa y contiene la     *
 * funcionalidad para captar los comandos y responder al   *
 * usuario ante la gestión de los mismos.                  *
 *********************************************************** 
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "handler.h"
#include "parser.h"

int main(int argc, char ** argv){
	
	//Bucle infinito de ejecución del shell:
	while(1){
		
		//Creación de las variables a utilizar:
		char lectura[100];
		char * linea = malloc(strlen(lectura) * sizeof(char));
		char * archivo = malloc(20 * sizeof(char)); 
		int direccionar = 0, falla;
		
		//Lectura del texto ingresado por el usuario:		
		printf("$ ");
		scanf(" %[^\n]", lectura);		
		
		//Eliminación de espacios y validación de direccionamiento:		
		direccionar = eliminarEspacios(lectura,linea,archivo);
		
		//Validación del comando de salida:
		if(!strcmp("exit",linea)){
			break;
		}
		
		//Ejecución del fork:
		int rc = fork();
		
		//Validación de error al ejecutar el fork:
		if(rc < 0){
			fprintf(stderr,"Fallo en el fork: No se pudo crear el proceso.\n");
			exit(1);
		
		//Validación del proceso hijo:
		}else if(rc == 0){
			
			//Ejecución del comando y validación de errores:
			falla = ejecutar(linea,archivo,direccionar);
			
			//Validación de existencia del comando:
			if(falla != 1){ 
				printf("Error: No existe el comando.\n");
			}
			
		//Validación del proceso padre:
		}else{
			wait(NULL);
		}		
		
	}
			
	return 0;
	
}
	
				
			
