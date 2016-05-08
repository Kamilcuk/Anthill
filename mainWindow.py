from PyQt5.QtWidgets import QMainWindow
from PyQt5.QtWidgets import QGraphicsView
from PyQt5.QtWidgets import QGraphicsScene
from PyQt5.QtWidgets import QFileDialog

from PyQt5.QtGui import QPen
from PyQt5.QtGui import QBrush
from PyQt5.QtGui import QColor
from PyQt5.QtGui import QCursor, QMouseEvent
from PyQt5.QtCore import QPoint
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
        self.worldWidth=100
        self.worldHeight=50
        self.simulationFramerate=10

        self.pixelSize=5
        w=self.worldWidth * self.pixelSize
        h=self.worldHeight * self.pixelSize


        self.ui.graphicsView.setScene(QGraphicsScene(0,0,w,h))


        # mouse event handlers for painting on simulation map:
        self.lastMousePos = None
        self.drawOnMap = anthill.Painter.drawObstacles # current painting function

        def handleMousePress(event):
            self.lastMousePos = None
            self.handlePainterOption()
            self.ui.graphicsView.setMouseTracking(True)

        def handleMouseRelease(event):
            self.lastMousePos = None
            self.ui.graphicsView.setMouseTracking(False)

        def handleMouseMove(event):
            mouse_in_map_coords = (
                self.ui.graphicsView.mapToScene(
                    self.ui.graphicsView.mapFromGlobal(QCursor.pos())))
            coords = list( map(lambda p: int(p / self.pixelSize),
                [mouse_in_map_coords.x(), mouse_in_map_coords.y()]))
            coords[1] -= 1 # fix misalignment
            if self.lastMousePos is None: self.lastMousePos = coords

            self.drawOnMap(self.world, self.lastMousePos[0],
                self.lastMousePos[1], coords[0], coords[1])
            self.lastMousePos = coords

        self.ui.graphicsView.mousePressEvent = handleMousePress
        self.ui.graphicsView.mouseReleaseEvent = handleMouseRelease
        self.ui.graphicsView.mouseMoveEvent = handleMouseMove



        # configure world
        self.world = anthill.World();
        self.world.setDimensions(self.worldWidth, self.worldHeight)

        self.paused = True

    def __exit__(self):
        self.refreshTimer.stop()


    def drawPheromoneMap(self,map,
            baseRGB=(50,30,100),
            baseRGB2=(10,10,10) ):

        data=map.getMap()
        data=list(data)

        s=self.pixelSize
        scene=self.ui.graphicsView.scene()

        for x,a in enumerate(data):
            if(x>self.worldWidth):
                break
            for y,b in enumerate(a):
                if(y>self.worldHeight):
                    break
                if(b<0.1):
                    # too weak to be sensed
                    continue

                alpha=40+b*10
                if(alpha>100):
                    # clamp
                    alpha=100

                pbrush=QBrush(QColor(baseRGB[0],baseRGB[1],baseRGB[2], alpha))
                ppen=QPen(QBrush(QColor(baseRGB2[0],baseRGB2[1],baseRGB2[2], alpha)),0)
                scene.addRect(x*s,y*s,s,s,pen=ppen,brush=pbrush)

    def drawEntities(self,ents,qpen,qbrush,ellipse=False,drawRange=None):
        ents=[ents[a] for a in range(len(ents))]

        s=self.pixelSize
        scene=self.ui.graphicsView.scene()

        for ent in ents:
            x=ent.getPos().posX()
            y=ent.getPos().posY()
            if(ellipse):
                scene.addEllipse(x*s,y*s,s,s,pen=qpen,brush=qbrush)
            else:
                scene.addRect(x*s,y*s,s,s,pen=qpen,brush=qbrush)

            if(drawRange!=None):
                rangeBrush=QBrush(QColor(255,255,255,30))
                scene.addEllipse((x-drawRange)*s, (y-drawRange)*s,
                                 drawRange*2*s,drawRange*2*s,
                                 brush=rangeBrush)


    def handlePainterOption(self):
        if self.ui.painterButtonGroup.checkedId() == 1:
            self.drawOnMap = anthill.Painter.drawFoods
        elif self.ui.painterButtonGroup.checkedId() == 2:
            self.drawOnMap = anthill.Painter.drawObstacles
        elif self.ui.painterButtonGroup.checkedId() == 3:
           self.drawOnMap = anthill.Painter.drawPheromoneToFood
        elif self.ui.painterButtonGroup.checkedId() == 4:
           self.drawOnMap = anthill.Painter.drawPheromoneFromFood

    @pyqtSlot()
    def refresh(self):

        if self.paused:
            return

        scene=self.ui.graphicsView.scene()
        scene.clear()
        #self.graphicsItems={}

        w=self.worldWidth * self.pixelSize
        h=self.worldHeight * self.pixelSize

        bgBrush=QBrush(QColor(50,120,50,255))
        bgPen=QPen(QBrush(QColor()),0)
        scene.addRect(0,0,w,h,pen=bgPen,brush=bgBrush)

        self.world.simulationStep()

        s=self.pixelSize

        # draw pheromone map
        pheromoneColors=[ (50,50,155),
                 (255,50,50),
                 (0,255,0)]
        pMaps=self.world.getPheromoneMaps()
        pMaps=[pMaps[p] for p in range(len(pMaps))]
        for i,m in enumerate(pMaps):
            self.drawPheromoneMap(m,baseRGB=(pheromoneColors[i]))

        # draw obstacles
        obstacles=self.world.getObstacles()
        obstaclePen=QPen(QBrush(QColor()),0)
        obstacleBrush=QBrush(QColor(50,50,50,255))
        self.drawEntities(obstacles,obstaclePen,obstacleBrush)

        # draw foods
        foods=self.world.getFoods()
        foodPen=QPen(QBrush(QColor()),0)
        foodBrush=QBrush(QColor(200,200,200,70))
        self.drawEntities(foods,foodPen,foodBrush)

        # draw creatures
        creatures=self.world.getCreatures()
        creaturePen=QPen(QBrush(QColor()),0)
        creatureBrush=QBrush(QColor(100,100,50,200))
        self.drawEntities(creatures,creaturePen,creatureBrush,True,7)

        # draw anthills
        anthills=self.world.getAnthills()
        anthillPen=QPen(QBrush(QColor()),0)
        anthillBrush=QBrush(QColor(200,20,20,150))
        self.drawEntities(anthills,anthillPen,anthillBrush)

        # update statistics
        stats = self.world.getStatistics()
        if stats:
            _translate = QtCore.QCoreApplication.translate
            self.ui.stats_label.setText(_translate("MainWindow", stats.print()))

    def restartTimer(self):
        self.refresh()
        if self.refreshTimer:
            self.refreshTimer.stop()
        self.refreshTimer=QTimer(self)
        self.refreshTimer.timeout.connect(self.refresh)
        self.refreshTimer.start(1000/self.simulationFramerate)


    def on_startSimulationButton_released(self):
        _translate = QtCore.QCoreApplication.translate
        if self.paused:
            if self.refreshTimer is None:
                self.restartTimer()

                # todo: some dialog asking for params?
                obstaclesParams = anthill.ObstaclesParams()
                anthill.WorldGenerator.placeObstacles(self.world,
                    obstaclesParams)

                foodsParams = anthill.FoodsParams()
                anthill.WorldGenerator.placeFoods(self.world, foodsParams)

                anthillParams = anthill.AnthillParams()
                anthill.WorldGenerator.placeAnthill(self.world, anthillParams)

                antsParams = anthill.AntsParams()
                anthill.WorldGenerator.placeAnts(self.world, antsParams)

                # todo: ask for pheromone decay rates
                anthill.WorldGenerator.initPheromoneMaps(self.world,
                    0.07, 0.07, 0.07)

                self.world.startSimulation()

            self.ui.startSimulationButton.setText(_translate(
                "MainWindow", "Pause Simulation"))
            self.paused = False
        else:
            self.ui.startSimulationButton.setText(_translate(
                "MainWindow", "Start Simulation"))
            self.paused = True

    def on_stopSimulationButton_released(self):
        if self.refreshTimer:
            _translate = QtCore.QCoreApplication.translate
            self.ui.startSimulationButton.setText(_translate(
                "MainWindow", "Start Simulation"))
            self.paused = True

            self.refresh()

            self.refreshTimer.stop()
            self.refreshTimer=None

            self.world.stopSimulation()


    def on_saveStateButton_released(self):
        self.paused = True
        path=QFileDialog.getSaveFileName(self, "Save File",
                                    ".", "All Files (*)")
        path=path[0]
        print("Saving state to: ",path)
        if(path and path!=''):
            self.world.saveState(path)
        self.paused = False

    def on_loadStateButton_released(self):
        self.paused = True
        self.world.stopSimulation()
        path=QFileDialog.getOpenFileName(self, "Open File",
                                    ".", "All Files (*)")
        path=path[0]
        if(path and path!=''):
            self.world.loadState(path)
            self.restartTimer()
            self.world.startSimulation()
        self.paused = False

    def on_framerateBox_valueChanged(self):
        self.simulationFramerate=self.ui.framerateBox.value()
        if(self.refreshTimer):
            self.refreshTimer.stop()
            self.refreshTimer.start(1000/self.simulationFramerate)

    def on_actionExit_triggered(self):
        self.close()

    def on_actionAbout_triggered(self):
        print("Anthill simulator version 0. ")
