import sys
from ROOT import TFile, TTree
from array import array

pawfile  = sys.argv[1]
outname  = sys.argv[2]
treename = sys.argv[3]

outfile = TFile.Open(outname,"recreate")
outtree = TTree(treename,"tree created from paw data")

# Create the branches
#   70002        0  .56667      .83878     -46.082      .55845E-02  42.523      8.1742      1.4849      238.00      19998.     
var1  = array('i',[ -1 ])
var2  = array('i',[ -1 ])
var3  = array('f',[ -1 ])
var4  = array('f',[ -1 ])
var5  = array('f',[ -1 ])
var6  = array('f',[ -1 ])
var7  = array('f',[ -1 ])
var8  = array('f',[ -1 ])
var9  = array('f',[ -1 ])
var10 = array('f',[ -1 ])
var11 = array('f',[ -1 ])

outtree.Branch("ev",var1, "ev/I")
outtree.Branch("pa",var2, "pa/I")
outtree.Branch("th",var3, "th/F")
outtree.Branch("ph",var4, "ph/F")
outtree.Branch("pc",var5, "pc/F")
outtree.Branch("d0",var6, "d0/F")
outtree.Branch("ee",var7, "ee/F")
outtree.Branch("eh",var8, "eh/F")
outtree.Branch("de",var9, "de/F")
outtree.Branch("sa",var10,"sa/F")
outtree.Branch("mu",var11,"mu/F")

# read data
pawdata = open(pawfile,"r")
data = pawdata.readlines()

for evt in data:
    evtData = evt.split()
    var1[0]  = int(evtData[0])
    var2[0]  = int(evtData[1])
    var3[0]  = float(evtData[2])
    var4[0]  = float(evtData[3])
    var5[0]  = float(evtData[4])
    var6[0]  = float(evtData[5])
    var7[0]  = float(evtData[6])
    var8[0]  = float(evtData[7])
    var9[0]  = float(evtData[8])
    var10[0] = float(evtData[9])
    var11[0] = float(evtData[10])

    outtree.Fill()

outtree.Write()
outfile.Close()
