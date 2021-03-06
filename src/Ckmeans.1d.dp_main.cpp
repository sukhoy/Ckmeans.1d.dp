/* Ckmeans_1d_dp_main.cpp --- wrapper function for "kmeans_1d_dp()"
 *
 * Copyright (C) 2010-2016 Mingzhou Song and Haizhou Wang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 Created: Oct 10, 2010

 Haizhou Wang
 Computer Science Department
 New Mexico State University
 hwang@cs.nmsu.edu

 Modified:
 March 20, 2014. Joe Song. Removed parameter int *k from the function.
 Added "int *Ks" and "int *nK" to provide a range of the number of clusters
 to search for. Made other changes.
 March 29, 2014. Haizhou Wang. Replaced "int *Ks" and "int *nK" by
 "int *minK" and "int *maxK".
 May 29, 2017. Joe Song. Change size from integer to double.
 */

#include <string>
#include <iostream>
#include <R_ext/Rdynload.h>

#include "Ckmeans.1d.dp.h"

/*Wrapper function to call kmeans_1d_dp()*/
extern "C" {
  /*
   x: An array containing input data to be clustered.
   length: length of the one dimensional array.
   minK: Minimum number of clusters.
   maxK: Maximum number of clusters.
   cluster:  An array of cluster IDs for each point in x.
   centers:  An array of centers for each cluster.
   withinss: An array of within-cluster sum of squares for each cluster.
   size:     An array of (weighted) sizes of each cluster.
   */

  void Ckmeans_1d_dp(double *x, int* length, double *y, int * ylength,
                     int* minK, int *maxK, int* cluster,
                     double* centers, double* withinss, double * size, // int* size,
                     double* BICs,
                     char ** estimate_k, char ** method)
  {
    // std::cout << method[0] << std::endl;

    // Call C++ version one-dimensional clustering algorithm*/
    if(*ylength != *length) { y = 0; }

    kmeans_1d_dp(x, (size_t)*length, y, (size_t)(*minK), (size_t)(*maxK),
                 cluster, centers, withinss, size, BICs,
                 std::string(estimate_k[0]), std::string(method[0]),
                 L2);

    // Change the cluster numbering from 0-based to 1-based
    for(size_t i=0; i< *length; ++i) {
      cluster[i] ++;
    }
  }

  void Ckmedian_1d_dp(double *x, int* length, double *y, int * ylength,
                      int* minK, int *maxK, int* cluster,
                      double* centers, double* withinss, double * size, // int* size,
                      double* BICs,
                      char ** estimate_k, char ** method)
  {
    // std::cout << method[0] << std::endl;

    // Call C++ version one-dimensional clustering algorithm*/
    if(*ylength != *length) { y = 0; }

    kmeans_1d_dp(x, (size_t)*length, y,
                 (size_t)(*minK), (size_t)(*maxK),
                 cluster, centers, withinss, size, BICs,
                 std::string(estimate_k[0]), std::string(method[0]),
                 L1);

    // Change the cluster numbering from 0-based to 1-based
    for(size_t i=0; i< *length; ++i) {
      cluster[i] ++;
    }
  }

  void Cksegs_1d_dp(double *x, int* length, double *y, int * ylength,
                    int* minK, int *maxK, int* cluster,
                    double* centers, double* withinss, double * size, // int* size,
                    double* BICs,
                    char ** estimate_k, char ** method)
  {
    // std::cout << method[0] << std::endl;

    // Call C++ version one-dimensional clustering algorithm*/
    if(*ylength != *length) { y = 0; }

    kmeans_1d_dp(x, (size_t)*length, y, (size_t)(*minK), (size_t)(*maxK),
                 cluster, centers, withinss, size, BICs,
                 std::string(estimate_k[0]), std::string(method[0]),
                 L2Y);

    // Change the cluster numbering from 0-based to 1-based
    for(size_t i=0; i< *length; ++i) {
      cluster[i] ++;
    }
  }

} // End of extern "C"


static const
  R_CMethodDef cMethods[] = {
    {"Ckmeans_1d_dp",  (DL_FUNC) & Ckmeans_1d_dp, 13},
    {"Ckmeans_1d_dp",  (DL_FUNC) & Ckmedian_1d_dp, 13},
    {"Ckmeans_1d_dp",  (DL_FUNC) & Cksegs_1d_dp, 13},
    {NULL}
  };

static const
  R_CallMethodDef callMethods[] = {
    {"Ckmeans_1d_dp", (DL_FUNC) & Ckmeans_1d_dp, 13},
    {"Ckmeans_1d_dp", (DL_FUNC) & Ckmedian_1d_dp, 13},
    {"Ckmeans_1d_dp", (DL_FUNC) & Cksegs_1d_dp, 13},
    {NULL} };

void R_init_Ckmeans_1d_dp(DllInfo *info)
{
  /* Register the .C and .Call routines.
   No .Fortran() or .External() routines,
   so pass those arrays as NULL.
   */
  R_registerRoutines(info,
                     cMethods, callMethods,
                     NULL, NULL);

  R_useDynamicSymbols(info, TRUE);
}
