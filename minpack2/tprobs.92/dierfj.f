      subroutine dierfj(n,x,fvec,fjac,ldfjac,task,a,b,c,nint)
      character*(*) task
      integer n,ldfjac,nint
      double precision a,b,c
      double precision x(n),fvec(n),fjac(ldfjac,n)
c
c     **********
c
c     Subroutine dierfj
c
c     This subroutine computes the function and Jacobian matrix of the
c     Incompressible Elastic Rod problem. This problem arises in the 
c     analysis of a thin incompressible elastic rod clamped at the 
c     origin and acted on by a vertical force Q, a horizontal force P, 
c     and a torque M.
c 
c     The Elastic Rod problem is modeled by the system of 
c     ordinary differential equations
c
c                     x' = cos(theta)
c                     y' = sin(theta)
c                     theta' = Qx - Py + M
c
c     with the boundary conditions
c
c                     x(0) = 0, x(1) = a
c                     y(0) = 0, y(1) = b
c                     theta(0) = 0, theta(1) = c.
c
c     In this formulation (a,b) and c are the location and local angle
c     of the right end of the incompressable elastic rod of unit length.
c     The boundary value problem is discretized by a k-stage collocation
c     scheme to obtain a system of nonlinear equations.
c
c     The subroutine statement is:
c
c       subroutine dierfj(n,x,fvec,fjac,ldfjac,task,a,b,c,nint)
c
c     where
c
c       n is an integer variable.
c         On entry n is the number of variables.
c            For the Inverse Elastic Rod problem n must equal 15*nint+3.
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
c       a is a double precision variable.
c         On entry a is the X-coordinate of the right end of the rod.
c         On exit a is unchanged.
c
c       b is a double precision variable.
c         On entry b is the Y-coordinate of the right end of the rod.
c         On exit b is unchanged.
c
c       c is a double precision variable.
c         On entry c is the local angle at the right end of the rod.
c         On exit c is unchanged.
c
c       nint is an integer variable.
c         On entry nint is the number of subintervals in the
c            k-stage collocation.
c         On exit nint is unchanged.
c
c     MINPACK-2 Project. October 1992.
c     Argonne National Laboratory and University of Minnesota.
c     Brett M. Averick and Guo-Liang Xue
c
c     **********
      integer  s,cpts,maxdeg,dim
      parameter(s=3,cpts=4,maxdeg=1,dim=maxdeg+cpts-1)
      double precision len,zero,one,xt
      parameter(len=1.0d0,zero=0.0d0,one=1.0d0)

      integer i,j,k,l,m,e,ideg,npi,ivar
      integer deg(s),var(s),eqn(s),sumdeg(0:s)
      double precision rho(cpts),rhnfhk(cpts,0:dim,0:dim,0:maxdeg),
     +       icond(s),w(maxdeg+1,s),dw(maxdeg+1,cpts+maxdeg,s)
      double precision h,hm,nf,rhoijh

      data (rho(i), i = 1, cpts)
     +     /0.694318413734436035D-1,0.330009490251541138D0,
     +      0.669990539550781250D0,0.930568158626556396D0/
      data (deg(i), i = 1, s) /1,1,1/
      data (icond(i), i = 1, s) /0.0d0,0.0d0,0.0d0/

c     Check input arguments for errors.

      if (nint .le. 0) task = 'ERROR: NINT MUST BE .GT. 0'
      if (n .ne. 15*nint + 3) task = 'ERROR: N MUST .EQ. 15*NINT + 3'
      if (task(1:5) .eq. 'ERROR') return

c     Initialization. 

      h = len/dble(nint)

c     Store all possible combinations of rho, h, and n factorial.

      hm = one
      do 40 m = 0, maxdeg
         do 30 i = 1, cpts
            rhoijh = hm
            do 20 j = 0, dim
               nf = one
               do 10 k = 0, dim
                  rhnfhk(i,j,k,m) = rhoijh/nf
                  nf = nf*dble(k+1)
   10          continue
               rhoijh = rhoijh*rho(i)
   20       continue
   30    continue
         hm = hm*h
   40 continue

      ideg = 0
      sumdeg(0) = 0
      do 50, i = 1, s
         ideg = ideg + deg(i)
         sumdeg(i) = sumdeg(i-1) + deg(i)
   50 continue
      npi = s*cpts + ideg

c     Compute the standard starting point if task = 'XS'.

      if (task .eq. 'XS') then
         do 60, i = 1, n
            x(i) = zero
   60    continue
         xt = zero
         ivar = 0
         do 70, i = 1, nint
            x(ivar+1) = xt
            x(ivar+2) = one
            xt = xt + h
            ivar = ivar + npi
   70    continue

         return

      endif

c     Evaluate the function if task = 'F', the Jacobian matrix if 
c     task = 'J', or both if task = 'FJ'.

c     Initialize arrays.

      do 90, i = 1, n
         if (task .eq. 'F' .or. task .eq. 'FJ') fvec(i) = zero
         if (task .eq. 'J' .or. task .eq. 'FJ') then
            do 80, j = 1, n
               fjac(i,j) = zero
   80       continue
         endif
   90 continue

      do 120, k = 1, s
         do 110, j = 1, maxdeg + 1
            w(j,k) = zero
            do 100, l = 1, cpts + maxdeg
               dw(j,l,k) = zero
  100       continue
  110    continue
  120 continue
                      
