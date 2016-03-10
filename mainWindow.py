from PyQt5.QtWidgets import QMainWindow
from PyQt5.QtWidgets import QGraphicsScene

from PyQt5.QtGui import QPen
from PyQt5.QtGui import QBrush
from PyQt5.QtGui import QColor
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import pyqtSlot
#from PyQt5.QtCore import pyqtSignal

from Ui_MainWindow import Ui_MainWindow

import anthill
#from timer import Timer

class MainWindow(QMainWindow):
    def __init__(self,path=''):
        super(MainWindow, self).__init__()

        self.ui=Ui_MainWindow()
        self.ui.setupUi(self)

        self.refreshTimer=None

        #self.ui.graphics.setOptimizationFlags(
        #    QGraphicsView.DontAdjustForAntialiasing
        #    | QGraphicsView.DontClipPainter
        #    | QGraphicsView.DontSavePainterState)
        #self.ui.graphics.setTransformationAnchor(self.ui.graphics.NoAnchor)
        #self.ui.graphics.setResizeAnchor(self.ui.graphics.NoAnchor)
        #self.on_tab_currentChanged(self.ui.tab.currentIndex())
        #self.showMaximized()

        # world/simulation parameters - hardcoded - test
        self.worldWidth=300
        self.worldHeight=200
        self.simulationFramerate=10

        w=self.worldWidth
        h=self.worldHeight

        self.ui.graphicsView.setScene(QGraphicsScene(0,0,w,h))

        # configure world
        self.world=anthill.World()
        self.world.setDimensions(w,h)
        self.world.setSimulationFramerate(self.simulationFramerate)

    def __exit__(self):
        self.refreshTimer.stop()


    @pyqtSlot()
    def refresh(self):
        scene=self.ui.graphicsView.scene()
        scene.clear()
        #self.graphicsItems={}

        w=self.worldWidth
        h=self.worldHeight

        bgBrush=QBrush(QColor(50,120,50,255))
        bgPen=QPen(QBrush(QColor()),0)
        scene.addRect(0,0,w,h,pen=bgPen,brush=bgBrush)

        # draw ants
        for ant in self.world.getAnts():
            x=ant.getX()
            y=ant.getY()
            antPen=QPen(QBrush(QColor()),0)
            antBrush=QBrush(QColor(100,100,50,255))
            scene.addRect(x-3,y-3,6,6,pen=antPen,brush=antBrush)

    def on_startSimulationButton_released(self):
        if self.refreshTimer is None:
            self.refresh()

            self.refreshTimer=QTimer(self)
            self.refreshTimer.timeout.connect(self.refresh)
            self.refreshTimer.start(1000/self.simulationFramerate)

            self.world.startSimulation()

    def on_stopSimulationButton_released(self):
        if self.refreshTimer:
            self.refresh()

            self.refreshTimer.stop()
            self.refreshTimer=None

            self.world.stopSimulation()
