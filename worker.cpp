#include <stdio.h> 
#include <mpi.h>
#include <iostream>
using namespace std;
int main(int argc, char ** argv) {

    MPI_Init( &argc, &argv );

    int myrank, parent_rank, parent_size;
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

    MPI_Comm parentcomm;
    MPI_Comm_get_parent( &parentcomm );
    MPI_Comm_rank(parentcomm, &parent_rank);
    MPI_Comm_remote_size(parentcomm, &parent_size); 
    if (parent_size!= 1) cout<<"Something's wrong with the parent"<<endl;
    int parent_id;

    if (parentcomm == MPI_COMM_NULL) cout<<"No parent!"<<endl;
    else {
        int parent_id;
        //MPI_Bcast(&parent_id, 1, MPI_INT, 0, parent_comm);
        if(myrank == 0){
            MPI_Recv(&parent_id, 1, MPI_INT, 0, 0, parentcomm, MPI_STATUS_IGNORE);
            cout<<"Child "<<myrank<<" receive message from parent: "<<parent_id<<std::endl;
        }
        MPI_Bcast(&parent_id, 1, MPI_INT, 0, parentcomm);

        std::cout<<"Child "<<myrank<<" of Parent "<<parent_id<<std::endl;
        MPI_Send(&myrank, 1, MPI_INT, 0, 0, parentcomm);
        //MPI_Bcast(&myrank,1, MPI_INT,myrank,parentcomm);
        int message;
        int rootproc;
        if(myrank == 0)
            rootproc = MPI_ROOT;
        else
            rootproc = MPI_PROC_NULL;
        //MPI_Bcast(&myrank,1, MPI_INT,parent_rank,parentcomm);

        // if(myrank == 0)
        //     MPI_Bcast(&myrank,1, MPI_INT,MPI_ROOT,parentcomm);
        // else
        //     MPI_Bcast(&message, 1, MPI_INT, MPI_ANY_SOURCE, parentcomm);

        MPI_Comm intra_comm;
        MPI_Intercomm_merge( parentcomm, 1, &intra_comm );
        MPI_Comm_rank(intra_comm, &myrank);
        

        if ( myrank == 0 ) {
            int tmp_size;
            MPI_Comm_remote_size( parentcomm, &tmp_size );
            cout << "child size of parent comm world"<< tmp_size <<endl;

            MPI_Comm_size( MPI_COMM_WORLD, &tmp_size );
            cout << "child size of child comm world" << tmp_size <<endl;

            MPI_Comm_size( intra_comm, &tmp_size );
            cout << "child size of intra comm world"<< tmp_size <<endl;
        }
       
    }


    MPI_Finalize();

    return 0;
}