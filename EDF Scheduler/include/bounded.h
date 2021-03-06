/* bounded.h */
#ifndef BOUNDED
#define BOUNDED

#ifndef BNPROC 
#define BNPROC 25
#endif

struct boundent {
  uint32 creation_time;
  uint32 execution_time;
  uint32 deadline;
  uint32 scheduling_time;
  uint32 quantum;
  char   name[16];
  pid32  pid;
  uint32 eligible;
};

/*
struct blist {
  struct boundent bprentd;
  struct blist *next;
};
*/
//extern struct blist *bprlist;
extern struct boundent blist[BNPROC];
extern int32           bprcount;                   /* Currently active processes      */
extern int32           bprtotal;                   /* Total bounded processes created */
extern int32           creation_time;              /* Currently executing process     */
extern int32           flag;
#endif /* BOUNDED */
