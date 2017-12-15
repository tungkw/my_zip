- [TokRZ](#1)
    - [input](#1.1)
        - [read_nova](#1.1.1)
            - [interp1d3l](#1.1.1.1)
        - [gridpnt](#1.1.2)
        - [print_input](#1.1.2)
    - 分组
    - [initial](1.2)
        - [map_nova](1.2.1)
            - [qshep2](1.2.1.1)
                - [store2](1.2.1.1.1)
                - [getnp2](1.2.1.1.2)
                - [setup2](1.2.1.1.3)
                - [givens](1.2.1.1.4)
                - [rotate](1.2.1.1.5)
            - [qs2grd](1.2.1.2)
            - [interp1d3l](1.2.1.3)
            - [estimate_pst1](1.2.1.4)
            - [last_grid](1.2.1.5)
                - [interp1d3l](1.2.1.5.1)
        - [funmax](1.2.2)
        - [recrd_init](1.2.3)
        - [energy](1.2.4)
            - [integ](1.2.4.1)
            - [dfftw_plan_dft_r2c_1d](1.2.4.1)
            - [dfftw_execute_dft_r2c](1.2.4.1)
            - [dfftw_destroy_plan](1.2.4.1)
        - [perturbation](1.2.5)
            - [energy](1.2.5.1)
    - [init_dgn](1.3)
        - [interp1d3l](1.3.1)
    - [setdt](1.4)
    - [recrd](1.5)
    - [energy](1.6)
    - [stepon](1.7)
        - [right](1.7.1)
            - [bndry8_x_ex](1.7.1.1)
                - [bndry8_ex](1.7.1.1.1)
                    - [valb8_atlastgrid](1.7.1.1.1.1)
                        - [map_xz2st](1.7.1.1.1.1.1)
                        - [smth_st_nrk](1.7.1.1.1.1.2)
                        - [interp1d2l](1.7.1.1.1.1.3)
                        - [interp1d3l](1.7.1.1.1.1.4)
            - [convt](1.7.1.2)
            - [current](1.7.1.3)
                - [bndry3_ex](1.7.1.3.1)
            - [Efield](1.7.1.4)
        - [bndry8_x_ex](1.7.2)

<p id="1"></p>
# TokRZ
## input
### read_nova
#### interp1d3l
### gridpnt
### print_input
## 分组
## initial
### map_nova
#### qshep2
##### store2
##### getnp2
##### setup2
##### givens
##### rotate
#### qs2grd
#### interp1d3l
#### estimate_pst1
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
## stepon
### right
#### bndry8_x_ex
##### bndry8_ex
##### valb8_atlastgrid
##### map_xz2st
##### smth_st_nrk
##### interp1d2l
##### interp1d3l
#### convt
#### current
##### bndry3_ex
#### Efield
### bndry8_x_ex
