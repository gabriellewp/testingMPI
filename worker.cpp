#include "mpi.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main (int argc, char ** argv) {
    int size; 
    MPI_Comm intercomm; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_get_parent(&intercomm); 
    int iam;
    MPI_Comm_rank(MPI_COMM_WORLD, &iam);
    if (intercomm == MPI_COMM_NULL) cout<<"No parent!"<<endl;
    else
        int parent_id;
        //MPI_Bcast(&parent_id, 1, MPI_INT,0, parent);
        std::cout<<"Child "<<iam<<" of Parent "<<parent_id<<std::endl
    MPI_Comm_remote_size(intercomm, &size); 
    if (size != 1) cout<<"Something's wrong with the parent"<<endl;

    MPI_Finalize();
    return 0;
}