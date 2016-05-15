from PyQt5.QtWidgets import QMainWindow
from PyQt5.QtWidgets import QGraphicsView
from PyQt5.QtWidgets import QGraphicsScene
from PyQt5.QtWidgets import QFileDialog
from PyQt5.QtWidgets import QSizePolicy

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
from WorldGeneratorParamsDialog import WorldGeneratorParamsDialog

import ctypes

import anthill
#from timer import Timer

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure
class Statistics():
	def __init__(self, stat_label, parent, width=3, height=4, dpi=100):
		self.fig = Figure(figsize=(width, height), dpi=dpi)
		self.axes = self.fig.add_subplot(111)
		self.axes.hold(False)
		
		self.canvas = FigureCanvasQTAgg(self.fig)
		parent.addWidget(self.canvas, 1, 0, 1, 1)
		
		self.canvas.setSizePolicy(QSizePolicy.Expanding,QSizePolicy.Expanding)
		self.canvas.updateGeometry()
		
		self.stat_label = stat_label;
		self.reset()

	def refresh(self, stats):
		# stats.print()
		_translate = QtCore.QCoreApplication.translate
		self.stat_label.setText(_translate("MainWindow", stats.print()))
		# counters
		foodCnt = stats.foodCnt()
		self.foods.append(foodCnt.existing)
		self.stepNumbers.append(stats.stepNumber())
		# plot
		self.axes.plot(self.stepNumbers, self.foods)
		self.axes.set_xlabel("Numer kroku symulacji")
		self.axes.set_ylabel("Ilosc jedzenia")
		self.canvas.draw()

	def reset(self):
		self.foods = []
		self.stepNumbers = []

