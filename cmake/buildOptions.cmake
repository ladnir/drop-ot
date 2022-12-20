
macro(EVAL var)
     if(${ARGN})
         set(${var} ON)
     else()
         set(${var} OFF)
     endif()
endmacro()

option(DROP_OT_FETCH_AUTO      "automatically download and build dependencies" OFF)
option(DROP_OT_ENABLE_SSE      "build with sse" ON)
option(DROP_OT_ENABLE_AVX      "build with avx" ON)


#option(DROP_OT_FETCH_CRYPTOTOOLS		"download and build CRYPTOTOOLS" OFF))
EVAL(DROP_OT_FETCH_CRYPTOTOOLS_AUTO 
	(DEFINED DROP_OT_FETCH_CRYPTOTOOLS AND DROP_OT_FETCH_CRYPTOTOOLS) OR
	((NOT DEFINED DROP_OT_FETCH_CRYPTOTOOLS) AND (DROP_OT_FETCH_AUTO)))
    


message(STATUS "dropOt options\n=======================================================")

message(STATUS "Option: DROP_OT_FETCH_AUTO        = ${DROP_OT_FETCH_AUTO}")
message(STATUS "Option: DROP_OT_FETCH_CRYPTOTOOLS = ${DROP_OT_FETCH_CRYPTOTOOLS}\n")
message(STATUS "Option: DROP_OT_ENABLE_SSE        = ${DROP_OT_ENABLE_SSE}\n")
message(STATUS "Option: DROP_OT_ENABLE_AVX        = ${DROP_OT_ENABLE_AVX}\n")


