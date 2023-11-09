#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "fileOperations.h"

int main(int argc, char** argv){
    
    //auto serverConnection=initClient("127.0.0.1", 60000);
    connection_t serverConnection=initClient("127.0.0.1", 60000);

    fileOperation_t op;
    op.operation=opReadFile;
    op.readFile.fileName="dummy.txt"; 


    std::vector<unsigned char> rpcOut; //TODO might need to have a size on declaration, just intuition though
    packOperation(rpcOut, op);

    sendMSG(serverConnection, rpcOut);





} 