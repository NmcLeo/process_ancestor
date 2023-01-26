#include <linux/kernel.h> 
#include <linux/syscalls.h>
struct array_stats_s {
	long min;
	long max;
	long sum;
};
SYSCALL_DEFINE3(
	array_stats, /* syscall name */
	struct array_stats_s*, stats, /* where to write stats */
	long*, data, /* data to process */
	long, size) /* # values in data */
{
	long min;
	long max;
	long sum;
	int i;
	long dataInK;
	if(size <= 0)
	{
		return -EINVAL;
	}
	//printk("value of size is: %ld \n", size);
	//struct array_stats_s statsInK;
	for(i = 0; i < size; i++)
	{

		if (copy_from_user(&dataInK, data+i, sizeof(long)) != 0)
		{
			//printk("Failed to read from user\n");
			return -EFAULT;
		}
		// printk("I's value is: %ld \n", dataInK);
		if(i == 0)
		{
			min = dataInK;
			max = dataInK;
			sum = dataInK;
		}
		else
		{
			if(min > dataInK)
				min = dataInK;
			if(max < dataInK)
				max = dataInK;
			sum = sum+dataInK;
		}
	}
	if (copy_to_user(&stats->min, &min , sizeof(long)) != 0)
	{
		//printk("Failed to write to user\n");
        return -EFAULT;
	}
	if (copy_to_user(&stats->max, &max , sizeof(long)) != 0)
	{
		//printk("Failed to write to user\n");
        return -EFAULT;
	}
	if (copy_to_user(&stats->sum, &sum , sizeof(long)) != 0)
	{
		//printk("Failed to write to user\n");
        return -EFAULT;
	}
	return 0;
}