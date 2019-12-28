#include "proc.h"

#define MAX_NR_PROC 4

void naive_uload(PCB *pcb, const char *filename);
void context_kload(PCB *pcb, void *entry);
void context_uload(PCB *pcb, const char *filename);

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
int fg_pcb = 1;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}

void init_proc() {
  Log("Initializing processes...");

  // init_proc()
  //context_kload(&pcb[0], (void *)hello_fun);
  //context_uload(&pcb[0], "/bin/events");
  context_uload(&pcb[0], "/bin/hello");
  context_uload(&pcb[1], "/bin/pal");
  context_uload(&pcb[2], "/bin/pal");
  context_uload(&pcb[3], "/bin/pal");
  
  switch_boot_pcb();

  // load program here
  // naive_uload(NULL, "/bin/init");
}

static uint32_t schedule_count = 0;

_Context* schedule(_Context *prev) {
  // save the context pointer
  current->cp = prev;

  // always select pcb[0] as the new process
  // current = &pcb[0];
  if (schedule_count % 1000 == 0){
    current = &pcb[0];
  }
  else{
    current = &pcb[fg_pcb];
  }
  schedule_count++;
  // then return the new context
  return current->cp;
  //return NULL;
}
