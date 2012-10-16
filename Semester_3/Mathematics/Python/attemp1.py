import matplotlib.pyplot as plot
import math

xs = [0.01*x for x in range(-1000,1000)] #That's -10 to 10 in steps of 0.01
#ys = [math.floor(math.sin(x)*4) for x in xs]
#left crop function
#ys = [math.ceil(0.01*(x)) for x in xs]
#right crop function
#ys = [math.ceil(-0.01*(x-1)) for x in xs]
#centre crop centre
#ys = [math.ceil(0.01*(x))*math.ceil(-0.01*(x-1)) for x in xs]
#m=0.00001
#a=5
#b=a+1
#ys= [math.floor(m*(x-(a)))+1 for x in xs]
#ys= [math.floor(m*(-x + (b)))+1 for x in xs]

#*math.floor(m*(-x + (b)))+1


def Gamma_X(t):
    #return t - math.pi
    #return (1.2**t)*math.cos(t)
    return (t)*(1.2**t)*math.cos(t)
    #return t

def Gamma_Y(t):
    #return math.sin(t - math.pi) + math.pi*math.tan( math.pi / 6)
    #return (1.2**t)*math.sin(t)
    return (t)*(1.1**t)*math.sin(t)
    #return math.cos(t)
    #return t

#alpha=2*math.pi
alpha=20
#alpha=5
#k=3
k=5


tt=[0.01*t for t in range(10000)] #max will be 1000

m=0.0000001

def C(t,a,b):
    #return math.ceil(m*(t-a))*math.ceil(m*(-t+b))
    #return math.floor(m*(t-(a-1)))*math.floor(m*(-t + (b+1)))
    #return ( (math.floor(m*(t - (a))) + 1) * (math.floor(m*(-t + (b))) + 1) )
    return ( (math.ceil(m*(t - a))) * (math.floor(m*(-t + (b))) + 1) )

def TX(theta,X,Y):
    return ((X*math.cos(theta)) - (Y*math.sin(theta)))    

def TY(theta,X,Y):
    return ((X*math.sin(theta)) + (Y*math.cos(theta)))    
    
def Gamma_n_X(t,n,alpha,k):
    return C(t,(n-1)*alpha,(n*alpha))*TX((((n-1)*2*math.pi)/k),Gamma_X(t- ((n-1)*alpha)),Gamma_Y(t - ((n-1)*alpha)))

def Gamma_n_Y(t,n,alpha,k):
    return C(t,(n-1)*alpha,(n*alpha))*TY((((n-1)*2*math.pi)/k),Gamma_X(t- ((n-1)*alpha)),Gamma_Y(t - ((n-1)*alpha)))


#xt=[C(t,0,alpha)*t*math.cos(t) +
    #C(t,alpha,2*alpha)*TX((1*2*math.pi)/k,(t-alpha)*math.cos(t-alpha),(t-alpha)*math.sin(t-alpha))    
    #for t in tt]
#xt = [  Gamma_n_X(t,2,alpha,k) for t in tt]
xt = [Gamma_n_X(t,1,alpha,k) + Gamma_n_X(t,2,alpha,k) + Gamma_n_X(t,3,alpha,k) + Gamma_n_X(t,4,alpha,k) + Gamma_n_X(t,5,alpha,k) for t in tt]
#xt = [  Gamma_n_X(t,1,alpha,k) + Gamma_n_X(t,2,alpha,k) + Gamma_n_X(t,3,alpha,k) for t in tt]

#yt=[C(t,0,alpha)*t*math.sin(t) +
#    C(t,alpha,2*alpha)*TY((1*2*math.pi)/k,(t-alpha)*math.cos(t-alpha),(t-alpha)*math.sin(t-alpha)) for t in tt]

#yt = [ Gamma_n_Y(t,2,alpha,k) for t in tt]
yt = [ Gamma_n_Y(t,1,alpha,k) + Gamma_n_Y(t,2,alpha,k) + Gamma_n_Y(t,3,alpha,k) + Gamma_n_Y(t,4,alpha,k) + Gamma_n_Y(t,5,alpha,k) for t in tt]
#yt = [ Gamma_n_Y(t,1,alpha,k) + Gamma_n_Y(t,2,alpha,k) + Gamma_n_Y(t,3,alpha,k) for t in tt]


xt= [t for t in tt]
yt= [math.sin(2*t)*math.cos(t/5) for t in tt]

#plot.plot(xt, yt,marker='o', linestyle='--', color='r')
plot.plot(xt,yt)

#plot.xlim([-8,8])
#plot.ylim([-8,8])
plot.grid(True)
#plot.savefig("arjit_galaxy2_floor_test.png")
#plot.savefig("spiral_lessdense2_clone.png")
plot.show()
