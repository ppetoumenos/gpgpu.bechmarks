//-------------------------------------------------------------------------//
//                                                                         //
//  This benchmark is an OpenCL version of the NPB BT code. This OpenCL    //
//  version is developed by the Center for Manycore Programming at Seoul   //
//  National University and derived from the OpenMP Fortran versions in    //
//  "NPB3.3-OMP" developed by NAS.                                         //
//                                                                         //
//  Permission to use, copy, distribute and modify this software for any   //
//  purpose with or without fee is hereby granted. This software is        //
//  provided "as is" without express or implied warranty.                  //
//                                                                         //
//  Information on NPB 3.3, including the technical report, the original   //
//  specifications, source code, results and information on how to submit  //
//  new results, is available at:                                          //
//                                                                         //
//           http://www.nas.nasa.gov/Software/NPB/                         //
//                                                                         //
//  Send comments or suggestions for this OpenCL version to                //
//  cmp@aces.snu.ac.kr                                                     //
//                                                                         //
//          Center for Manycore Programming                                //
//          School of Computer Science and Engineering                     //
//          Seoul National University                                      //
//          Seoul 151-744, Korea                                           //
//                                                                         //
//          E-mail:  cmp@aces.snu.ac.kr                                    //
//                                                                         //
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
// Authors: Sangmin Seo, Gangwon Jo, Jungwon Kim, Jun Lee, Jeongho Nah,    //
//          and Jaejin Lee                                                 //
//-------------------------------------------------------------------------//

#ifndef __BT_H__
#define __BT_H__


#define CLASS_S 0
#define CLASS_W 1
#define CLASS_A 2
#define CLASS_B 3
#define CLASS_C 4
#define CLASS_D 5
#define CLASS_E 6

#if CLASS == CLASS_S
#define PROBLEM_SIZE   12
#define NITER_DEFAULT  60
#define DT_DEFAULT     0.010
#endif

#if CLASS == CLASS_W
#define PROBLEM_SIZE   24
#define NITER_DEFAULT  200
#define DT_DEFAULT     0.0008
#endif

#if CLASS == CLASS_A
#define PROBLEM_SIZE   64
#define NITER_DEFAULT  200
#define DT_DEFAULT     0.0008
#endif

#if CLASS == CLASS_B
#define PROBLEM_SIZE   102
#define NITER_DEFAULT  200
#define DT_DEFAULT     0.0003
#endif

#if CLASS == CLASS_C
#define PROBLEM_SIZE   162
#define NITER_DEFAULT  200
#define DT_DEFAULT     0.0001
#endif

#if CLASS == CLASS_D
#define PROBLEM_SIZE   408
#define NITER_DEFAULT  250
#define DT_DEFAULT     0.00002
#endif

#if CLASS == CLASS_E
#define PROBLEM_SIZE   1020
#define NITER_DEFAULT  250
#define DT_DEFAULT     0.4e-5
#endif

#ifndef PROBLEM_SIZE
#error "Unknown CLASS!"
#endif

#define AA            0
#define BB            1
#define CC            2
#define BLOCK_SIZE    5

#define IMAX      PROBLEM_SIZE
#define JMAX      PROBLEM_SIZE
#define KMAX      PROBLEM_SIZE
#define IMAXP     IMAX/2*2
#define JMAXP     JMAX/2*2

#define max(x,y)    ((x) > (y) ? (x) : (y))

//--------------------------------------------------------------------------
// from set_constants()
//--------------------------------------------------------------------------
#define c1        1.4
#define c2        0.4
#define c3        0.1
#define c4        1.0
#define c5        1.4

#define dnxm1     (1.0 / (double)(PROBLEM_SIZE-1))
#define dnym1     (1.0 / (double)(PROBLEM_SIZE-1))
#define dnzm1     (1.0 / (double)(PROBLEM_SIZE-1))

#define c1c2      (c1 * c2)
#define c1c5      (c1 * c5)
#define c3c4      (c3 * c4)
#define c1345     (c1c5 * c3c4)

#define conz1     (1.0-c1c5)

#define tx1       (1.0 / (dnxm1 * dnxm1))
#define tx2       (1.0 / (2.0 * dnxm1))
#define tx3       (1.0 / dnxm1)

#define ty1       (1.0 / (dnym1 * dnym1))
#define ty2       (1.0 / (2.0 * dnym1))
#define ty3       (1.0 / dnym1)

#define tz1       (1.0 / (dnzm1 * dnzm1))
#define tz2       (1.0 / (2.0 * dnzm1))
#define tz3       (1.0 / dnzm1)

#define dx1       0.75
#define dx2       0.75
#define dx3       0.75
#define dx4       0.75
#define dx5       0.75

#define dy1       0.75
#define dy2       0.75
#define dy3       0.75
#define dy4       0.75
#define dy5       0.75

#define dz1       1.0
#define dz2       1.0
#define dz3       1.0
#define dz4       1.0
#define dz5       1.0

