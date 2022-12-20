include(${CMAKE_CURRENT_LIST_DIR}/preamble.cmake)

message(STATUS "DROP_OT_THIRDPARTY_DIR=${DROP_OT_THIRDPARTY_DIR}")


set(PUSHED_CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH "${DROP_OT_THIRDPARTY_DIR};${CMAKE_PREFIX_PATH}")


#######################################
# CRYPTOTOOLS

macro(FIND_CRYPTOTOOLS)
    set(ARGS ${ARGN})

    #explicitly asked to fetch CRYPTOTOOLS
    if(FETCH_CRYPTOTOOLS)
        list(APPEND ARGS NO_DEFAULT_PATH PATHS ${DROP_OT_THIRDPARTY_DIR})
    endif()
    
    find_package(cryptoTools ${ARGS})

    if(TARGET oc::cryptoTools)
        set(CRYPTOTOOLS_FOUND ON)
    else()
        set(CRYPTOTOOLS_FOUND  OFF)
    endif()
endmacro()

if(DROP_OT_FETCH_CRYPTOTOOLS_AUTO)
    FIND_CRYPTOTOOLS(QUIET)
    include(${CMAKE_CURRENT_LIST_DIR}/../thirdparty/getCryptoTools.cmake)
endif()

FIND_CRYPTOTOOLS(REQUIRED)



# resort the previous prefix path
set(CMAKE_PREFIX_PATH ${PUSHED_CMAKE_PREFIX_PATH})
