#define RUTA_ARCHIVO "/bin/ls"

#include <iostream>
#include <stdlib.h>
#include <sys/shm.h>

using namespace std;

void* compartir_memoria(int tam, int id=0)
{
    key_t clave = ftok(RUTA_ARCHIVO, id);
    cout << "Para liberar la memoria ejecute: ipcrm -M " << clave << endl;
    if (clave == -1)
	{
		cerr << "No consigo clave para memoria compartida" << endl;
		exit(0);
	}
    int id_memoria = shmget(clave, tam, 0777 | IPC_CREAT);
    if (id_memoria == -1)
	{
		cerr << "No consigo Id para memoria compartida" << endl;
		exit (0);
	}
    void* ptr = shmat(id_memoria, (char *)0, 0);
    if (ptr == NULL)
	{
		cerr << "No consigo memoria compartida" << endl;
		exit (0);
	}
	return ptr;
}

#undef RUTA_ARCHIVO