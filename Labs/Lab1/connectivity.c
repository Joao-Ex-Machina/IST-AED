/******************************************************************************
 * (c) 2010-2019 AED Team
 * Last modified: abl 2019-02-22
 * (mod) 2021-2022 Joao Barreiros C. Rodrigues nº99968
 * 		   Henrique Ramos Delfino      nº99957
 * 		   IST-LEEC
 * Last modified: 2021-10-03
 *
 * NAME
 *   connectivity.c
 *
 * DESCRIPTION
 *   Algorithms for solving the connectivity problem -  QF QU WQU CWQU
 *   For each method count number of entry pairs and the number of links
 *
 * COMMENTS
 *   Code for public distribution
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>

#include "connectivity.h"

#define DEBUG 0

/******************************************************************************
 * quick_find()
 *
 * Arguments: id - array with connectivity information
 *            N - size of array
 *            fp - file pointer to read data from
 *            quietOut - to reduce output to just final count
 * Returns: (void)
 * Side-Effects: pairs of elements are read and the connectivity array is
 *               modified
 *
 * Description: Quick Find algorithm
 *****************************************************************************/

void quick_find(int *id, int N, FILE * fp, int quietOut)
{
   int i, p, q, t;
   int pairs_cnt = 0;            /* connection pairs counter */
   int links_cnt = 0;            /* number of links counter */
   int qf_unions = 0;
   int qf_search = 0;
   int print_ctrl = 0;
   int Group_flag = 0;
   int Group_cnt = 0;
   /* initialize; all disconnected */
   for (i = 0; i < N; i++) {
      id[i] = i;
   }

   /* read while there is data */
   while (fscanf(fp, "%d %d", &p, &q) == 2) {
      pairs_cnt++;
      /* do search first */
      qf_search ++;
      if (id[p] == id[q]) {
         /* already in the same set; discard */
#if (DEBUG == 1)
         printf("\t%d %d\n", p, q);
#endif
         continue;
      }

      /* pair has new info; must perform union */
      for (t = id[p], i = 0; i < N; i++) {
         if (id[i] == t) {
	    qf_unions ++;
            id[i] = id[q];

         }
      }
      links_cnt++;
      if (!quietOut)
         printf(" %d %d\n", p, q);
   }
   /*Print Groups*/
     for(i = 0; i < N; i++){
   	for (print_ctrl = 0; print_ctrl< N; print_ctrl++){
		if(id[print_ctrl] ==i){
			printf("%d-", print_ctrl);
			Group_flag = 1;
			}
   	}
        printf ("\b \b"); /* erase last '-' char*/	
	if(Group_flag == 1){
		printf("\n");
		Group_cnt++;
	}
	Group_flag = 0;
     }
   printf("Number of Groups: %d \n", Group_cnt);
   printf("QF: The number of links performed is %d for %d input pairs.Total of %d search ops and %d union ops\n",
          links_cnt, pairs_cnt, qf_search, qf_unions);
   return;
}


/******************************************************************************
 * quick_union()
 *
 * Arguments: id - array with connectivity information
 *            N - size of array
 *            fp - file pointer to read data from
 *            quietOut - to reduce output to just final count
 * Returns: (void)
 * Side-Effects: pairs of elements are read and the connectivity array is
 *               modified
 *
 * Description: Quick Union algorithm
 *****************************************************************************/

void quick_union(int *id, int N, FILE * fp, int quietOut)
{

   int i, j, p, q;
   int pairs_cnt = 0;            /* connection pairs counter */
   int links_cnt = 0;		 /* number of links counter */
   int qu_unions = 0;
   int qu_search = 0;
   /* initialize; all disconnected */
   for (i = 0; i < N; i++) {
      id[i] = i;
   }

   /* read while+ there is data */
   while (fscanf(fp, "%d %d", &p, &q) == 2) {
      pairs_cnt++;
      i = p;
      j = q;

      /* do search first */
	
      while (i != id[i]) {
         i = id[i];
	 qu_search++;
      }
      while (j != id[j]) {
         j = id[j];
	 qu_search++;
      }
      if (i == j) {
         /* already in the same set; discard */
#if (DEBUG == 1)
         printf("\t%d %d\n", p, q);
#endif
         continue;
      }

      /* pair has new info; must perform union */
      id[i] = j;
      links_cnt++;
      qu_unions++;
      if (!quietOut)
         printf(" %d %d\n", p, q);
   }
   printf("QU: The number of links performed is %d for %d input pairs.Total of %d search ops and %d union ops\n",
          links_cnt, pairs_cnt, qu_search, qu_unions);
}


