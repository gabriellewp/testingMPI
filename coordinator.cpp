#include <stdio.h>
#include <mpi.h>
#include <iostream>
using namespace std;
int main(int argc, char ** argv) {

    MPI_Init( &argc, &argv );
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    MPI_Comm child_comm;
    int  num_processes_to_spawn = 2;
    MPI_Comm_spawn( "./worker2", MPI_ARGV_NULL,
                    num_processes_to_spawn, MPI_INFO_NULL,
                    0, MPI_COMM_WORLD,
                    &child_comm, MPI_ERRCODES_IGNORE );
    MPI_Bcast(&rank,1,MPI_INT,MPI_ROOT,child_comm);
    int child_id;
    MPI_Status status;
    for (int i=0; i < num_processes_to_spawn; i++) {
        MPI_Recv(&child_id, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, child_comm, &status);
        cout << "Spawner received from child rank" << child_id <<endl;
    };
    //MPI_Bcast(&child_id, 1, MPI_INT, MPI_ANY_SOURCE, child_comm);


    
    // std::cout<<"Spawner received broadcast from child rank "<<child_id<<endl;

    MPI_Comm intra_comm;
    MPI_Intercomm_merge( child_comm, 0, &intra_comm );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    //MPI_Bcast(&child_id, 1, MPI_INT, MPI_ANY_SOURCE, intra_comm);
    //broadcast after intercomm merge
    int message;
    if(rank == 0){
        MPI_Bcast(&rank,1, MPI_INT,rank,intra_comm);
    }
    else{
        MPI_Bcast(&message, 1, MPI_INT, MPI_ANY_SOURCE, intra_comm);
        cout << "rank" << rank << "received: "<<message <<endl;
    }
    if ( rank == 0 ) {
        int tmp_size;
        MPI_Comm_size( intra_comm, &tmp_size );
        cout << "size of intra comm world " << tmp_size  <<endl;

        MPI_Comm_remote_size( child_comm, &tmp_size );
        cout << "size of child comm world "<< tmp_size <<endl;

        MPI_Comm_size( MPI_COMM_WORLD, &tmp_size );
        cout << "size of parent comm world"<< tmp_size <<endl;
    }

    MPI_Finalize();

    return 0;
}