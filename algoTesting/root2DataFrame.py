
class PhotonCluster:
	def __init__(self):
		total_energy =0
		core_energy=0
		for x in range(0,49):
		 	towers.append(0)
	def __init__(self,treeEvent,i):
		total_energy = treeEvent.sub_clus_e[i]
		core_energy = treeEvent.sub_clus_ecore[i]
		towers=[]
		#print(total_energy)
		towers.append(treeEvent.tower0[i])
		towers.append(treeEvent.tower1[i])
		towers.append(treeEvent.tower2[i])
		towers.append(treeEvent.tower3[i])
		towers.append(treeEvent.tower4[i])
		towers.append(treeEvent.tower5[i])
		towers.append(treeEvent.tower6[i])
		towers.append(treeEvent.tower7[i])
		towers.append(treeEvent.tower8[i])
		towers.append(treeEvent.tower9[i])
		towers.append(treeEvent.tower10[i])
		towers.append(treeEvent.tower11[i])
		towers.append(treeEvent.tower12[i])
		towers.append(treeEvent.tower13[i])
		towers.append(treeEvent.tower14[i])
		towers.append(treeEvent.tower15[i])
		towers.append(treeEvent.tower16[i])
		towers.append(treeEvent.tower17[i])
		towers.append(treeEvent.tower18[i])
		towers.append(treeEvent.tower19[i])
		towers.append(treeEvent.tower20[i])
		towers.append(treeEvent.tower21[i])
		towers.append(treeEvent.tower22[i])
		towers.append(treeEvent.tower23[i])
		towers.append(treeEvent.tower24[i])
		towers.append(treeEvent.tower25[i])
		towers.append(treeEvent.tower26[i])
		towers.append(treeEvent.tower27[i])
		towers.append(treeEvent.tower28[i])
		towers.append(treeEvent.tower29[i])
		towers.append(treeEvent.tower30[i])
		towers.append(treeEvent.tower31[i])
		towers.append(treeEvent.tower32[i])
		towers.append(treeEvent.tower33[i])
		towers.append(treeEvent.tower34[i])
		towers.append(treeEvent.tower35[i])
		towers.append(treeEvent.tower36[i])
		towers.append(treeEvent.tower37[i])
		towers.append(treeEvent.tower38[i])
		towers.append(treeEvent.tower39[i])
		towers.append(treeEvent.tower40[i])
		towers.append(treeEvent.tower41[i])
		towers.append(treeEvent.tower42[i])
		towers.append(treeEvent.tower43[i])
		towers.append(treeEvent.tower44[i])
		towers.append(treeEvent.tower45[i])
		towers.append(treeEvent.tower46[i])
		towers.append(treeEvent.tower47[i])
		towers.append(treeEvent.tower48[i])
	def getRow(self):
		r = [self.total_energy,self.core_energy]
		r.extend(self.towers)
		return r

def makePhotonClusters(chain):
	rClusters = []
	for i in range(0,len(chain.sub_clus_e)):
		if chain.sub_clus_e[i] >1:
			rClusters.append(PhotonCluster(chain,i))
	return rClusters

	

def processTree(tree):
	rClusters = []
	for jentry in xrange(tree.GetEntriesFast()):
		#get and verify the tree within the chain
		ientry = chain.LoadTree(jentry)
		if ientry <0:
			break
		#get and verify the next entry 
		nb = chain.GetEntry(jentry)
		if nb <=0:
			continue
		rClusters.extend(makePhotonClusters(chain))
	return rClusters

def makeDataFrame(l_cluster):
	names = ['total_energy','core_energy']
	for i in range(0,49):
		names.append("tower" + str(i))
		
	df  = pd.DataFrame(columns=names)
	for cluster in l_cluster:
		df.append(pd.DataFrame(columns=names,data=cluster.getRow()),ignore_index=True)
	return df




from ROOT import TChain
import pandas as pd

chain = TChain("subtractedTree")
chain.Add("pythdata.root")
df = makeDataFrame(processTree(chain))
print(df.head())

