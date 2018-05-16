
#ifndef PARSER
#define PARSER

//Cabeceras de los métodos implementados:
int eliminarEspacios(char * linea, char * salida,char * archivo);
int contarComandos(char separador, char * linea, int salida);
void separarComandos(char * separador, char * linea, char ** comandos);
void ejecutarPipe(char** args1,char** args2, int red, char * archivo);
int existe(char * archivo);
int ejecutar(char * linea,char * archivo, int flag2);

#endif
