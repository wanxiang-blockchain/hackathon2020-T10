#*******************************************************************************
#  Copyright (c) 2017 IBM Corp.
#
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v1.0
#  and Eclipse Distribution License v1.0 which accompany this distribution.
#
#  The Eclipse Public License is available at
#     http://www.eclipse.org/legal/epl-v10.html
#  and the Eclipse Distribution License is available at
#    http://www.eclipse.org/org/documents/edl-v10.php.
#
#  Contributors:
#     Ian Craggs - initial version
#*******************************************************************************/

# mqtt Library

cmake_minimum_required(VERSION 3.4)

if (CROSS_COMPILE)
	message(STATUS "cross compile...")
	SET(CMAKE_SYSTEM_NAME ${SYSTEM_NAME})
	SET(CMAKE_SYSTEM_PROCESSOR ${TARGET_HOST}) 
endif()

project(mqtt)

set(ostype "linux")

aux_source_directory(./MQTTPacket MQTTPACKET_SRCS)
set(MQTTCLIENT_SRCS MQTTClient/MQTTClient.c)

if(ostype STREQUAL "linux")
	#add_definitions(-DOS_LINUX)
	add_definitions(-DMQTTCLIENT_PLATFORM_HEADER=MQTTLinux.h)
	message(STATUS "compile linux ...")
	set(MQTTCLIENT_SRCS ${MQTTCLIENT_SRCS} MQTTClient/MQTTLinux.c)
	set(EX_HEAD_FILE MQTTClient/MQTTLinux.h )
elseif(ostype STREQUAL "freertos")
	#add_definitions(-DOS_FREERTOS)
	add_definitions(-DMQTTCLIENT_PLATFORM_HEADER=MQTTFreeRTOS.h)
	message(STATUS "compile freertos ...")
	set(MQTTCLIENT_SRCS ${MQTTCLIENT_SRCS} MQTTClient/MQTTFreeRTOS.c)
	set(EX_HEAD_FILE MQTTClient/MQTTFreeRTOS.h )
else()
	#add_definitions(-DOS_CC3200)
	add_definitions(-DMQTTCLIENT_PLATFORM_HEADER=MQTTCC3200.h)
	message(STATUS "compile CC3200 ...")
	set(MQTTCLIENT_SRCS ${MQTTCLIENT_SRCS} MQTTClient/MQTTCC3200.c)
	set(EX_HEAD_FILE MQTTClient/MQTTCC3200.h )
endif()

set(EX_HEAD_FILE ${EX_HEAD_FILE} MQTTPacket/MQTTPacket.h )
set(EX_HEAD_FILE ${EX_HEAD_FILE} MQTTPacket/MQTTConnect.h )
set(EX_HEAD_FILE ${EX_HEAD_FILE} MQTTPacket/MQTTFormat.h )
set(EX_HEAD_FILE ${EX_HEAD_FILE} MQTTPacket/MQTTPublish.h )
set(EX_HEAD_FILE ${EX_HEAD_FILE} MQTTPacket/MQTTSubscribe.h )
set(EX_HEAD_FILE ${EX_HEAD_FILE} MQTTPacket/MQTTUnsubscribe.h )
set(EX_HEAD_FILE ${EX_HEAD_FILE} MQTTPacket/StackTrace.h)
set(EX_HEAD_FILE ${EX_HEAD_FILE} MQTTClient/MQTTClient.h)

INCLUDE_DIRECTORIES(./MQTTPacket)
INCLUDE_DIRECTORIES(./MQTTClient)

add_library(mqtt SHARED ${MQTTPACKET_SRCS} ${MQTTCLIENT_SRCS})
add_library(mqtt_static STATIC ${MQTTPACKET_SRCS} ${MQTTCLIENT_SRCS})

SET_TARGET_PROPERTIES(mqtt_static PROPERTIES OUTPUT_NAME "mqtt")   	#更改静态库输出名字
GET_TARGET_PROPERTY(OUTPUT_VALUE mqtt_static OUTPUT_NAME)				#获取静态库输出名字

INSTALL(TARGETS mqtt LIBRARY DESTINATION lib)
INSTALL(TARGETS mqtt_static ARCHIVE DESTINATION lib)
INSTALL(FILES ${EX_HEAD_FILE} DESTINATION include/mqtt)

