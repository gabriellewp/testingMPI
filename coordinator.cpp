#include "mpi.h"
#include <iostream>
#include <stdlib.h>

using namespace std;


int main (int argc, char ** argv) {
    int world_size, universe_size, *univ_sizep, flag;
    int spawnError[universe_size-1];

    MPI_Comm everyone; //intercomm

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if  (world_size != 1) cout<<"Top heavy with management, preferable to have 1 proc on coordinator side."<<endl;
    int iam;

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
    cout << "spawning workers with universe size" <<  universe_size << endl;
    MPI_Comm_spawn("worker", MPI_ARGV_NULL, universe_size-1,  
             MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,  
             spawnError); 

    MPI_Comm_rank(MPI_COMM_WORLD, &iam);
    MPI_Bcast(&iam,1,MPI_INT,MPI_ROOT,everyone);

    MPI_Finalize();

    return 0;

}