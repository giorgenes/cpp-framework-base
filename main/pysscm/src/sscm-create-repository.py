#!/usr/bin/python
# Copyright (C) 2004 Giorgenes Gelatti
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.


import scm
import sys

if len(sys.argv) < 2:
	print "usage: " + sys.argv[0] + " <path>"
	sys.exit(0)

try:
	rep = scm.Repository(sys.argv[1], 1)
	print "repository created"
except Exception, ex:
	print ex
