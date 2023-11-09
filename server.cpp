#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "fileOperations.h"
#include "filemanager.h"


void attendOperation(int clientId){ //TODO copied code, needs modification
	std::vector<unsigned char> rpcIn;
	// Se usa recvMSG() para en recibir y guardar los datos de la operacion (como su tipo, etc). Dichos datos se guardan en el vector rpcIn
	recvMSG(clientId, rpcIn);
	// Se utiliza desempaquetaOperacion() para sacar el objeto de tipo operacion_t del mensaje recibido
	operacion_t op=unpackOperation(rpcIn);
	   
	// Se realiza el computo dependiendo del tipo de operacion que nos pida el cliente
	switch(op.operationType){
		case opListFiles://TODO generated code
		{
			std::vector<std::string> res;
			res=listFiles();
			sendMSG(clientId,res);
		}break;

		case opReadFile:
		{

		}break;

		case opWriteFile:
		{

		}break;

		
		default:
		{	
			std::cout<<"Error: función no definida\n";
		}		
	};
}


int main(int argc, char** argv)
{

    //auto serverSocket=initServer(60000); //Not usre why out is 
	int serverSocket=initServer(60000);

    while(1){
		// Esperar conexiones en puerto
        while(!checkClient()){
            usleep(1000);
        }
		
		// Una vez hay conexion, se atienden las peticiones del cliente 
        int clientId=getLastClientID();

        
	    attendOperation(clientId);
        

	    closeConnection(clientId);
    }

    close(serverSocket);
    return 0;
}