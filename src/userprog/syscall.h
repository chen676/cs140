#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <list.h>
#include "filesys/file.h"

/*store opened files info*/
struct file_info_block {
	struct file *f;                /*file structure for the opened file*/
	char *file_name;               /*file_name for the opened file*/
	int fd;                        /*file descriptor for the opened file*/
	struct list_elem elem;         /*list elem for thread's opened_file_list*/
};

void syscall_init (void);
void user_exit(int exit_code);
void close_file_by_fib(struct file_info_block *fib);
void print_global_file_list();

#endif /* userprog/syscall.h */
