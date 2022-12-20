
project(dropOt)


#############################################
#              Build dropOt                 #
#############################################

file(GLOB_RECURSE SRCS *.cpp)
 

add_library(dropOt ${SRCS})

target_include_directories(dropOt PUBLIC 
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>
                    $<INSTALL_INTERFACE:include>) 
target_include_directories(dropOt PUBLIC 
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
                    $<INSTALL_INTERFACE:include>) 


target_link_libraries(dropOt oc::cryptoTools)

