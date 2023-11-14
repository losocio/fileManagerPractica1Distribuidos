#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "utils.h"
#include "fileOperations.h"
#include "filemanager.h"

#define PORT 60000
#define FILEMANAGERPATH "./dirprueba/"

void handleOperation(int clientId){ 
	std::vector<unsigned char> rpcIn;
	
	recvMSG(clientId, rpcIn);
	
	FileOperation *opRecv=unpackOperation(rpcIn);

	FileManager *fm=new FileManager(FILEMANAGERPATH);
	
	switch(opRecv->opType){
		case opListFiles: //TODO finish
		{	
			std::vector<std::string*>* resList=fm->listFiles();
			
			for (auto elem : *resList) std::cout << *elem << " "; //TODO testing
    		std::cout << std::endl; //TODO testing

			sendMSG(clientId, *resList);

			fm->freeListedFiles(resList);

			//TODO testing
			std::cout<<"List files operation succesful\n";
		}break;

		case opReadFile:
		{
			char* fileName=const_cast<char*>(opRecv->fileName.c_str());
			char* buffer;
			unsigned long int dataLength;
			fm->readFile(fileName, buffer, dataLength);
			
			std::string strRead(buffer, dataLength);

			std::vector<unsigned char> resRead;
			pack(resRead, strRead);
			sendMSG(clientId, resRead);

			//TODO testing
			std::cout<<"Read file operation succesful\n";
		}break;

		case opWriteFile:
		{ 	
			char* fileName=const_cast<char*>(opRecv->fileName.c_str());
			char* data=const_cast<char*>(opRecv->data.c_str());
		
			unsigned long int dataLength=std::strlen(data);

			
			fm->writeFile(fileName, data, dataLength);
			
			//No need to send anything back, its just write operation
			
			//TODO testing
			std::cout<<"Write file operation succesful\n";
		}break;

		default:
		{	
			std::cout<<"Error: función no definida\n";
		}		
	};
}


int main(int argc, char** argv){

	int serverSocket=initServer(PORT);

    while(true){
		
        while(!checkClient()){
            usleep(1000);
        }
		
        int clientId=getLastClientID();

	    handleOperation(clientId);

	    closeConnection(clientId);
    }

    close(serverSocket);
    return 0;
}