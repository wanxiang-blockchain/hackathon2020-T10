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

# MQTTPacket Library

cmake_minimum_required(VERSION 3.4)

if (CROSS_COMPILE)
	message(STATUS "cross compile...")
	#include (${CMAKE_SOURCE_DIR}/../cross_compile.cmake)
	SET(CMAKE_SYSTEM_NAME ${SYSTEM_NAME})
	SET(CMAKE_SYSTEM_PROCESSOR ${TARGET_HOST}) 
endif()

project(MQTTPacket)

aux_source_directory(. MQTTPACKET_SRCS)

set(EX_HEAD_FILE MQTTPacket.h)

add_library(MQTTPacket SHARED ${MQTTPACKET_SRCS})
add_library(MQTTPacket_static STATIC ${MQTTPACKET_SRCS})

SET_TARGET_PROPERTIES(MQTTPacket_static PROPERTIES OUTPUT_NAME "MQTTPacket")   	#更改静态库输出名字
GET_TARGET_PROPERTY(OUTPUT_VALUE MQTTPacket_static OUTPUT_NAME)				#获取静态库输出名字

INSTALL(TARGETS MQTTPacket LIBRARY DESTINATION lib)
INSTALL(TARGETS MQTTPacket_static ARCHIVE DESTINATION lib)
INSTALL(FILES ${EX_HEAD_FILE} DESTINATION include)
