
set(DEP_NAME            cryptoTools)          
set(GIT_REPOSITORY      "https://github.com/ladnir/cryptoTools.git")
set(GIT_TAG             "1c67d0a557698da8d228c91b4b2e9199d8649e2b" )

set(CLONE_DIR "${CMAKE_CURRENT_LIST_DIR}/${DEP_NAME}")
set(BUILD_DIR "${CLONE_DIR}/out/build/${DROP_OT_CONFIG}")
set(LOG_FILE  "${CMAKE_CURRENT_LIST_DIR}/log-${DEP_NAME}.txt")


include("${CMAKE_CURRENT_LIST_DIR}/fetch.cmake") 

option(CRYPTOTOOLS_DEV "always build CRYPTOTOOLS" OFF)

if(NOT ${DEP_NAME}_FOUND OR CRYPTOTOOLS_DEV)

    find_program(GIT git REQUIRED)
    set(DOWNLOAD_CMD  ${GIT} clone --recursive ${GIT_REPOSITORY})
    set(CHECKOUT_CMD  ${GIT} checkout ${GIT_TAG})
    set(SUBMODULE_CMD   ${GIT} submodule update --recursive)
    set(CONFIGURE_CMD ${CMAKE_COMMAND} -S ${CLONE_DIR} -B ${BUILD_DIR} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                       -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE} 
                       -DFETCH_AUTO=ON 
                       -DVERBOSE_FETCH=true
                       -DENABLE_CIRCUITS=OFF
                       -DENABLE_RELIC=${DROP_OT_ENABLE_RELIC} 
                       -DRELIC_GIT_REPOSITORY=https://github.com/ladnir/relic
                       -DRELIC_GIT_TAG=0.6.0-patch
                       -DENABLE_SODIUM=${DROP_OT_ENABLE_SODIUM} 
                       -DENABLE_BOOST=false
                       -DENABLE_SSE=${DROP_OT_ENABLE_SSE}
                       -DENABLE_AVX=${DROP_OT_ENABLE_AVX}
                       -DENABLE_PIC=${DROP_OT_ENABLE_PIC}
                       -DENABLE_ASAN=${DROP_OT_ENABLE_ASAN}
                       -DCRYPTO_TOOLS_STD_VER=${DROP_OT_STD_VER}
                       )
    set(BUILD_CMD     ${CMAKE_COMMAND} --build ${BUILD_DIR} --config ${CMAKE_BUILD_TYPE})
    set(INSTALL_CMD   ${CMAKE_COMMAND} --install ${BUILD_DIR} --config ${CMAKE_BUILD_TYPE} --prefix ${DROP_OT_THIRDPARTY_DIR})


    message("============= Building ${DEP_NAME} =============")
    if(NOT EXISTS ${CLONE_DIR})
        run(NAME "Cloning ${GIT_REPOSITORY}" CMD ${DOWNLOAD_CMD} WD ${CMAKE_CURRENT_LIST_DIR})
    endif()

    run(NAME "Checkout ${GIT_TAG} " CMD ${CHECKOUT_CMD}  WD ${CLONE_DIR})
    run(NAME "submodule"       CMD ${SUBMODULE_CMD} WD ${CLONE_DIR})
    run(NAME "Configure"       CMD ${CONFIGURE_CMD} WD ${CLONE_DIR})
    run(NAME "Build"           CMD ${BUILD_CMD}     WD ${CLONE_DIR})
    run(NAME "Install"         CMD ${INSTALL_CMD}   WD ${CLONE_DIR})

    message("log ${LOG_FILE}\n==========================================")
else()
    message("${DEP_NAME} already fetched.")
endif()

install(CODE "
    execute_process(
        COMMAND ${SUDO} \${CMAKE_COMMAND} --install ${BUILD_DIR}  --config ${CMAKE_BUILD_TYPE} --prefix \${CMAKE_INSTALL_PREFIX}
        WORKING_DIRECTORY ${CLONE_DIR}
        RESULT_VARIABLE RESULT
        COMMAND_ECHO STDOUT
    )
")
