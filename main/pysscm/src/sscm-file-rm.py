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

usage = "usage: %prog files ..."
parser = optparse.OptionParser(usage=usage)

(options, args) = parser.parse_args()

if len(args) < 1:
	parser.print_help()
	sys.exit(0)

try:
	root = scm_util.find_component_root()

	files = scm_util.get_files(root)
	for i in range(len(args)):
		abspath = os.path.abspath(args[i])
		#print abspath
		files.append("-." + abspath[len(root):] + "\n")
		print "File '." + abspath[len(root):] + "' removed"
	
	#print files
	scm_util.write_files(root, files)

except Exception, ex:
	print ex
