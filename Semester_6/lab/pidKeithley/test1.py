# gedit /etc/gpib.conf
# sudo gpib_config --minor 0
import Gpib

source=Gpib.Gpib('source')
multi=Gpib.Gpib('multi')

# sleep()

while(1):
	multi.write(':DATA?')
	resistance=float(multi.read(100))
	print resistance
	if(resistance>100):
		source.write(':OUTP:STAT 1')
	else:
		source.write(':OUTP:STAT 0')



# multi.close()
# source.close()