#define dxmax     max(dx3, dx4)
#define dymax     max(dy2, dy4)
#define dzmax     max(dz2, dz3)

#define dssp      (0.25 * max(dx1, max(dy1, dz1) ))

#define c4dssp    (4.0 * dssp)
#define c5dssp    (5.0 * dssp)

#define dt        DT_DEFAULT
#define dttx1     (dt*tx1)
#define dttx2     (dt*tx2)
#define dtty1     (dt*ty1)
#define dtty2     (dt*ty2)
#define dttz1     (dt*tz1)
#define dttz2     (dt*tz2)

#define c2dttx1   (2.0*dttx1)
#define c2dtty1   (2.0*dtty1)
#define c2dttz1   (2.0*dttz1)

#define dtdssp    (dt*dssp)

#define comz1     dtdssp
#define comz4     (4.0*dtdssp)
#define comz5     (5.0*dtdssp)
#define comz6     (6.0*dtdssp)

#define c3c4tx3   (c3c4*tx3)
#define c3c4ty3   (c3c4*ty3)
#define c3c4tz3   (c3c4*tz3)

#define dx1tx1    (dx1*tx1)
#define dx2tx1    (dx2*tx1)
#define dx3tx1    (dx3*tx1)
#define dx4tx1    (dx4*tx1)
#define dx5tx1    (dx5*tx1)

#define dy1ty1    (dy1*ty1)
#define dy2ty1    (dy2*ty1)
#define dy3ty1    (dy3*ty1)
#define dy4ty1    (dy4*ty1)
#define dy5ty1    (dy5*ty1)

#define dz1tz1    (dz1*tz1)
#define dz2tz1    (dz2*tz1)
#define dz3tz1    (dz3*tz1)
#define dz4tz1    (dz4*tz1)
#define dz5tz1    (dz5*tz1)

#define c2iv      2.5
#define con43     (4.0/3.0)
#define con16     (1.0/6.0)

#define xxcon1    (c3c4tx3*con43*tx3)
#define xxcon2    (c3c4tx3*tx3)
#define xxcon3    (c3c4tx3*conz1*tx3)
#define xxcon4    (c3c4tx3*con16*tx3)
#define xxcon5    (c3c4tx3*c1c5*tx3)

#define yycon1    (c3c4ty3*con43*ty3)
#define yycon2    (c3c4ty3*ty3)
#define yycon3    (c3c4ty3*conz1*ty3)
#define yycon4    (c3c4ty3*con16*ty3)
#define yycon5    (c3c4ty3*c1c5*ty3)

#define zzcon1    (c3c4tz3*con43*tz3)
#define zzcon2    (c3c4tz3*tz3)
#define zzcon3    (c3c4tz3*conz1*tz3)
#define zzcon4    (c3c4tz3*con16*tz3)
#define zzcon5    (c3c4tz3*c1c5*tz3)
//--------------------------------------------------------------------------


#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64: enable
#else
#ifdef cl_amd_fp64
#pragma OPENCL EXTENSION cl_amd_fp64: enable
#endif
#endif

#ifdef cl_amd_printf
#pragma OPENCL EXTENSION cl_amd_printf: enable
#endif

#include "bt_dim.h"

/* Below constants should be the same as those in bt.c!! */
#ifdef USE_CPU
#define EXACT_RHS1_DIM      EXACT_RHS1_DIM_CPU
#define EXACT_RHS5_DIM      EXACT_RHS5_DIM_CPU
#define INITIALIZE2_DIM     INITIALIZE2_DIM_CPU
#define COMPUTE_RHS1_DIM    COMPUTE_RHS1_DIM_CPU
#define COMPUTE_RHS2_DIM    COMPUTE_RHS2_DIM_CPU
#define COMPUTE_RHS6_DIM    COMPUTE_RHS6_DIM_CPU
#define X_SOLVE_DIM         X_SOLVE_DIM_CPU
#define Y_SOLVE_DIM         Y_SOLVE_DIM_CPU
#define Z_SOLVE_DIM         Z_SOLVE_DIM_CPU
#define ADD_DIM             ADD_DIM_CPU

#else //GPU
#define EXACT_RHS1_DIM      EXACT_RHS1_DIM_GPU
#define EXACT_RHS5_DIM      EXACT_RHS5_DIM_GPU
#define INITIALIZE2_DIM     INITIALIZE2_DIM_GPU
#define COMPUTE_RHS1_DIM    COMPUTE_RHS1_DIM_GPU
#define COMPUTE_RHS2_DIM    COMPUTE_RHS2_DIM_GPU
#define COMPUTE_RHS6_DIM    COMPUTE_RHS6_DIM_GPU
#define X_SOLVE_DIM         X_SOLVE_DIM_GPU
#define Y_SOLVE_DIM         Y_SOLVE_DIM_GPU
#define Z_SOLVE_DIM         Z_SOLVE_DIM_GPU
#define ADD_DIM             ADD_DIM_GPU

#endif

#endif //__BT_H__
