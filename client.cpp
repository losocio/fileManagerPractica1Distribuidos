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
    //Each operation is in it's own scope for ease of use

    //List files operation
    {
        std::cout<<"List files operation:\n";

        //Initialize connection to server
        connection_t serverConnection=initClient(IP, PORT);

        std::vector<unsigned char> rpcOut; 

        //Create a FileOperation object with the desired operation type
        FileOperation *opList=new FileOperation(opListFiles); //Won't compile if I use a file operation that isnt defined in the enum

        //No need to pack the data, the operation type is enough

        //Pack the operation type, no extra data necessary for listFiles
        packOperation(rpcOut, opList);

        //Send the message to the server
        sendMSG(serverConnection.serverId, rpcOut);
        
        std::vector<unsigned char> resList;

        //Receive the response from the server
        recvMSG(serverConnection.serverId, resList);

        //Unpack the response until the vector is empty
        std::vector<std::string> vectorList;
        while(!resList.empty()){
            vectorList.push_back(unpack<std::string>(resList));
        }

        //Print the list of files
        for(auto elem : vectorList) std::cout<<elem<<" ";
        std::cout<<"\n\n\n";

        //Free memory
        delete opList;

        //Close connection
        closeConnection(serverConnection.serverId);
    }



    //Read file operation
    {
        std::cout<<"Read file operation:\n";

        //Initialize connection to server
        connection_t serverConnection=initClient(IP, PORT);

        std::vector<unsigned char> rpcOut; 
        
        //Create a FileOperation object with the desired operation type
        FileOperation *opRead=new FileOperation(opReadFile);
        
        //Adding necessary file name for read operation
        opRead->fileName="dummy.txt";

        //Pack the operation type and the data
        packOperation(rpcOut, opRead);

        //Send the message to the server
        sendMSG(serverConnection.serverId, rpcOut);
        std::vector<unsigned char> resRead; 
        
        //Receive the response from the server
        recvMSG(serverConnection.serverId, resRead);

        //Unpack the response
        std::string strRead=unpack<std::string>(resRead); 

        //Print the constents of read file    
        std::cout<<strRead<<"\n\n\n";

        //Free memory
        delete opRead;

        //Close connection
        closeConnection(serverConnection.serverId);
    }



    //Write file operation
    {
        //Initialize connection to server
        connection_t serverConnection=initClient(IP, PORT);

        std::vector<unsigned char> rpcOut; 
        
        //Create a FileOperation object with the desired operation type
        FileOperation *opWrite=new FileOperation(opWriteFile);

        //Adding necessary file name and data for write operation
        opWrite->fileName="newFile.txt";
        opWrite->data="filler text for testing new file";

        //Pack the operation type and the data
        packOperation(rpcOut, opWrite);

        //Send the message to the server
        sendMSG(serverConnection.serverId, rpcOut);
        
        //No need to recieve anything back, its just write operation

        //Free memory
        delete opWrite;

        //Close connection
        closeConnection(serverConnection.serverId);
    }
    

    return 0;
}

/*
Old main function

int main(int argc, char** argv){
    
    //auto serverConnection=initClient("127.0.0.1", 60000);
    connection_t serverConnection=initClient("127.0.0.1", 60000);

    std::vector<unsigned char> rpcOut; 

    //List files test
    //fileOperation_t op;
    //opList.opType=opListFiles;

    FileOperation *opList=new FileOperation(opListFiles);
    
    packOperation(rpcOut, opList);

    sendMSG(serverConnection.serverId, rpcOut);
    
    std::vector<std::string*>* resList; 
    //std::vector<std::string*> resList; 
    recvMSG(serverConnection.serverId, *resList);


    //Read files test

    FileOperation *opRead=new FileOperation(opReadFile);
    //opRead->opType=opReadFile;
    opRead->fileName="dummy.txt";

    packOperation(rpcOut, opRead);

    sendMSG(serverConnection.serverId, rpcOut);
    std::vector<std::string*>* resRead; 
    //std::vector<std::string*> resRead;
    recvMSG(serverConnection.serverId, *resRead);

    
    
    //Write file test

    //Close connection
    closeConnection(serverConnection.serverId);
    return 0;
}
*/
