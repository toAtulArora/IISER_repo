import winsound, msvcrt
#stdscr = curses.initscr()
#c = stdscr.getch()
run=1
#red males (total) etc.
rm=0
rf=0

wm=0
wf=0

#last red etc.
lr=0
lw=0

lm=0
lf=0

#top value
trm=0
trf=0

twm=0
twf=0

#remove count
removecount=0

def CommonAction(a):
    if(a=='c' or a=='C'):
        lm=0
        lf=0
        lm=0
        lf=0
        global removecount
        removecount=1
        winsound.PlaySound('audio/count_removed.wav',winsound.SND_ASYNC)        
    elif(a=='x' or a=='X'):
        winsound.PlaySound('audio/appterminated.wav',winsound.SND_FILENAME)
        return False
    return True


while(run):
    #if(msvcrt.kbhit()==True):
    #print('Color')
    lr=0
    lw=0

    lm=0
    lf=0
    
    removecount=0
    a=msvcrt.getch()
    
    if(a=='r' or a=='R'):
        winsound.PlaySound('audio/red.wav',winsound.SND_ASYNC)
        lr=1
    elif(a=='w' or a=='W'):
        winsound.PlaySound('audio/white.wav',winsound.SND_ASYNC)
        lw=1
    else:        
        if(CommonAction(a)==False):
            break
        #break

    if(removecount==0):
        a=msvcrt.getch()        
        if((lr==1 and (a=='r' or a=='R')) or (lw==1 and (a=='w' or a=='W'))):
            winsound.PlaySound('audio/female.wav',winsound.SND_ASYNC)
            lf=1
        elif((lr==1 and (a=='w' or a=='W')) or (lw==1 and (a=='r' or a=='R'))):
            winsound.PlaySound('audio/male.wav',winsound.SND_ASYNC)
            lm=1
        else:
            if(CommonAction(a)==False):
                break
            #break

    #this basically is updated if the last times input is confirmed
    if(removecount==0):
        rm=trm
        rf=trf
        wm=twm
        wf=twf

    #this is to display the latest info, while preserving the values for the top term for next iteration in case of correction
    if(lr==1 and lm==1):
        trm=rm+1
    elif(lr==1 and lf==1):
        trf=rf+1
    elif(lw==1 and lm==1):
        twm=wm+1
    elif(lw==1 and lf==1):
        twf=wf+1
    elif(lr==0 and lm==0 and lw==0 and lf==0):
        twm=rm
        twf=wf
        trm=rm
        trf=rf


    print('Red (Male:'+str(trm)+', Female:'+str(trf)+')')
    print('White (Male:'+str(twm)+', Female:'+str(twf)+')')
    print('--')