#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "utils.h"
#include "fileOperations.h"

int main(int argc, char** argv){
    
    //auto serverConnection=initClient("127.0.0.1", 60000);
    connection_t serverConnection=initClient("127.0.0.1", 60000);

    std::vector<unsigned char> rpcOut; //TODO might need to have a size on declaration, just intuition though


    //List files test
    fileOperation_t opList;
    opList.opType=opListFiles;

    packOperation(rpcOut, opList);

    sendMSG(serverConnection.serverId, rpcOut); //TODO serverConnection.serverId not same type as what func asks for, might need a cast
    //void sendMSG(int clientID, std::vector<t> &data);
    std::vector<std::string*>* resList; 
    //std::vector<std::string*> resList; //TODO might break something later on and need to be like the previous line
    recvMSG(serverConnection.serverId, *resList);//TODO mismatched type, might need a cast
    //void recvMSG(int clientID, std::vector<t> &data);

    //TODO recorre el vector y printar los ficheros
    //TODO generated code
    std::cout<<"Lista de ficheros en el directorio de prueba:\n";
    for(unsigned int i=0;i<resList->size();++i)
    {
        std::cout<<"Fichero: "<<resList->at(i)->c_str()<<std::endl;
    }



    //Read files test
    fileOperation_t opRead;
    opRead.opType=opReadFile;
    opRead.readFile.fileName="dummy.txt"; 

    packOperation(rpcOut, opRead);

    sendMSG(serverConnection.serverId, rpcOut);
    std::vector<std::string*>* resRead; //TODO might figure out the pointer thing looking around here
    //std::vector<std::string*> resRead;
    recvMSG(serverConnection.serverId, *resRead);

    
    
    //TODO Write file test

    //Close connection
    closeConnection(serverConnection.serverId);
    return 0;
} 