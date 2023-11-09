#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "fileOperations.h"

int main(int argc, char** argv){
    
    //auto serverConnection=initClient("127.0.0.1", 60000);
    connection_t serverConnection=initClient("127.0.0.1", 60000);

    fileOperation_t op;
    op.operationType=opReadFile;
    op.readFile.fileName="dummy.txt"; 


    std::vector<unsigned char> rpcOut; //TODO might need to have a size on declaration, just intuition though
    packOperation(rpcOut, op);

    sendMSG(serverConnection, rpcOut);
    std::vector<std::string*>* res;
    recvMSG(serverConnection, res);

    // TODO generated code
    std::cout<<"Lista de ficheros en el directorio de prueba:\n";
    for(unsigned int i=0;i<res->size();++i)
    {
        std::cout<<"Fichero: "<<res->at(i)->c_str()<<std::endl;
    }

    closeConnection(serverConnection.serverId);
    return 0;
} 