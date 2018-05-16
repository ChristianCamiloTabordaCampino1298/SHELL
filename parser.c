
/* 
 ***********************************************************
 * parser.c contiene los métodos necesarios para gestionar *
 * el comando principal y extraer las partes significativas*
 * del mismo.                                              *
 *********************************************************** 
 */ 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//Método para eliminar los espacios sobrantes y validar si hay direccionamiento:
int eliminarEspacios(char * linea, char * salida, char * archivo){
	
	//Creación de las variables a utilizar:
	int espacios = 1, contador = 0, contador2 = 0, direccionar = 0;
	
	//Bucle para recorrer la cadena de texto:
	for(int x=0; x<strlen(linea); x++){
		
		//Validación en caso de espacios al final de la línea:
		if(x == strlen(linea)-1 && linea[x] == ' '){ 
		}else{
						
			//Validación del direccionamiento:
			if(linea[x] == '>' || direccionar == 1){ 
				direccionar = 1;
				
				//Validación para almacenar el nombre del archivo:
				if(linea[x] != ' ' && linea[x] != '>'){  
					archivo[contador2] = linea[x];  
					contador2++;								
				}
				contador++;
				
			}else{
				
				//Copiado de la parte significativa de la línea original:
				if(linea[x] != ' ' && direccionar == 0){
					espacios = 0;
					salida[contador] = linea[x];
					contador++;
					
				}else{
					
					//Validación para copiar sólo un espacio por palabra:
					if(espacios == 0 && direccionar == 0){
						espacios = 1;
						salida[contador] = linea[x];
						contador++;
					}
					
				}
				
			}
			
		}	
		
	}
	
	return direccionar;
	   
}

//Método para obtener la cantidad de comandos o de argumentos por comando:
int contarComandos(char separador, char * linea, int salida){
	
	//Creación de las variables a utilizar:
	salida = 1;
	
	//Bucle para contar cada comando o argumento:
	for(int x=0; x<strlen(linea); x++){
		if(linea[x] == separador){
			salida++;
		}
	}
	
	//Ajuste de la cantidad en caso de contar argumentos:	
	if(separador == ' '){
		if(linea[0] == separador && linea[strlen(linea)-1] == separador){
			salida -= 2;
		}else if(linea[0] == separador || linea[strlen(linea)-1] == separador){
			salida--;
		}
	}
	
	return salida;
	
}

//Método para separar los comandos o los argumentos en un arreglo:
void separarComandos(char * separador, char * linea, char ** comandos){
	
	//Creación de las variables a utilizar:
	int contador = 0;
	char * token = strtok(linea,separador);
	
	//Bucle para filtrar los comandos o argumentos:
	while(token != NULL){
		strcpy(comandos[contador],token);
		contador++;
		token = strtok(NULL,separador);
	}
	
}


