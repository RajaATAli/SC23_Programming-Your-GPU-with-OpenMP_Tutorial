#include <stdio.h>
#include <omp.h>
#define N 10000000
#define TOL  0.0000001
//
//  This is a simple program to add two vectors
//  and verify the results.
//
//  History: Written by Tim Mattson, November 2017
//
int main()
{

    float a[N], b[N], c[N], res[N];
    int err=0;

    double init_time, compute_time, test_time;
    init_time    = -omp_get_wtime();

   // fill the arrays
   #pragma omp parallel for
   for (int i=0; i<N; i++){
      a[i] = (float)i;
      b[i] = 2.0*(float)i;
      c[i] = 0.0;
      res[i] = i + 2*i;
   }

   init_time    +=  omp_get_wtime();
   compute_time  = -omp_get_wtime();
   
   // add two vectors - this is the compute intensive part
   // Offload computation to GPU
   #pragma omp target
   #pragma omp loop
   for (int i=0; i<N; i++){
      c[i] = a[i] + b[i];
   }

   printf("There are %d devices \n", omp_get_num_devices());


   compute_time +=  omp_get_wtime();
   test_time     = -omp_get_wtime();

   // test results
   #pragma omp parallel for reduction(+:err)
   for(int i=0;i<N;i++){
      float val = c[i] - res[i];
      val = val*val;
      if(val>TOL) err++;
   }

   test_time    +=  omp_get_wtime();
   
   printf(" vectors added with %d errors\n",err);

   printf("Init time:    %.3fs\n", init_time);
   printf("Compute time: %.3fs\n", compute_time);
   printf("Test time:    %.3fs\n", test_time);
   printf("Total time:   %.3fs\n", init_time + compute_time + test_time);
   return 0;
}

// Threads
// Environment Variable
// OpenMP Common Core (Slides) -> Overview of 
// Directives, runtime library routines
// Pragma Open Threads
// OMP_GET_NUM_THREADS
// OMP_SET_NUM_THREADS
// GNU Compiler - OpenMP Compiler 
// Look at campus servers - threads will parallel
// What is concurrency -> muliple execuation agents that are unordered 
// **FAIR Scheduling** 
// GPU -> Not fairly scheduled (throughput vs latency)
// Reads vs Writes (Datarace) -> synchronization