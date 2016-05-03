from cv2blur import CV2Blur
from cv2nothing import CV2Nothing
from cv2add import CV2Add
from cv2multiply import CV2Multiply
from cv2gausblur import CV2GausBlur

class CV2Selector(object):
        def __init__(self):
		self.selections = { "nothing"  : CV2Nothing(),
                                    "blur"     : CV2Blur() ,
                                    "add"      : CV2Add(),
                                    "multiply" : CV2Multiply(),
                                    "gausblur" : CV2GausBlur()}
                
                self.selection = None

	def select(self,name):
		assert name in self.selections.keys()
		self.selection = self.selections[name]

	def apply(self,image):
		return self.selection.apply(image)