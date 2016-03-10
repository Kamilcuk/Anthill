# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created by: PyQt5 UI code generator 5.5.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.OptionsGroupBox = QtWidgets.QGroupBox(self.centralwidget)
        self.OptionsGroupBox.setMinimumSize(QtCore.QSize(150, 200))
        self.OptionsGroupBox.setObjectName("OptionsGroupBox")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.OptionsGroupBox)
        self.verticalLayout.setObjectName("verticalLayout")
        self.startSimulationButton = QtWidgets.QPushButton(self.OptionsGroupBox)
        self.startSimulationButton.setObjectName("startSimulationButton")
        self.verticalLayout.addWidget(self.startSimulationButton)
        self.stopSimulationButton = QtWidgets.QPushButton(self.OptionsGroupBox)
        self.stopSimulationButton.setObjectName("stopSimulationButton")
        self.verticalLayout.addWidget(self.stopSimulationButton)
        spacerItem = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem)
        self.horizontalLayout.addWidget(self.OptionsGroupBox)
        self.tabWidget = QtWidgets.QTabWidget(self.centralwidget)
        self.tabWidget.setMinimumSize(QtCore.QSize(100, 100))
        self.tabWidget.setObjectName("tabWidget")
        self.PreviewTab = QtWidgets.QWidget()
        self.PreviewTab.setObjectName("PreviewTab")
        self.gridLayout = QtWidgets.QGridLayout(self.PreviewTab)
        self.gridLayout.setObjectName("gridLayout")
        self.graphicsView = QtWidgets.QGraphicsView(self.PreviewTab)
        self.graphicsView.setObjectName("graphicsView")
        self.gridLayout.addWidget(self.graphicsView, 0, 0, 1, 1)
        self.tabWidget.addTab(self.PreviewTab, "")
        self.StatsTab = QtWidgets.QWidget()
        self.StatsTab.setObjectName("StatsTab")
        self.gridLayout_3 = QtWidgets.QGridLayout(self.StatsTab)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.label = QtWidgets.QLabel(self.StatsTab)
        self.label.setObjectName("label")
        self.gridLayout_3.addWidget(self.label, 0, 0, 1, 1)
        self.tabWidget.addTab(self.StatsTab, "")
        self.horizontalLayout.addWidget(self.tabWidget)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 19))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.OptionsGroupBox.setTitle(_translate("MainWindow", "Options"))
        self.startSimulationButton.setText(_translate("MainWindow", "Start Simulation"))
        self.stopSimulationButton.setText(_translate("MainWindow", "Stop Simulation"))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.PreviewTab), _translate("MainWindow", "Simulation Preview"))
        self.label.setText(_translate("MainWindow", "There will be for example statistics, graphs etc."))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.StatsTab), _translate("MainWindow", "Stats"))

