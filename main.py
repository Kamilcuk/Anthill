#!/usr/bin/env python

from mainWindow import MainWindow
from PyQt5.QtWidgets import QApplication

app=QApplication([])
mainWindow=MainWindow()
mainWindow.show()
app.exec()