c     Satisfy initial conditions at t = 0.  yi(0) = icond(i).

      do 130, i = 1, s
         if (task .eq. 'F' .or. task .eq. 'FJ') then
            fvec(i) = x((i-1)*cpts+sumdeg(i-1)+1) - icond(i)
         endif
         if (task .eq. 'J' .or. task .eq. 'FJ') then
            fjac(i,(i-1)*cpts+sumdeg(i-1)+1) = one
         endif
  130 continue

c     Set up the collocation equations.

      do 200, i = 1, nint
         do 190, k = 1, cpts
            do 170, e = 1, s
               var(e) = (i - 1)*npi + (e - 1)*cpts + sumdeg(e-1)
               eqn(e) = s + (i - 1)*npi + (e - 1)*cpts
               do 160, m = 1, deg(e) + 1
                  w(m,e) = zero
                  do 140, j = m, deg(e)
                     w(m,e) = w(m,e) + x(var(e)+j)*rhnfhk(k,j-m,j-m,j-m)
                    dw(m,j,e) =  rhnfhk(k,j-m,j-m,j-m)
  140             continue
                  do 150, j = 1, cpts
                     w(m,e) = w(m,e) + x(var(e)+deg(e)+j)*
     +                        rhnfhk(k,deg(e)+j-m,deg(e)+j-m,deg(e)-m+1)
                     dw(m,deg(e)+j,e) = 
     +                        rhnfhk(k,deg(e)+j-m,deg(e)+j-m,deg(e)-m+1)
  150             continue
  160          continue
  170       continue

            if (task .eq. 'F' .or. task .eq. 'FJ') then
               fvec(eqn(1)+k) = w(2,1) - cos(w(1,3))
               fvec(eqn(2)+k) = w(2,2) - sin(w(1,3))
               fvec(eqn(3)+k) = w(2,3) - x(n-2)*w(1,1) + x(n-1)*w(1,2)
     +                                                           - x(n)
            endif

            if (task .eq. 'J' .or. task .eq. 'FJ') then
               do 180, j = 1, cpts + deg(1)
                  fjac(eqn(1)+k,var(1)+j) = dw(2,j,1) 
                  fjac(eqn(3)+k,var(1)+j) = -x(n-2)*dw(1,j,1)
                  fjac(eqn(2)+k,var(2)+j) = dw(2,j,2)
                  fjac(eqn(3)+k,var(2)+j) = x(n-1)*dw(1,j,2)
                  fjac(eqn(1)+k,var(3)+j) = sin(w(1,3))*dw(1,j,3)
                  fjac(eqn(2)+k,var(3)+j) = - cos(w(1,3))*dw(1,j,3)
                  fjac(eqn(3)+k,var(3)+j) = dw(2,j,3)  
  180          continue
               fjac(eqn(3)+k,n-2) = -w(1,1)
               fjac(eqn(3)+k,n-1) =  w(1,2)
               fjac(eqn(3)+k,n) = -one
            endif
  190    continue
  200 continue

c     Set up the continuity equations.

      do 280, i = 1, nint

         do 240, e = 1, s
            var(e) = (i - 1)*npi + (e - 1)*cpts + sumdeg(e-1)
            eqn(e) = s + (i - 1)*npi + s*cpts + sumdeg(e-1)
            do 230, m = 1, deg(e)
               w(m,e) = zero
               do 210, j = m, deg(e)
                  w(m,e) = w(m,e) + rhnfhk(1,0,j-m,j-m)*x(var(e)+j)
                  dw(m,j,e) = rhnfhk(1,0,j-m,j-m)
  210          continue
               do 220, j = 1, cpts
                  w(m,e) = w(m,e) + x(var(e)+deg(e)+j)*
     +                              rhnfhk(1,0,deg(e)+j-m,deg(e)-m+1)
                  dw(m,deg(e)+j,e) = rhnfhk(1,0,deg(e)+j-m,deg(e)-m+1)
  220          continue
  230       continue
  240    continue
         if (i .eq. nint) goto 290

         do 270, e = 1, s
            do 260, m = 1, deg(e)
               if (task .eq. 'F' .or. task .eq. 'FJ') 
     +            fvec(eqn(e)+m) = x(var(e)+npi+m) - w(m,e)
               if (task .eq. 'J' .or. task .eq. 'FJ') then
                  fjac(eqn(e)+m,var(e)+npi+m) = one
                  do 250, j = 1, cpts + deg(e)
                     fjac(eqn(e)+m,var(e)+j) = -dw(m,j,e)
  250             continue
               endif
  260       continue
  270    continue
  280 continue

  290 continue

      if (task .eq. 'F' .or. task .eq. 'FJ') then
         fvec(n-2) = w(1,1) - a
         fvec(n-1) = w(1,2) - b
         fvec(n) = w(1,3) - c
      endif
      if (task .eq. 'J' .or. task .eq. 'FJ') then
         var(1) = n - npi - 3
         var(2) = var(1) + 5
         var(3) = var(2) + 5
         do 300, j = 1, 5
            fjac(n-2,var(1)+j) = dw(1,j,1)
            fjac(n-1,var(2)+j) = dw(1,j,2)
            fjac(n,var(3)+j) = dw(1,j,3)
  300    continue
      endif      

      return

      end
