#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#include "memoria.h"
#include "proceso.h"
#include "cpu_info.h"
#define QUANTUM 100000

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
			cout << "Corriendo ";
			Proceso* proc = cola_cpu->get_proceso(slot);
			string nombre = proc->nombre;
			if(proc->id == -1 )
			{
				if(proc->estado == CREADO)
				{
					string p = nombre +"&";
					
					system(p.c_str());
					proc->id = getProcId(nombre);
				}
				else
				{
					proc->estado = TERMINADO;
					set_free();
					return;
				}
				
			}
			cout << "proceso: " << nombre ;
			char buffer [10];
			string parar ="kill -stop " + itoa(proc->id);
			string continuar ="kill -cont " + itoa(proc->id);
			proc->estado = CORRIENDO;
			system(continuar.c_str());
			usleep(QUANTUM);
			system(parar.c_str());
			proc->estado = LISTO;
			/*cout.flush();
			wait(10);
			proc->estado = TERMINADO;*/
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