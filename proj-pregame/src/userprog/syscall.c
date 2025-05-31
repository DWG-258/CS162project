#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "userprog/process.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

static void syscall_handler(struct intr_frame*);

void syscall_init(void) { intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall"); }

int check_user_ptr(const void *uaddr)
{
  if(uaddr==NULL||!is_user_vaddr(uaddr))
  {

    process_exit();
    return -1;
  }
  if (!pagedir_get_page(thread_current()->pcb->pagedir, uaddr))
  {
 
    process_exit();
    return -1;
  }
  return 0;
}
int check_str_ptr(const char* str)
{
  

  if(str==NULL||!is_user_vaddr(str))
  {
    
    return -1;
  }
  if (!pagedir_get_page(thread_current()->pcb->pagedir, str))
  {
   
    return -1;
  }

}

static void syscall_handler(struct intr_frame* f UNUSED) {
  check_user_ptr(f->esp);
  uint32_t* args = ((uint32_t*)f->esp);

  /*
   * The following print statement, if uncommented, will print out the syscall
   * number whenever a process enters a system call. You might find it useful
   * when debugging. It will cause tests to fail, however, so you should not
   * include it in your final submission.
   */

  // printf("System call number: %d\n", args[0]); 
switch (args[0])
{
case SYS_EXIT:
  /* code */
  f->eax = args[1];
  printf("%s: exit(%d)\n", thread_current()->pcb->process_name, args[1]);
  //将状态保存
   thread_current()->exit_status=args[1];

  process_exit();
  break;
case SYS_PRACTICE:
  
  f->eax=args[1]+1;
  break;
case SYS_HALT:

  break;
  //TO DO ,正确传递args
case SYS_EXEC:

 char* cmd_line=(char*)args[1];
 //处理错误指针
 if(check_str_ptr(cmd_line)==-1)
 {
  f->eax=-1;
  break;
 }

  //
  int tid= process_execute(cmd_line);
  
  f->eax=tid;


  if (tid == TID_ERROR)
  {
    f->eax=-1;
  }

  break; 
case SYS_WAIT:
pid_t pid=args[1];


f->eax=process_wait(pid);
break;


 
case SYS_WRITE:
//write函数调用
  printf("%s",(char*)args[2]);
  break; 


default:
  break;
}

}
