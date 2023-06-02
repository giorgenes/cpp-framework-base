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
import optparse
import sys

usage = "usage: %prog tgz_file"
parser = optparse.OptionParser(usage=usage)

(options, args) = parser.parse_args()

if len(args) < 1:
	parser.print_help()
	sys.exit(0)

try:
	list = scm.generate_file_list(args[0])
	print list
except Exception, ex:
	print ex
	
