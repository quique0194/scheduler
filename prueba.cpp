#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <unistd.h>
using namespace std;

void limpiar_linea(int line_width = 79){
	cout << "\r";
	cout << setw(line_width) << setfill(' ');
	cout << "\r";
}

void wait(int secs){
	for(int i=0; i<secs; ++i){
		sleep(1);
		cout << '.';
		cout.flush();
	}
	cout <<endl;
}

int main(){
	cout << "mierda" << endl;
	wait(10);

	return 0;
}