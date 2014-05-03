from math import *
import pylab
#alternate is
#import math
#just that you've to write math.something everytime
n=8
N=2**n
r=32.0    #its the period (put the decimal for context based floating)
Y=234     #temporarily, later we'll plot 
print "Number of bits:",n
print "Number of numbers:",N
print "Period:",r

A=int(N/r)
if( (N/r)%10 > 0):
	A=A+1
print "A (in accordance with preskill):", A

#y=Y
#summation=0
x1=[]
x2=[]
f=open('workfile','w')
for y in range(0,N-1):
        summation=0        
        for j in range(0,A-1):
                theta=2*pi*j*r*y/N
                summation+=cos(theta) + 1j*sin(theta)
                #print abs(summation)
        result=(1.0/(A*N))*(abs(summation)**2)
        x2.append(result)
        x1.append(y)
        print y,result
        # plot (y,result)
        f.write('{0}\t{1}\n'.format(y,result))
pylab.plot(x1,x2)
pylab.show()
