#pragma once
#include <vector>
#include <string>

//Enum to define the type of operation
typedef enum fileOperationsEnum_t{
    opListFiles=1,
    opReadFile=2,
    opWriteFile=3
}fileOperationsEnum_t;

//Class to define the operation
class FileOperation{
    public:
    //Operation type
    fileOperationsEnum_t opType;

    //Operation parameters and data
    std::string fileName;
    std::string data;

    //Default constructor I had to define because the compiler deleted it for whatever reason
    FileOperation(fileOperationsEnum_t op){
        opType=op;
        switch(op){
            case opListFiles:
                //Nothing to copy
                break;
            
            case opReadFile:   
                fileName = ""; 
                break;

            case opWriteFile:
                fileName = ""; 
                data = "";
                break;
            
            default:
                std::cout<<"Error: función no definida\n";
        }
    }

    //Copy constructor I had to define because the compiler deleted it for whatever reason
    FileOperation(const FileOperation& other)
    : opType(other.opType)
    {
        switch(opType){
            case opListFiles:
                //Nothing to copy
                break;
            
            case opReadFile:
                fileName = other.fileName;
                break;

            case opWriteFile:
                fileName = other.fileName;
                data = other.data;
                break;
            
            default:
                std::cout<<"Error: función no definida\n";
        }
    }

    //Default destructor
    ~FileOperation(){}

};

/*
Ignore, this is just a note for myself from old code

The reason why the default constructor and copy constructor are deleted by the compiler is because 
the struct contains a union with a std::string member.
In C++, if a struct or class contains a member that has a non-trivial default and copy constructors (like std::string),
the compiler will not automatically generate a default and copy constructors for the struct or class.
This is why you need to define a default constructor and a copy constructor for FileOperation yourself
*/

/*
Data is transformed into binary data and stored in a vector for communication 
because data is sent in binary, probably more efficient than using strings
For whatever reason, unsigned char is used to store binary data
*/

/*
Template function is used to pack data into a vector
If the function is called again on the same vector
the new data will be appended to the end of the vector
*/
template<typename T>
inline void pack(std::vector<unsigned char> &packet, T data){

	int size=packet.size();
	unsigned char *ptr=(unsigned char*)&data;
	packet.resize(size+sizeof(T));
	std::copy(ptr, ptr+sizeof(T), packet.begin()+size);

}

//Implementation of pack for strings
template <>
inline void pack(std::vector<unsigned char> &packet, std::string data){
    int size=packet.size();
    unsigned char *ptr=(unsigned char*)data.c_str();
    int dataSize=data.size()+1;
    packet.resize(size+dataSize);
	std::copy(ptr, ptr+dataSize, packet.begin()+size);
}

//Function that packs the operation type and the data depending on the operation type
inline void packOperation(std::vector<unsigned char> &packet, FileOperation* op){

    //Pack the operation type
	pack(packet, op->opType); //No need to cast, taken care of in the pack() function

    //The class isnt packed, its contents are packed
    //because we want to minimize the size of the packet

    //Pack the data depending on the operation type
    switch(op->opType){
        //List files operation
        case opListFiles: 
            //In this case there is no data to pack
            break;
        
        //Read file operation
        case opReadFile: 
            //Pack the fileName
            pack(packet, op->fileName);
            break;

        //Write file operation
        case opWriteFile: 
            //Pack the fileName and the data
            pack(packet, op->fileName);
            pack(packet, op->data);
            break;

        default:
            std::cout<<"Error: function not defined\n";
    }
}


/*
Template function to unpack data from a vector
If the function is called again on the same vector
the next data will be unpacked from the beginning of the vector
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

//Implementation of unpack for strings
template <>
inline std::string unpack(std::vector<unsigned char> &packet){
    std::string data;

    int dataSize = 0;
    int packetSize=packet.size();

    /*
    Each iteration it increases dataSize by 1 
    and takes one char from the vector and appends it to the string
    It breaks out of the loop when it finds null terminator
    */
    for(unsigned char character : packet){
        dataSize++;
        if (character == '\0') {
            break;  //Stop when null terminator is found
        }
        data += character;
    }

    for(int i=dataSize;i<packetSize;i++){
		packet[i-dataSize]=packet[i];	
	}
	
	packet.resize(packetSize-dataSize);
	return data;
}

//Function that unpacks the operation type and the data depending on the operation type
inline FileOperation* unpackOperation(std::vector<unsigned char> &packet){

    //Unpack the operation type
	fileOperationsEnum_t opType=unpack<fileOperationsEnum_t>(packet);

    //Create a FileOperation object with the unpacked operation type
    FileOperation *op=new FileOperation((fileOperationsEnum_t)opType); //Cast IS necessary

	switch(op->opType){
        //List files operation
        case opListFiles:
            //No data to unpack
            break;

        //Read file operation
        case opReadFile:    
            //Unpack the fileName
            op->fileName=unpack<std::string>(packet);
            break;

        //Write file operation
        case opWriteFile: 
            //Unpack the fileName and the data
            op->fileName=unpack<std::string>(packet);
            op->data=unpack<std::string>(packet);
            break;

        default:
			std::cout<<"Error: function not defined\n";		
	};
	return op;
}