#!/usr/bin/env python

import sys
from PyQt4 import Qt

a = Qt.QApplication(sys.argv)

# Our function to call when the button is clicked
def sayHello():
    print ("Hello, World!")

# Instantiate the button
hellobutton = Qt.QPushButton("Say 'Hello world!'",None)

# And connect the action "sayHello" to the event "button has been clicked"
a.connect(hellobutton, Qt.SIGNAL("clicked()"), sayHello)

# The rest is known already...
#a.setMainWidget(hellobutton)
hellobutton.show()
a.exec_()




#import tkinter as tk

# root = tk.Tk()
# #root.geometry('300x200')
# root.geometry('500x250+500+200')
# text = tk.Text(root,
#     background='black',
#     foreground='white',
#     font=('Comic Sans MS', 12))
#
# text.pack()
# def onKeyPress(event):
#     print(event.char)
#     #text.insert('end', 'You pressed %s\n' % (event.char, ))
# def onKeyRelease(event):
#     print(event.char)
#     #text.insert('end', 'You pressed %s\n' % (event.char, ))
# root.bind('<KeyPress>', onKeyPress)
# root.bind('<KeyRelease>', onKeyRelease)
# root.mainloop()

# import tkinter
# import random
# class MainWindow:
#     def __init__(self, t):
#         self.width = 400
#         self.height = 400
#         self.i = tkinter.PhotoImage(width=self.width, height=self.height)
#         colors = [[random.randint(0,255) for i in range(0,3)] for j in range(0,self.)]
#         row = 0
#         col = 0
#         for color in colors:
#             self.i.put('#%02x%02x%02x' % tuple(color),(row,col))
#             col += 1
#             if col == 100:
#                 row +=1
#                 col = 0
#         c = tkinter.Canvas(t, width=self.width, height=self.height)
#         c.pack()
#         c.create_image(0, 0, image=self.i, anchor=tkinter.NW)
#
# t = tkinter.Tk()
# a = MainWindow(t)
# t.mainloop()

