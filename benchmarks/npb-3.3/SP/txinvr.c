#include <libcecl.h>
//-------------------------------------------------------------------------//
//                                                                         //
//  This benchmark is an OpenCL version of the NPB SP code. This OpenCL    //
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

//---------------------------------------------------------------------
// block-diagonal matrix-vector multiplication
//---------------------------------------------------------------------
void txinvr() {
  cl_int ecode;

  if (timeron) timer_start(t_txinvr);

  ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_txinvr, TXINVR_DIM, NULL,
                               txinvr_gws, txinvr_lws, 0, NULL, NULL);
  clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");
  CHECK_FINISH();

  if (timeron) timer_stop(t_txinvr);
}
