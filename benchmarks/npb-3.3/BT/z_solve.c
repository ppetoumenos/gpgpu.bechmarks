#include <libcecl.h>
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

#include "header.h"
#include "timers.h"

//---------------------------------------------------------------------
// Performs line solves in Z direction by first factoring
// the block-tridiagonal matrix into an upper triangular matrix,
// and then performing back substitution to solve for the unknow
// vectors of each line.
//
// Make sure we treat elements zero to cell_size in the direction
// of the sweep.
//---------------------------------------------------------------------
void z_solve() {
  cl_int ecode;

  if (timeron) timer_start(t_zsolve);

  if (Z_SOLVE_DIM == 3) {
    ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_z_solve1, Z_SOLVE_DIM, NULL,
                                 z_solve1_gws, z_solve1_lws, 0, NULL, NULL);
    clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");

    ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_z_solve2, Z_SOLVE_DIM, NULL,
                                 z_solve2_gws, z_solve2_lws, 0, NULL, NULL);
    clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");

    ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_z_solve3, Z_SOLVE_DIM, NULL,
                                 z_solve3_gws, z_solve3_lws, 0, NULL, NULL);
    clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");

    ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_z_solve, 2, NULL, z_solve_gws,
                                 z_solve_lws, 0, NULL, NULL);
    clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");
    CHECK_FINISH();
  } else {
    ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_z_solve, Z_SOLVE_DIM, NULL,
                                 z_solve_gws, z_solve_lws, 0, NULL, NULL);
    clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");
    CHECK_FINISH();
  }

  if (timeron) timer_stop(t_zsolve);
}
