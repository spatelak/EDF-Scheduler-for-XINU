/*  main.c  - main */

#include <xinu.h>
#include "rt_create.h"
#include "rt_plan_and_resume.h"
#include "test.h"

process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	rt_create((void *)test1, INITSTK, 10, 100, "job1", 1);
	rt_plan_and_resume();
	/*
	rt_create((void *)test2, INITSTK, 20, 100, "job1", 1);
	rt_create((void *)test2, INITSTK, 20,  80, "job2", 1);
	rt_create((void *)test2, INITSTK, 60,  90, "job3", 1);
	rt_create((void *)test2, INITSTK, 70, 100, "job4", 1);
	kill(rt_create((void *)test3, INITSTK, 30, 120, "job5", 1));
	//rt_create((void *)test2, INITSTK, 30, 120, "job6", 1);
	rt_plan_and_resume();
	
	rt_create((void *)test1, INITSTK, 10, 100, "job1", 1);
	rt_create((void *)test2, INITSTK, 20,  80, "job2", 1);
	rt_create((void *)test3, INITSTK, 50,  90, "job3", 1);
	rt_create((void *)test2, INITSTK, 70, 160, "job4", 1);
	rt_create((void *)test3, INITSTK, 30, 190, "job5", 1);
	rt_plan_and_resume();
	*/

	//rt_create((void *)test1, INITSTK, 30, 190, "job5", 1);
	//rt_plan_and_resume();
	//resume(create((void *)test3, INITSTK, INITPRIO, "job6", 1, 1));
	/*
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	 Wait for shell to exit and recreate it 

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	*/
	  
	return OK;
    
}
