#include <libcecl.h>
/***************************************************************************
 *cr
 *cr            (C) Copyright 2010 The Board of Trustees of the
 *cr                        University of Illinois
 *cr                         All Rights Reserved
 *cr
 ***************************************************************************/

/* 
 * Main entry of dense matrix-matrix multiplication kernel
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <malloc.h>
#include <vector>
#include <iostream>
#include <CL/cl.h>
#include <parboil.h>

// I/O routines
extern bool readColMajorMatrixFile(const char *fn, int &nr_row, int &nr_col, std::vector<float>&v);
extern bool writeColMajorMatrixFile(const char *fn, int, int, std::vector<float>&);
extern char* readFile(const char*);

// Parameters of tile sizes
#define TILE_SZ 16

#define CHECK_ERROR(errorMessage)           \
  if(clStatus != CL_SUCCESS)                \
  {                                         \
     std::cout<<errorMessage<<" Error!\n";  \
     std::cout<<"Line: "<<__LINE__<<"\n";   \
     exit(1);                               \
  }

void basicSgemm( char transa, char transb, int m, int n, int k, float alpha, cl_mem A, int lda, cl_mem B, int ldb, float beta, cl_mem C, int ldc, cl_kernel clKernel, cl_command_queue clCommandQueue )
{
  if ((transa != 'N') && (transa != 'n')) {
    std::cerr << "unsupported value of 'transa' in regtileSgemm()" << std::endl;
    return;
  }
  
  if ((transb != 'T') && (transb != 't')) {
    std::cerr << "unsupported value of 'transb' in regtileSgemm()" << std::endl;
    return;
  }
  
  // In this code we assume the matrix sizes are multiple of tile size
  if ((m%TILE_SZ) || (n%TILE_SZ)) {
    std::cerr << "unsupported size of matrix. m should be multiple of " << TILE_SZ
      << "; n should be multiple of " << TILE_SZ << std::endl;
  }

  size_t db[2] = {TILE_SZ,TILE_SZ};
  size_t dg[2] = {m/TILE_SZ*db[0],n/TILE_SZ*db[1]};

  cl_int clStatus;
 
  clStatus = CECL_SET_KERNEL_ARG(clKernel,0,sizeof(cl_mem),(void*)&A);
  clStatus = CECL_SET_KERNEL_ARG(clKernel,1,sizeof(int),(void*)&lda);
  clStatus = CECL_SET_KERNEL_ARG(clKernel,2,sizeof(cl_mem),(void*)&B);
  clStatus = CECL_SET_KERNEL_ARG(clKernel,3,sizeof(int),(void*)&ldb);
  clStatus = CECL_SET_KERNEL_ARG(clKernel,4,sizeof(cl_mem),(void*)&C);
  clStatus = CECL_SET_KERNEL_ARG(clKernel,5,sizeof(int),(void*)&ldc);
  clStatus = CECL_SET_KERNEL_ARG(clKernel,6,sizeof(int),(void*)&k);
  clStatus = CECL_SET_KERNEL_ARG(clKernel,7,sizeof(float),(void*)&alpha);
  clStatus = CECL_SET_KERNEL_ARG(clKernel,8,sizeof(float),(void*)&beta);
  CHECK_ERROR("CECL_SET_KERNEL_ARG")

  clStatus = CECL_ND_RANGE_KERNEL(clCommandQueue,clKernel,2,NULL,dg,db,0,NULL,NULL);
  CHECK_ERROR("CECL_ND_RANGE_KERNEL")

  clStatus = clFinish(clCommandQueue); 
  CHECK_ERROR("clFinish")
}

main (int argc, char *argv[]) {

  struct pb_Parameters *params;
  struct pb_TimerSet timers;

  size_t A_sz, B_sz, C_sz;
  int matArow, matAcol;
  int matBrow, matBcol;
  std::vector<float> matA, matBT;

  pb_InitializeTimerSet(&timers);

  /* Read command line. Expect 3 inputs: A, B and B^T 
     in column-major layout*/
  params = pb_ReadParameters(&argc, argv);
  if ((params->inpFiles[0] == NULL) 
      || (params->inpFiles[1] == NULL)
      || (params->inpFiles[2] == NULL)
      || (params->inpFiles[3] != NULL))
    {
      fprintf(stderr, "Expecting three input filenames\n");
      exit(-1);
    }

  cl_int clStatus;
  cl_platform_id clPlatform;
  clStatus = clGetPlatformIDs(1,&clPlatform,NULL);
  CHECK_ERROR("clGetPlatformIDs")

  cl_context_properties clCps[3] = {CL_CONTEXT_PLATFORM,(cl_context_properties)clPlatform,0};
  cl_context clContext = CECL_CREATE_CONTEXT_FROM_TYPE(clCps,CL_DEVICE_TYPE_GPU,NULL,NULL,&clStatus);
  CHECK_ERROR("CECL_CREATE_CONTEXT_FROM_TYPE")
   
  cl_device_id clDevice;
  clStatus = clGetDeviceIDs(clPlatform,CL_DEVICE_TYPE_GPU,1,&clDevice,NULL);
  CHECK_ERROR("clGetDeviceIDs")

  cl_command_queue clCommandQueue = CECL_CREATE_COMMAND_QUEUE(clContext,clDevice,CL_QUEUE_PROFILING_ENABLE,&clStatus);
  CHECK_ERROR("CECL_CREATE_COMMAND_QUEUE")

  pb_SetOpenCL(&clContext, &clCommandQueue);

  const char* clSource[] = {readFile("src/opencl_base/kernel.cl")};
  cl_program clProgram = CECL_PROGRAM_WITH_SOURCE(clContext,1,clSource,NULL,&clStatus);
  CHECK_ERROR("CECL_PROGRAM_WITH_SOURCE")

  char clOptions[50];
  sprintf(clOptions,"");

  clStatus = CECL_PROGRAM(clProgram,1,&clDevice,clOptions,NULL,NULL);
  CHECK_ERROR("CECL_PROGRAM")

  cl_kernel clKernel = CECL_KERNEL(clProgram,"mysgemmNT",&clStatus);
  CHECK_ERROR("CECL_KERNEL")

  /* Read in data */
  pb_SwitchToTimer(&timers, pb_TimerID_IO);

  // load A
  readColMajorMatrixFile(params->inpFiles[0],
      matArow, matAcol, matA);
  // copy A to device memory
  A_sz = matArow*matAcol*sizeof(float);

  // load B^T
  readColMajorMatrixFile(params->inpFiles[2],
      matBcol, matBrow, matBT);

  pb_SwitchToTimer( &timers, pb_TimerID_COMPUTE );
  B_sz = matBrow*matBcol*sizeof(float);

  // allocate space for C
  C_sz = matArow*matBcol*sizeof(float);

  // OpenCL memory allocation
  std::vector<float> matC(matArow*matBcol);
  cl_mem dA = CECL_BUFFER(clContext,CL_MEM_READ_ONLY,A_sz,NULL,&clStatus);
  CHECK_ERROR("CECL_BUFFER")
  cl_mem dB = CECL_BUFFER(clContext,CL_MEM_READ_ONLY,B_sz,NULL,&clStatus);
  CHECK_ERROR("CECL_BUFFER")
  cl_mem dC = CECL_BUFFER(clContext,CL_MEM_WRITE_ONLY,C_sz,NULL,&clStatus);
  CHECK_ERROR("CECL_BUFFER")

  // Copy A and B^T into device memory
  pb_SwitchToTimer( &timers, pb_TimerID_COPY );
  clStatus = CECL_WRITE_BUFFER(clCommandQueue,dA,CL_FALSE,0,A_sz,&matA.front(),0,NULL,NULL);
  CHECK_ERROR("CECL_WRITE_BUFFER")
  clStatus = CECL_WRITE_BUFFER(clCommandQueue,dB,CL_FALSE,0,B_sz,&matBT.front(),0,NULL,NULL);
  CHECK_ERROR("CECL_WRITE_BUFFER")

  for(int i=0;i<matC.size();i++)
	matC[i] = 0.0f;

  clStatus = CECL_WRITE_BUFFER(clCommandQueue,dC,CL_TRUE,0,C_sz,&matC.front(),0,NULL,NULL);
  CHECK_ERROR("CECL_WRITE_BUFFER")

  pb_SwitchToTimer( &timers, pb_TimerID_KERNEL );

  // Use standard sgemm interface
  basicSgemm('N', 'T', matArow, matBcol, matAcol, 1.0f, \
      dA, matArow, dB, matBcol, 0.0f, dC, matArow, clKernel, clCommandQueue);

  if (params->outFile) {
    pb_SwitchToTimer( &timers, pb_TimerID_COPY );
    CECL_READ_BUFFER(clCommandQueue,dC,CL_TRUE,0,C_sz,&matC.front(),0,NULL,NULL);
   
    /* Write C to file */
    pb_SwitchToTimer(&timers, pb_TimerID_IO);
    writeColMajorMatrixFile(params->outFile,
	matArow, matBcol, matC); 
  }

  pb_SwitchToTimer(&timers, pb_TimerID_NONE);

  double GPUtime = pb_GetElapsedTime(&(timers.timers[pb_TimerID_KERNEL]));
  std::cout<< "GFLOPs = " << 2.* matArow * matBcol * matAcol/GPUtime/1e9 << std::endl;
  pb_PrintTimerSet(&timers);
  pb_FreeParameters(params);

  free((void*)clSource[0]);

  clStatus = clReleaseKernel(clKernel);
  clStatus = clReleaseProgram(clProgram);
  clStatus = clReleaseMemObject(dA);
  clStatus = clReleaseMemObject(dB);
  clStatus = clReleaseMemObject(dC);
  clStatus = clReleaseCommandQueue(clCommandQueue);
  clStatus = clReleaseContext(clContext); 
  
  return 0;
}
