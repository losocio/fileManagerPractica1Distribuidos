#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "fileOperations.h"

int main(int argc, char** argv){
    
    //auto serverConnection=initClient("127.0.0.1", 60000);
    connection_t serverConnection=initClient("127.0.0.1", 60000);

    std::vector<unsigned char> rpcOut; //TODO might need to have a size on declaration, just intuition though


    //List files test
    fileOperation_t opList;
    opRead.operationType=opListFiles;

    packOperation(rpcOut, opList);

    sendMSG(serverConnection, rpcOut);
    std::vector<std::string*>* resList;
    recvMSG(serverConnection, resList);

    //TODO recorre el vector y printar los ficheros
    //TODO generated code
    std::cout<<"Lista de ficheros en el directorio de prueba:\n";
    for(unsigned int i=0;i<res->size();++i)
    {
        std::cout<<"Fichero: "<<res->at(i)->c_str()<<std::endl;
    }



    //Read files test
    fileOperation_t opRead;
    opRead.operationType=opReadFile;
    opRead.readFile.fileName="dummy.txt"; 

    packOperation(rpcOut, opRead);

    sendMSG(serverConnection, rpcOut);
    std::vector<std::string*>* resRead; //TODO might figure out the pointer thing looking around here
    recvMSG(serverConnection, resRead);

    
    
    //Write file test

    //Close connection
    closeConnection(serverConnection.serverId);
    return 0;
} 