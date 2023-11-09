#pragma once
#include <vector>

typedef enum{
    opListFiles=1,
    opReadFile=2,
    opWriteFile=3
}fileOperationsEnum;


typedef struct __attribute__((packed)){
    //Operation type
    fileOperationsEnum operationType;
    
    //Operation parameters and data
    union{

        struct{ //No parameters
            
        }listFiles;

        struct{
            std::string fileName;
        }readFile;

        struct{
            std::string fileName;
            std::string data;
            unsigned long int dataLength; //TODO might be unnecessary
        }writeFile;
    };
}fileOperation_t;


//Data is transformed into binary data and stored in a vector for communication 
//because data is sent in binary, probably more efficient than using strings
//For whatever reason, unsigned char is used to store binary data

/*
This function is used to pack data into a vector
If the function is called again on the same vector,
the new data will be appended to the end of the vector
*/
template<typename T>
inline void pack(std::vector<unsigned char> &packet, T data){
	
	int size=packet.size();
	unsigned char *ptr=(unsigned char*)&data;
	packet.resize(size+sizeof(T));
	std::copy(ptr, ptr+sizeof(T), packet.begin()+size);
	
}

//Using vector instead of string because it holds binary data
inline void packOperation(std::vector<unsigned char> &packet, fileOperation_t op){

    //Pack the operation type
    unsigned char opType=op.operacionType;
	pack(packet, opType);

    //Pack the data depending on the operation type
    switch(op.operationType){
        case opListFiles: 
        {
            //In this case there is no data to pack

            //pack(packet, op.listFiles); //TODO might be necessary
        }break;
        
        case opReadFile: 
        {
            //The struct isnt packed, its contents are packed
            //because we want to minimize the size of the packet
            pack(packet, op.readFile.fileName);
        }break;

        case opWriteFile: 
        {
            pack(packet, op.writeFile.fileName);
            pack(packet, op.writeFile.data);
            pack(packet, op.writeFile.dataLength);
        }break;

        default:
        {
            std::cout<<"Error: función no definida\n";
        }
    }
}

/*
This function is used to unpack data from a vector
its the opposite of pack
*/
template<typename T>
inline T unpack(std::vector<unsigned char> &packet){	
	T data;
	int dataSize=sizeof(T);
	int packetSize=packet.size();
	T* ptr=(T*)packet.data();
	data=ptr[0];
	
	for(int i=dataSize;i<packetSize;i++)
	{
		packet[i-dataSize]=packet[i];	
	}
	
	packet.resize(packetSize-dataSize);
	return data;
}

//TODO copied code, needs modification
inline fileOperation_t unpackOperation(std::vector<unsigned char> &packet){
	
    fileOperation_t op;
	unsigned char opType=unpack<unsigned char>(packet);
	op.operationType=(fileOperationsEnum)opType;
	
	switch(op.operationType){
        case opListFiles:
        {
            //In this case there is no data to unpack

            //op.listFiles=unpack<struct listFiles>(packet, op.listFiles); //TODO might be necessary, but also wrong
        }break;

        case opReadFile: 
        {   
            op.opReadFile.fileName=unpack<std::string>(packet);
        }break;

        case opWriteFile: 
        {
            op.opWriteFile.fileName=unpack<std::string>(packet);
            op.opWriteFile.data=unpack<std::string>(packet);
            op.opWriteFile.dataLength=unpack<unsigned long int>(packet); //TODO might be unnecessary
        }break;

        default:
		{	
			std::cout<<"Error: función no definida\n";
		}		
	};
	return op;
}