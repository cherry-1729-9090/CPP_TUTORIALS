# need to specify the minimum version of the cmake we need to install to run this project
cmake_minimum_required(VERSION 3.16) # Last stable version

# Project name
project("Single producer single consumer")

# add executable files - cpp files
add_executable(output src/SPSC_with_lock.cpp)
add_executable(output2 src/SPSC_with_rb.cpp)
add_executable(output3 src/SPSC_with_SHM.cpp)

# link external libraries like threading
# We first need to find the package and then link it with the project
find_package(Threads REQUIRED)
target_link_libraries(output PRIVATE Threads::Threads)
target_link_libraries(output2 PRIVATE Threads::Threads) # for the spsc with atomics
target_link_libraries(output3 PRIVATE Threads::Threads)
target_link_libraries(output3 PRIVATE rt) # Memory support for SHM_rbf

# op - <name of the desired output file>

# add_executable(op <path to the file with main fn>)
# target_link_libraries(op PRIVATE/PUBLIC/DEFAULT/RESTRICTED <path of library>)











