cmake_minimum_required(VERSION 3.4)


project(exam_demo)

set(LIB_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(INC_PATH ${CMAKE_INSTALL_PREFIX}/include)

INCLUDE_DIRECTORIES(${INC_PATH})
INCLUDE_DIRECTORIES(${INC_PATH}/sdk)
INCLUDE_DIRECTORIES(../)

#beef poe save demo compile
aux_source_directory(. DEMO_SRCS)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")
add_executable(exam_demo ${DEMO_SRCS})

TARGET_LINK_LIBRARIES(exam_demo ${LIB_PATH}/libsamurai.so)
TARGET_LINK_LIBRARIES (exam_demo pthread)
#TARGET_LINK_LIBRARIES(BeefPoeSave ${LIB_PATH}/libcurl.so)

INSTALL(TARGETS exam_demo RUNTIME DESTINATION bin)

