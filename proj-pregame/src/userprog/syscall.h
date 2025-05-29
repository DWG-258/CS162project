#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
int check_user_ptr(const void *uaddr);
int check_str_ptr(const char *str);
void syscall_init(void);

#endif /* userprog/syscall.h */
