#!/usr/bin/python

# node info
class RNode:
	def __init__(self, id, name, flist, nlist, did):
		self.id = id # node id
		self.name = name # node name
		self.flist = flist # face id list
		self.nlist = nlist # neighbour id list, -1 being outside domain, -2 being hosts
		self.did = did # domain id

# precessed data format
class Evil:
	def __init__(self, time, nname, fname, prefix, num):
		self.time = time # time
		self.nname = nname # node name
		self.fname = fname # name of the node NACK comes from
		self.prefix = prefix # prefix of the NACK
		self.num = num # number of NACK

	def printEvil(self):
		print(self.time, self.nname, self.fname, self.prefix, self.num)

# member of evillist
class EvilMember:
	def __init__(self, time, prefix, list):
		self.time = time
		self.prefix = prefix
		self.list = list # [[nname1, fname1, num1], [nname2, fname2, num2]]
 

r1 = RNode(16, '/r1', [256, 257, 258], [-2, -2, 24], 0)
r2 = RNode(17, '/r2', [256, 257, 258], [-2, -2, 24], 0)
r3 = RNode(18, '/r3', [256, 257, 258], [-2, -2, 25], 0)
r4 = RNode(19, '/r4', [256, 257, 258], [-2, -2, 25], 0)
r5 = RNode(20, '/r5', [256, 257, 258], [-2, -2, 26], 0)
r6 = RNode(21, '/r6', [256, 257, 258], [-2, -2, 26], 0)
r7 = RNode(22, '/r7', [256, 257, 258], [-2, -2, 27], 0)
r8 = RNode(23, '/r8', [256, 257, 258], [-2, -2, 27], 0)
r9 = RNode(24, '/r9', [256, 257, 258], [16, 17, 28], 1)
r10 = RNode(25, '/r10', [256, 257, 258], [18, 19, 28], 1)
r11 = RNode(26, '/r11', [256, 257, 258], [20, 21, 29], 1)
r12 = RNode(27, '/r12', [256, 257, 258], [22, 23, 29], 1)
r13 = RNode(28, '/r13', [256, 257, 258], [24, 25, -1], 1)
r14 = RNode(29, '/r14', [256, 257, 258], [26, 27, -1], 1)
r15 = RNode(30, '/r15', [256, 257, 258], [-1, -1, 31], 2)
r16 = RNode(31, '/r16', [256, 257], [30, -2], 2)

ctrlfacelist = ['256', '257', '258', '259', '256', '257', '258', '259', '260', '261', '260', '261', '262', '262', '256', '257']

NodeList = [r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15, r16]
NameList = [n.name for n in NodeList]
IdList = [n.id for n in NodeList]
# Domain1List = [r1, r2, r3, r4]
# Domain2List = [r5, r6, r7]

# dataFormat = ['1.04017', '/r4', '/p1/evil', '257', '10']

# def dataProcess(data):
# 	# ret = []
# 	ret = data.split()
# 	print(ret)


def dataProcess(data):
	try:
		ret = []
		datalist = data.split('\t')[:-1]
		# print(datalist)
		comm = datalist[0].split()
		time = float(comm[0])
		# print(123)
		node = NodeList[NameList.index(comm[1])]
		# print(234)


		for data in datalist[1:]:
			# time = float(data[0])
			# node = NodeList[NameList.index(data[1])]
			# print(node.id)
			data = data.split()
			prefix = data[0]
			face = int(data[1])
			num = int(data[2])
			fromid = node.nlist[node.flist.index(face)]

			if fromid > 0:
				fromname = NodeList[IdList.index(fromid)].name
			elif fromid == -1:
				fromname = 'outside'
			else:
				fromname = 'other'

			ret.append(Evil(time, node.name, fromname, prefix, num))
		
		for r in ret:
			r.printEvil()

		# fromid = node.nlist[node.flist.index(face)]
		# print(fromid)
		# if fromid > 0:
		# 	fromnode = NodeList[IdList.index(fromid)]
		# 	print(fromnode.id)
		# elif fromid == -1:
		# 	pass
		# else:
		# 	pass

		# return 1

	except Exception, errmsg:
		print("Error when processing data.")
		print(errmsg)
		return

	# fromid = node.nlist[node.flist.index(face)] # the node NACK comes from
	# ret = [] # return [time, nodename, fromnodename, prefix, number]
	# # print(fromid)
	# if fromid > 0:
	# 	fromname = NodeList[IdList.index(fromid)].name
	# elif fromid == -1:
	# 	fromname = 'outside'
	# else:
	# 	fromname = 'other'
	# ret = Evil(time, node.name, fromname, prefix, num)

	return ret


def getdepth(root, n, f, t):
    b = [j for j, x in enumerate(f) if x == root[-1]]
    if not b:
        if root[-1] not in ['outside', 'other']:
            t.append(root)
        return 
    for i in b:
        getdepth(root + [n[i]], n, f, t)
