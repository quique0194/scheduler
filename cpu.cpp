#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#include "memoria.h"
#include "proceso.h"
#include "cpu_info.h"
#define QUANTUM 5000000		// nanosegundos

void wait(int secs){
	for(int i=0; i<secs; ++i){
		sleep(1);
		cout << '.';
		cout.flush();		
	}
	cout <<endl;
}

string itoa(int number)
{
    string n="0123456789";
    string resp;
    for(int j=number%10; number!=0 ; number/=10,j=number%10)
    {
        //resp.push_back(n[j]);
        resp.insert(0,1,n[j]);
    }
    //for(int i = 0 ; i<resp.size()/2 ; i++)  swap(resp[i],resp[resp.size() -1-i]);
	return resp;
}


class Cpu{
public:
	Cpu(int cola_cpu_id){
		cola_cpu = (ColaDeCpus*)compartir_memoria(sizeof(ColaDeCpus), cola_cpu_id);
        cola_cpu->init();

        slot = cola_cpu->get_slot();
        cout << "slot: " << slot << endl;
        if(slot==-1){
        	cout << "No se puede tener más cpus" << endl;
        	exit(0);
        }
	}



	void run(){
		if( is_running() ){
			Proceso* proc = cola_cpu->get_proceso(slot);
			if(proc->estado == CREADO)
			{
				proc->id = crear_realproc(proc);
			}
			else if(proc->estado == LISTO){
				reanudar_realproc(proc);
			}

			proc->estado = CORRIENDO;
			cout << "Corriendo proceso: " << proc->nombre << endl;
			cout.flush();

			usleep(QUANTUM);
			parar_realproc(proc);
			proc->estado = LISTO;
			set_free();
		}
		
	}


private:

	int crear_realproc(Proceso* proc){
		string nombre = proc->nombre;
		string p = nombre +"&";
		system(p.c_str());
		return getProcId(nombre);
	}

	void reanudar_realproc(Proceso* proc){
		string continuar ="kill -cont " + itoa(proc->id);
		system(continuar.c_str());
	}

	void parar_realproc(Proceso* proc){
		string parar ="kill -stop " + itoa(proc->id);
		system(parar.c_str());
	}

	bool is_running(){
		return cola_cpu->is_running(slot);
	}

	void set_running(){
		cola_cpu->set_running(slot);
	}

	void set_free(){
		cola_cpu->set_free(slot);
	}

	int slot;

	ColaDeCpus * cola_cpu;
};


int main(){
	Cpu cpu(1);

	while(1){
		cpu.run();
	}
	return 0;
}