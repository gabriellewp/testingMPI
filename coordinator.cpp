#include "mpi.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main (int argc, char ** argv) {
    int world_size, universe_size, *univ_sizep, flag;
    int spawnError[universe_size-1];

    MPI_Comm intercomm1, intercomm2; //intercomm

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
    if(iam == 0){ //only process 0 spawns porcesses
        MPI_Comm_spawn("worker", MPI_ARGV_NULL, universe_size-1,  
             MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm1,  
             spawnError);
        }
     
    //multi-stage spawning
    // MPI_Comm_spawn("worker", MPI_ARGV_NULL, (universe_size-1)/2,  
    //          MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm1,  
    //          spawnError); 
    // MPI_Comm_spawn("worker", MPI_ARGV_NULL, (universe_size-1)/2,  
    //          MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm2,  
    //          spawnError);


    MPI_Comm_rank(MPI_COMM_WORLD, &iam);
    MPI_Bcast(&iam,1,MPI_INT,MPI_ROOT,intercomm1);
    cout << "test broadcast parent "<<endl;
    
    
    MPI_Comm intra_comm;
    MPI_Intercomm_merge(intercomm1, 0, &intra_comm);
    if(iam == 0){ 
        int temp_size;
        //checking size after merging
        MPI_Comm_size(intra_comm, &temp_size);
        cout << "size of intra comm: "<<temp_size<<endl;
        MPI_Comm_remote_size(intercomm1, &temp_size);
        cout << "size of child comm: "<<temp_size<<endl;
        MPI_Comm_size(MPI_COMM_WORLD, &temp_size);
        cout << "size of parent comm: "<<temp_size<<endl;
    }

    // //test our new intra_comm with broadcasting message
    // MPI_Comm_rank(intra_comm, &iam);
    // if(iam == 0){
    //     MPI_Bcast(&iam, 1, MPI_INT, 0, intra_comm);
    //     //int inter_source = status.MPI_SOURCE;
    //     //std::cout<<"Receive response from child "<<child_id;
    // }else{
    //     int messageInt;
    //     MPI_Bcast(&messageInt, 1, MPI_INT, 0, intra_comm);
    //     cout << "Rank"<<iam<<"receives message from rank 0: "<< messageInt<<endl;
    // }
        

    MPI_Finalize();

    return 0;

}