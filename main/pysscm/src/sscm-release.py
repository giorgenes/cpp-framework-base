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
import scm_util
import string

def get_info(dir):
	fp = file(dir + "/.sscm/info")
	list = []
	item = fp.readline()
	list.append(string.rstrip(item))
	item = fp.readline()
	list.append(string.rstrip(item))
	item = fp.readline()
	list.append(string.rstrip(item))
	fp.close()
	return list


if len(sys.argv) < 2:
	print "usage: " + sys.argv[0] + " <component path>"
	sys.exit(0)



try:
	tmp = scm_util.make_tgz(sys.argv[1])
	info = get_info(sys.argv[1])
	files = scm_util.get_files(sys.argv[1])
	rep = scm.Repository(info[0])
	cp = rep.get_component(info[1])
	new_version = cp.make_release(tmp, info[2], files)
	os.system("rm " + sys.argv[1] + " -r")
	print "Component commited. New version is '" + new_version + "'"
except Exception, ex:
	print ex
	


