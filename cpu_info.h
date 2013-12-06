#ifndef CPU_INFO_H
#define CPU_INFO_H

#define NRO_CPUS 2

struct CpuInfo{
	bool conectado;
	bool running;			// 0 libre, 1 corriendo
	Proceso proceso;
};

class ColaDeCpus{
public:
	void init(){
		if(!ini){
			ini = 1;
			for(int i=0; i<NRO_CPUS; ++i){
				cpus[i].conectado = 0;
				cpus[i].running = 0;	
				cpus[i].proceso.estado == TERMINADO;
				cpus[i].proceso.sprio = 0;
				cpus[i].proceso.dprio = 0;
			}
			
		}
	}


	bool exists_free_cpu(){
		for( int i=0; i<NRO_CPUS && cpus[i].conectado; ++i ){
			if( !cpus[i].running )
				return 1;
		}
		return 0;
	}

	int get_free_cpu(){
		for( int i=0; i<NRO_CPUS && cpus[i].conectado; ++i )
			if( !cpus[i].running ){
				return i;
			}
		return -1;			
	}

	int get_slot(){
		for( int i=0; i<NRO_CPUS; ++i )
			if( !cpus[i].conectado ){
				cpus[i].conectado = 1;
				return i;
			}				
		return -1;
	}

	void set_proceso(int slot, Proceso* proc){
		cpus[slot].proceso = *proc;
	}

	Proceso* get_proceso(int slot){
		return &cpus[slot].proceso;
	}

	void set_running(int slot){
		cpus[slot].running = 1;
	}

	bool is_running(int slot){
		return cpus[slot].running;
	}

	void set_free(int slot){
		cpus[slot].running = 0;
	}

private:
	CpuInfo cpus[NRO_CPUS];

	bool ini;					// Verifica si el objeto ya fue inicializado
};

#endif