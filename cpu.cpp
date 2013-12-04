#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#include "memoria.h"
#include "proceso.h"
#include "cpu_info.h"

void wait(int secs){
	for(int i=0; i<secs; ++i){
		sleep(1);
		cout << '.';
		cout.flush();		
	}
	cout <<endl;
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
			cout << "Corriendo ";
			Proceso* proc = cola_cpu->get_proceso(slot);
			cout << "proceso: " << proc->nombre ;
			cout.flush();
			wait(5);
			proc->estado = TERMINADO;
			set_free();
		}
		
	}


private:

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