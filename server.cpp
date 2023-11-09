#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "fileOperations.h"
#include "filemanager.h"

#define FILEMANAGERPATH "./dirprueba/"

/*
For some reason, the communications are only packaged and then unpackaged
in a client to server communication and not vice versa
*/

void attendOperation(int clientId){ //TODO copied code, needs modification
	std::vector<unsigned char> rpcIn;
	
	recvMSG(clientId, rpcIn);

	operacion_t op=unpackOperation(rpcIn);
	
	FileManager *fm=new FileManager(FILEMANAGERPATH);

	switch(op.operationType){
		case opListFiles: //TODO generated code
		{
			std::vector<std::string*>* listedFiles;
			listedFiles=fm->listFiles();
			sendMSG(clientId, listedFiles);

			fm->freeListedFiles(listedFiles);
		}break;

		case opReadFile: //TODO generated code
		{
			std::string res;
			fm->readFile(op.readFile.fileName,,);
			//void readFile(char* fileName, char* &data, unsigned long int & dataLength);
			sendMSG(clientId,res);
		}break;

		case opWriteFile: //TODO generated code
		{
			fm->writeFile(op.writeFile.fileName, op.writeFile.data, op.writeFile.dataLength);
			//void writeFile(char* fileName, char* data, unsigned long int dataLength);
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

        
	    attendOperation(clientId);
        

	    closeConnection(clientId);
    }

    close(serverSocket);
    return 0;
}