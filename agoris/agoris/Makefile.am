# Makefile.am: help build program sources
# $Id: Makefile.am,v 1.1 2001/07/16 19:10:25 baueran Exp $

# Copyright (c) 2001  Andreas Bauer <baueran@in.tum.de>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
# USA.


AM_CXXFLAGS=-Wall

lib_LTLIBRARIES = libagoris.la

libagoris_la_SOURCES = Board.cc Square.cc Search.cc Game.cc Eval.cc\
	Board.hh Square.hh Search.hh Game.hh Eval.hh

library_includedir = $(includedir)/agoris
library_include_HEADERS = Board.hh Square.hh Search.hh Game.hh Eval.hh


#bin_PROGRAMS = agoris 
#agoris_SOURCES = Agoris.cc Board.cc Square.cc Search.cc Game.cc Eval.cc\
#Board.hh Square.hh Search.hh Game.hh Eval.hh

INCLUDES = -I$(includedir) -I$(top_srcdir)/src

LIBS = @LIBS@
