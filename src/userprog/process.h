#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"
#include "threads/synch.h"

struct wait_info_block {
	tid_t tid;               /*thread's tid*/
	struct thread *t;        /*pointer to thread*/
	int exit_code;           /*code for exit status*/
	struct list_elem elem;   /*list elem for children list of its parent*/
	struct lock l;           /*lock for this struct itself*/
	struct cond c;           /*cond for wait from the parent*/
};

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

/*self defined*/
#define MAX_FILE_NAME 14
void get_cmd(const char *full_line, char* cmd);


#endif /* userprog/process.h */
