#*****************************************************************************
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Mar 22 13:36:14 2023
#  Last Modified : <230322.1427>
#
#  Description	
#
#  Notes
#
#  History
#	
#*****************************************************************************
#
#    Copyright (C) 2023  Robert Heller D/B/A Deepwoods Software
#			51 Locke Hill Road
#			Wendell, MA 01379-9728
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# 
#
#*****************************************************************************


import Part
from FreeCAD import Base
import FreeCAD as App

from abc import ABCMeta, abstractmethod, abstractproperty

class  USB_A_Upright(object):
    _height = 14.2
    _length = 11.0
    _width  = 6.42
    _pindia = 0.4
    _pinspace = 1.4
    _pinlength = 2.5
    _pinoff_fromsidemp = 2.55
    _sidempoff_fromedge = 7.4
    _sidemplen = 2.79
    _sidempW = 1.2
    _sidempL = 1.9
    _sidempspace = 5.5
    _frontmpoff_fromsidemp = -3.1
    _frontmplen = 2.5
    _frontmpW = 1.0
    _frontmpL = 1.3
    def __init__(self,name,origin):
        self.name = name
        if not isinstance(origin,Base.Vector):
            raise RuntimeError("origin is not a Vector!")
        self.origin = origin
        borig = origin.add(Base.Vector(-(self._width/2.0),0,0))
        self.body = Part.makePlane(self._width,self._length,borig).extrude(Base.Vector(0,0,self._height))
        sidemp1orig = origin.add(Base.Vector((self._sidempspace/2.0),self._sidempoff_fromedge,0))
        sidemp2orig = origin.add(Base.Vector(-(self._sidempspace/2.0),self._sidempoff_fromedge,0))
        sidemp1 = Part.makePlane(self._sidempW,self._sidempL,sidemp1orig.add(Base.Vector(-self._sidempW/2,-self._sidempL/2,0)))\
                    .extrude(Base.Vector(0,0,-self._sidemplen))
        sidemp2 = Part.makePlane(self._sidempW,self._sidempL,sidemp2orig.add(Base.Vector(-self._sidempW/2,-self._sidempL/2,0)))\
                    .extrude(Base.Vector(0,0,-self._sidemplen))
        self.body = self.body.fuse(sidemp1)
        self.body = self.body.fuse(sidemp2)
        frommporig = origin.add(Base.Vector(0,self._sidempoff_fromedge+self._frontmpoff_fromsidemp,0))
        frommp = Part.makePlane(self._frontmpW,self._frontmpL,frommporig.add(Base.Vector(-self._frontmpW/2,-self._frontmpL/2,0)))\
                    .extrude(Base.Vector(0,0,-self._frontmplen))
        self.body = self.body.fuse(frommp)
        pincenterorig = origin.add(Base.Vector(0,self._sidempoff_fromedge+self._pinoff_fromsidemp,0))
        p1orig = pincenterorig.add(Base.Vector(-(1.5*self._pinspace),0,0))
        for p in range(4):
            porig = p1orig.add(Base.Vector(self._pinspace*p,0,0))
            pin = Part.Face(Part.Wire(Part.makeCircle(self._pindia/2.0,porig)))\
                    .extrude(Base.Vector(0,0,-self._pinlength))
            self.body = self.body.fuse(pin)
    def show(self):
        doc = App.activeDocument()
        obj = doc.addObject("Part::Feature",self.name)
        obj.Shape = self.body
        obj.Label=self.name
        obj.ViewObject.ShapeColor=tuple([.9,.9,.9])
        

if __name__ == '__main__':
    App.ActiveDocument=App.newDocument("temp")
    x = USB_A_Upright("USBA",Base.Vector(0,0,0))
    x.show()
    Gui.SendMsgToActiveView("ViewFit")
    Gui.activeDocument().activeView().viewIsometric()
    
