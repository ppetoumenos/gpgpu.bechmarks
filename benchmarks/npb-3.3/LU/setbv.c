#include <libcecl.h>
//-------------------------------------------------------------------------//
//                                                                         //
//  This benchmark is an OpenCL version of the NPB LU code. This OpenCL    //
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

#include "applu.incl"

//---------------------------------------------------------------------
// set the boundary values of dependent variables
//---------------------------------------------------------------------
void setbv() {
  DTIMER_START(t_setbv);

  cl_int ecode;

  ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_setbv1, SETBV1_DIM, NULL,
                               setbv1_gws, setbv1_lws, 0, NULL, NULL);
  clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");

  ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_setbv2, SETBV2_DIM, NULL,
                               setbv2_gws, setbv2_lws, 0, NULL, NULL);
  clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");

  ecode = CECL_ND_RANGE_KERNEL(cmd_queue, k_setbv3, SETBV3_DIM, NULL,
                               setbv3_gws, setbv3_lws, 0, NULL, NULL);
  clu_CheckError(ecode, "CECL_ND_RANGE_KERNEL()");
  CHECK_FINISH();

  DTIMER_STOP(t_setbv);
}
