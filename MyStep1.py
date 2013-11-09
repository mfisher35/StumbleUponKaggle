#Author: Matthew Fisher
#First: TF-IDF transformation 
#Second: Boosted Decision Tree Classification using the remaining variables

# -*- coding: utf-8 -*-
from sklearn import metrics,preprocessing,cross_validation
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.ensemble import AdaBoostClassifier
import sklearn.linear_model as lm
from sklearn.externals.six.moves import zip
import pylab as pl
from sklearn.datasets import make_gaussian_quantiles
from sklearn.ensemble import AdaBoostClassifier
from sklearn.externals.six.moves import xrange
from sklearn.metrics import accuracy_score
from sklearn.tree import DecisionTreeClassifier
import pandas as p
import numpy as np
import sys


stops = ["0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","about","after","all","also","an","and","another","any","are","as","at","be","because","been","before","being","between","both","but","by","came","can","come","could","did","do","does","each","else","for","from","get","got","had","has","have","he","her","here","him","himself","his","how","if","in","into","is","it","its","just","like","make","many","me","might","more","most","much","must","my","never","no","now","of","on","only","or","other","our","out","over","re","said","same","see","should","since","so","some","still","such","take","than","that","the","their","them","then","there","these","they","this","those","through","to","too","under","up","use","very","want","was","way","we","well","were","what","when","where","which","while","who","will","with","would","you","your"]
	

print '======'
print 'TF-IDF'
print '======'
print ''
print 'reading data...'

train_data = list(np.array(p.read_table('train.tsv'))[:,2])
test_data = list(np.array(p.read_table('test.tsv'))[:,2])
nentries_train = len(train_data)

train_isEvergreen = np.array(p.read_table('train.tsv'))[:,-1]

tfv = TfidfVectorizer(min_df=3,  max_features=None, strip_accents='unicode',  
      analyzer='word',stop_words=stops,token_pattern=r'\w{1,}',ngram_range=(1, 3), use_idf=1,smooth_idf=1,sublinear_tf=1)

rd = lm.LogisticRegression(penalty='l2', dual=True, tol=0.0001, 
                           C=1, fit_intercept=True, intercept_scaling=1.0, 
                           class_weight=None, random_state=None)

all_data = train_data + test_data

tfv.fit(all_data)

print 'transforming text to tfv vectors...'
all_data = tfv.transform(all_data)

X_train = all_data[:nentries_train]
X_test = all_data[nentries_train:]

print "20 Fold CV Score: ", np.mean(cross_validation.cross_val_score(rd, X_train, train_isEvergreen, cv=20, scoring='roc_auc'))

print "training on full data"
rd.fit(X_train,train_isEvergreen)
test_pred = rd.predict_proba(X_test)[:,1]
train_pred = rd.predict_proba(X_train)[:,1]

testfile = p.read_csv('test.tsv', sep="\t", na_values=['?'], index_col=1)
pred_df = p.DataFrame(test_pred, index=testfile.index, columns=['label'])
#pred_df.to_csv('benchmark.csv')




print '======'
print 'BDT'
print '======'
print ''

def cleanQs(n):
 if isinstance(n, basestring):
   if '?' in n:
    return 3
 return float(n) 


def processAlchemy(n):
  topics = ['culture_politics','gaming','religon','business','computer_internet','science_technology','sports','culture_politics','recreation','health','arts_entertainment','weather','?']

  for i in range(0,len(topics)):
    if topics[i] in n:
      return i
  return len(topics)-1


all_train_data = np.array(p.read_table('train.tsv'))
all_test_data = np.array(p.read_table('test.tsv'))

all_train_o = list((all_train_data)[:,3:])
all_test_o = list((all_test_data)[:,3:])


train_ids = list(all_train_data[:,1])
train_ids = list(all_test_data[:,1])


bdt_train_input = []
bdt_test_input = []

print 'Cleaning data...'

for i in range(0,len(all_train_o)):
 bdt_train_input.append(all_train_o[i].tolist())
 bdt_train_input[i][0] = processAlchemy(bdt_train_input[i][0])
 bdt_train_input[i][0:0] = [train_pred[i]]

 for j in range(0,len(bdt_train_input[0])):
   bdt_train_input[i][j] = cleanQs(bdt_train_input[i][j])


for i in range(0,len(all_test_o)):
 bdt_test_input.append(all_test_o[i].tolist())
 bdt_test_input[i][0] = processAlchemy(bdt_test_input[i][0])
 bdt_test_input[i][0:0] = [test_pred[i]]

 for j in range(0,len(bdt_test_input[0])):
   bdt_test_input[i][j] = cleanQs(bdt_test_input[i][j])


####################
print "TRAIN"
for i in range(0,len(bdt_train_input)):
 for j in range(0,len(bdt_train_input[0])):
   sys.stdout.write("%.8f	" % bdt_train_input[i][j])
#   print ("%.8f	" % bdt_train_input[i][j])
 print ""
##
print "TEST"
for i in range(0,len(bdt_test_input)):
 for j in range(0,len(bdt_test_input[0])):
   sys.stdout.write("%.8f	" % bdt_test_input[i][j])
#   print ("%.8f	" % bdt_test_input[i][j])
 print ""
####################

#print 'Making numpy bdt objects...'
#
#np_bdt_train_input = np.ndarray(shape=(len(bdt_train_input),len(bdt_train_input[0])-1), dtype=float, order='F')
#np_bdt_test_input = np.ndarray(shape=(len(bdt_test_input),len(bdt_test_input[0])), dtype=float, order='F')
#
#
#for i in range(0,len(bdt_train_input)):
# for j in range(0,len(bdt_train_input[0])-1):
#   np_bdt_train_input[i][j] = bdt_train_input[i][j]
#
#for i in range(0,len(bdt_test_input)):
# for j in range(0,len(bdt_test_input[0])):
#   np_bdt_test_input[i][j] = bdt_test_input[i][j]
#
#bdt_real = AdaBoostClassifier(
#    DecisionTreeClassifier(max_depth=2),
#    n_estimators=500,
#    learning_rate=1)
#
#print "20 Fold CV Score: ", np.mean(cross_validation.cross_val_score(bdt_real, np_bdt_train_input, train_isEvergreen, cv=20, scoring='roc_auc'))
#
#bdt_real.fit(np_bdt_train_input, train_isEvergreen)
#bdt_test_output = bdt_real.predict_proba(bdt_test_input)[:,1]
#bdt_train_output = bdt_real.predict_proba(bdt_test_input)[:,1]
#
#pred_dff = p.DataFrame(bdt_test_output, index=testfile.index, columns=['label'])
#pred_dff.to_csv('submit.csv')


#print bdt_test_output

