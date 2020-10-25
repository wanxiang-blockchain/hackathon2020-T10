#!/usr/bin/python
# -*- coding: UTF-8 -*-
import hashlib
import json
from time import time
from urllib.parse import urlparse
from uuid import uuid4
import requests
from flask import Flask, jsonify, request
import math
from argparse import ArgumentParser
import cryptos
import pymysql
import mqtt_client
# privkey = cryptos.sha256('preh hahahahah')
# pubkey = cryptos.privtopub(privkey)
# print(privkey)
# print(pubkey)
# -*- coding=utf-8 -*-

class MysqlOperation():
    def __init__(self):
        # self.devID = []
        # self.pubKey = []
        # self.stuName = []
        # self.stuIDCardNo = []
        # self.topic = []
        # self.answer = []
        # self.sign = []
        # self.poeHash = []
        self.values = []
        self.status = ['Register', 'Logout']
        # connect database
        # self.db = pymysql.connect(host="127.0.0.1", port=3306, user="root", password="123456", database="pubkey")

    def mysql_insert(self,values):
        db = pymysql.connect(host="127.0.0.1", port=3306, user="root", password="123456", database="ca_sys")
        cursor = db.cursor()

        sql = "insert into device(id,GpenID,GpenPubKey,Status) values(null,%s,%s,%s) "
        try:
            cursor.execute(sql, (values['GpenID'], values['GpenPubKey'], self.status[0]))
            # cursor.execute(sql)
            db.commit()
        except:
            db.rollback()
        db.close()
    def mysql_delete(self):
        db = pymysql.connect(host="127.0.0.1", port=3306, user="root", password="123456", database="ca_sys")
        cursor = db.cursor()
        sql = "delete from device where id=1"
        cursor.execute(sql)
        db.commit()
        db.close()
    def mysql_update(self,id):
        db = pymysql.connect(host="127.0.0.1", port=3306, user="root", password="123456", database="ca_sys")
        cursor = db.cursor()

        # update 表名 set 指定字段 =replace(指定字段,'要替换的字符串','想要的字符串') where 条件
        if id:
            sql = "update device set Status ='Logout' where id=%s" % (id)
            info = "device logout successfully"
            print("device logout successfully")
        else:
            info = "device is not in database"
            print("device is not in database")
        cursor.execute(sql)

        db.commit()
        db.close()
        return info
    def value_get(self,values):
        # self.devID = values['devID']
        # self.pubKey = values['pubKey']
        # # print(devID,pubKey)
        self.mysql_insert(values)
    def mysql_find(self,search):
        db = pymysql.connect(host="127.0.0.1", port=3306, user="root", password="123456", database="ca_sys")
        cursor = db.cursor()

        sql = "select id,GpenID,GpenPubKey,Status from device"
        cursor.execute(sql)
        # result = cursor.fetchone()
        a =  cursor.fetchall()
        # print(a)
        # result = dict(map(reversed,a)) #quchong
        result = dict((x, [y, z, s]) for x, y, z, s in a)
        # result = cursor.fetchmany(7)
        print(result)
        # print(result.keys())
        # print(list(result.values())[0][0])
        for k,v in result.items():
            # print (v[0])
            if search == v[0]:
                info = f"The devID is in database,the status of device is {v[2]} "
                print("The devID is in database ")
                return k,info
            else:
                info = "Searching ..."
                print("Searching ...")
        db.commit()
        db.close()
#实例化一个Flask节点
app = Flask(__name__)
mysql = MysqlOperation()

#get public key from http protocol

# {
#  	"devID": "c4779eba88ff97f1be622606abc965956ddbd5560409b15a73d9ec67033f1b58",
# 	"pubKey": "0461ec0048d5262d59c2ed924d457410378d74907fb490622f6706f2a3ebc5c8139be92477f5e7c53b0053387382fa5d265534c8ac60d96805d95960cf12530d72"
# }
@app.route('/ca_sys/data', methods=['POST'])
def data_get():
    # 获取请求的参数，得到参数的json格式数据
    mysql.values = request.get_json()
    print('request parameters:%s' % (mysql.values))

    # 检查请求的参数是否合法，包含sender,recipient,amount几个字段
    required = ['GpenID', 'GpenPubKey']
    if not all(k in mysql.values for k in required):
        return 'Missing values', 400
    else:
        mysql.value_get(mysql.values)
        response = {
            'GpenID': mysql.values['GpenID'],
            'GpenPubKey': mysql.values['GpenPubKey'],
            'Result': "Successful"
        }
        print(response)
    return jsonify(response), 200


@app.route('/ca_sys/transmit', methods=['POST'])
def data_transmit():
    print("test")
    #构造返回响应信息
    response = mqtt_client.MqttServer.get_value()
    return jsonify(response), 200
if __name__ == '__main__':


    parser = ArgumentParser()
    parser.add_argument('-p', '--port', default=7000, type=int, help='port to listen on')
    args = parser.parse_args()
    port = args.port
    app.run(host='127.0.0.1', port=port)
    # id = mysql.mysql_find('0000100008ff97f1be622606abc965956ddbd5560409b15a73d9ec67033f1b58')
    # print(id)
    # print(type(id))
    # mysql.mysql_update(str(id))
    # mysql.mysql_delete()