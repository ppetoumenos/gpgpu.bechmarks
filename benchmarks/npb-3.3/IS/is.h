//-------------------------------------------------------------------------//
//                                                                         //
//  This benchmark is an OpenCL version of the NPB IS code. This OpenCL    //
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

#ifndef __IS_H__
#define __IS_H__

#ifdef __OPENCL_VERSION__
#define CLASS_S 0
#define CLASS_W 1
#define CLASS_A 2
#define CLASS_B 3
#define CLASS_C 4
#define CLASS_D 5
#define CLASS_E 6
#else
#define CLASS_S 'S'
#define CLASS_W 'W'
#define CLASS_A 'A'
#define CLASS_B 'B'
#define CLASS_C 'C'
#define CLASS_D 'D'
#define CLASS_E 'E'
#endif

//#define USE_ORIGINAL_BUCKET

/******************/
/* default values */
/******************/
#ifndef CLASS
#define CLASS CLASS_S
#endif


/*************/
/*  CLASS S  */
/*************/
#if CLASS == CLASS_S
#define  TOTAL_KEYS_LOG_2    16
#define  MAX_KEY_LOG_2       11
#define  NUM_BUCKETS_LOG_2   9
#endif


/*************/
/*  CLASS W  */
/*************/
#if CLASS == CLASS_W
#define  TOTAL_KEYS_LOG_2    20
#define  MAX_KEY_LOG_2       16
#ifdef USE_ORIGINAL_BUCKET
#define  NUM_BUCKETS_LOG_2   10
#else
#define  NUM_BUCKETS_LOG_2   9
#endif
#endif

/*************/
/*  CLASS A  */
/*************/
#if CLASS == CLASS_A
#define  TOTAL_KEYS_LOG_2    23
#define  MAX_KEY_LOG_2       19
#ifdef USE_ORIGINAL_BUCKET
#define  NUM_BUCKETS_LOG_2   10
#else
#define  NUM_BUCKETS_LOG_2   9
#endif
#endif


/*************/
/*  CLASS B  */
/*************/
#if CLASS == CLASS_B
#define  TOTAL_KEYS_LOG_2    25
#define  MAX_KEY_LOG_2       21
#ifdef USE_ORIGINAL_BUCKET
#define  NUM_BUCKETS_LOG_2   10
#else
#define  NUM_BUCKETS_LOG_2   9
#endif
#endif


/*************/
/*  CLASS C  */
/*************/
#if CLASS == CLASS_C
#define  TOTAL_KEYS_LOG_2    27
#define  MAX_KEY_LOG_2       23
#ifdef USE_ORIGINAL_BUCKET
#define  NUM_BUCKETS_LOG_2   10
#else
#define  NUM_BUCKETS_LOG_2   9
#endif
#endif


/*************/
/*  CLASS D  */
/*************/
#if CLASS == CLASS_D
#define  TOTAL_KEYS_LOG_2    31
#define  MAX_KEY_LOG_2       27
#ifdef USE_ORIGINAL_BUCKET
#define  NUM_BUCKETS_LOG_2   10
#else
#define  NUM_BUCKETS_LOG_2   9
#endif
#endif


#if CLASS == CLASS_D
#define  TOTAL_KEYS          (1L << TOTAL_KEYS_LOG_2)
#else
#define  TOTAL_KEYS          (1 << TOTAL_KEYS_LOG_2)
#endif
#define  MAX_KEY             (1 << MAX_KEY_LOG_2)
#define  NUM_BUCKETS         (1 << NUM_BUCKETS_LOG_2)
#define  NUM_KEYS            TOTAL_KEYS
#define  SIZE_OF_BUFFERS     NUM_KEYS


#define  MAX_ITERATIONS      10
#define  TEST_ARRAY_SIZE     5


/*************************************/
/* Typedef: if necessary, change the */
/* size of int here by changing the  */
/* int type to, say, long            */
/*************************************/
#if CLASS == 'D'
typedef  long INT_TYPE;
#else
typedef  int  INT_TYPE;
#endif

#endif //__IS_H__
