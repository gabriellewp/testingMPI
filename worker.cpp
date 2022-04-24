#include "mpi.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main (int argc, char ** argv) {
    int parent_size, parent_proc, parent_rank; 
    MPI_Comm parent_comm; //parent comm
    MPI_Init(&argc, &argv); 

    //parent world
    MPI_Comm_get_parent(&parent_comm); 
    MPI_Comm_rank(parent_comm, &parent_rank);
    MPI_Comm_remote_size(parent_comm, &parent_size); 

    //my world
    int iam;
    MPI_Comm_rank(MPI_COMM_WORLD, &iam);


    cout<<"test worker"<<iam <<endl;
    if (parent_size!= 1) cout<<"Something's wrong with the parent"<<endl;

    if (parent_comm == MPI_COMM_NULL) cout<<"No parent!"<<endl;
    else {
        int parent_id;
        MPI_Bcast(&parent_id, 1, MPI_INT, 0, parent_comm);
        //MPI_Barrier(intercomm);
        std::cout<<"Child "<<iam<<" of Parent "<<parent_id<<std::endl;

        MPI_Comm intra_comm;
        MPI_Intercomm_merge(parent_comm, 1, &intra_comm);

        if(iam  == 0){
            int temp_size;
            MPI_Comm_size(parent_comm, &temp_size);
            cout << "size of parent comm: "<<temp_size<<endl;
            MPI_Comm_remote_size(MPI_COMM_WORLD, &temp_size);
            cout << "size of child comm: "<<temp_size<<endl;
            MPI_Comm_size(intra_comm, &temp_size);
            cout << "size of intra comm: "<<temp_size<<endl;
        }
    }

    MPI_Finalize();
    return 0;
}