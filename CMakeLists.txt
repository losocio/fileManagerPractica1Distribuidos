cmake_minimum_required (VERSION 2.8.11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (clientFileManager)
add_executable (clientFileManager filemanager.cpp filemanager.h utils.cpp utils.h client.cpp fileOperations.h)
target_link_libraries(clientFileManager pthread)

project (serverFileManager)
add_executable (serverFileManager filemanager.cpp filemanager.h utils.cpp utils.h server.cpp fileOperations.h)
target_link_libraries(serverFileManager pthread)

