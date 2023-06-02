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

parser = optparse.OptionParser()

parser.add_option("-R", "--set-default-repository", dest="repository",
                  help="set the user current repository to REPOSITORY")


(options, args) = parser.parse_args()

if len(sys.argv) < 2:
	parser.print_help()
	sys.exit(0)

if options.repository:
	dir = os.environ['HOME'] + "/.config"
	if not os.path.exists(dir):
		os.mkdir(dir)
	dir = os.environ['HOME'] + "/.config/sscm"
	if not os.path.exists(dir):
		os.mkdir(dir)
	
	fp = file(dir + "/repository", "w")
	fp.write(options.repository)
	fp.close()
