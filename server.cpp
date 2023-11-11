#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string> //TODO might not be needed
#include <vector>
#include "utils.h"
#include "fileOperations.h"
#include "filemanager.h"

#define FILEMANAGERPATH "./dirprueba/"

/*
For some reason, the communications are only packaged and then unpackaged
in a client to server communication and not vice versa
*/

void handleOperation(int clientId){ //TODO copied code, needs modification
	std::vector<unsigned char> rpcIn;
	
	recvMSG(clientId, rpcIn);

	//fileOperation_t op;
	//op=unpackOperation(rpcIn);

	//TODO this might not work
	FileOperation *op=unpackOperation(rpcIn);
	//FileOperation *op=new FileOperation();
	//op=unpackOperation(rpcIn);
	
	FileManager *fm=new FileManager(FILEMANAGERPATH);
	std::cout<<"server.cpp: op->opType: "<<op->opType<<"\n"; //TODO
	//switch(op.opType){
	switch(op->opType){
		case opListFiles: //TODO generated code, but i think its done
		{	
			std::vector<std::string*>* listedFiles=fm->listFiles();
			//std::vector<std::string*> listedFiles; //TODO might break something later on and need to be like the previous line

			std::cout<<"server.cpp: In case opListFiles: listedFiles reference="<<listedFiles<<"\n"; //TODO
			std::cout<<"assuming listedFiles is correct cause making printer is tedious\n"; //TODO

			sendMSG(clientId, **listedFiles); //TODO mismatched type, gotta check it
			//sendMSG(clientId, *listedFiles); //TODO how it was before, wrong

			/*
			std::vector<unsigned char> serializedList = vectorToString(*listedFiles);
			sendMSG(clientId, serializedList);
			*/

			fm->freeListedFiles(listedFiles);

			//TODO testing
			std::cout<<"List files operation succesful\n";
		}break;

		case opReadFile: //TODO generated code
		{
			/*
			//TODO change op. to op->
			std::string readFile;
			fm->readFile(&op.readFile.fileName, , &op.readFile.dataLength);
			//void readFile(char* fileName, char* &data, unsigned long int &dataLength);
			sendMSG(clientId, readFile);
			*/

			//TODO testing
			std::cout<<"Read file operation succesful\n";
		}break;

		case opWriteFile: //TODO generated code
		{ 	//TODO last parameter is correct, still have to figure out the rest
			//TODO change op. to op->
			//fm->writeFile(op.writeFile.fileName, op.writeFile.data, op.writeFile.dataLength);
			//void writeFile(char* fileName, char* data, unsigned long int dataLength);
			
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
    //auto serverSocket=initServer(60000); //Not sure why auto is used here
	int serverSocket=initServer(60000);

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