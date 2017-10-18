#include <xinu.h>
#include "test.h"
#include "rt_create.h"
#include "rt_plan_and_resume.h"

process test1(int32 x) {
  //rt_create((void *)test4, INITSTK, 70, 100, "job10000", 1);
  //rt_plan_and_resume();
  kprintf("test1 = %d\n", x);
  return OK;
}

process test2(int32 x) {
  uint32 count = 0;
  while(1) {
    kprintf("test2 = %d\n", count++);
  }
  return OK;
}

process test3(int32 x) {
  while(1) {
    kprintf("test3 = %d\n", x++);
    sleep(3);
  }
  return OK;
}

process test4() {
  while(1) kprintf("test4\n");
  return OK;
}
