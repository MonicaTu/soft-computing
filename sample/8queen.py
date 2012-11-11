import threading
import InstantVidget as iv

class EightQueen:
	def __init__( self ):
		self.queen = [[0]*8,[0]*8,[0]*8,[0]*8,[0]*8,[0]*8,[0]*8,[0]*8]
		self.idx = 0
		self.t = None
		self.vidget = iv.callback.GetVidget()
		iv.callback.SetCallback( iv.EVidgetMessage_Command, self.OnCommand )
		iv.callback.SetCallback( iv.EVidgetMessage_Destroy, self.OnDestroy )
		iv.callback.SetCallback( iv.EVidgetMessage_Function, self.OnFunction )
		self.vidget.GetChildById( 40 ).text = iv.GetSettings().GetString( "QueenEdit", "EditBox" )
		"""
		Following are example calls for retrieving xml/vrx data.
		Note that the data is predefined, modifying the content in xml will not reflect
		any change on created vidgets.
		"""
		print "Count: ", iv.callback.GetXml().GetChildCount()
		print "Child element child Count: ", iv.callback.GetXml().GetChildElement(0).GetChildCount()
		print "pos = ", iv.callback.GetXml().GetChildElement(0).GetAttribute( "pos" )

	def FillQueen( self, l ):
		fail = 0;
		for i in range( 0, 8 ):
			fail = 0;
			for j in range( 0, l ):
				if self.queen[j][i] == 1 :
					fail = 1
					break
				if (l-j + i) < 8 and self.queen[j][l-j+i] :
					fail = 1
					break
				if  (i -l+j) >= 0 and self.queen[j][i-l+j] :
					fail = 1
					break
			if fail :
				continue

			self.queen[l][i] = 1
			if l < 7 :
				self.FillQueen( l + 1 )
			else :
				self.idx += 1
				for x in range(0,8):
					for y in range(0,8):
						child = self.vidget.GetChildById(10000).GetChildById( 1000+ x*100+y )
						(px,py,s,a) = child.GetPosition()

						if self.queen[x][y] == 0:
							child.text = ""
						else :
							child.text = "Q"

						child.Move( px, py, s, self.queen[x][y]*200+55, False )
				self.vidget.GetChildById(10000).GetChildById( 2 ).text = "Solution: " + str(self.idx)
			self.queen[l][i] = 0
		if l == 0 :
			self.t = None
			self.idx = 0

	def OnCommand( self, id, param ):
		if id == 1:
			if self.t == None:
				self.t = threading.Thread( None, self.FillQueen, None, ( 0, ) )
				self.t.start()

	def OnFunction( self, func, param ):
		if func == "Start":
			if self.t == None:
				self.t = threading.Thread( None, self.FillQueen, None, ( 0, ) )
				self.t.start()
		elif func == "Message":
			msgBtn = self.vidget.GetChildById( 22 )
			if msgBtn == None:
				r = iv.MessageBox( "Title", "Do you want to create a motion test button?", iv.EVMBStyle_YesNoCancel )
				if iv.EVMBResult_Yes == r:
					iv.CreateVidget( '<Button pos="150,300,80,30" id="22" frame="1" text="Motion" function="Motion" needFocus="1"/>', self.vidget )
			else:
				r = iv.MessageBox( "Title", "Do you want to destroy the motion test button?", iv.EVMBStyle_YesNoCancel )
				if iv.EVMBResult_Yes == r:
					msgBtn.Destroy()
		elif func == "Motion":
			msgBtn = self.vidget.GetChildById( 22 )
			if msgBtn != None:
				(x,y,s,a) = msgBtn.GetPosition()
				if s < 2 :
					msgBtn.Move( x-10, y+10, s*1.1, a, False )
				else:
					msgBtn.Move( x+10, y-10, s*0.92, a, False )

	def OnDestroy( self, param1, param2 ):
		iv.GetSettings().SetString( "QueenEdit", self.vidget.GetChildById( 40 ).text )
		if self.t != None:
			self.t.join()
			self.t = None
		self.vidget = None

def Init():
	myCallback = EightQueen()
