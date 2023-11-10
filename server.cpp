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

	fileOperation_t op=unpackOperation(rpcIn);
	
	FileManager *fm=new FileManager(FILEMANAGERPATH);

	switch(op.opType){
		case opListFiles: //TODO generated code, but i think its done
		{	
			std::vector<std::string*>* listedFiles;
			//std::vector<std::string*> listedFiles; //TODO might break something later on and need to be like the previous line
			listedFiles=fm->listFiles();
			sendMSG(clientId, *listedFiles); //TODO might have to use &listedFiles or some other reference, probably related to previous comment

			fm->freeListedFiles(listedFiles);
		}break;

		case opReadFile: //TODO generated code
		{
			/*
			std::string readFile;
			fm->readFile(&op.readFile.fileName, , &op.readFile.dataLength);
			//void readFile(char* fileName, char* &data, unsigned long int &dataLength);
			sendMSG(clientId, readFile);
			*/
			std::cout<<"Read file operation\n";
		}break;

		case opWriteFile: //TODO generated code
		{ 	//TODO last parameter is correct, still have to figure out the rest
			//fm->writeFile(op.writeFile.fileName, op.writeFile.data, op.writeFile.dataLength);
			//void writeFile(char* fileName, char* data, unsigned long int dataLength);
			std::cout<<"Write file operation\n";
		}break;

		default:
		{	
			std::cout<<"Error: función no definida\n";
		}		
	};
}


int main(int argc, char** argv)
{

    //auto serverSocket=initServer(60000); //Not sure why aout is used here
	int serverSocket=initServer(60000);

    while(1){
		
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