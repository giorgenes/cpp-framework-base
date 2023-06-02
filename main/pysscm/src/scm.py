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
import datetime

class Component:
	__name = ''
	__path = ''
	def __init__(self, p_name, p_path):
		self.__name = p_name
		self.__path = p_path

	def get_name(self):
		return self.__name

	def get_version(self, p_version):
		path = self.__path + self.__name + "--" + p_version + ".tar.gz"
		if os.path.exists(path):
			return path
		else:
			raise Exception("version not found")

	def get_file_ids_list(self, p_version):
		path = self.__path + self.__name + "--" + p_version + ".list.gz"
		if os.path.exists(path):
			list = []
			fp = os.popen("gunzip -c " + self.__path + self.__name + "--" + p_version + ".list.gz", "r")
			list = fp.readlines()
			fp.close()
			return list
		else:
			raise Exception("version not found")

	def mk_list_file(self, p_version):
		list = generate_file_list(self.get_version(p_version))
		self.mk_list_file_from_list(list, p_version)
		
	def mk_list_file_from_list(self, p_list, p_version):
		fp = os.popen("gzip > " + self.__path + "/" + self.__name + "--" + p_version + ".list.gz", "w")
		fp.writelines(p_list)
		fp.close()

	def get_version_list(self):
		list = []
		fp = file(self.__path + 'versions.lst', 'r')
		versions = fp.readlines()
		fp.close()
		for i in range(len(versions)):
			sp = string.split(string.strip(versions[i]), ' ')
			try:
				fp = file(self.__path + 'version_children--' + sp[0] + '.lst')
				children = fp.readlines()
				fp.close()
			except:
				children = []

			s = sp[0] + ';' + sp[1]
			for j in range(len(children)):
				s += ';' + string.strip(children[j])
			list.append(s)
		return list

		
	def check_files(self, p_merged_list, p_tgz_path):
		#separa os arquivos dos ids
		shouldbe_list = []
		for i in range(len(p_merged_list)):
			id_file_list = string.split(p_merged_list[i], '=')
			shouldbe_list.append(string.strip(id_file_list[1]))

		#print "should be: ", shouldbe_list
		file_id_is_list = generate_file_list(p_tgz_path)
		is_list = []
		for i in range(len(file_id_is_list)):
			id_file_list = string.split(file_id_is_list[i], '=')
			is_list.append(string.strip(id_file_list[1]))

		#print "is: ", is_list


		mark1 = []
		mark2 = []
		for i in range(len(shouldbe_list)):
			mark1.append(0)
		
		for i in range(len(is_list)):
			mark2.append(0)
			
		for i in range(len(shouldbe_list)):
			achou = 0
			pos = 0
			for j in range(len(is_list)):
				if shouldbe_list[i] == is_list[j]:
					achou = 1
					pos = j
					break
			if achou:
				mark1[i] = 1
				mark2[j] = 1

		missing = []
		added = []
		for i in range(len(shouldbe_list)):
			if mark1[i] == 0:
				missing.append(shouldbe_list[i])
	
		for i in range(len(is_list)):
			if mark2[i] == 0:
				added.append(is_list[i])
		
		if len(missing) > 0 or len(added) > 0:
			for i in range(len(missing)):
				print "-", missing[i]
			for i in range(len(added)):
				print "+", added[i]

			raise Exception("there are files missing/added to the component, fix above errors and try again")
		
		
	def add_version_entry(self, p_version):
		d = datetime.date.today()
		sd = str(d.year) + '-' + str(d.month) + '-' + str(d.day)
		fp = file(self.__path + 'versions.lst', 'a')
		fp.write(p_version + " " + sd + '\n')
		fp.close()
		
	def make_release(self, tgz_path, base_version_plus_patch, p_file_list):
		idx = string.index(base_version_plus_patch, "--")
		base_patch   = base_version_plus_patch[idx+2:]
		base_version = base_version_plus_patch[0:idx]
		#print base_patch
		#print base_version
		base_path = self.__path + self.__name + "--" + base_version + "--"
	
		ori_file_id_list = self.get_file_ids_list(base_version_plus_patch)
		#print "mod: ", p_file_list
		#print "ori: ", ori_file_id_list
		merged_list = get_merged_list(ori_file_id_list, p_file_list)
		#print "merged: ", merged_list
		self.check_files(merged_list, tgz_path)
		i = 0
		fname = ""
		while(1):
			patch = get_next_patch(base_patch, i)
			i = i+1
			fname = base_path + patch + ".tar.gz"
			#print fname
			if(not os.path.exists(fname)):
				break

		
		if os.system("cp " + tgz_path + " " + fname) != 0:
			raise Exception("could not copy file")

		self.mk_list_file_from_list(merged_list, base_version + "--" + patch)
		self.add_version_entry(base_version + "--" + patch)
		self.add_version_child(base_version_plus_patch, base_version + "--" + patch)
		return base_version + "--" + patch

	def add_version_child(self, p_father, p_child):
		fp = file(self.__path + 'version_children--' + p_father + '.lst', 'a')
		fp.write(p_child + '\n')
		fp.close()
		

	def make_version(self, p_base_version_plus_patch, p_new_version):
		src_tgz = self.__path + self.__name + "--" + p_base_version_plus_patch + ".tar.gz"
		list_tgz = self.__path + self.__name + "--" + p_base_version_plus_patch + ".list.gz"
		if not os.path.exists(src_tgz):
			raise Exception("component version '" + p_base_version_plus_patch + "' not found")
		
		new_src_tgz = self.__path + self.__name + "--" + p_new_version + "--init.tar.gz"
		new_list_tgz = self.__path + self.__name + "--" + p_new_version + "--init.list.gz"
		if os.path.exists(new_src_tgz):
			raise Exception("component version '" + p_new_version + "' already exists")

		os.system("cp " + src_tgz + " " + new_src_tgz)
		os.system("cp " + list_tgz + " " + new_list_tgz)
		self.add_version_entry(p_new_version + "--init")
		self.add_version_child(p_base_version_plus_patch, p_new_version + "--init")

