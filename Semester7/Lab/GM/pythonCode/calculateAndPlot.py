import csv
import numpy as np
import matplotlib.pyplot as plt

x=[]
y=[]
errorY=[]

with open('../data/exp4lead') as csvfile:
    readingsReader=csv.reader(csvfile,delimiter='\t')
    for row in readingsReader:
        #print row
        temp=0.0
        rowI=iter(row)
        next(rowI)
        nonZero=0
        for element in rowI:
            if(element):
                nonZero+=1
                temp=temp+float(element)
        mean=temp/nonZero
        #print "Testing" + str(nonZero) + str(len(row))
        #mean=temp/len(row)
        variance=0
        rowI=iter(row)
        next(rowI)
        nonZero=0
        for element in rowI:
            if(element):
                nonZero+=1            
                variance=variance+((float(element)-mean)**2)
        #print str(mean) + '\t' + str(variance**0.5)
        variance=variance/nonZero
        x.append(row[0])
        y.append(mean)
        errorY.append((variance**0.5))
        print row[0],int(mean),int((variance**0.5))

#x=np.array(finalList)
#print x[0]
#plt.figure()
#plt.errorbar(finalList[0])
print errorY
plt.figure()
#plt.xscale('log')
#plt.yscale('log')
plt.grid()
plt.errorbar(x,y,yerr=errorY,ecolor='r',fmt='--o',capthick=2)
plt.show()
