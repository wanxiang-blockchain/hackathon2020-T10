# -*- coding: utf-8 -*-

import tkinter as tk
from PIL import Image, ImageTk
from collections import OrderedDict
import xlrd
import time
import os
import shutil
import sys
import ca_server






class AppInterface(object):
    def __init__(self):
        self.root = tk.Tk()
        self.file_opt = options = {}
        self.mysql = ca_server.MysqlOperation()
        self.inquire_entry = []
        self.logout_entry = []
        self.listbox = []


    def ui_create(self):
        # canvas = tk.Canvas(self.root,
        #                    width=200,  # 指定Canvas组件的宽度
        #                    height=90,  # 指定Canvas组件的高度
        #                    bg='white')  # 指定Canvas组件的背景色
        # # im = Tkinter.PhotoImage(file='img.gif')     # 使用PhotoImage打开图片
        # path = os.getcwd()
        #
        # path = path + '\\' + "wx.png"
        # image = Image.open(path)
        #
        # im = ImageTk.PhotoImage(image)
        #
        # canvas.create_image(100,50, image=im)  # 使用create_image将图片添加到Canvas组件中
        # canvas.create_text(110, 20,  # 使用create_text方法在坐标（302，77）处绘制文字
        #                    text='Copyright (C) 2020 WXBLOCKCHAIN'  # 所绘制文字的内容
        #                    , fill='black')  # 所绘制文字的颜色为灰色
        # canvas.place(relx=0.5, rely=0.5, anchor="w")  # 将Canvas添加到主窗口

        # column 默认值是 0
        # tk.Label(self.root, text="用户名").grid(row=0, sticky="w")
        # tk.Label(self.root, text="密码").grid(row=1, sticky="w")
        #
        # tk.Entry(self.root).grid(row=0, column=2)
        # tk.Entry(self.root, show="*").grid(row=1, column=2)
        #
        # photo = tk.PhotoImage(path)
        # tk.Label(self.root, image=photo).grid(row=0, column=2, rowspan=2, padx=5, pady=5)  # rowspan=2 跨两行
        #
        # tk.Button(text="提交", width=10).grid(row=2, columnspan=3, pady=5)  # columnspan=3 跨三列





        tk.Button(self.root, text="Inquire", width=10, height=1, bg="white", relief="groove",
                  command=self.database_inquire).grid(row=1,column=0)
        self.inquire_entry = tk.Entry(self.root,relief="sunken",width=70)
        self.inquire_entry.grid(row=1, column=1)


        tk.Button(self.root, text="Logout", width=10, height=1, bg="white", relief="groove",
                  command=self.database_logout).grid(row=2,column=0)
        tk.Entry(self.root)
        self.logout_entry = tk.Entry(self.root,relief="sunken",width=70)
        self.logout_entry.grid(row=2, column=1)


        self.listbox = tk.Listbox(self.root,width=85,yscrollcommand=True)
        self.listbox.grid(row=3, column=0,columnspan=2)
        # yscrollbar = tk.Scrollbar(self.listbox, command=self.listbox.yview)
        # yscrollbar.grid(row=3, column=3)
        # self.listbox.config(yscrollcommand=yscrollbar.set)

        self.root.title("Public Key System")
        # self.root.wm_state('zoomed') 弹出时以窗口最大化显示
        # self.root.attributes("-fullscreen", True)
        self.root.mainloop()

    def callback(self,event):
        print('点击位置：', event.x, event.y)

    def get_event_value(self):

        frame = tk.Frame(self.root, width=200, height=200)
        frame.bind("<Motion>", self.callback)
        frame.grid(row=0, column=2)

    def get_inquire_value(self):
        return self.inquire_entry.get()

    def get_logout_value(self):
        return self.logout_entry.get()

    def database_inquire(self):
        str = self.get_inquire_value()
        print(str)
        id,info = self.mysql.mysql_find(str)
        self.listbox.insert("end", info)
        self.listbox.insert("end",f"Device number is {id}")
        print(id)
        print(type(id))

    def database_logout(self):
        str = self.get_logout_value()
        id,info = self.mysql.mysql_find(str)
        self.listbox.insert("end",info)
        self.listbox.insert("end",f"Device number is {id}")

        info = self.mysql.mysql_update(id)
        self.listbox.insert("end", info)


app = AppInterface()
if __name__ == '__main__':
    app.ui_create()