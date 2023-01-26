#include <stdio.h> 
#include <unistd.h> 
#include <sys/syscall.h> 
#include<stdlib.h>
#include <errno.h>
#include <assert.h>
#define _ARRAY_STAT_  549
// for a 64 bit system
struct array_stats_s {
	long min;
	long max;
	long sum;
};
int main(int argc, char *argv[]) 
{ 
	struct array_stats_s stats = {0, 0, 0};
	struct array_stats_s *statsPtr = &stats;
	
	long* longArray;
	longArray = calloc(10, sizeof(long));
	*longArray = 3;
	*(longArray+1) = 4;
	*(longArray+2) = 13;
	*(longArray+3) = 9;
	*(longArray+4) = 21;
	*(longArray+5) = 3;
	*(longArray+6) = 17;
	*(longArray+7) = 42;
	*(longArray+8) = 99;
	*(longArray+9) = -99;
	long size = 10;
	printf("First test, test positive ans negative values:\n");
	int result = syscall(_ARRAY_STAT_,statsPtr,longArray, size);
	assert(result==0);
	assert(stats.min==-99);
	assert(stats.max==99);
	assert(stats.sum==112);
	if(result == 0)
	{
		if(stats.min == -99&&stats.max == 99&&stats.sum == 112)
		{
			printf("Pass! \n");
		}
		else
		{
			if(stats.min != -99)
			{
				printf("Min is wrong, the min we got is: %ld\n", stats.min);
			}
			if(stats.max != 99)
			{
				printf("Max is wrong, the max we got is: %ld\n", stats.max);
			}
			if(stats.sum != 112)
			{
				printf("Sum is wrong, the sum we got is: %ld\n", stats.sum);
			}
		}
	}
	else
	{
		printf("Failed! Error number: %d\n", result);
	}
	free(longArray);
	
	longArray = NULL;
	size = 1;
	printf("Second test, test empty array. :\n");
	result = syscall(_ARRAY_STAT_,statsPtr,longArray, size);
	assert(result==-1);
	assert(errno==EFAULT);
	if(result == 0)
	{
		printf("Something wrong, the size of the array 0, this test shouldn't return 0.\n");
	}
	else{
		if (errno == EFAULT)
		{
			printf("Pass! \n");
		}
		else
		{
			printf("Wrong error message. \n");
		}	
	} 
	
	longArray = calloc(0, sizeof(long));
	size = 0;
	printf("Third test, test size=0.  :\n");
	result = syscall(_ARRAY_STAT_,statsPtr,longArray, size);
	assert(result==-1);
	assert(errno==EINVAL);
	if(result == 0){
		printf("Something wrong, the size of this test is 0, this test shouldn't return 0.\n");
	}
	else{
		if (errno == EINVAL){
			printf("Pass! \n");
		}
		else{
			printf("Wrong error message. \n");
		}	
	} 
	free(longArray);
	
	longArray = calloc(1, sizeof(long));
	*longArray = 5;
	size = 1;
	printf("Fourth test, test array size=1: \n");
	result = syscall(_ARRAY_STAT_,statsPtr,longArray, size);
	assert(result==0);
	assert(stats.min==5);
	assert(stats.max==5);
	assert(stats.sum==5);
	if(result == 0){
		if(stats.min == 5&&stats.max == 5&&stats.sum == 5){
			printf("Pass! \n");
		}
		else{
			if(stats.min != 5)
			{
				printf("Min is wrong, the min we got is: %ld\n", stats.min);
			}
			if(stats.max != 5)
			{
				printf("Max is wrong, the max we got is: %ld\n", stats.max);
			}
			if(stats.sum != 5)
			{
				printf("Sum is wrong, the sum we got is: %ld\n", stats.sum);
			}
		}
	}
	else
	{
		printf("Failed! Error number: %d\n", result);
	}
	free(longArray);

	// Fifth test, test large array
	printf("Fifth test, test large array: \n");
	#define large_num (256*256)

	longArray = calloc(large_num, sizeof(long));
	for (long i=0; i< large_num; i++){
		*(longArray+i) = i;
	}
	size = large_num;
	result = syscall(_ARRAY_STAT_,statsPtr,longArray, size);
	long s=(256*256-1)*(256*128);
	assert(result==0);
	assert(stats.min==0);
	assert(stats.max==256*256-1);
	assert(stats.sum==s);
	if(result == 0)
	{
		if(stats.min == 0&&stats.max == 256*256-1&&stats.sum == s)
		{
			printf("Pass! \n");
		}
		else
		{
			if(stats.min != 0)
			{
				printf("Min is wrong, the min we got is: %ld\n", stats.min);
			}
			if(stats.max != 256*256-1)
			{
				printf("Max is wrong, the max we got is: %ld\n", stats.max);
			}
			if(stats.sum != s)
			{
				printf("Sum is wrong, the sum we got is: %ld\n", stats.sum);
			}
		}
	}
	else
	{
		printf("Failed! Error number: %d\n", result);
	}
	free(longArray);

	// Sixth test, test size=-1 
	longArray = calloc(0, sizeof(long));
	size = -1;
	printf("Sixth test, test size=-1 :\n");
	result = syscall(_ARRAY_STAT_,statsPtr,longArray, size);	
	assert(result==-1);
	assert(errno==EINVAL);
	if(result == 0){
		printf("Something wrong, the size of this test is -1, this test shouldn't return 0.\n");
	}
	else{
		if (errno == EINVAL){
			printf("Pass! \n");
		}
		else{
			printf("Wrong error message. \n");
		}	
	} 
	free(longArray);


	// Seventh test, test size=-1000
	longArray = calloc(0, sizeof(long));
	size = -1000;
	printf("Seventh test, test size=-1000 :\n");
	result = syscall(_ARRAY_STAT_,statsPtr,longArray, size);
	assert(result==-1);
	assert(errno==EINVAL);
	if(result == 0){
		printf("Something wrong, the size of this test is -1000, this test shouldn't return 0.\n");
	}
	else{
		if (errno == EINVAL){
			printf("Pass! \n");
		}
		else{
			printf("Wrong error message. \n");
		}	
	} 
	free(longArray);

	// 8th test, test bad stats pointer
	longArray = calloc(1, sizeof(long));
	*longArray = 3;
	size=1;
	printf("9th test, test bad stats pointer\n");
	result = syscall(_ARRAY_STAT_,NULL,longArray, size);
	assert(result==-1);
	assert(errno==EFAULT);
	if(result == 0)
	{
		printf("Something wrong, the stats pointer is null, this test shouldn't return 0.\n");
	}
	else{
		if (errno == EFAULT)
		{
			printf("Pass! \n");
		}
		else
		{
			printf("Wrong error message. \n");
		}	
	} 
	free(longArray);

	// 9th test, test read-only stats pointer
	longArray = calloc(1, sizeof(long));
	*longArray = 3;
	size=1;
	printf("10th test, test read-only stats pointer\n");
	result = syscall(_ARRAY_STAT_,(void*)1,longArray, size);
	assert(result==-1);
	assert(errno==EFAULT);
	if(result == 0)
	{
		printf("Something wrong, the stats is read only, this test shouldn't return 0.\n");
	}
	else{
		if (errno == EFAULT)
		{
			printf("Pass! \n");
		}
		else
		{
			printf("Wrong error message. \n");
		}	
	} 
	free(longArray);

	// 10th test, test bad size, too large
	longArray = calloc(1, sizeof(long));
	*longArray = 3;
	size=1024*1024;
	printf("10th test, test bad size\n");
	result = syscall(_ARRAY_STAT_,NULL,longArray, size);
	assert(result==-1);
	assert(errno==EFAULT);
	if(result == 0)
	{
		printf("Something wrong, the stats pointer is null, this test shouldn't return 0.\n");
	}
	else{
		if (errno == EFAULT)
		{
			printf("Pass! \n");
		}
		else
		{
			printf("Wrong error message. \n");
		}	
	} 
	free(longArray);
	
    return 0; 
} 