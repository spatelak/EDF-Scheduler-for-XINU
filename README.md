# EDF-Scheduler-for-XINU

Some details about EDF Scheduler:

=> The OS selects the process with closest deadline to execute first. If two jobs have the same deadline, OS chooses the one which has the longest execution time. If two jobs have same execution time, OS chooses the job which was created first.<br />=> All the jobs should be created before making the plan.<br />=> No new bounded jobs are accepted for the duration while other bounded jobs are executing.