class Repository:
	__path = ''

	def __init__(self, p_path, p_create = 0):
		if not self.repository_exists(p_path):
			if p_create:
				self.create_repository(p_path)
			else:
				raise Exception("Invalid repository")
		else:
			if p_create:
				raise Exception("Repository already exists")
		self.__path = p_path
			

	def repository_exists(self, p_path):
		try:
			os.stat(p_path)
			magic_path = p_path + "/.files/magic"
			fp = file(magic_path)
			magic = fp.readline()
			fp.close()
			if magic == "scm":
				return 1
			else:
				return 0
		except:
			return 0

	def component_exists(self, p_name):
		if os.path.exists(self.__path + "/" + p_name):
			return 1
		else:
			return 0

	def add_component_to_list(self, p_component):
		fp = file(self.__path + "/.files/components.list", 'a')
		fp.write(p_component + '\n')
		fp.close()

	def get_components_list(self):
		fp = file(self.__path + "/.files/components.list", 'r')
		list = fp.readlines()
		fp.close()
		return list
		
	def add_component(self, p_component, p_tgz):
		if self.component_exists(p_component):
			raise Exception("Component already exists")
		try:
			os.mkdir(self.__path + "/" + p_component)
			cmd = "cp " + p_tgz
			ffile = self.__path
			ffile += "/" + p_component 
			ffile += "/" + p_component + "--init--init.tar.gz"
			cmd +=	" " + ffile
			#print cmd
			ret = os.system(cmd)
			if ret != 0:
				raise Exception("erro copiando tgz")
			cp = self.get_component(p_component)
			cp.mk_list_file("init--init")
			cp.add_version_entry("init--init")
			self.add_component_to_list(p_component)
			
		except:
			raise

	def create_repository(self, path):
		try:
			os.mkdir(path)
			os.mkdir(path + "/.files")
			os.mkdir(path + "/.files/backup")
			magic_path = path + "/.files/magic"
			fp = file(magic_path, "w")
			fp.write("scm")
			fp.close()
		except:
			raise Exception("nao foi possivel criar " + path)



	def get_component(self, p_component):
		if not self.component_exists(p_component):
			raise Exception("Component doesnt exist")
		
		path = self.__path + "/" + p_component + "/"
		return Component(p_component, path)


def get_next_patch(base, idx):
	if(base == "init"):
		return str(idx) + '.1'
	else:
		if idx == 0:
			dot_idx = string.rindex(base, '.')
			n = string.atoi(base[dot_idx+1:])
			return base[0:dot_idx+1] + str(n+1)
		else:
			return base + '.' + str(idx) + '.1'

def add2list(p_list, p_file):
	id = gen_file_name(p_file)
	if id != '':
		for i in range(len(p_list)):
			sp = string.split(p_list[i], "=")
			id_file = string.strip(sp[1])
			if id_file == string.strip(p_file):
				raise Exception("File " + p_file + " already in component")

		p_list.append(id + "=" + p_file)
		
def delfromlist(p_list, p_file):
	file = string.strip(p_file)
	deleted = 0
	for i in range(len(p_list)):
		sp = string.split(p_list[i], "=")
		id_file = string.strip(sp[1])
		if id_file == file:
			p_list[i] = sp[0] + "=*\n"
			deleted = 1
	if not deleted:
		raise Exception("File " + file + " NOT in component")

def movefromlist(p_list, p_file):
	file = string.strip(p_file)
	moved = string.split(file, ';')
	deleted = 0
	for i in range(len(p_list)):
		sp = string.split(p_list[i], "=")
		id_file = string.strip(sp[1])
		if id_file == moved[0]:
			p_list[i] = sp[0] + "=" + moved[1] + "\n"
			deleted = 1
	if not deleted:
		raise Exception("File " + moved[0] + " NOT in component")


def get_merged_list(p_file_list, p_diff_list):
	list = p_file_list
	for i in range(len(p_diff_list)):
		if p_diff_list[i][0] == '+':
			add2list(p_file_list, p_diff_list[i][1:])
			pass
		if p_diff_list[i][0] == '-':
			delfromlist(p_file_list, p_diff_list[i][1:])
			pass
		if p_diff_list[i][0] == '=':
			movefromlist(p_file_list, p_diff_list[i][1:])
			pass
		#print i, p_diff_list[i]
	return list



# FIXME: nao criar um id que ja existe.
def gen_file_name(p_path):
	path = string.strip(p_path)
	if not path[-1] == '/':
		if path[0:2] == './':
			path = path[2:]
		path = string.replace(path, '-', '_')
		path = string.replace(path, '/', '_')
		path = string.replace(path, '.', '_')
		path = string.replace(path, '=', '_')
		return path
	else:
		return ''

	
def generate_file_list(tgz_path):
	list = []
	cmd = "tar -tzf " + tgz_path
	fp = os.popen(cmd, "r")
	
	l = 'a'
	while l:
		l = fp.readline()
		if not l:
			break
		striped = string.strip(l)
		fname = gen_file_name(striped)
		if fname:
			list.append(fname + "=" + striped + "\n")
	fp.close()
	return list

