cmake_minimum_required(VERSION 2.5)
project(server)

#编译外部库：ffmpeg、openssl及x265等
#execute_process(COMMAND sh ${CMAKE_SOURCE_DIR}/vendor.sh "${CMAKE_SOURCE_DIR}" "${CMAKE_INSTALL_PREFIX}")

#设置常用变量


#编译源码设置

set(LIB_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(INC_PATH ${CMAKE_INSTALL_PREFIX}/include)

INCLUDE_DIRECTORIES(${INC_PATH})
INCLUDE_DIRECTORIES(${INC_PATH}/sdk)
INCLUDE_DIRECTORIES(../)

aux_source_directory(${CMAKE_SOURCE_DIR}/src 	DIR_IOV_SRCS)


set(IOV_SRCS ${IOV_SRCS} ${DIR_IOV_SRCS})

add_executable(server ${IOV_SRCS})

#头文件路径设置
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/inc)
target_include_directories(server PRIVATE ${INC_PATH})

target_link_libraries(server ${LIB_PATH}/libgo.so)
target_link_libraries(server ${LIB_PATH}/libsamurai.so)

#INSTALL(TARGETS iov RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/build)
INSTALL(TARGETS server RUNTIME DESTINATION bin)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/web_root/ DESTINATION bin)








