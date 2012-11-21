#set yrange[0:10]
y(x)=m*x + c
fit y(x) "ac.txt" using ($1):($2) via m,c
stddev_y = sqrt(FIT_WSSR / (FIT_NDF + 1 ))


#the centre refers to the pivot of the text
set label "Brass Wire" font "Calibri,20" at screen 0.5,0.98 center

set label sprintf("\
Slope of Best Fit Line          : %+4.4f\n\
Intercept of Best Fit Line      : %+4.4f\n\
Standard Deviation              : %4.4f\n\
",m,c,stddev_y) font "Lucida Console,12" at screen 0.02,0.25

set label "\
Performed by: Vivek Sagar and Atul Singh Arora"\
font "Lucida Console,10" at screen 0.02,0.05

set title "Manifestation of Linear Relation" font "Calibri,14"
set ylabel "Fundamental Length Inverse (1/m)" font "Calibri,12"
set xlabel "Frequency (Hertz)" font "Calibri,12"
set bmargin 25
set tmargin 10
#the number of minor divisions, for x & y
set mxtics 5
set mytics 5
set nokey
set grid  xtics ytics mxtics mytics

set pointsize 2
#lw is linewidth, lt is linetype

plot [0:] y(x),"ac.txt" using ($1):($2):(stddev_y) with yerrorbars

set term svg enhanced font "Lucida Console,10" enhanced size 768, 1024
set output "steel_wire.svg"
replot 


pause -1