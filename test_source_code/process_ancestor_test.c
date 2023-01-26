#include <stdio.h> 
#include <unistd.h> 
#include <sys/syscall.h> 
#include<stdlib.h>
#include <errno.h>
#include <assert.h>
#define _ARRAY_PROCESS_INFO_  550
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

void display_process_info(struct process_info *process) {
	printf("pid is %li\n", process->pid);
	printf("name is %s\n", process->name);
	printf("nvcsw is %li\n", process->nvcsw);
	printf("nivcsw is %li\n", process->nivcsw);
	printf("state is %li\n", process->state);
	printf("uid is %li\n", process->uid);
	printf("children number is %li\n", process->num_children);
	printf("sibling number is %li\n", process->num_siblings);
	printf("\n");
};

int main(int argc, char *argv[]) 
{ 

	printf("Test1, process info array size=50, larger than the actural number of ancestors. \n");
	struct process_info info_array_1[50];
	long* num_filled;
	// num_filled = calloc(sizeof(long), 1);
	num_filled = calloc(0, sizeof(long));
	int result = syscall(_ARRAY_PROCESS_INFO_, &info_array_1, 50, num_filled);
	assert(result==0);

	if(result == 0)
	{
		for(int i = 0; i < *num_filled; i++)
		{
			if(info_array_1[i].pid==0){
				printf("array item %d\n", i);
				display_process_info(&(info_array_1[i]));
				break;
			}
			printf("array item %d\n", i);
			display_process_info(&(info_array_1[i]));
			// printf("process %d\n", i);
			// printf("pid: %ld\n", info_array_1[i].pid);
			// printf("Name: %s\n", info_array_1[i].name);
			// printf("state: %ld\n", info_array_1[i].state);
			// printf("uid: %ld\n", info_array_1[i].uid);
			// printf("nvcsw: %ld\n", info_array_1[i].nvcsw);
			// printf("nivcsw: %ld\n", info_array_1[i].nivcsw);
			// printf("num_children: %ld\n", info_array_1[i].num_children);
			// printf("num_siblings: %ld\n\n", info_array_1[i].num_siblings);
		}
	}
	else
	{
		if (errno == EFAULT)
		{
			printf("read/write fail. \n");
		}
		else
		{
			printf("size<=0  \n");
		}	
		// printf("First test failed.\n");
	}
	
	printf("Test2, process info array size=1, less than real ancestors number.  \n");
	struct process_info info_array_2[1];
	result = syscall(_ARRAY_PROCESS_INFO_, &info_array_2, 1, num_filled);
	assert(result==0);
	assert(*num_filled==1);
	if(result == 0){
		if(*num_filled == 1){
			printf("num_filled: %ld\n", *num_filled);
			for(int i = 0; i < *num_filled; i++){
				printf("array item %d\n", i);
				display_process_info(&(info_array_2[i]));
				// printf("process %d\n", i);
				// printf("pid: %ld\n", info_array_1[i].pid);
				// printf("Name: %s\n", info_array_1[i].name);
				// printf("state: %ld\n", info_array_1[i].state);
				// printf("uid: %ld\n", info_array_1[i].uid);
				// printf("nvcsw: %ld\n", info_array_1[i].nvcsw);
				// printf("nivcsw: %ld\n", info_array_1[i].nivcsw);
				// printf("num_children: %ld\n", info_array_1[i].num_children);
				// printf("num_siblings: %ld\n\n", info_array_1[i].num_siblings);
			}
		}
		else{
			printf("Wrong number_filled:  %ld\n", *num_filled);
		}
	}
	else{
		if (errno == EFAULT)
		{
			printf("read/write fail. \n");
		}
		else
		{
			printf("size<=0  \n");
		}	
		// printf("Second test failed.\n");
	}
	
	printf("Test3, process info array size=0 \n");
	struct process_info info_array_3[0];
	result = syscall(_ARRAY_PROCESS_INFO_, &info_array_3, 0, num_filled);
	assert(result==-1);
	assert(*num_filled==0);

	if(result != 0){
		printf("Third test Pass, Error number is: %d.\n", result);
	}
	else
	{
		printf("Third test failed.\n");
	}
	free(num_filled);
	printf("Test4, wrong pointer info_array \n");
	long size4=32;
	struct process_info* info_array_4=NULL;
	long num_filled4;
	int result4= syscall(_ARRAY_PROCESS_INFO_, info_array_4, size4, &num_filled4);
	assert(result4==-1);
	assert(num_filled4==0);
    return 0; 
} 