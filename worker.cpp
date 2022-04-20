#include "mpi.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main (int argc, char ** argv) {
    int parent_size; 
    MPI_Comm intercomm; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_get_parent(&intercomm); 
    int iam;
    MPI_Comm_rank(MPI_COMM_WORLD, &iam);
    MPI_Comm_remote_size(intercomm, &parent_size); 
    if (parent_size!= 1) cout<<"Something's wrong with the parent"<<endl;

    if (intercomm == MPI_COMM_NULL) cout<<"No parent!"<<endl;
    else {
        int parent_id;
        MPI_Bcast(&parent_id, 1, MPI_INT,0, intercomm);
        std::cout<<"Child "<<iam<<" of Parent "<<parent_id<<std::endl;
    }

    MPI_Finalize();
    return 0;
}