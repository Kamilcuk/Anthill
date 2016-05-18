from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import QDialog
from PyQt5.QtWidgets import QFormLayout
from PyQt5.QtWidgets import QDialogButtonBox
from PyQt5.QtWidgets import QLabel
from PyQt5.QtWidgets import QLineEdit

from Ui_WorldGenDialog import Ui_WorldGenDialog

import anthill

class WorldGeneratorParamsDialog(QDialog):
    def __init__(self, parent = None):
        super(WorldGeneratorParamsDialog, self).__init__(parent)

        self.ui = Ui_WorldGenDialog()
        self.ui.setupUi(self)

        self.prepareDefaultParams()

    def prepareDefaultParams(self):
        self.obstaclesParams = anthill.ObstaclesParams()
        self.foodsParams = anthill.FoodsParams()
        self.anthillParams = anthill.AnthillParams()
        self.antsParams = anthill.AntsParams()
        self.worldWidth = 200
        self.worldHeight = 200

    def processResults(self):
        self.pheroToFoodCoef = self.ui.toFoodCoef.value() / (15*100.0)
        self.pheroFromFoodCoef = self.ui.fromFoodCoef.value() / (15*100.0)
        self.pheroAnthillCoef = self.ui.toFoodCoef.value() / (15*100.0)
        self.obstaclesParams.applyObstacleFrequency(self.ui.obstacleFreq.value())
        self.foodsParams.applyFoodGenerosity(self.ui.foodGenerosity.value())
        self.antsParams.applyNumAnts(self.ui.numAnts.value())
        self.antsParams.applyRatioScouts(self.ui.percentScouts.value())
        self.worldWidth = self.ui.worldWidth.value()
        self.worldHeight = self.ui.worldHeight.value()
