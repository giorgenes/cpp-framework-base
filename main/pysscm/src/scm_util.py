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

import os
import string

def make_tgz(path):
	tmp = mktemp()
	cmd = "tar --exclude .sscm -C " + path + " -czf " + tmp + " . 2> /dev/null"
	#print cmd
	ret = os.system(cmd)
	if ret != 0:
		raise Exception("Could not create tgz file")
	return tmp

def get_current_repository():
	path = os.environ['HOME'] + "/.config/sscm/repository"
	if os.path.exists(path):
		fp = file(path, "r")
		rep = fp.readline()
		fp.close()
		return rep
	else:
		return ""


def mktemp():
	fp = os.popen("mktemp /tmp/sscm.XXXXXX")
	path = fp.readline()
	fp.close()
	path = string.strip(path)
	return path

class Config:
	__config_path = ""
	def __init__(self):
		self.__config_path = os.environ['HOME'] + "/.config"
		if not os.path.exists(self.__config_path):
			os.mkdir(self.__config_path)
		self.__config_path = self.__config_path + "/sscm"
		if not os.path.exists(self.__config_path):
			os.mkdir(self.__config_path)

		
	def get_repository(self, p_rep):
		if p_rep:
			return p_rep
		else:
			path = self.__config_path + "/repository"
			if os.path.exists(path):
				fp = file(path, "r")
				rep = fp.readline()
				fp.close()
				return rep
			else:
				return ''

def find_component_root():
	path = os.getcwd()
	while path != '':
		if os.path.exists(path + "/.sscm"):
			return path
		idx = string.rindex(path, '/')
		path = path[0:idx]
	raise Exception("no component found")

def get_files(dir):
	try:
		fp = file(dir + "/.sscm/modfiles")
		list = fp.readlines()
		fp.close()
	except:
		list = []
	return list

def write_files(dir, list):
	fp = file(dir + "/.sscm/modfiles", "w")
	list = fp.writelines(list)
	fp.close()

