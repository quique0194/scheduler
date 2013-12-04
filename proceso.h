#define TAM_BUF_PROC 10

#include "semaforo.h"

#define TERMINADO 0
#define CORRIENDO 1
#define PAUSADO 2

struct Proceso{
	int id;
	char nombre[8];
	Proceso* next;
	int estado;				// 0 terminado, 1 corriendo, 2 pausado
	int prioridad;
	int nro_inst;
};

class ColaDeProcesos{
public:
	void init(){
		if(!ini)
		{
			ini = true;
			first = 0;
			n = 0;
			mutex = sem_get(1);
			full = sem_get(2);
			empty = sem_get(3);
			sem_set(mutex, 1);
			sem_set(full, 0);
			sem_set(empty, TAM_BUF_PROC);
		}

	}

	void insertar(Proceso &proc){
		down(empty);
			down(mutex);
				int pos = ( first + n ) % TAM_BUF_PROC;
				procesos[pos] = proc;
				++n;
			up(mutex);
		up(full);
	}

	void retirar(Proceso *proc){
		down(full);
			down(mutex);
				*proc = procesos[first];
				first = (first+1) % TAM_BUF_PROC;
				--n;
			up(mutex);
		up(empty);
	}

	bool is_empty(){
		return !n;
	}
//private:
	int first;
	int n; 			// Nro de procesos en cola
	Proceso procesos[TAM_BUF_PROC];
	int mutex;		// Semaforo
	int full;		// Semaforo 
	int empty;		// Semaforo 

	bool ini;		// Verifica si el objeto ya fue inicializado
};