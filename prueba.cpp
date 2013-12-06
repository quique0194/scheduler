#include <iomanip>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
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



int main(){
	
	system("gedit&");
	cout<< getProcId("gedit")<<"\n\n\n";
	string t;
	cin>>t;
	string parar = "kill -stop "+t;
	string correr = "kill -cont "+t;
	while(1)
	{
		usleep(1000000);

		system(correr.c_str());
		cout<<"..1..\n";
		usleep(1000000);
		system(parar.c_str());
		cout<<"..2..\n";
	}

	return 0;
}