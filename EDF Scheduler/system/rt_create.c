/* rt_create.c: rt_create */

#include <xinu.h>
#include "rt_create.h"

int32 bprtotal;
int32 creation_time;
int32 flag;

/**
 * Creates a bounded process and sets its entries in the data structure
 */
int rt_create(void *funcaddr, uint32 size, uint32 execution_time, uint32 deadline, char *name, int32 arg)
{

  struct boundent *bprptr;
  pid32           pid;
  uint32          i;
  intmask         mask;
  
  mask = disable();
  if (execution_time < 0 || deadline < 0 || deadline < execution_time || bprcount >= 25) {
    restore(mask);
    return SYSERR;
  }
 
  pid = create(funcaddr, size, 10000, name, 1, arg);
 
  if (pid != SYSERR && bprcount < 25 && flag == 0) {
    bprptr = &blist[bprcount];

    bprptr->creation_time  = creation_time++;
    bprptr->execution_time = execution_time;
    bprptr->deadline       = deadline;
    bprptr->pid            = pid;
    bprptr->eligible       = 1;
    bprptr->name[PNMLEN-1] = NULLCH;
    for (i = 0; i < PNMLEN - 1 && (bprptr->name[i] = name[i]) != NULLCH; i++);

    bprcount++;
    bprtotal++;
 
    proctab[pid].type       = 1;
    proctab[pid].hasExpired = 0;
  }

  restore(mask);
  return OK;
}
