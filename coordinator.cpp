#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <boost/mpi.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "message.hpp"
using namespace std;


std::string serializeMessage(const Message &m) {
  std::stringstream ss;
  boost::archive::binary_oarchive oa{ss};

  oa << m;

  return ss.str();
}

Task deserializeTask(const std::string &in) {
  std::stringstream ss(in);
  std::cout << "A" << std::endl;
  boost::archive::binary_iarchive ia{ss};
  std::cout << "b" << std::endl;

  Message obj; // Foo must be default-constructible
  ia >> obj;

  std::cout << "C" << std::endl;

  return obj;
}

// MPI_Datatype createMPIType(){
//     MPI_Datatype new_type;
//     int count = 3;
//     int blocklens[] = { 7,7,1 };

//     MPI_Aint indices[3];
//     indices[0] = (MPI_Aint)offsetof(struct block, nrows);
//     indices[1] = (MPI_Aint)offsetof(struct block, ncols);
//     indices[2] = (MPI_Aint)offsetof(struct block, bt);
//     MPI_Datatype newT;
//     MPI_Datatype old_types[] = {MPI_UINT32_T,MPI_UINT32_T, MPI_UINT8_T};
//     MPI_Type_create_struct(count, blocklens, indices, old_types, &new_type);
//     //resized new type
//     MPI_Aint extent = &blocklens[2] - &blocklens[0];
//     MPI_Type_create_resized(new_type, indices[0], (MPI_Aint)sizeof(struct block), &newT);
//     MPI_Type_commit( &newT);
//     return newT;
// }
//there must be another struct for block type-specific info

int main(int argc, char ** argv) {

    MPI_Init( &argc, &argv );
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    MPI_Comm child_comm;
    int  num_processes_to_spawn = 2;
    MPI_Comm_spawn( "./worker", MPI_ARGV_NULL,
                    num_processes_to_spawn, MPI_INFO_NULL,
                    0, MPI_COMM_WORLD,
                    &child_comm, MPI_ERRCODES_IGNORE );
    for(int i= 0; i <num_processes_to_spawn; i++){
        MPI_Send(&rank,1, MPI_INT, i, 0, child_comm);
    } 
    //MPI_Send(&rank,1, MPI_INT, 0,0, child_comm);
    MPI_Bcast(&rank,1,MPI_INT,MPI_ROOT,child_comm);
    int child_id;
    MPI_Status status;
    //only. valid with 
    // for (int i=0; i < num_processes_to_spawn; i++) {
    //     MPI_Recv(&child_id, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, child_comm, &status);
    //     cout << "Spawner received from child rank" << child_id <<endl;
    // };
    //MPI_Bcast(&child_id, 1, MPI_INT, MPI_ANY_SOURCE, child_comm);


    
    // std::cout<<"Spawner received broadcast from child rank "<<child_id<<endl;

    MPI_Comm intra_comm;
    MPI_Intercomm_merge( child_comm, 0, &intra_comm );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    //MPI_Bcast(&child_id, 1, MPI_INT, MPI_ANY_SOURCE, intra_comm);
    //broadcast after intercomm merge das
    int message;
    // if(rank == 0){
    //     MPI_Bcast(&rank,1, MPI_INT,rank,intra_comm);
    // }
    // else{
    //     MPI_Bcast(&message, 1, MPI_INT, MPI_ANY_SOURCE, intra_comm);
    //     cout << "rank" << rank << "received: "<<message <<endl;
    // }

    if ( rank == 0 ) {
        int tmp_size;
        MPI_Comm_size( intra_comm, &tmp_size );
        cout << "size of intra comm world " << tmp_size  <<endl;

        MPI_Comm_remote_size( child_comm, &tmp_size );
        cout << "size of child comm world "<< tmp_size <<endl;

        MPI_Comm_size( MPI_COMM_WORLD, &tmp_size );
        cout << "size of parent comm world"<< tmp_size <<endl;
    }

    //uncomment below to send message by creating a new MPI Type
    //MPI_Datatype bb_type = createMPIType();


    // int btinfolen  = 10;
    // //trying to send dynamic block 
    // struct block bodyblock;
    // bodyblock.btinfo = new uint8_t[btinfolen];   
    // if(rank == 0){
    //     bodyblock.nrows = 2;
    //     bodyblock.ncols = 2;
    //     for (uint8_t i=0; i < btinfolen; i++){
    //         bodyblock.btinfo[i] = i;
    //     }    
    //     cout << "start send body block"<<endl;
    //     MPI_Send(&bodyblock, 1, bb_type, 1, 00, MPI_COMM_WORLD);
    //     cout << "start send body block btinfo"<<endl;
    //     MPI_Send(bodyblock.btinfo, btinfolen, MPI_UINT8_T, 1, 01, MPI_COMM_WORLD);
    // }else if(rank==1){
    //     MPI_Recv( &bodyblock, 1, bb_type, 0, 00, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    //     cout << "receive body block"<<endl;
    //     MPI_Recv( bodyblock.btinfo, btinfolen, MPI_UINT8_T, 0, 01, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    //     cout << "receive btinfo"<<endl;
    // }  

    //MPI_Type_free( &bb_type );
    
    
    //serialization using boost
    Message m = ;
    std::string serMsg = serializeMessage(m);
    int serLen = serMsg.size();
    if(rank == 0){
        MPI_Send((void*) serMsg.data(),serLen,MPI_BYTE,1,00,MPI_COMM_WORLD); 
    }else if(rank==1){
        MPI_Probe(1,00,MPI_COMM_WORLD,&mStatus);
        int number_amount;
        MPI_Get_count(&mStatus, MPI_BYTE, &number_amount);


        char tdata[number_amount + 1];
        MPI_Recv(tdata, number_amount, MPI_BYTE, 1, 8888, MPI_COMM_WORLD, &mStatus);
    }
    MPI_Finalize();

    return 0;
}