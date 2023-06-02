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
import os
import optparse
import scm_util
import string

usage = "usage: %prog old_file new_file "
parser = optparse.OptionParser(usage=usage)

(options, args) = parser.parse_args()

if len(args) < 2:
	parser.print_help()
	sys.exit(0)

try:
	root = scm_util.find_component_root()

	files = scm_util.get_files(root)
	
	abspath1 = os.path.abspath(args[0])
	abspath2 = os.path.abspath(args[1])
	files.append("=." + abspath1[len(root):] + ";." + abspath2[len(root):]  + "\n")
	
	#print files
	scm_util.write_files(root, files)
	print "File '." + abspath1[len(root):] + "' moved to '" + abspath2[len(root):] + "'"

except Exception, ex:
	print ex
