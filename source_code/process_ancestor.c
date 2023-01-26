#include <linux/kernel.h> 
#include <linux/syscalls.h>
#include <linux/sched.h> 
#define ANCESTOR_NAME_LEN 16
struct process_info { 
	long pid;                     /* Process ID */ 
	char name[ANCESTOR_NAME_LEN]; /* Program name of process */ 
	long state;                   /* Current process state */ 
	long uid;                     /* User ID of process owner */ 
	long nvcsw;                   /* # voluntary context switches */ 
	long nivcsw;                  /* # involuntary context switches */ 
	long num_children;            /* # children process has */ 
	long num_siblings;            /* # sibling process has */ 
}; 
SYSCALL_DEFINE3( 
		process_ancestors,                /* syscall name for macro */ 
		struct process_info*, info_array, /* array of process info strct */ 
		long, size,                       /* size of the array */ 
		long*, num_filled)                /* # elements written to array */ 
{ 
	long i;
	struct task_struct* cur_task;
	struct process_info info;
	long countOfChildren;
	struct list_head * 	currChild;
	struct list_head * 	firstChild;
	long countOfSibling;
	struct list_head * 	currentSibling;
	struct list_head * 	firstSibling;
	int x;
	int zero;
	zero = 0;
	i = 0;
	cur_task = current;
	if(size<=0)
	{
		if(copy_to_user(num_filled, &zero, sizeof(int)) != 0)
		{
			//printk("Failed to write numfilled.\n");
			return -EFAULT;
		}
		return -EINVAL;
	}
	for(i = 0; i <size; i++)
	{
		if(i!=0){
			cur_task = cur_task->parent;
		}
		info.pid = cur_task->pid;
		for (x = 0; x < 16; x++){
			(info.name)[x] = (cur_task->comm)[x];
		}
		info.uid = cur_task->cred->uid.val;
		info.state = cur_task->state;
		info.nvcsw = cur_task->nvcsw;
		info.nivcsw = cur_task->nivcsw;
		countOfChildren = 0;
		currChild = (&cur_task->children)->next;
		firstChild= &cur_task->children;
		countOfChildren = countOfChildren+1;
		while(currChild != firstChild){
			currChild = currChild->next;
			countOfChildren = countOfChildren+1;
		}
		info.num_children = countOfChildren;
		countOfSibling = 0;
		currentSibling = (&cur_task->sibling)->next;
		firstSibling= &cur_task->sibling;
		countOfSibling = countOfSibling+1;
		while(currentSibling != firstSibling){
			currentSibling = currentSibling->next;
			countOfSibling = countOfSibling+1;
		}
		info.num_siblings = countOfSibling;
		if(copy_to_user(info_array+i, &info, sizeof(info)) != 0){
			//printk("Failed to write info.\n");
			return -EFAULT;
		}
		if(cur_task->parent == cur_task){
			break;
		}
	}
	if(copy_to_user(num_filled, &i, sizeof(long)) != 0){
		//printk("Failed to write numfilled.\n");
		return -EFAULT;
	}
	return 0;
}