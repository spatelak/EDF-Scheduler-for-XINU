/* resched.c - resched, resched_cntl */

#include <xinu.h>


struct	defer	Defer;

//int32 bprcount;
//int32 bprtotal;
int32 flag;
struct boundent blist[BNPROC];

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
/**
 *
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
	uint32 isBounded = 0;   /* Flag to check bounded jobs           */
	uint32 j = 0;           /* For looping */
	
	/* If rescheduling is deferred, record attempt and return */
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */
	ptold = &proctab[currpid];

	/* Set the flag is the current process is bounded */
	if (ptold->type == 1) isBounded = 1;

	if (ptold->prstate == PR_CURR) {
	  /* If resched happened because of quantum expiry, kill the current process if it is bounded */	  
	  if (ptold->type == 1 && ptold->hasExpired == 1) {
	    ptold->hasExpired = 0;
	    hook(currpid, preempt);
	    kill(currpid);
	    return;
	  }
	  
	  /* Process remains eligible */
	  if (ptold->prprio >= firstkey(readylist) && ptold->type == 1) {
	    return;
	  }
	  if (ptold->prprio > firstkey(readylist)) {
	    return;
	  }

	  /* Old process will no longer remain current if it is not bounded */
	  if (ptold->type == 0) {
	    ptold->prstate = PR_READY;
	    insert(currpid, readylist, ptold->prprio);
	  }
	}

	/* If bounded process changes state from current to something else, then kill it */
	else {
	  if (ptold->type == 1) {
	    if (ptold->prstate != PR_SUSP) bprcount--;
	    else                           bprtotal++;
	    
	    if (ptold->prstate != PR_FREE) {
	      hook(currpid, preempt);
	      kill(currpid);
	    }
	  }
	}

	/* If finished executing last bounded job, printf "Bounded jobs ended" */
	if (isBounded == 1 && bprcount == 0) {
	  flag      = 0;
	  bprtotal  = 0;
	  isBounded = 0;
	  kprintf("Bounded jobs ended\n");
	}

	/* Force context switch to highest priority ready process */
	currpid        = dequeue(readylist);
	ptnew          = &proctab[currpid];
	ptnew->prstate = PR_CURR;

	/* Reset time slice for process */
	if (ptnew->type == 0) preempt = QUANTUM;
	else  {
	  for (j = 0; j < bprtotal; j++) {
	    if (blist[j].pid == currpid) {
	      preempt = blist[j].quantum;
	      break;
	    }
	  }	  
	  //while (blist[bprtotal - bprcount].eligible == 0 && bprcount > 0) bprcount--;
	  //preempt = blist[bprtotal - bprcount].quantum;
	}
	
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
/**
 *
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
