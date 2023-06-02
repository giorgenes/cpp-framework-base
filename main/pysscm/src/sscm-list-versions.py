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

usage = "usage: %prog [options] <component name>"
parser = optparse.OptionParser(usage=usage)

parser.add_option("-d", "--repository", dest="repository",
                  help="use repository REPOSITORY")

(options, args) = parser.parse_args()

if len(args) < 1:
	parser.print_help()
	sys.exit(0)
	
try:
	cfg = scm_util.Config()
	repository = cfg.get_repository(options.repository)
	rep = scm.Repository(repository)
	cp = rep.get_component(args[0])
	list = cp.get_version_list()

	for i in range(len(list)):
		sp = string.split(string.strip(list[i]), ';')
		print "Version:", sp[0]
		print "Date:", sp[1]
		s = ''
		if len(sp) > 2:
			for j in range(len(sp)-2):
				s += sp[j+2] + ' '
		print 'Derived versions:', s
		if i < len(list)-1:
			print ''

except Exception, ex:
	print ex
	
