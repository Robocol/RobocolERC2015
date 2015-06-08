#include "comunicacion.h"

/*
** ===================================================================
**     Método      :  become_daemon
*/
/*!
**     @resumen
**          Convierte un proceso en un demonio cuando es llamado. Tomado
**		del libro Linux Programming interface
**			
**     @param
**          flags     	   	- Banderas de configuración del demonio
**     @return
**                         	- Estado salida del método. 
*/
/* ===================================================================*/
int becomeDaemon(int flags){
	int maxfd, fd;

	/* Crea un proceso hijo y retorna 0 si tuvo éxito, -1 si hay errores */
	switch (fork()) {
		case -1: return -1;
		case 0: break;
		default: _exit(EXIT_SUCCESS);
	} 

	/* Inicia una nueva sesión para que el proceso sea ejecutado */
	if (setsid() == -1)
		return -1; /* Llega a ser el líder de la sesión */
	
	/* Evita que haya nuevos procesos líderes de sesión */
	switch (fork()) {
		case -1: return -1;
		case 0: break;
		default: _exit(EXIT_SUCCESS);
	}

	if (!(flags & BD_NO_UMASK0))
		umask(0); /* Garantiza que los archivos creados tengan
			los permisos necesarios */

	if (!(flags & BD_NO_CHDIR))
		chdir("/"); /* Cambia el directorio de trabajo a root
				para permitir que los demás sistemas
				de archivos puedan desmontarse */

	if (!(flags & BD_NO_CLOSE_FILES)) { /* Cierra todos los fd heredados */
		maxfd = sysconf(_SC_OPEN_MAX);

	if (maxfd == -1) /* Limit is indeterminate... */
		maxfd = BD_MAX_CLOSE;	/* so take a guess */

	for (fd = 0; fd < maxfd; fd++)
		close(fd);
	}
	if (!(flags & BD_NO_REOPEN_STD_FDS)) {
		close(STDIN_FILENO);
		
		/* Redirige los archivos de salida (0,1 y 2) a /dev/null */
		fd = open("/dev/null", O_RDWR);
		if (fd != STDIN_FILENO)	/* 'fd' uld be 0 */
			return -1;

		if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
			return -1;
	
		if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
			return -1;
	}
	return 0;

}

/*
** ===================================================================
**     Método      :  sighupHandler
*/
/*!
**     @resumen
**          Maneja las señales del demonio, en especial la señal SIGHUP. 
**			
**     @param
**          sig     	   	- Vector de señal
*/
/* ===================================================================*/
static void sighupHandler(int sig){
	hupReceived = 1;
}

int diagnosticar_com(dispositivo* disp){

}

int seleccionar_com(dispositivo* disp){

}

int publicar_mensaje(void* mensaje, ){

}

int esperar_mensaje(){

}


int gettimeofday(struct timeval * tv , struct timezone * tz ){

}
