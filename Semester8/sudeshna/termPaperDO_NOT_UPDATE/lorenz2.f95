program lorenz                  
use gnuplot_fortran
implicit none
!dt=4.0E-2,
real, parameter :: sigma=10,rho=29,rho2=35, beta=8.0/3, gamma=20.0 , delta=5.0 ,dt=4.0E-3,epsilon=2.85,maxT=20, maxEps=7,deltaEps=0.1, edg=2.0
integer, parameter :: maxI = maxT/dt, snakeSize=10, maxSpheres=200, maxEpsI=maxEps/deltaEps
integer :: i,j,k,l
real :: relativeDistance,t,eps

logical :: plotGraphs = .true., plotBoth=.false.
logical :: findDimension = .false.

real, dimension(3) :: x0 = (/ 0.8, 0.3068, 7.0 /), y0 = (/ -2.8, -0.3068, -7.0 /), yf= (/ 20,0,0/)
real, dimension(maxI,3) :: xp,yp !yf = reshape( (/ (40,i=1,3*maxI) /), (/ maxI, 3 /) )
real, dimension(2*maxI,3) :: xy
real, dimension (3) :: x,xc,k1,k2,k3,k4
real, dimension (3) :: y,yc,m1,m2,m3,m4
real, dimension (maxI,2) :: dis
real, dimension (maxEpsI,2) :: epscount

type sphereLike
   integer :: count
   real, dimension(3):: origin
end type sphereLike

type(sphereLike), dimension(maxI)::sphere
   
call startPlot()

call setXrange(0.0,100.0)
call setYrange(0.0,100.0)
call setZrange(0.0,100.0)

call srand(100)

write (*,*) "Iterating through..(will do ", maxI, "iterations)"

i=1
t=0
do while (t<maxT) !i<3000)
   t=t+dt

   k1=xDot(x0)
   k2=xDot(x0 + 0.5*dt*k1)
   k3=xDot(x0 + 0.5*dt*k2)
   k4=xDot(x0 + dt*k3)
   xc = x0 + dt*(1.0/6)*(k1+2*k2+2*k3+k4)
   x0=xc

   k1=yDot(xc,1,y0)
   k2=yDot(xc,1,y0 + 0.5*dt*k1)
   k3=yDot(xc,1,y0 + 0.5*dt*k2)
   k4=yDot(xc,1,y0 + dt*k3)
   yc = y0 + dt*(1.0/6)*(k1+2*k2+2*k3+k4)
   y0=yc



   xp(i,:)=xc
   yp(i,:)=yc + yf
   dis(i,2)=sqrt(sum((xc-yc)*(xc-yc)))
   dis(i,1)=t

   xy(1:i,:)=xp(1:i,:)
   xy(i:2*i,:)=yp(1:i,:)
   if(plotGraphs) then
      if(plotBoth) then
         call nextPlot3d(xy(1:2*i,1),xy(1:2*i,2),xy(1:2*i,3))
      else
         call nextPlot3d(xp(1:i,1),xp(1:i,2),xp(1:i,3))
      end if
      call nextPlot2d(dis(:i,1),dis(:i,2))
   end if
   i=i+1

end do


if(findDimension) then
   write (*,*) "Finding the box dimension.."

   j=0
   eps=1.0E-10
   sphere(:)%origin(1) = xp(:,1)
   sphere(:)%origin(2) = xp(:,2)
   sphere(:)%origin(3) = xp(:,3)


   do while (eps<maxEps)
      j=j+1
      eps=eps + deltaEps
      do k=1,maxI
         sphere(k)%count=0
         do i=1,maxI
            sphere(k)%count=sphere(k)%count + isInsideSphere(sphere(k)%origin,eps,xp(i,:))
         end do
      end do
      epscount(j,1)=eps
      epscount(j,2)=sum(sphere(1:maxSpheres)%count)/real(maxSpheres)

   end do

   call plot2dSave(log(epscount(1:j,1)),log(epscount(1:j,2)+1.0E-13),filename="dimensionLogLog.pdf",picFormat=1)
   call plot2dSave(epscount(1:j,1),epscount(1:j,2),filename="dimension.pdf",picFormat=1)

   open(file="dimension.dat",unit=4)
   do i=2,j
      write (4,*) epscount(i,1),epscount(i,2)
   end do
end if

call endPlot()
call system("xdg-open result3d.avi")


contains
  function xDot(x)
    real, dimension(3)::xDot,x
    xDot(1)=sigma*(x(2)-x(1))
    xDot(2)=rho*x(1) - x(2) - gamma*x(1)*x(3)
    xDot(3)=(delta*x(1)*x(2)) - (beta*x(3))
  end function xDot

  function yDot(x,l,y)
    real, dimension(3)::yDot,y,x
    real :: u
    integer :: l
    if(l==1) then
       u=x(1)
       yDot(1)=sigma*(y(2)-y(1))
       yDot(2)=rho*u - y(2) - gamma*u*y(3)
       yDot(3)=(delta*u*y(2)) - (beta*y(3))
    else if(l==2) then
       yDot(1)=sigma*(x(2)-y(1))
       yDot(2)=rho*y(1) - x(2) - gamma*y(1)*y(3)
       yDot(3)=(delta*y(1)*x(2)) - (beta*y(3))
    else if(l==3) then
       yDot(1)=sigma*(y(2)-y(1))
       yDot(2)=rho*y(1) - y(2) - gamma*y(1)*x(3)
       yDot(3)=(delta*y(1)*y(2)) - (beta*x(3))
    else
       stop "Not implmented"
    end if
  end function yDot

  function isInsideSphere(r0,eps,r)
    real, dimension(3) :: r0,r
    real :: eps
    integer:: isInsideSphere
    isInsideSphere=0
    if(eps*eps > sum( (r0-r)*(r0-r)) )  then
       isInsideSphere=1
    end if
  end function isInsideSphere

  subroutine initOrigin(spheres)
    type(sphereLike), dimension(:) :: spheres
    do l=1,maxSpheres
       spheres(l)%origin(1)=(0.5-rand())*2.0*edg
       spheres(l)%origin(2)=(0.5-rand())*2.0*edg
       spheres(l)%origin(3)=(0.5-rand())*2.0*edg
    end do
  end subroutine
  
end program lorenz
