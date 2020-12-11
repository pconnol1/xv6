#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

typedef struct display_ds {
  char *name;
  uint checked;
  uint not_checked;
 } display_ds;

void
display_ds_store(display_ds *b)
{
  cprintf("-> %s: %d, not %s: %d\n",
          b->name, b->checked,
          b->name, b->not_checked);
}

int
sys_myMemory(void)
{
  
  uint d_flags, t_flags;//initializing two flags- one for directory and table 
  pde_t *pde;
  pte_t *pte;
    

  display_ds present = { "present", 0, 0 },
    writable = { "writable", 0, 0 },
    user = { "user", 0, 0 };
  
  int i = 0, j = 0;
  int check_flags=0;
  
  for (i = 0; i < NPDENTRIES; i++) {
    pde = &myproc()->pgdir[i];
    d_flags = PTE_FLAGS(*pde);
     
     check_flags = (d_flags & PTE_P) || (d_flags & PTE_W) || (d_flags & PTE_U);

    if (check_flags) {
      for (j = 0; j < NPTENTRIES;  j++) {
        pte = (pte_t *)get_pagetable(pde, (void *) j);

        if(pte != 0) {
          t_flags = PTE_FLAGS(*pte);

          if (t_flags & PTE_P) {
            present.checked++;
          } else {
            present.not_checked++;
          }

          if (t_flags & PTE_W) {
            writable.checked++;
          } else {
            writable.not_checked++;
          }

          if (t_flags & PTE_U) {
            user.checked++;
          } else {
            user.not_checked++;
          }

        }
      }
    } else {
      present.not_checked += NPTENTRIES;
      writable.not_checked += NPTENTRIES;
      user.not_checked += NPTENTRIES;
    }

  }

  cprintf("page table entry information:\n");
  display_ds_store(&present);
  display_ds_store(&writable);
  display_ds_store(&user);

  int num_used_pages = PGROUNDUP(myproc()->sz) / PGSIZE,
    num_free_pages = check_free_list_elements();

  cprintf("Process and system page info:\n");
  cprintf("-> Total number of pages used by the process: %d\n", num_used_pages);
  cprintf("-> Total number of free pages available in the system: %d\n", num_free_pages);

  return 0;
}