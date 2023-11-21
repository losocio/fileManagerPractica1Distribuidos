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

//Function to handle the operation requested by the client
void handleOperation(int clientId){ 
	std::vector<unsigned char> rpcIn;
	
	//Receive the request from the client
	recvMSG(clientId, rpcIn);
	
	//Unpack the operation to know the requested operation
	FileOperation *opRecv=unpackOperation(rpcIn);

	//Create a FileManager object to do operations on files
	FileManager *fm=new FileManager(FILEMANAGERPATH);
	
	//Switch to handle different types of operations
	switch(opRecv->opType){
		//List files operation
		case opListFiles:
		{	
			//Call listFiles function
			std::vector<std::string*>* filesList=fm->listFiles();

			std::vector<unsigned char> resList;

			//Iterate through the vector and pack each string
			for(auto elem : *filesList){
				pack(resList, *elem);
			}

			//Send response to the client
			sendMSG(clientId, resList);

			//Free memory
			fm->freeListedFiles(filesList);

			std::cout<<"List files operation succesful!\n\n";
		}break;

		//Read file operation
		case opReadFile:
		{
			//Cast to char* of fileName because the function readFile takes char* as parameter
			char* fileName=const_cast<char*>(opRecv->fileName.c_str());

			char* buffer;
			unsigned long int dataLength;
			//Call readFile function, takes fileName and outputs text into buffer and its length into dataLength
			fm->readFile(fileName, buffer, dataLength);
			
			//Cast buffer to string with a string type constructor
			std::string strRead(buffer, dataLength);

			//Pack the string
			std::vector<unsigned char> resRead;
			pack(resRead, strRead);

			//Send response to the client
			sendMSG(clientId, resRead);

			std::cout<<"Read file operation succesful!\n\n";
		}break;

		//Write file operation
		case opWriteFile:
		{ 	
			//Cast to char* of fileName and data because the function writeFile takes char* as parameters
			char* fileName=const_cast<char*>(opRecv->fileName.c_str());
			char* data=const_cast<char*>(opRecv->data.c_str());
		
			//Calculate dataLength
			unsigned long int dataLength=std::strlen(data);

			//Call writeFile function
			fm->writeFile(fileName, data, dataLength);
			
			//No need to send any response, its just write operation
			
			std::cout<<"Write file operation succesful!\n\n";
		}break;

		default:
			std::cout<<"Error: function not defined\n\n";		
	};
}


int main(int argc, char** argv){

	//Start server
	int serverSocket=initServer(PORT);

	//Attend clients indefinitely
    while(true){
		
		//Wait for client to connect
        while(!checkClient()){
            usleep(1000);
        }
		
		//Get client id
        int clientId=getLastClientID();

		//Handle operation requested by client
	    handleOperation(clientId);

		//Close connection with client
	    closeConnection(clientId);
    }

	//Close server
    close(serverSocket);

    return 0;
}
