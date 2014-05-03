from math import *
# import pylab
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons

##########GLOBALS plus initialization
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

x1=[]
x2=[]
#####################

def updateArray():
        global x1
        global x2
        x1[:]=[]
        x2[:]=[]
        print r
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
                # print y,result
                # plot (y,result)
                f.write('{0}\t{1}\n'.format(y,result))

def update(val):
        global r
        r=sR.val
        updateArray()
        # print x1,x2
        l.set_xdata(x1)
        l.set_ydata(x2)
        fig.canvas.draw_idle()

###################

fig, ax=plt.subplots()
plt.subplots_adjust(left=0.25,bottom=0.25)
axcolor='lightgoldenrodyellow'
updateArray()
l,=plt.plot(x1,x2)

axRSlider=plt.axes([0.25,0.1,0.65,.03],axisbg=axcolor)
sR=Slider(axRSlider,'Period (R)',1,N-1,valinit=32)


sR.on_changed(update)

plt.show()
#alternate is
#import math
#just that you've to write math.something everytime

# pylab.plot(x1,x2)
# pylab.show()
