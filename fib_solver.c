

#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/stat.h>


//
//Fib. Solver using Fork()
// Solver code to be forked up:
long fib(int number)
{
	if (number < 2 )
	{
		return number;
	}
	else
	{
		pid = fork();
		if (pid < 0) { /*Error Occured */
		fprintf(stderr, "%s\n", );
		return -1;
		}
		else if(pid == 0) /*Child Process */
	}
}

long fib(long number)
{
	long result = 0;
	if (number < 2 )
	{
		return number;
	}
	else
	{
		while (number != 2)
		{
			result 
		}
	}
}
int main(int argc, char *argv[])
{
	pid_t pid, pid1;
	int fibNumber;
	int i = argv[0] - '0';
	//once we get the input from the user, we can procede to calculate the fib sequence.
	// i represents how deep we have to go. 
	long fib = 0;

	//since fork really only works if there's a solid number of tasks, we need to count them.
	if (i > 8)
	{
		//fork may be effective here:
		for(int j = 0; j <= i; j ++)
		{
			pid = fork();
			//We are now running 2 processes. Holy crow.
			if (pid == 0) // child proc
			{
				fib += fib(
			}
		}

	}

	return 0;
}

