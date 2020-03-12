      subroutine dficfj(n,x,fvec,fjac,ldfjac,task,r,nint)
      character*(*) task
      integer n,ldfjac,nint
      double precision r
      double precision x(n),fvec(n),fjac(ldfjac,n)
c     **********
c
c     Subroutine dficfj
c
c     This subroutine computes the function and Jacobian matrix of the 
c     Flow in a Channel problem. This problem arises in the analysis
c     of fluid injection through one side of a vertical channel. 
c
c     The problem is modeled by the ordinary differential equation
c
c                 u'''' = R*(u'*u'' - u*u''')
c
c     with boundary conditions 
c
c                 u(0) = u'(0) = 0
c                 u(1) = 1, u'(1) = 0.    
c
c     In this formulation R is the Reynolds number, u is the potential 
c     function, and u' is the tangential velocity of the fluid.  
c     The boundary value problem is discretized by a k-stage collocation 
c     scheme to obtain a system of nonlinear equations.
c
c     The subroutine statement is:
c
c       subroutine dficfj(n,x,fvec,fjac,ldfjac,task,r,nint)
c
c     where
c
c       n is an integer variable.
c         On entry n is the number of variables.
c            For the flow in a channel problem n = 8*nint.
c         On exit n is unchanged.
c
c       x is a double precision array of dimension n.
c         On entry x specifies the vector x if task = 'F', 'J', or 'FJ'.
c            Otherwise x need not be specified.
c         On exit x is unchanged if task = 'F', 'J', or 'FJ'. Otherwise
c            x is set according to task.
c
c       fvec is a double precision array of dimension n.
c         On entry fvec need not be specified.
c         On exit fvec contains the function evaluated at x if
c            task = 'F' or 'FJ'.
c
c       fjac is a double precision array of dimension (ldfjac,n).
c         On entry fjac need not be specified.
c         On exit fjac contains the Jacobian matrix evaluated at x if
c            task = 'J' or 'FJ'.
c
c       ldfjac is an integer variable.
c          On entry ldfjac is the leading dimension of fjac.
c          On exit ldfjac is unchanged.
c
c       task is a character variable.
c         On entry task specifies the action of the subroutine:
c
c            task               action
c            ----               ------
c             'F'     Evaluate the function at x.
c             'J'     Evaluate the Jacobian matrix at x.
c             'FJ'    Evaluate the function and the Jacobian at x.
c             'XS'    Set x to the standard starting point xs.
c
c         On exit task is unchanged.
c
c       r is a double precision variable.
c         On entry r is the Reynolds number.
c         On exit r is unchanged. 
c
c       nint is an integer variable.
c         On entry nint is the number of subintervals in the 
c            k-stage collocation.
c         On exit nint is unchanged.
c
c     MINPACK-2 Project. October 1992.
c     Argonne National Laboratory and University of Minnesota.
c     Brett M. Averick.
c
c     **********
      integer deg,cpts,bc,npi,dim
      parameter(deg=4,cpts=4,bc=2,dim=deg+cpts-1,npi=cpts+deg)
      double precision zero,one,two,three,six,twelve 
      parameter(zero=0.0d0,one=1.0d0,two=2.0d0,three=3.0d0,six=6.0d0,
     +          twelve=12.0d0)

      integer i,j,k,m,var,eqn
      double precision h,xt,hm,rhoijh,nf
      double precision rho(cpts),dw(deg+1,cpts+deg),
     +       rhnfhk(cpts,0:dim,0:dim,0:deg),w(deg+1)

      data (rho(i), i = 1, cpts)
     +     /0.694318413734436035D-1,0.330009490251541138D0,
     +      0.669990539550781250D0,0.930568158626556396D0/
      
c     Check input arguments for errors.

      if (nint .le. 0) task = 'ERROR: NINT MUST BE .GT. 0'
      if (n .ne. 8*nint) task = 'ERROR: N MUST .EQ. 8*NINT'
      if (task(1:5) .eq. 'ERROR') return

c     Initialization.

      h = one/dble(nint)

c     Compute the standard starting point if task = 'XS'

      if (task .eq. 'XS') then

c        The standard starting point corresponds to the solution of the
c        flow in a channel problem with R = 0.

         xt = zero
         do 20 i = 1, nint
            var = (i - 1)*npi
            x(var+1) = xt*xt*(three - two*xt)
            x(var+2) = six*xt*(one - xt)
            x(var+3) = six*(one - two*xt)
            x(var+4) = -twelve
            do 10 j = 1, cpts
               x(var+deg+j) = zero
   10       continue
            xt = xt + h
   20    continue

         return

      endif

