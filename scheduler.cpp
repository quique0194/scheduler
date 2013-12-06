#include <iostream>
#include <unistd.h>

using namespace std;

#include "memoria.h"
#include "proceso.h"
#include "cpu_info.h"

#define MAX_NRO_PROC 100


class Scheduler{
public:
    Scheduler(int cola_proc_id, int cola_cpu_id){
        cola_proc = (ColaDeProcesos*)compartir_memoria(sizeof(ColaDeProcesos), cola_proc_id);
        cola_proc->init();

        cola_cpu = (ColaDeCpus*)compartir_memoria(sizeof(ColaDeCpus), cola_cpu_id);
        cola_cpu->init();

        ready_procs = 0;
        running_procs = 0;
        empty_procs = &procesos[0];
        for( int i=0; i<MAX_NRO_PROC-1; ++i )
            procesos[i].next = &procesos[i+1];
        procesos[MAX_NRO_PROC-1].next = 0;
    }

    void set_prioridad(Proceso* proc){
        proc->prioridad = 1;
    }

    void get_nuevos_procesos(){
        while( !cola_proc->is_empty() ){
            Proceso* proc = pop_empty_proc();
            cola_proc->retirar(proc);
            proc->estado = CREADO;
            set_prioridad(proc);
            cout << "Recibido: " << proc->nombre << endl;
            push_ready_proc(proc);
        }
    }

    void sched(){
        while( exists_ready_proc() ){
            int free_cpu = cola_cpu->get_free_cpu();
            if( free_cpu == -1 ){
                return;
            }
            //cur->estado = CORRIENDO;
            Proceso* ready = cola_cpu->get_proceso(free_cpu);
         	if (ready->estado == LISTO)	
         	{
                Proceso* p = pop_empty_proc();      // Saca un espacio vacio para el proceso
                *p = *ready;                        // Copia el proceso en el espacio vacio
         		push_ready_proc(p);                 // Añade el espacio ahora lleno a la lista de ready
         	}
            Proceso* cur = pop_ready_proc();
            cola_cpu->set_proceso(free_cpu, cur);
            cola_cpu->set_running(free_cpu);
            cout << "\t\t\tProcesando: " << cur->nombre << endl;
            cout << "\t\t\tCpu: " << free_cpu << endl;

        }
    }

private:
    bool exists_empty_proc(){
        return empty_procs;
    }

    bool exists_ready_proc(){
        return ready_procs;
    }

    Proceso* pop_empty_proc(){
        Proceso* ret = empty_procs;
        empty_procs = empty_procs->next;
        return ret;
    }

    Proceso* pop_ready_proc(){
        Proceso* ret = ready_procs;
        ready_procs = ready_procs->next;
        return ret;
    }

    void push_empty_proc(Proceso* proc){
        proc->next = empty_procs;
        empty_procs = proc;
    }

    void push_ready_proc(Proceso* proc){
        proc->next = ready_procs;
        ready_procs = proc;
    }

    Proceso procesos[MAX_NRO_PROC];
    ColaDeProcesos * cola_proc;         // En memoria compartida
    ColaDeCpus * cola_cpu;            // En memoria compartida

    Proceso* empty_procs;
    Proceso* ready_procs;
    Proceso* running_procs;
};

int main(){
    

    Scheduler sched(0,1);

    while(1){
        sched.get_nuevos_procesos();
        sched.sched();
    }
    
    return 0;
}