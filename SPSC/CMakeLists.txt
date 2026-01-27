# need to specify the minimum version of the cmake we need to install to run this project
cmake_minimum_required(VERSION 3.16) # Last stable version

# Project name
project("Single producer single consumer")

# add executable files - cpp files
add_executable(output src/SPSC.cpp)

# link external libraries like threading
# We first need to find the package and then link it with the project
find_package(Threads REQUIRED)
target_link_libraries(output PRIVATE Threads::Threads)


# op - <name of the desired output file>

# add_executable(op <path to the file with main fn>)
# target_link_libraries(op PRIVATE/PUBLIC/DEFAULT/RESTRICTED <path of library>)











