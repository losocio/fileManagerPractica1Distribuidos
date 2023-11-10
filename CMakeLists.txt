cmake_minimum_required (VERSION 2.8.11)

project (clientFileManager)
add_executable (clientFileManager filemanager.cpp filemanager.h utils.cpp utils.h client.cpp fileOperations.h)

project (serverFileManager)
add_executable (serverFileManager filemanager.cpp filemanager.h utils.cpp utils.h server.cpp fileOperations.h)