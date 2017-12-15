- [数据结构](#0)
- [TokRZ](#1)
    - [input](#1.1)
        - [read_nova](#1.1.1)
            - [interp1d3l](#1.1.1.1)
        - [gridpnt](#1.1.2)
        - [print_input](#1.1.2)
    - [initial](#1.2)
        - [map_nova](#1.2.1)
            - [qshep2](#1.2.1.1)
                - [store2](#1.2.1.1.1)
                - [getnp2](#1.2.1.1.2)
                - [setup2](#1.2.1.1.3)
                - [givens](#1.2.1.1.4)
                - [rotate](#1.2.1.1.5)
            - [qs2grd](#1.2.1.2)
            - [interp1d3l](#1.2.1.3)
            - [estimate_pst1](#1.2.1.4)
            - [last_grid](#1.2.1.5)
                - [interp1d3l](#1.2.1.5.1)
        - [funmax](#1.2.2)
        - [recrd_init](#1.2.3)
        - [energy](#1.2.4)
            - [integ](#1.2.4.1)
            - [dfftw_plan_dft_r2c_1d](#1.2.4.1)
            - [dfftw_execute_dft_r2c](#1.2.4.1)
            - [dfftw_destroy_plan](#1.2.4.1)
        - [perturbation](#1.2.5)
            - [energy](#1.2.5.1)
    - [init_dgn](#1.3)
        - [interp1d3l](#1.3.1)
    - [setdt](#1.4)
    - [recrd](#1.5)
    - [energy](#1.6)
    - [stepon](#1.7)
        - [right](#1.7.1)
            - [bndry8_x_ex](#1.7.1.1)
                - [bndry8_ex](#1.7.1.1.1)
                    - [valb8_atlastgrid](#1.7.1.1.1.1)
                        - [map_xz2st](#1.7.1.1.1.1.1)
                        - [smth_st_nrk](#1.7.1.1.1.1.2)
                        - [interp1d2l](#1.7.1.1.1.1.3)
                        - [interp1d3l](#1.7.1.1.1.1.4)
            - [convt](#1.7.1.2)
            - [current](#1.7.1.3)
                - [bndry3_ex](#1.7.1.3.1)
            - [Efield](#1.7.1.4)
        - [bndry8_x_ex](#1.7.2)

<p id="0"></p>
# 数据结构
## 进程相关
nrk !进程号  
nsize !进程数  
nsizexz !xz面的进程数

nprx = 8 !进程的x方向数量  
npry = 1 !进程的y方向数量  
nprz = 8 !进程的z方向数量  
nprxz = nprx * nprz!进程的xz方向数量  

nrkx() !每个进程所在的x方向的分组  
nrky() !每个进程所在的y方向的分组  
nrkz() !每个进程所在的z方向的分组  
nrkxz() !每个进程所在的xz方向的分组

## 坐标
### 柱坐标
(R,phi,Z)  
mxt=256,myt=32,mzt=256

- #### 全局坐标
xxt(mxt)  
yyt(-1:myt+2)  
zzt(mzt)  

- #### 局部坐标
mx=mxt/nprx+4, mxm=mxt/nprx, mxn=mx*nprx  
mz=mzt/nprz+4, mzm=mzt/nprz, mzn=mz*nprz  
my=myt/npry+4, mym=myt/npry, myn=my*npry  
xx(mx)  
yy(my)  
zz(mz)  

- #### 过渡数组
    - 柱坐标对应的环坐标theta  
局部：thxz(mx,mz): theta coordinates in (R,Z) grid;  
全局：tht(mxt,mzt)
    - 对应的环坐标psi  
局部：tpxz(mx,mz): R.Z.<->S(psi).P(pol). transit   angle in (R,Z);   
全局：tpt(mxt,mzt)
    - tcxz(mx,mz): tc=Ing(Jcb/R^2)dth  

### 环坐标
环坐标只用在切面的转换，所以只有二维  

(R, Z) => (theta, psi)  
nthe=101,npsi=111  

- #### 全局坐标
n2th=2*(nthe-1)  ! 乘2是为了一圈360度
xxst(n2th+5,npsi),zzst(n2th+5,npsi) in (theta,psi) grid;  

- #### 局部坐标
npsip=npsi+1  
ids=1  
nda=4*ids  
mps=npsip,mpsa=npsi,mps4=mpsa-nda  
xxs(n2th+5,mps4:mps),zzs(n2th+5,mps4:mps) in (theta,psi) bandary grid;  
        mps4 = 111 - 4  
        mps  = 111 + 1
        每个进程分配到所有的角度，以及6圈psi？  

## 网格
dxt(mxt)  
dzt(mzt)  
dyt(-1:myt+2)  
dy(my)  
dx(mx)  
dz(mz)  

## 变量定义
当前时间：time  
边界格式：lbnd = 30    
    0:fix;
    1:free;
    3:free v,fix b;
    10:fix vr,br;
    free others;
    20:fix p,vr,br, free others;

### 不知道用来干嘛的
- parameter
        nthe2=nthe+2,nthe3=nthe+3  
        ndat=(npsi-1) * (n2th-1)  
        ndat12 =(npsi-1) * (nthe+4)  
        ndat34=(npsi-1) * (nthe+4)  
        mbm=4*mxt+4*mzt  
        mr=4  
        mdgn=1  
        thst(n2th+5): theta coordinates in (theta,psi) grid;  
        tpst(n2th+5,npsi): R.Z.<->S(psi).P(pol). transit angle in (theta,psi);   tps(n2th+5,mps4:mps) for bndry  
        usually,  
           !th=arc/n2th  
           !tp=atan2(psdz,psdx)=atan2(bx,-bz) =>   cos(tp)=psdx/|grps|=-bz/bp;sin(tp)=psdz/|grps|=bx/bp   
        (xxt, tt)  
        !,nprbm=min(npr,2 * nprx+2 * nprz)  
        integer, parameter : myx=my*mx,myz=my*mz,mxz=mx*mz,myx8=myx*8,myz8=myz*8,mxz8=mxz*8,myx3=myx*3,myz3=myz*3,mxz3=mxz*3   
        integer, parameter   mbm_nrk=4*mx+4*mz,nvmax=min(mxt,mzt)/2  
        double precision, parameter : fac=1./myt,pi=dacos(-1.d0)   
        complex(kind=16), parameter : c0=cmplx(0,0),c1=cmplx(0,1.d0)  

- grid
        double precision, dimension(mxt,mzt) :: pst,pst_dx,pst_dz,rr2t,rrt,tht,tpt,tcht
        double precision, dimension(mx,mz)   :: psi,psi_dx,psi_dz,rr2,rr,thxz,tpxz,tcxz
        double precision, dimension(n2th+5):: thst
        double precision, dimension(n2th+5,npsi):: psst,xxst,zzst,tpst,tst,rst

        double precision aa,aa2,ab,psia,psiam,psia1,psmin,psmax
        double precision xzero,xmin,xmax,zmin,zmax,dxx,dyy,dzz,xmg,zmg

---
<p id="1"></p>
# TokRZ
- 初始化数值：
        nstep=0 !初始步长
        nstop=400000 !总步长
        nrcd=10000 !记录周期
        ndgn=10
        neng=10
        ncd=10
        nper=100
        nrank_dgn=nsize-1
        mtor_dgn=4

- 定义基本变量：
[input](#1.1)

- 进程处理

- 数据分配

- 计算差分格式的系数，定义一些差分格式的宏：
[initia](#1.2)

- [init_dgn](#1.3)

- 进入循环
    - 计算时间dt：
    [setdt](#1.4)

    - 周期性记录数据：
    [recrd](#1.5)

    - ？：
    [energy](#1.6)

    - 计算：
    [stepon](#1.7)

<p id="1.1"></p>
## input
    subroutine input
    ! --------------------------
    !  This routine inputs parameters and define basic variables
    !   LRSTRT: =.f., starting from t=0; =.t., continueing from
    !           steps as given bz NST.
    !   NEND:   the final steps intended for the current run ,
    !           including the steps from the previous run.
    !   NSP:    time step interval for diagnostic plots.
    !   NSMTHX:  the number of rows starting from incoming
    !           boundary for the smoothing region in r-direction.
    !   ETA:    exact inverse of magnetic Renolds number.
    !   GAMMA:  adiabatic constant.
    !   BETA:   ratio of kinetic to magnetic pressure
    !   NCASE:  case number of the run.
    !   NST:    beginning data file number for the current run.
    !   NINT:   data file number increment.
    ! --------------------------

读入数据：
[read_nova](#1.1.1)

初始化网格：
[gridpnt](#1.1.2)

输出状态：
[print_input](#1.1.3)

<p id="1.1.1"></p>
### read_nova
读数据


<p id="1.1.1.1"></p>
#### interp1d3l

    subroutine interp1d3l(x1,x2,x3,x4,y1,y2,y3,y4,y,ans)
        double precision x1,x2,x3,x4,y1,y2,y3,y4,y,ans
    !cray  lcm(cubic)
    !c
    !c      this routine returns interpolated value
    !c
        d1 = (y1-y2)*(y1-y3)*(y1-y4)
        d2 = (y2-y1)*(y2-y3)*(y2-y4)
        d3 = (y3-y1)*(y3-y2)*(y3-y4)
        d4 = (y4-y1)*(y4-y2)*(y4-y3)
        ans = x1*(y-y2)*(y-y3)*(y-y4)/d1 &
            + x2*(y-y1)*(y-y3)*(y-y4)/d2 &
            + x3*(y-y1)*(y-y2)*(y-y4)/d3 &
            + x4*(y-y1)*(y-y2)*(y-y3)/d4
        return
    end

<p id="1.1.2"></p>
### gridpnt
初始化网格
<p id="1.1.3"></p>
### print_input
<p id="1.2"></p>
## initial
    ! ============ 计算差分格式的系数，定义一些差分格式的宏，map_nova =============

    !----------------
    ! Defines coordinates system and specifies initial configuration.
    ! Normlization convention:
    !   1. Density --- normalised to asymtotic value, i.e., rho=1
    !   2. Magnetic field --- normalised to asymtotic value, i.e.,
    !                         b0=1.
    !   3. Velocity --- normalised to asymtotic Alfven speed, VA=1, a
    !                   natural result of 1. and 2.
    !   4. Length --- normalised to a.
    !   5. Time --- normalised to a/VA.
    !---------------

<p id="1.2.1"></p>
### map_nova
#### qshep2
##### store2
##### getnp2
##### setup2
##### givens
##### rotate
<p id="1.2.2"></p>
#### qs2grd
<p id="1.2.3"></p>
#### interp1d3l
<p id="1.2.4"></p>
#### estimate_pst1
<p id="1.2.5"></p>
#### last_grid
##### interp1d3l
### funmax
### recrd_init
### energy
#### integ
#### dfftw_plan_dft_r2c_1d
#### dfftw_execute_dft_r2c
#### dfftw_destroy_plan
### perturbation
#### energy
## init_dgn
### interp1d3l
## setdt
## recrd
## energy
<p id="1.7"></p>
## stepon
    !     This routine time-advances X's bz fourth order in time and second
    !     order in space Runge-Kotta differential scheme.
    !     note: X is alwazs the up-to-date value while Xm being the
    !           intermediate value, and Xdif is increment

ms=1
me=8
ml=1
tt=time
tt1=time+dt/6.
tt2=time
irk=1
- 定义局部函数：
        !  define statement functions
        !  d2fc= d2 f / dx2 with third-order accuracy central difference
        !      d2fc(fm2,fm1,f0,fp1,fp2,a,b,c,d)= &
        !       a*(fp1-f0)+b*(f0-fm1)+c*(fp2-f0)+d*(f0-fm2)
        !  d1fc= d f / dx  with fourth-order accuracy central difference
        d1fc(fm2,fm1,f0,fp1,fp2,a,b,c,d)= &
            a*(fp1-f0)+b*(f0-fm1)+c*(fp2-f0)+d*(f0-fm2)
        !  d1f2= d f / dx  with second-order accuracy central difference
        d1f2(fm1,f0,fp1,xm1,x0,xp1)= &
            ((xm1-x0)/(xp1-x0)*(fp1-f0) &
            -(xp1-x0)/(xm1-x0)*(fm1-f0))/(xm1-xp1)
        !  d1fm= d f / dx  with  one-sided difference involving -2 -1 and 0
        !  points
        d1fm(fm2,fm1,f0,xm2,xm1,x0)= &
            ( (xm2-x0)/(xm1-x0)*(fm1-f0) &
            -(xm1-x0)/(xm2-x0)*(fm2-f0) ) / (xm2-xm1)
        !  d1fbp= d f / dx  with  one-sided-bias  difference involving -1 0  1 and 2
        !  points
        d1fbp(fp2,fp1,f0,fm1,a,b,c)= &
            a*(fm1-f0)+b*(fp1-f0)+c*(fp2-f0)
        !  d1fbm= d f / dx  with  one-sided-bias  difference involving -2 -1  0 and 1
        !  points
        d1fbm(fm2,fm1,f0,fp1,a,b,c)= &
            a*(f0-fp1)+b*(f0-fm1)+c*(f0-fm2)
        !  d1xf2= d Rf / dx  with second-order accuracy central difference
        d1xf2(fm1,f0,fp1,xm1,x0,xp1)= &
            ((xm1-x0)/(xp1-x0)*(xp1*fp1-x0*f0) &
            -(xp1-x0)/(xm1-x0)*(xm1*fm1-x0*f0))/(xm1-xp1)
- 计算右端项：
[right](#1.7.1)

xfold(:,:,:,:)=x(:,:,:,:)
xm(:,:,:,:)=xfold(:,:,:,:)+xdif(:,:,:,:)*dt/6.
x(:,:,:,:)=xfold(:,:,:,:)+xdif(:,:,:,:)*dt/2.

tt=time+dt/2.
tt1=time+dt/2.
tt2=time+dt/6.
irk=2
call right
xm(:,:,:,:)=xm(:,:,:,:)+xdif(:,:,:,:)*dt/3.
x(:,:,:,:)=xfold(:,:,:,:)+xdif(:,:,:,:)*dt/2.

tt1=time+5.*dt/6.
tt2=time+dt/2.
irk=3
call right
xm(:,:,:,:)=xm(:,:,:,:)+xdif(:,:,:,:)*dt/3.
x(:,:,:,:)=xfold(:,:,:,:)+xdif(:,:,:,:)*dt

time=time+dt
tt1=time+dt
tt2=time+5.*dt/6.
irk=4
call right
x(:,:,:,:)=xm(:,:,:,:)+xdif(:,:,:,:)*dt/6.

caf=0.75d0*(0.5+0.5*dtanh((time-40)/5.))
call bndry8_x_ex(lbnd) !lbnd=30
! if(eta_from_t) call calculate_eta

<p id="1.7.1"></p>
### right
<p id="1.7.1.1"></p>
#### bndry8_x_ex
<p id="1.7.1.1.1"></p>
##### bndry8_ex
<p id="1.7.1.1.2"></p>
##### valb8_atlastgrid
<p id="1.7.1.1.3"></p>
##### map_xz2st
<p id="1.7.1.1.4"></p>
##### smth_st_nrk
<p id="1.7.1.1.5"></p>
##### interp1d2l
<p id="1.7.1.1.6"></p>
##### interp1d3l
<p id="1.7.1.2"></p>
#### convt
<p id="1.7.1.3"></p>
#### current
<p id="1.7.1.3.1"></p>
##### bndry3_ex
<p id="1.7.1.4"></p>
#### Efield
<p id="1.7.2"></p>
### bndry8_x_ex