/******************************************************************************
 * weighted_quick_union()
 *
 * Arguments: id - array with connectivity information
 *            N - size of array
 *            fp - file pointer to read data from
 *            quietOut - to reduce output to just final count
 * Returns: (void)
 * Side-Effects: pairs of elements are read and the connectivity array is
 *               modified
 *
 * Description: Weighted Quick Union algorithm
 *****************************************************************************/

void weighted_quick_union(int *id, int N, FILE * fp, int quietOut)
{

   int i, j, p, q;
   int *sz = (int *) malloc(N * sizeof(int));
   int pairs_cnt = 0;            /* connection pairs counter */
   int links_cnt = 0;            /* number of links counter */
   int wqu_search = 0;
   int wqu_unions = 0;

   /* initialize; all disconnected */
   for (i = 0; i < N; i++) {
      id[i] = i;
      sz[i] = 1;
   }

   /* read while there is data */
   while (fscanf(fp, "%d %d", &p, &q) == 2) {
      pairs_cnt++;

      /* do search first */
      for (i = p; i != id[i]; i = id[i]){
        wqu_search++;
      }
      for (j = q; j != id[j]; j = id[j]){
      	wqu_search++;
      }
      if (i == j) {
         /* already in the same set; discard */
#if (DEBUG == 1)
         printf("\t%d %d\n", p, q);
#endif
         continue;
      }

      /* pair has new info; must perform union; pick right direction */
      if (sz[i] < sz[j]) {
         id[i] = j;
         sz[j] += sz[i];
      }
      else {
         id[j] = i;
         sz[i] += sz[j];
      }
      links_cnt++;
      wqu_unions++;
      if (!quietOut)
         printf(" %d %d\n", p, q);
   }
   printf("WQU: The number of links performed is %d for %d input pairs. Total of %d search ops and %d union ops\n",
          links_cnt, pairs_cnt, wqu_search, wqu_unions);
}


/******************************************************************************
 * compressed_weighted_quick_union()
 *
 * Arguments: id - array with connectivity information
 *            N - size of array
 *            fp - file pointer to read data from
 *            quietOut - to reduce output to just final count
 * Returns: (void)
 * Side-Effects: pairs of elements are read and the connectivity array is
 *               modified
 *
 * Description: Compressed Weighted Quick Union algorithm
 *****************************************************************************/

void compressed_weighted_quick_union(int *id, int N, FILE * fp, int quietOut)
{
   int i, j, p, q, t, x;
   int *sz = (int *) malloc(N * sizeof(int));
   int pairs_cnt = 0;            /* connection pairs counter */
   int links_cnt = 0;            /* number of links counter */
   int cwqu_search = 0;
   int cwqu_unions = 0;
   int cwqu_compression =0;      /*number of path compression operations*/ 

   /* initialize; all disconnected */
   for (i = 0; i < N; i++) {
      id[i] = i;
      sz[i] = 1;
   }

   /* read while there is data */
   while (fscanf(fp, "%d %d", &p, &q) == 2) {
      pairs_cnt++;

      /* do search first */
      for (i = p; i != id[i]; i = id[i])
	      cwqu_search++;
      for (j = q; j != id[j]; j = id[j])
	      cwqu_search++;
      if (i == j) {
         /* already in the same set; discard */
#if (DEBUG == 1)
         printf("\t%d %d\n", p, q);
#endif
         continue;
      }

      /* pair has new info; must perform union; pick right direction */
      if (sz[i] < sz[j]) {
         id[i] = j;
         sz[j] += sz[i];
         t = j;
      }
      else {
         id[j] = i;
         sz[i] += sz[j];
         t = i;
      }
      cwqu_unions++;
      links_cnt++;

      /* retrace the path and compress to the top */
      for (i = p; i != id[i]; i = x) {
         x = id[i];
         id[i] = t;
	 cwqu_compression++;
      }
      for (j = q; j != id[j]; j = x) {
         x = id[j];
         id[j] = t;
	 cwqu_compression++;
      }
      if (!quietOut)
         printf(" %d %d\n", p, q);
   }
   printf("CWQU: The number of links performed is %d for %d input pairs. Total of %d search ops %d union ops and %d compress ops\n",
          links_cnt, pairs_cnt, cwqu_search, cwqu_unions, cwqu_compression);

   return;
}
