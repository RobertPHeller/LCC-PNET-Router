##-*- makefile -*-############################################################
#
#  System        : 
#  Module        : 
#  Object Name   : $RCSfile$
#  Revision      : $Revision$
#  Date          : $Date$
#  Author        : $Author$
#  Created By    : Robert Heller
#  Created       : Wed Mar 15 13:51:40 2023
#  Last Modified : <230711.0917>
#
#  Description	
#
#  Notes
#
#  History
#	
#  $Log$
#
##############################################################################
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
##############################################################################


all: docs

SRCS =  ../main.cxx $(wildcard ../*.hxx) \
	$(wildcard ../PNETStack/include/*.hxx) \
	$(wildcard ../PNETStack/*.cxx) \
	$(wildcard ../targets/*/*.hxx) \
	Revision.h KitBooklet.h
	
Revision.h: ../targets/pb.linux.armv7a/Revision.hxxout
	echo '/** @page Revision Build Revisions' > $@
	grep '^[[:space:]]*"[^"]*",$$' $< |sed 's/^[[:space:]]*"/ * - /g'|sed 's/",$$//g' >> $@
	echo '*/' >> $@
	
.PHONY:
docs: html pdf man

html latex : $(SRCS) Makefile Doxyfile
	doxygen Doxyfile
	
man : ../main.cxx
	doxygen Doxyfile.man
	
pdf: latex
	$(MAKE) -C latex
	touch pdf
	
kit-book.pdf: kitbooklet
	$(MAKE) -C kitbooklet
	mv kitbooklet/refman.pdf kit.pdf
	pdfbook2 kit.pdf	

kitbooklet: ../main.cxx KitBooklet.h Revision.h Makefile \
		Doxyfile.KitBooklet
	doxygen Doxyfile.KitBooklet

clean::
	rm -rf latex html warnings man pdf
	
veryclean:: clean
