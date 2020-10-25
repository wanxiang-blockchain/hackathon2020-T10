cmake_minimum_required(VERSION 3.4)

if (CROSS_COMPILE)
	message(STATUS "cross compile...")
	SET(CMAKE_SYSTEM_NAME ${SYSTEM_NAME})
	SET(CMAKE_SYSTEM_PROCESSOR ${TARGET_HOST}) 
endif()

project(samurai_c_sdk)

aux_source_directory(./ecdsa ECDSA_SRCS)
aux_source_directory(./cjson CJSON_SRCS)
aux_source_directory(./apis APIS_SRCS)

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -W")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wextra")
#set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wimplicit-function-declaration")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wredundant-decls")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wstrict-prototypes")
#set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wundef")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wshadow")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wpointer-arith")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wformat")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wreturn-type")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wsign-compare")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wmultichar")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wformat-nonliteral")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Winit-self")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wuninitialized")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wformat-security")
#set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Werror")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -Wno-unused-parameter")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -DVALGRIND=1")

set(EX_HEAD_FILE apis/application_api.h apis/algorithm_api.h apis/http_client.h apis/mqtt_api.h ecdsa/hexutils.h cjson/cJSON.h types.h)

INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(./ecdsa)
INCLUDE_DIRECTORIES(./cjson)
INCLUDE_DIRECTORIES(./apis)

INCLUDE_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/include)

set(ostype "linux")
if(ostype STREQUAL "linux")
	add_definitions(-DMQTTCLIENT_PLATFORM_HEADER=MQTTLinux.h)
elseif(ostype STREQUAL "freertos")
	add_definitions(-DMQTTCLIENT_PLATFORM_HEADER=MQTTFreeRTOS.h)
else()
	add_definitions(-DMQTTCLIENT_PLATFORM_HEADER=MQTTCC3200.h)
endif()

set(SAMURAI_SRC ${ECDSA_SRCS} ${CJSON_SRCS} ${APIS_SRCS} ${TLV_SRCS})


add_library(samurai SHARED ${SAMURAI_SRC})
add_library(samurai_static STATIC ${SAMURAI_SRC})

if (USE_SYS_CURL)
	TARGET_LINK_LIBRARIES(samurai curl)
	TARGET_LINK_LIBRARIES(samurai_static curl)
else()
	TARGET_LINK_LIBRARIES(samurai ${CMAKE_INSTALL_PREFIX}/lib/libcurl.so)
	TARGET_LINK_LIBRARIES(samurai_static ${CMAKE_INSTALL_PREFIX}/lib/libcurl.so)
endif()

TARGET_LINK_LIBRARIES(samurai ${CMAKE_INSTALL_PREFIX}/lib/libmqtt.so)
TARGET_LINK_LIBRARIES(samurai_static ${CMAKE_INSTALL_PREFIX}/lib/libmqtt.so)

SET_TARGET_PROPERTIES(samurai_static PROPERTIES OUTPUT_NAME "samurai")   	#更改静态库输出名字
GET_TARGET_PROPERTY(OUTPUT_VALUE samurai_static OUTPUT_NAME)				#获取静态库输出名字

INSTALL(TARGETS samurai LIBRARY DESTINATION lib)
INSTALL(TARGETS samurai_static ARCHIVE DESTINATION lib)
INSTALL(FILES ${EX_HEAD_FILE} DESTINATION include/sdk)