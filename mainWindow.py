from PyQt5.QtWidgets import QMainWindow
from PyQt5.QtWidgets import QGraphicsView
from PyQt5.QtWidgets import QGraphicsScene

from PyQt5.QtGui import QPen
from PyQt5.QtGui import QBrush
from PyQt5.QtGui import QColor
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import pyqtSlot
#from PyQt5.QtCore import pyqtSignal

from PyQt5 import QtCore, QtGui, QtWidgets

from Ui_MainWindow import Ui_MainWindow
import ctypes

import anthill
#from timer import Timer

class MainWindow(QMainWindow):
    def __init__(self,path=''):
        super(MainWindow, self).__init__()

        self.ui=Ui_MainWindow()
        self.ui.setupUi(self)

        self.refreshTimer=None

        self.ui.graphicsView.setOptimizationFlags(
            QGraphicsView.DontAdjustForAntialiasing
            | QGraphicsView.DontClipPainter
            | QGraphicsView.DontSavePainterState)
        #self.ui.graphics.setTransformationAnchor(self.ui.graphics.NoAnchor)
        #self.ui.graphics.setResizeAnchor(self.ui.graphics.NoAnchor)
        #self.on_tab_currentChanged(self.ui.tab.currentIndex())
        #self.showMaximized()

        # world/simulation parameters - hardcoded - test
        self.worldWidth=50
        self.worldHeight=50
        self.simulationFramerate=10

        self.pixelSize=10
        w=self.worldWidth * self.pixelSize
        h=self.worldHeight * self.pixelSize


        self.ui.graphicsView.setScene(QGraphicsScene(0,0,w,h))

        # configure world
        self.world = anthill.World();
        self.world.setDimensions(w,h)
        self.world.setSimulationFramerate(self.simulationFramerate)

        self.maxPheromone=1

    def __exit__(self):
        self.refreshTimer.stop()

    def drawPheromoneMap(self,map,
            baseRGB=(50,30,100),
            baseRGB2=(10,10,10) ):

        data=map.getMapCopy()
        data=list(data)

        s=self.pixelSize
        scene=self.ui.graphicsView.scene()

        for x,a in enumerate(data):
            if(x>self.worldWidth):
                break
            for y,b in enumerate(a):
                if(y>self.worldHeight):
                    break
                alpha=255*b/self.maxPheromone
                self.maxPheromone=max(self.maxPheromone,b)
                if(alpha<0.001):
                    continue
                if(alpha>255):
                    alpha=255

                pbrush=QBrush(QColor(baseRGB[0],baseRGB[1],baseRGB[2], alpha))
                ppen=QPen(QBrush(QColor(baseRGB2[0],baseRGB2[1],baseRGB2[2], alpha)),0)
                scene.addRect(x*s,y*s,s,s,pen=ppen,brush=pbrush)

    def drawEntities(self,ents,qpen,qbrush,ellipse=False):
        ents=[ents[a] for a in range(len(ents))]

        s=self.pixelSize
        scene=self.ui.graphicsView.scene()

        for ent in ents:
            x=ent.getLoc().posX()
            y=ent.getLoc().posY()
            if(ellipse):
                scene.addEllipse(x*s,y*s,s,s,pen=qpen,brush=qbrush)
            else:
                scene.addRect(x*s,y*s,s,s,pen=qpen,brush=qbrush)


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
        # na razie może być, później się zrobi wątek w c++. ms
        self.world.simulationStep()

        s=self.pixelSize

        # draw pheromone map
        pMaps=self.world.getPheromoneMaps()
        pMaps=[pMaps[p] for p in range(len(pMaps))]
        for i,m in enumerate(pMaps):
            self.drawPheromoneMap(m,baseRGB=(
                    255*(i%2), 20, 200*((i+1)%2)))

        # draw obstacles
        obstacles=self.world.getObstacles()
        obstaclePen=QPen(QBrush(QColor()),0)
        obstacleBrush=QBrush(QColor(50,50,50,255))
        self.drawEntities(obstacles,obstaclePen,obstacleBrush)

        # draw foods
        foods=self.world.getFoods()
        foodPen=QPen(QBrush(QColor()),0)
        foodBrush=QBrush(QColor(200,200,200,150))
        self.drawEntities(foods,foodPen,foodBrush)

        # draw ants
        ants=self.world.getAnts()
        antPen=QPen(QBrush(QColor()),0)
        antBrush=QBrush(QColor(100,100,50,200))
        self.drawEntities(ants,antPen,antBrush,True)

        # draw anthills
        anthills=self.world.getAnthills()
        anthillPen=QPen(QBrush(QColor()),0)
        anthillBrush=QBrush(QColor(200,20,20,150))
        self.drawEntities(anthills,anthillPen,anthillBrush)

        # show statistics Kamil
        s = self.world.getStatistics()
        s2 = s.print()
        _translate = QtCore.QCoreApplication.translate
        self.ui.stat_label.setText(_translate("MainWindow", s2))


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
            
    def on_saveStateButton_released(self):
        self.world.saveState()
            
    def on_loadStateButton_released(self):
        self.world.loadState()
            
    def on_framerateBox_valueChanged(self):
        self.simulationFramerate=self.ui.framerateBox.value()
        if(self.refreshTimer):
            self.refreshTimer.stop()
            self.refreshTimer.start(1000/self.simulationFramerate)

    def on_actionExit_triggered(self):
        self.close()

    def on_actionAbout_triggered(self):
        print("Anthill simulator version 0. ")
