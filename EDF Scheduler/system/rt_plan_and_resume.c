#include <xinu.h>
#include <stdlib.h>
#include "rt_plan_and_resume.h"

int32  bprcount;
int32 flag;
//struct boundent blist[BNPROC];

/* Prototype for the comparator */
int comp(const void *s1, const void *s2);

/**
 * Creates the plan for the bounded jobs and prints them to the console
 * Inserts the bounded jobs in the readylist
 * Starts the execution of the bounded jobs
 */
int rt_plan_and_resume()
{
  intmask          mask;
  uint32           i;
  uint32           curr_time = 0;
  
  mask = disable();
  if (bprcount == 0 || flag == 1) {
    restore(mask);
    return (pri16) SYSERR;
  }

  flag = 1;
  qsort((void *) blist, bprcount, sizeof(struct boundent), comp);
  for (i = 0; i < bprcount; i++) {
    if (blist[i].deadline > curr_time) {
      blist[i].scheduling_time = curr_time;
      blist[i].quantum         = min(blist[i].deadline - curr_time, blist[i].execution_time);
      curr_time               += min(blist[i].deadline - curr_time, blist[i].execution_time);
    }
    else {
      blist[i].eligible = 0;
    }
  }

  for (i = 0; i < bprcount; i++) if(blist[i].eligible == 1) kprintf("%s: %d\n", blist[i].name, blist[i].scheduling_time);
  for (i = 0; i < bprcount; i++) if(blist[i].eligible == 0) kprintf("%s: \"cannot schedule\"\n", blist[i].name);
  for (i = 0; i < bprtotal; i++) if (blist[i].eligible == 0) kill(blist[i].pid);
  for (i = 0; i < bprtotal; i++) {
    if(blist[i].eligible == 1) {
      (&proctab[blist[i].pid])->prstate = PR_READY;
      insert(blist[i].pid, readylist, 10000);
    }
  }

  resched();
  
  restore(mask);
  return OK;
}

/**
 * Sorts the array according to increasing deadline, decreasing execution time and/or increasing creation time
 */
int comp(const void *s1, const void *s2)
{
  struct boundent *bpr1 = (struct boundent *) s1;
  struct boundent *bpr2 = (struct boundent *) s2;

  if (bpr1->deadline == bpr2->deadline) {
    if (bpr1->execution_time == bpr2->execution_time) {
      return bpr1->creation_time - bpr2->creation_time;
    }

    return bpr2->execution_time - bpr1->execution_time;
  }

  return bpr1->deadline - bpr2->deadline;
}
