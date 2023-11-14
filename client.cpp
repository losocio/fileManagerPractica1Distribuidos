#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "utils.h"
#include "fileOperations.h"

#define IP "127.0.0.1"
#define PORT 60000

int main(int argc, char** argv){
    
    //List files operation
    {
        connection_t serverConnection=initClient(IP, PORT);

        std::vector<unsigned char> rpcOut; 

        FileOperation *opList=new FileOperation(opListFiles);

        packOperation(rpcOut, opList);

        sendMSG(serverConnection.serverId, rpcOut);
        std::vector<unsigned char> resList;
        //std::vector<std::string*> resRead;
        
        recvMSG(serverConnection.serverId, resList);

        //TODO unpack the vector of strings
        //dont have to use unpackOperation() cause it doesnt have an operation
        std::vector<std::string> vectorList=unpack<>();

        std::string strRead=unpack<std::string>(resList); 

        std::cout<<strRead<<"\n";

        delete opList;

        //Close connection
        closeConnection(serverConnection.serverId);
    }


    //Read file operation
    {
        connection_t serverConnection=initClient(IP, PORT);

        std::vector<unsigned char> rpcOut; 
        
        
        FileOperation *opRead=new FileOperation(opReadFile);
        opRead->fileName="dummy.txt";

        packOperation(rpcOut, opRead);

        sendMSG(serverConnection.serverId, rpcOut);
        std::vector<unsigned char> resRead; 
        
        recvMSG(serverConnection.serverId, resRead);

        std::string strRead=unpack<std::string>(resRead); 

        std::cout<<strRead<<"\n";

        delete opRead;

        //Close connection
        closeConnection(serverConnection.serverId);
    }


    //Write file operation
    {
        connection_t serverConnection=initClient(IP, PORT);

        std::vector<unsigned char> rpcOut; 
        
        FileOperation *opWrite=new FileOperation(opWriteFile);

        opWrite->fileName="newFile.txt";
        opWrite->data="filler text for testing new file";

        packOperation(rpcOut, opWrite);

        sendMSG(serverConnection.serverId, rpcOut);
        
        //No need to recieve anything back, its just write operation

        delete opWrite;

        //Close connection
        closeConnection(serverConnection.serverId);
    }
    
    return 0;
}

/*
int main(int argc, char** argv){
    
    //auto serverConnection=initClient("127.0.0.1", 60000);
    connection_t serverConnection=initClient("127.0.0.1", 60000);

    std::vector<unsigned char> rpcOut; 

    //List files test
    //fileOperation_t op;
    //opList.opType=opListFiles;

    FileOperation *opList=new FileOperation(opListFiles);
    
    packOperation(rpcOut, opList);

    sendMSG(serverConnection.serverId, rpcOut); //TODO serverConnection.serverId not same type as what func asks for, might need a cast
    
    std::vector<std::string*>* resList; 
    //std::vector<std::string*> resList; //TODO might break something later on and need to be like the previous line
    recvMSG(serverConnection.serverId, *resList);//TODO mismatched type, might need a cast
    
    //TODO the reason why it doesnt continue from here is because the server isnt responding


    //Read files test

    FileOperation *opRead=new FileOperation(opReadFile);
    //opRead->opType=opReadFile;
    opRead->fileName="dummy.txt";

    packOperation(rpcOut, opRead);

    sendMSG(serverConnection.serverId, rpcOut);
    std::vector<std::string*>* resRead; 
    //std::vector<std::string*> resRead;
    recvMSG(serverConnection.serverId, *resRead);

    
    
    //TODO Write file test

    //Close connection
    closeConnection(serverConnection.serverId);
    return 0;
}
*/
