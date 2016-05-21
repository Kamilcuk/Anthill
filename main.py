#!/usr/bin/env python3

from mainWindow import MainWindow
from PyQt5.QtWidgets import QApplication

app=QApplication([])
mainWindow=MainWindow()
mainWindow.show()
app.exec_()
