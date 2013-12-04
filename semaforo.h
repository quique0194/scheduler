#define RUTA_ARCHIVO "/bin/ls"
#define ID '3'

#include <iostream>
#include <sys/sem.h>
#include <stdlib.h>

using namespace std;

int sem_get(int id)		// id: Nro aleatorio
{
	key_t clave = ftok(RUTA_ARCHIVO, id);
	if (clave == -1)
	{
		cout << "No puedo conseguir clave de semáforo" << endl;
		exit(0);
	}
	int id_semaforo = semget(clave, 1, 0600 | IPC_CREAT);
	if (id_semaforo == -1)
	{
		cout << "No puedo crear semáforo" << endl;
		exit (0);
	}
	return id_semaforo;
}

void sem_set(int id_semaforo, int valor)	// 0 rojo 1 verde
{
	semctl(id_semaforo, 0, SETVAL, valor);
}


struct sembuf op_P [] =
{
  0, -1, 0	/* Decrementa semval o bloquea si cero */
};

void down( int semid )	/* Operación P */
{
	semop ( semid, op_P, 1 );
}

struct sembuf op_V [] =
{
   0, 1, 0		/* Incrementa en 1 el semáforo */
};

void up( int semid )	/* Operación V */
{
	semop ( semid, op_V, 1 );
}


#undef RUTA_ARCHIVO
#undef ID