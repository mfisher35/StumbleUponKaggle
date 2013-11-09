#! /usr/bin/env python


import sys
import os
import commands
import string
 
count = 0
TTT = 400
EEE = 310
DDD = 3

while DDD < 5:
 while EEE < 450:
  while TTT < 550:
    os.system("sed -i '/Loose/s/NTrees=TTT/NTrees=%i/g' Classification.C" % TTT)
    os.system("sed -i '/Loose/s/nEventsMin=EEE/nEventsMin=%i/g' Classification.C" % EEE)
    os.system("sed -i '/Loose/s/MaxDepth=DDD/MaxDepth=%i/g' Classification.C" % DDD)
    os.system("root -l -b -q Classification.C >  outputClass.txt")
    os.system("more outputClass.txt | grep Goodness | awk '{print $7 \"  \" $8}' | sed 's/)//g' | sed 's/(//g' > overtrain.txt")
    os.system("more outputClass.txt | grep ROC -A 2 | grep BDT | awk '{print $9}' > roc")
    os.system("more outputClass.txt | grep ROC -A 2 | grep BDT | awk '{print $11}' > sep")
    os.system("more outputClass.txt | grep ROC -A 2 | grep BDT | awk '{print $12}' > tmvasig")

    froc = open("roc", "r")
    troc = (froc.read().rstrip('\n'))

    fsep = open("sep", "r")
    tsep = (fsep.read().rstrip('\n'))

    ftsig = open("tmvasig", "r")
    ttsig = (ftsig.read().rstrip('\n'))

    fovr = open("overtrain.txt", "r")
    tovr = (fovr.read().rstrip('\n'))



    if count < 1:
      os.system("echo 'NTrees nEventsMin MaxDepth TMVA_OverTrS TMVA_OverTrB TMVA_ROC TMVA_Sep TMVA_Sig' >> FinalResultsL-SimpleFine.txt")
    os.system("echo '%i %i %i %s %s %s %s' >> FinalResultsL-SimpleFine.txt" % (TTT,EEE,DDD,tovr,troc,tsep,ttsig))


    os.system("sed -i '/Loose/s/NTrees=%i/NTrees=TTT/g' Classification.C" % TTT)
    os.system("sed -i '/Loose/s/nEventsMin=%i/nEventsMin=EEE/g' Classification.C" % EEE)
    os.system("sed -i '/Loose/s/MaxDepth=%i/MaxDepth=DDD/g' Classification.C" % DDD)
    TTT = TTT + 10
    count = count + 2
  EEE = EEE + 10
  TTT = 400
 DDD = DDD + 1
 EEE = 300
 TTT = 400

