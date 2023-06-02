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
import scm_util
import sys
import os
import optparse

usage = "usage: %prog [options] <component name> <initial files path>"
parser = optparse.OptionParser(usage=usage)

parser.add_option("-d", "--repository", dest="repository",
                  help="use repository REPOSITORY")


(options, args) = parser.parse_args()

if len(args) < 2:
	parser.print_help()
	sys.exit(0)


try:
	cfg = scm_util.Config()
	repository = cfg.get_repository(options.repository)
	rep = scm.Repository(repository)

	tgz_path = scm_util.make_tgz(args[1])
	rep.add_component(args[0], tgz_path)
	
	os.system("rm " + args[1] + " -r")
	print "component created"
except Exception, ex:
	print ex
	
