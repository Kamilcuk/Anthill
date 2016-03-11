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
        self.pixelSize=10

        self.worldWidth=50
        self.worldHeight=50

        self.simulationFramerate=10

        w=self.worldWidth * self.pixelSize
        h=self.worldHeight * self.pixelSize

        self.ui.graphicsView.setScene(QGraphicsScene(0,0,w,h))

        # configure world
        self.world=anthill.World()
        self.world.setDimensions(w,h)
        self.world.setSimulationFramerate(self.simulationFramerate)

        self.maxPheromone=1

    def __exit__(self):
        self.refreshTimer.stop()

    def drawPheromoneMap(self,map):
        t=map.getMapCopy()
        t=list(t)
        s=self.pixelSize
        scene=self.ui.graphicsView.scene()
        pbrush=QBrush(QColor(50,120,100,255))
        ppen=QPen(QBrush(QColor()),0)
        for x,a in enumerate(t):
            if(x>self.worldWidth):
                break
            for y,b in enumerate(a):
                if(y>self.worldHeight):
                    break
                self.maxPheromone=max(self.maxPheromone,b)
                pbrush=QBrush(QColor(50,120,100,abs(255*b/self.maxPheromone)))
                scene.addRect(x*s,y*s,s,s,pen=ppen,brush=pbrush)


    @pyqtSlot()
    def refresh(self):
        scene=self.ui.graphicsView.scene()
        scene.clear()
        #self.graphicsItems={}

        w=self.worldWidth * self.pixelSize
        h=self.worldHeight * self.pixelSize

        bgBrush=QBrush(QColor(50,120,50,255))
        bgPen=QPen(QBrush(QColor()),0)
        scene.addRect(0,0,w,h,pen=bgPen,brush=bgBrush)

        # symulacja tak? Kamil
        # na razie może być, później się zrobi wątek w c++. ms
        self.world.simulationStep()

        s=self.pixelSize

        # draw pheromone map
        pMaps=self.world.getPheromoneMaps()
        pMaps=[pMaps[p] for p in range(len(pMaps))]
        for m in pMaps:
            self.drawPheromoneMap(m)

        # draw ants
        # ants not working as iterable
        ants=self.world.getAnts()
        for ant in range(len(ants)):
            x=ants[ant].getLoc().posX()
            y=ants[ant].getLoc().posY()
            antPen=QPen(QBrush(QColor()),0)
            antBrush=QBrush(QColor(100,100,50,255))
            scene.addEllipse(x*s,y*s,s,s,pen=antPen,brush=antBrush)
        # draw foods
        # like ants
        foods=self.world.getFoods()
        for food in range(len(foods)):
            x=foods[food].getLoc().posX()
            y=foods[food].getLoc().posY()
            foodpen=QPen(QBrush(QColor()),0)
            foodbrush=QBrush(QColor(200,200,200,150))
            #scene.addrect(x-3,y-3,6,6,pen=foodpen,brush=foodbrush)
            scene.addRect(x*s,y*s,s,s,pen=foodpen,brush=foodbrush)
        # draw anthills
        anthills=self.world.getAnthills()
        anthills=[anthills[a] for a in range(len(anthills))]
        for anthill in anthills:
            x=anthill.getLoc().posX()
            y=anthill.getLoc().posX()
            anthillPen=QPen(QBrush(QColor()),0)
            anthillBrush=QBrush(QColor(200,20,20,255))
            scene.addEllipse(x*s,y*s,s,s,pen=anthillPen,brush=anthillBrush)

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

    def on_actionExit_triggered(self):
        self.close()

    def on_actionAbout_triggered(self):
        print("Anthill simulator version 0. ")