class MainWindow(QMainWindow):

    def __init__(self,path=''):
        super(MainWindow, self).__init__()

        self.ui=Ui_MainWindow()
        self.ui.setupUi(self)

        self.refreshTimer=None
        self.frameSkippingCounter=0

        self.ui.graphicsView.setOptimizationFlags(
            QGraphicsView.DontAdjustForAntialiasing
            | QGraphicsView.DontClipPainter
            | QGraphicsView.DontSavePainterState)

        #self.ui.graphics.setResizeAnchor(self.ui.graphics.NoAnchor)
        #self.on_tab_currentChanged(self.ui.tab.currentIndex())
        #self.showMaximized()

        self.simulationFramerate=10

        # mouse event handlers for painting on simulation map:
        self.lastMousePosOnMap = None
        self.drawOnMap = anthill.Painter.drawObstacles # current painting function
        self.drawingStuff = False

        # flag for moving the camera around
        self.draggingCamera = False
        self.lastMousePosGlobal = None

        def handleMousePress(event):
            if event.button() == QtCore.Qt.LeftButton:
                self.drawingStuff = True
                self.lastMousePosOnMap = None
                self.handlePainterOption()
                self.ui.graphicsView.setMouseTracking(True)
            elif event.button() == QtCore.Qt.RightButton:
                self.draggingCamera = True
                self.lastMousePosGlobal = None
                self.ui.graphicsView.setDragMode(
                    self.ui.graphicsView.ScrollHandDrag)
                self.ui.graphicsView.setMouseTracking(True)

        def handleMouseRelease(event):
            self.lastMousePosOnMap = None
            self.drawingStuff = False
            self.lastMousePosGlobal = None
            self.draggingCamera = False
            self.ui.graphicsView.setMouseTracking(False)
            self.ui.graphicsView.setDragMode(self.ui.graphicsView.NoDrag)

        def handleMouseMove(event):
            if self.drawingStuff:
                mouse_in_map_coords = (
                    self.ui.graphicsView.mapToScene(
                        self.ui.graphicsView.mapFromGlobal(QCursor.pos())))
                coords = list( map(lambda p: int(p),
                    [mouse_in_map_coords.x(), mouse_in_map_coords.y()]))
                coords[1] -= 1 # fix misalignment
                if self.lastMousePosOnMap is None:
                    self.lastMousePosOnMap = coords

                self.drawOnMap(self.world, self.lastMousePosOnMap[0],
                    self.lastMousePosOnMap[1], coords[0], coords[1])
                self.lastMousePosOnMap = coords

            if self.draggingCamera:
                if self.lastMousePosGlobal is None:
                    self.lastMousePosGlobal = QCursor.pos()
                delta = QCursor.pos() - self.lastMousePosGlobal
                self.ui.graphicsView.setTransformationAnchor(
                    self.ui.graphicsView.NoAnchor)
                self.ui.graphicsView.translate(delta.x(), delta.y())
                self.lastMousePosGlobal = QCursor.pos()


        self.ui.graphicsView.mousePressEvent = handleMousePress
        self.ui.graphicsView.mouseReleaseEvent = handleMouseRelease
        self.ui.graphicsView.mouseMoveEvent = handleMouseMove


        def handleWheelEvent(event):
            self.ui.graphicsView.setTransformationAnchor(
                self.ui.graphicsView.AnchorViewCenter)
            scaleFactor = 1.15
            if event.angleDelta().y() < 0:
                scaleFactor = 1.0 / scaleFactor
            self.ui.graphicsView.scale(scaleFactor, scaleFactor)

        self.ui.graphicsView.wheelEvent = handleWheelEvent


        # configure world
        self.world = anthill.World();

        self.paused = True
        
        self.statistics = Statistics(self.ui.stats_label, self.ui.gridLayout_3)
        
    def __exit__(self):
        self.refreshTimer.stop()


    def drawPheromoneMap(self,map,
            baseRGB=(50,30,100),
            baseRGB2=(10,10,10) ):

        data=map.getMap()
        data=list(data)
        scene=self.ui.graphicsView.scene()

        for x,a in enumerate(data):
            if(x > self.world.getDimensions().posX()):
                break
            for y,b in enumerate(a):
                if(y > self.world.getDimensions().posY()):
                    break
                if(b<0.1):
                    # too weak to be sensed
                    continue

                alpha=30+b*8
                if(alpha>100):
                    # clamp
                    alpha=100

                pbrush=QBrush(QColor(baseRGB[0],baseRGB[1],baseRGB[2], alpha))
                ppen=QPen(QBrush(QColor(baseRGB2[0],baseRGB2[1],baseRGB2[2], alpha)),0)
                scene.addRect(x,y,1,1,pen=ppen,brush=pbrush)

    def drawEntities(self,ents,qpen,qbrush,ellipse=False,drawRange=None):
        ents=[ents[a] for a in range(len(ents))]

        scene=self.ui.graphicsView.scene()

        for ent in ents:
            x=ent.getPos().posX()
            y=ent.getPos().posY()
            if(ellipse):
                scene.addEllipse(x,y,1,1,pen=qpen,brush=qbrush)
            else:
                scene.addRect(x,y,1,1,pen=qpen,brush=qbrush)

            if(drawRange!=None):
                rangepen=QPen(QBrush(QColor(0,0,0,30)),0)
                rangeBrush=QBrush(QColor(255,255,255,10))
                scene.addEllipse((x-drawRange), (y-drawRange),
                                 drawRange*2,drawRange*2,
                                 brush=rangeBrush, pen=rangepen)


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

        w = self.world.getDimensions().posX()
        h = self.world.getDimensions().posY()

        self.world.simulationStep()

        # update statistics
        # if ( stats bar opened? ): then ; Kamil, no need for stastics if we aren't looking at them
        stats = self.world.getStatistics();
        if stats:
            self.statistics.refresh(stats)

        self.frameSkippingCounter+=1
        if self.frameSkippingCounter > self.ui.frameSkipping.value():
            self.frameSkippingCounter=0

            if self.ui.graphicsView.scene() is None:
                self.ui.graphicsView.setScene(QGraphicsScene(0,0,w,h))
            scene = self.ui.graphicsView.scene()
            scene.clear()

            bgBrush=QBrush(QColor(50,120,50,255))
            bgPen=QPen(QBrush(QColor()),0)
            scene.addRect(0,0,w,h,pen=bgPen,brush=bgBrush)

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

                # ask user for world generation params
                dialog = WorldGeneratorParamsDialog(self)

                if not dialog.exec_():
                    # if user clicked CANCEL
                    return

                dialog.processResults()
                self.world.setDimensions(dialog.worldWidth, dialog.worldHeight)

                anthill.WorldGenerator.placeObstacles(self.world,
                    dialog.obstaclesParams)
                anthill.WorldGenerator.placeFoods(self.world,
                    dialog.foodsParams)
                anthill.WorldGenerator.placeAnthill(self.world,
                    dialog.anthillParams)
                anthill.WorldGenerator.placeAnts(self.world,
                    dialog.antsParams)
                anthill.WorldGenerator.initPheromoneMaps(self.world,
                    dialog.pheroToFoodCoef,
                    dialog.pheroFromFoodCoef,
                    dialog.pheroAnthillCoef)

                self.statistics.reset()
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
        self.simulationFramerate=max(self.ui.framerateBox.value(),1)
        if(self.refreshTimer):
            self.refreshTimer.stop()
            self.refreshTimer.start(1000/self.simulationFramerate)

    def on_actionExit_triggered(self):
        self.close()

    def on_actionAbout_triggered(self):
        print("Anthill simulator version 0. ")