c     Store all possible combinations of rho, h, and n factorial.

      hm = one
      do 60 m = 0, deg
         do 50 i = 1, cpts
            rhoijh = hm
            do 40 j = 0, dim
               nf = one
               do 30 k = 0, dim
                  rhnfhk(i,j,k,m) = rhoijh/nf
                  nf = nf*dble(k+1)
   30          continue
               rhoijh = rhoijh*rho(i)
   40       continue
   50    continue
         hm = hm*h
   60 continue

c     Evaluate the function if task = 'F', the Jacobian matrix if 
c     task = 'J', or both if task = 'FJ'.

c     Initialize arrays.

      do 80 j = 1, n
         if (task .eq. 'F' .or. task .eq. 'FJ') fvec(j) = zero
         if (task .eq. 'J' .or. task .eq. 'FJ') then
            do 70 i = 1, n
               fjac(i,j) = zero
  70        continue
         endif
  80  continue
      do 100 k = 1, npi
         do 90 j = 1, deg + 1
            dw(j,k) = zero
   90    continue
  100 continue

c     Set up the boundary equations at t = 0.  u(0) = 0, u'(0) = 0.

      if (task .eq. 'F' .or. task .eq. 'FJ') then
         fvec(1) = x(1)
         fvec(2) = x(2)
      endif
      if (task .eq. 'J' .or. task .eq. 'FJ') then
         fjac(1,1) = one
         fjac(2,2) = one
      endif


      do 220 i = 1, nint
         var = (i - 1)*npi

c        Set up the collocation equations.

         eqn = var + bc
         do 150 k = 1, cpts
            do 130 m = 1, deg + 1
               w(m) = zero
               do 110 j = m, deg
                  w(m) = w(m) + rhnfhk(k,j-m,j-m,j-m)*x(var+j)
                  dw(m,j) = rhnfhk(k,j-m,j-m,j-m)
  110          continue
               do 120 j = 1, cpts
                  w(m) = w(m) + rhnfhk(k,deg+j-m,deg+j-m,deg-m+1)
     +                                                     *x(var+deg+j)
                  dw(m,deg+j) = rhnfhk(k,deg+j-m,deg+j-m,deg-m+1)
  120          continue
  130       continue
            if (task .eq. 'F' .or. task .eq. 'FJ') 
     +         fvec(eqn+k) = w(5) - r*(w(2)*w(3) - w(1)*w(4))
            if (task .eq. 'J' .or. task .eq. 'FJ') then
               do 140 j = 1, npi
                  fjac(eqn+k,var+j) = dw(5,j) - r*(dw(2,j)*w(3)
     +                     + w(2)*dw(3,j) - dw(1,j)*w(4) - w(1)*dw(4,j)) 
  140          continue
            endif
  150    continue

c        Set up the continuity equations.

         eqn = var + bc + cpts
         do 180 m = 1, deg
            w(m) = zero
            do 160 j = m, deg
               w(m) = w(m) + rhnfhk(1,0,j-m,j-m)*x(var+j)
               dw(m,j) = rhnfhk(1,0,j-m,j-m)
  160       continue
            do 170 j = 1, cpts
               w(m) = w(m) + rhnfhk(1,0,deg+j-m,deg-m+1)*x(var+deg+j)
               dw(m,deg+j) = rhnfhk(1,0,deg+j-m,deg-m+1)
  170       continue
  180    continue
         if (i .eq. nint) goto 230
         if (task .eq. 'F' .or. task .eq. 'FJ') then
             do 190 m = 1, deg
               fvec(eqn+m) = x(var+cpts+deg+m) - w(m)
  190        continue
         endif
         if (task .eq. 'J' .or. task .eq. 'FJ') then
            do 210 m = 1, deg
               fjac(eqn+m,var+cpts+deg+m) = one
               do 200 j = 1, npi
                  fjac(eqn+m,var+j) = -dw(m,j)
  200          continue
  210       continue
         endif
  220 continue

c     Set up the boundary equations at t = 1.  u(1) = 1, u'(1) = 0.

  230 continue
      if (task .eq. 'F' .or. task .eq. 'FJ') then
         fvec(n-1) = w(1) - one
         fvec(n) = w(2)
      endif
      if (task .eq. 'J' .or. task .eq. 'FJ') then
         var = n - npi
         do 240 j = 1, npi
            fjac(n-1,var+j) = dw(1,j)
            fjac(n,var+j) = dw(2,j)
  240    continue
      endif

      return

      end
