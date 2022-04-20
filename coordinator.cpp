#include "mpi.h"
#include <iostream>
#include <stdlib.h>

using namespace std;


int main (int argc, char ** argv) {
    int n_proc, universe_size, *univ_sizep, flag;
    MPI_Comm everyone;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    if  (n_proc != 1) cout<<"Top heavy with management"<<endl;
    int iam;
    MPI_Comm_rank(MPI_COMM_WORLD, &iam);

    MPI_Attr_get(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &univ_sizep, &flag);

    if(!flag){
        cout<< "This MPI does not support UNIVERSE_SIZE"<<endl; //should create a handle for this, but later
        cout << "Enter the universe size: ";
        cin >> universe_size; 
    }else{
        universe_size = *univ_sizep;
    }
    if(universe_size ==1) cout<<"No room to start spawning workers"<<endl;



    //choose_worker_program(worker); 
    MPI_Comm_spawn("worker", MPI_ARGV_NULL, universe_size-1,  
             MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,  
             MPI_ERRCODES_IGNORE); 
    MPI_Finalize();

    return 0;

}