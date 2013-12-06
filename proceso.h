#define TAM_BUF_PROC 10

#include "semaforo.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#define CREADO 0
#define LISTO 1
#define CORRIENDO 2
#define TERMINADO 3

int getProcId(string procName)
{
    int pid = -1;

    DIR *dp = opendir("/proc");/*abre /proc (donde estan los procesos) */
    if (dp != NULL)
    {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            int id = atoi(dirp->d_name); /* salta las entradas no númericas*/
            if (id > 0)
            {
                string cmdPath = string("/proc/") + dirp->d_name + "/cmdline";
                ifstream cmdFile(cmdPath.c_str());
                string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    
                    size_t pos = cmdLine.find('\0');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    
                    pos = cmdLine.rfind('/');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    
                    if (procName == cmdLine)
                        pid = id;
                }
            }
        }
    }

    closedir(dp);

    return pid;
}


struct Proceso{
	int id;
	char nombre[8];
	Proceso* next;
	int estado;				// 0 creado, 1 listo, 2 corriendo , 3 terminado
	int sprio;				// prioridad estatica
	int dprio;				// prioridad dinamica
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