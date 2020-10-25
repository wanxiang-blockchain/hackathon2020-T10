#!/usr/bin/python
# -*- coding: UTF-8 -*-
import paho.mqtt.client as mqtt
import paho.mqtt.subscribe as subscribe
import re
from flask import Flask, jsonify, request
import math
import requests
import json
from argparse import ArgumentParser
# The callback for when the client receives a CONNACK response from the server.
header = {
        'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.89 Safari/537.36',
        'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'}

class MqttServer():

    def __init__(self):
        self.mqtt_topic = []
        self.mqtt_msg = []
    def on_connect(self,client, userdata, flags, rc):
        print("Connected with result code "+str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        # print(client.subscribe("chat"))

    # The callback for when a PUBLISH message is received from the server.
    def on_message(self,client, userdata, msg):

        self.mqtt_topic = msg.topic
        self.mqtt_msg = self.str_process(str(msg.payload.decode('utf-8')))
        self.http_post()
        print(f"mqtt_topic:{self.mqtt_topic}\nmqtt_msg:{self.mqtt_msg}\n")

    def get_value(self):
        return self.mqtt_msg
    def http_post(self):
        data = self.get_value()
        # json_str = json.dumps(data)
        # print(json_str)
        post_id = ['exam-answers','exam-checks']
        requests.post(f'http://172.25.0.231:9001/{post_id[0]}?token=ci_token01', data=data,headers=header)
        print("Send Successful!")
    def on_publish(self,mqttc, obj, mid):
        print("mid: " + str(mid))


    def on_subscribe(self,mqttc, obj, mid, granted_qos):
        print("Subscribed: " + str(mid) + " " + str(granted_qos))


    def on_log(self,mqttc, obj, level, string):
        print(string)

    def str_process(self,str):
        pattern = re.compile(r'.*?"(.*?)".*?"(.*?)"')
        result = pattern.findall(str)
        result = dict((x, y) for x, y in result)
        # print(result)
        return result
mqttx = MqttServer()

if __name__ == '__main__':
    client = mqtt.Client()
    client.on_connect = mqttx.on_connect
    client.on_message = mqttx.on_message
    # client.on_publish = mqttx.on_publish
    # client.on_subscribe = mqttx.on_subscribe
    # client.on_log = mqttx.on_log
    hostname = "172.25.0.231"
    client.username_pw_set('broker01', password='iot123')
    client.connect(hostname, 1883, 60)
    topics = ['/exam/+/answer']
    content = client.subscribe(topics[0], 0)
    print(mqttx.mqtt_topic, mqttx.mqtt_msg)
    client.loop_forever()
