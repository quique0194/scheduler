 #include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

using namespace std;

#include "memoria.h"
#include "proceso.h"


int main(){
	srand(time(0));

	ColaDeProcesos* cola = (ColaDeProcesos*)compartir_memoria(sizeof(ColaDeProcesos),0);
	cola->init();
	while(1){ 
		Proceso proc;
		cout << "------------------------" << endl;
		cout << "NUEVO PROCESO: " << endl;
		cout << "Nombre: ";
		cin >> proc.nombre;
		proc.id = -1;
		proc.estado = CREADO;

		cola->insertar(proc);
		cout << "Creado: " << proc.nombre << endl;
	}
    return 0;
}