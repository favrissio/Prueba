#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* valor = NULL;
	char* ip;
	char* puerto;

	//t_log* logger; No andaba logger en util.c si la definia aca la puse como extern en el util.h
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	if(logger != NULL){
		log_info(logger, "Hola! Soy un log");
	} else {
		log_info(logger, "No se pudo crear el archivo de log");
	}

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = (char*)malloc(1024*sizeof(char));
	ip = (char*)malloc(1024*sizeof(char));
	puerto = (char*)malloc(1024*sizeof(char));

	leer_config(config, valor, ip, puerto);

	/*if(config != NULL){
		// Obtengo el CLAVE de la config
		if(config_has_property(config, "CLAVE")){

			valor = config_get_string_value(config, "CLAVE");
			// Loggeamos el valor de config
		    log_info(logger, valor);
			} else {
				log_info(logger, "No existe la key CLAVE en al archivo de configuracion");
			}

			// Obtengo IP de la config
			if(config_has_property(config, "IP")){
				ip = config_get_string_value(config, "IP");
			} else {
				log_info(logger, "No existe la key IP en al archivo de configuracion");
			}

			// Obtengo PUERTO de la config
			if(config_has_property(config, "PUERTO")){
				puerto = config_get_string_value(config, "PUERTO");
			} else {
				log_info(logger, "No existe la key PUERTO en al archivo de configuracion");
			}

	} else {
		log_info(logger, "No se pudo leer el archivo de configuracion");
	}*/

	/* ---------------- LEER DE CONSOLA ---------------- */


	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	if(conexion != -1){
	  // Enviamos al servidor el valor de CLAVE como mensaje
	  log_info(logger, "Enviando mensaje al servidor ...");
	  enviar_mensaje(valor, conexion);
	  log_info(logger, "Mensaje enviado");
	}
	// Armamos y enviamos el paquete
	paquete(conexion);

	free(valor);
	free(ip);
	free(puerto);

    // Terminamos el programa
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	//t_log* nuevo_logger;

	//return nuevo_logger;

	t_log* nuevo_logger = logger = log_create("tp0.log", "cliente", 1, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("/home/utnso/tp0/client/cliente.config");

	return nuevo_config;
}

void leer_config(t_config* config, char* valor, char* ip, char* puerto){

	strcpy(valor,"fabricio");

	if(config != NULL){
			// Obtengo el CLAVE de la config
			if(config_has_property(config, "CLAVE")){
				strcpy(valor,config_get_string_value(config, "CLAVE"));
				// Loggeamos el valor de config
			} else {
				log_info(logger, "No existe la key CLAVE en al archivo de configuracion");
			}

			// Obtengo IP de la config
			if(config_has_property(config, "IP")){
				strcpy(ip,config_get_string_value(config, "IP"));
			} else {
			    log_info(logger, "No existe la key IP en al archivo de configuracion");
			}

			// Obtengo PUERTO de la config
			if(config_has_property(config, "PUERTO")){
				strcpy(puerto,config_get_string_value(config, "PUERTO"));
			} else {
				log_info(logger, "No existe la key PUERTO en al archivo de configuracion");
			}

		} else {
			log_info(logger, "No se pudo leer el archivo de configuracion");
		}
}

void leer_consola(t_log* logger)
{
	char* leido;

	puts("Ingrese el mensaje");
	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while(strcmp(leido,"")){
    	log_info(logger, leido);
    	// Leo la siguiente linea
    	puts("Ingrese el siguiente mensaje");
    	leido = readline("> ");
    }

	// ¡No te olvides de liberar las lineas antes de regresar!
    free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	puts("Ingrese el mensaje a agregar en el paquete");
	// Leemos y esta vez agregamos las lineas al paquete
	paquete = crear_paquete();

	leido = readline("> ");
	while(strcmp(leido,"")){
		int tamanio = strlen(leido) + 1;
		// Agrego al paquete
		agregar_a_paquete(paquete, leido, tamanio);
		puts("Ingrese el siguiente mensaje a agregar en el paquete");
		// Vuelvo a leer
		leido = readline("> ");
	}

	// Envio el paquete
	log_info(logger, "Enviando paquete...");
	enviar_paquete(paquete, conexion);
	log_info(logger, "paquete enviado");

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	// ELimino el paquete
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

}
