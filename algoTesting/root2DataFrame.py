
class Cluster:
	def __init__(self):
		self.total_energy =0
		self.core_energy=0
		for x in range(0,49):
		 	self.towers.append(0)
	def __init__(self,treeEvent,i):
		self.total_energy = treeEvent.sub_clus_e[i]
		self.core_energy = treeEvent.sub_clus_ecore[i]
		self.isPhoton = treeEvent.isPhoton[i]
		self.prob = treeEvent.sub_clus_prob[i]
		self.parent_pid = treeEvent.parent_pid[i]
		self.towers=[]
		#print(total_energy)
		self.towers.append(treeEvent.tower0[i])
		self.towers.append(treeEvent.tower1[i])
		self.towers.append(treeEvent.tower2[i])
		self.towers.append(treeEvent.tower3[i])
		self.towers.append(treeEvent.tower4[i])
		self.towers.append(treeEvent.tower5[i])
		self.towers.append(treeEvent.tower6[i])
		self.towers.append(treeEvent.tower7[i])
		self.towers.append(treeEvent.tower8[i])
		self.towers.append(treeEvent.tower9[i])
		self.towers.append(treeEvent.tower10[i])
		self.towers.append(treeEvent.tower11[i])
		self.towers.append(treeEvent.tower12[i])
		self.towers.append(treeEvent.tower13[i])
		self.towers.append(treeEvent.tower14[i])
		self.towers.append(treeEvent.tower15[i])
		self.towers.append(treeEvent.tower16[i])
		self.towers.append(treeEvent.tower17[i])
		self.towers.append(treeEvent.tower18[i])
		self.towers.append(treeEvent.tower19[i])
		self.towers.append(treeEvent.tower20[i])
		self.towers.append(treeEvent.tower21[i])
		self.towers.append(treeEvent.tower22[i])
		self.towers.append(treeEvent.tower23[i])
		self.towers.append(treeEvent.tower24[i])
		self.towers.append(treeEvent.tower25[i])
		self.towers.append(treeEvent.tower26[i])
		self.towers.append(treeEvent.tower27[i])
		self.towers.append(treeEvent.tower28[i])
		self.towers.append(treeEvent.tower29[i])
		self.towers.append(treeEvent.tower30[i])
		self.towers.append(treeEvent.tower31[i])
		self.towers.append(treeEvent.tower32[i])
		self.towers.append(treeEvent.tower33[i])
		self.towers.append(treeEvent.tower34[i])
		self.towers.append(treeEvent.tower35[i])
		self.towers.append(treeEvent.tower36[i])
		self.towers.append(treeEvent.tower37[i])
		self.towers.append(treeEvent.tower38[i])
		self.towers.append(treeEvent.tower39[i])
		self.towers.append(treeEvent.tower40[i])
		self.towers.append(treeEvent.tower41[i])
		self.towers.append(treeEvent.tower42[i])
		self.towers.append(treeEvent.tower43[i])
		self.towers.append(treeEvent.tower44[i])
		self.towers.append(treeEvent.tower45[i])
		self.towers.append(treeEvent.tower46[i])
		self.towers.append(treeEvent.tower47[i])
		self.towers.append(treeEvent.tower48[i])
	def getDict(self):
		r = {'isPhoton': self.isPhoton, "total_energy": self.total_energy, 'core_energy':self.core_energy, 'sProb': self.prob, 'pid': self.parent_pid}
		for i in range(0,len(self.towers)):
			r["tower"+str(i)] = self.towers[i]
		return r

def makeClusters(chain):
	rClusters = []
	for i in range(0,len(chain.sub_clus_e)):
		#print("chain:"+str(chain.isPhoton[i])+" check:"+str(isPhoton))
		if chain.sub_clus_e[i] >1:
			rClusters.append(Cluster(chain,i))
	return rClusters

def processTree(tree):
	rClusters = []
	for jentry in xrange(tree.GetEntriesFast()):
		#get and verify the tree within the chain
		ientry = chain.LoadTree(jentry)
		if ientry <0:
			print("tree error")
			break
		#get and verify the next entry 
		nb = chain.GetEntry(jentry)
		if nb <=0:
			continue
		rClusters.extend(makeClusters(chain))
	return rClusters

def makeDataFrame(l_cluster):
	names = ['isPhoton','total_energy','core_energy','sProb','pid']
	for i in range(0,49):
		names.append("tower" + str(i))
	rows = []
	for cluster in l_cluster:
		rows.append(cluster.getDict())

	return pd.DataFrame(columns = names,data=rows)

from ROOT import TChain
import pandas as pd

chain = TChain("subtractedTree")
chain.Add("subdata.root")
df = makeDataFrame(processTree(chain))
print(df.head())
df.to_csv("photonclusters.csv")


