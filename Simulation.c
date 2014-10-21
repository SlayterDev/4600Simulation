#include <stdio.h>
#include <math.h>
#include <time.h>

typedef struct {
	unsigned long burst;
	unsigned int memory;
} process;

typedef struct {
	unsigned long speed;
	unsigned int memory;
	process queue[50];
	int procCount;
} CPU;

process processes[50];
CPU cpus[5];
unsigned long maxBurst;

void setMaxBurst() {
	unsigned long max = 0;
	int i;
	for (i = 0; i < 50; i++) {
		if (processes[i].burst > max)
			max = processes[i].burst;
	}

	maxBurst = max;
}

void generateProcesses() {
	int i;
	for (i = 0; i < 50; i++) {
		process p;
		p.memory = (random() % (8388608 - 250)) + 250; // .25MB - 8GB
		p.burst = (random() % (long)((50*pow(10,12)) - (10*pow(10,6)))) + (10*pow(10,6));
		processes[i] = p;
	}
	setMaxBurst();
}

process getNewProcess() {
	process p;
	p.memory = (random() % (8388608 - 250)) + 250; // .25MB - 8GB
	p.burst = (random() % (long)((50*pow(10,12)) - (10*pow(10,6)))) + (10*pow(10,6));
	return p;
}

void clearCpus() {
	process zero;
	zero.memory = 0;
	zero.burst = 0;

	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 50; j++)
			cpus[i].queue[j] = zero;
	}
}

void printProcs() {
	int i, j;
	for (i = -1; i < 10; i++) {
		for (j = 0; j < 5; j++) {
			if (i < 0) {
				printf("%22s %d\t", "CPU", j);
				continue;
			}
			printf("m: %7d b: %10lu\t", cpus[j].queue[i].memory, cpus[j].queue[i].burst);
		}
		printf("\n");
	}
}

void printProcsMulti() {
	int i, j, maxCount = 0;
	for (i = 0; i < 5; i++) {
		if (cpus[i].procCount > maxCount)
			maxCount = cpus[i].procCount;
	}

	for (i = -1; i < maxCount; i++) {
		for (j = 0; j < 5; j++) {
			if (i < 0) {
				printf("%22s %d\t", "CPU", j);
				continue;
			}

			if (i < cpus[j].procCount) {
				printf("m: %7d b: %10lu\t", cpus[j].queue[i].memory, cpus[j].queue[i].burst);
			} else {
				printf("%25s", " \t");
			}
		}
		printf("\n");
	}
}

void reorderQueue(process *queue, int length) {
	int i,j,iMin;

	for (i = 0; i < length-1; i++) {
		iMin = i;
		for (j = i+1; j < length; j++) {
			if (queue[j].burst < queue[iMin].burst) {
				iMin = j;
			}

			if (length == 2)
				break;
		}

		if (iMin != i) {
			process temp = queue[i];
			queue[i] = queue[iMin];
			queue[iMin] = temp;
		}

		if (length == 2)
			break;
	}
}

unsigned long long getTurnaroundTimeProb1() { // Return the CPU with the longest time
	unsigned long long turnaround = 0, sum = 0;
	int i, j;
	for (i = 0; i < 5; i++) {
		sum = 0;
		for (j = 0; j < 10; j++) { // 10 process per CPU
			sum += cpus[i].queue[j].burst;
		}
		if (sum > turnaround)
			turnaround = sum;
	}

	return turnaround;
}

unsigned long long getTurnaroundTime() {
	unsigned long long turnaround = 0, sum = 0;
	int i, j;
	for (i = 0; i < 5; i++) {
		sum = 0;
		for (j = 0; j < cpus[i].procCount; j++) { // 10 process per CPU
			sum += cpus[i].queue[j].burst;
		}
		if (sum > turnaround)
			turnaround = sum;
	}

	return turnaround;
}

unsigned long long turnaroundTimeForCpu(int cpu) {
	unsigned long long sum = 0;
	int i;
	for (i = 0; i < cpus[cpu].procCount; i++) {
		sum += cpus[cpu].queue[i].burst;
	}

	return sum;
}

void prob1() { // All CPU's identical
	clearCpus();
	cpus[0].speed = 3 * (long)(pow(10,6));
	cpus[0].memory = 8388608; // 8GB
	cpus[1].speed = 3 * (long)(pow(10,6));
	cpus[1].memory = 8388608; // 8GB
	cpus[2].speed = 3 * (long)(pow(10,6));
	cpus[2].memory = 8388608; // 8GB
	cpus[3].speed = 3 * (long)(pow(10,6));
	cpus[3].memory = 8388608; // 8GB
	cpus[4].speed = 3 * (long)(pow(10,6));
	cpus[4].memory = 8388608; // 8GB

	// SJF
	int i, queuePos = 0;
	for (i = 0; i < 50; i++) {
		cpus[i % 5].queue[queuePos] = processes[i];
		reorderQueue(cpus[i % 5].queue, queuePos+1);

		if ((i+1) % 5 == 0 && i > 0) {
			queuePos++;
		}
	}

	// Get turnaround time in cycles
	unsigned long long turnaround = getTurnaroundTimeProb1();
	printProcs();
	printf("Total time for problem 1 = %llus\n", turnaround / cpus[0].speed);

}

void prob2() {
	clearCpus();
	cpus[0].speed = 3 * (long)(pow(10,6));
	cpus[0].memory = 2097152; // 2GB
	cpus[0].procCount = 0;
	cpus[1].speed = 3 * (long)(pow(10,6));
	cpus[1].memory = 2097152; // 2GB
	cpus[1].procCount = 0;
	cpus[2].speed = 3 * (long)(pow(10,6));
	cpus[2].memory = 4194304; // 4GB
	cpus[2].procCount = 0;
	cpus[3].speed = 3 * (long)(pow(10,6));
	cpus[3].memory = 4194304; // 4GB
	cpus[3].procCount = 0;
	cpus[4].speed = 3 * (long)(pow(10,6));
	cpus[4].memory = 8388608; // 8GB
	cpus[4].procCount = 0;

	int i, cpuI;
	for (i = 0; i < 50; i++) {
		if (processes[i].memory <= 2097152) {
			cpuI = (cpus[0].procCount < cpus[1].procCount) ? 0 : 1;
			cpus[cpuI].queue[cpus[cpuI].procCount] = processes[i];
			cpus[cpuI].procCount++;
			reorderQueue(cpus[cpuI].queue, cpus[cpuI].procCount);
		} else if (processes[i].memory <= 4194304) {
			cpuI = (cpus[2].procCount < cpus[3].procCount) ? 2 : 3;
			cpus[cpuI].queue[cpus[cpuI].procCount] = processes[i];
			cpus[cpuI].procCount++;
			reorderQueue(cpus[cpuI].queue, cpus[cpuI].procCount);
		} else if (processes[i].memory <= 8388608) {
			cpus[4].queue[cpus[4].procCount] = processes[i];
			cpus[4].procCount++;
			reorderQueue(cpus[4].queue, cpus[4].procCount);
		}
	}

	unsigned long long turnaround = getTurnaroundTime();
	printProcsMulti();
	printf("Total time for problem 2 = %llus\n", turnaround / cpus[0].speed);

}

void prob3() {
	clearCpus();
	cpus[0].speed = 2 * (long)(pow(10,6)); // 2 GHz
	cpus[0].memory = 8388608; // 8GB
	cpus[0].procCount = 0;
	cpus[1].speed = 2 * (long)(pow(10,6)); // 2 GHz
	cpus[1].memory = 8388608; // 8GB
	cpus[1].procCount = 0;
	cpus[2].speed = 3 * (long)(pow(10,6)); // 3 GHz
	cpus[2].memory = 8388608; // 8GB
	cpus[2].procCount = 0;
	cpus[3].speed = 3 * (long)(pow(10,6)); // 3 GHz
	cpus[3].memory = 8388608; // 8GB
	cpus[3].procCount = 0;
	cpus[4].speed = 4 * (long)(pow(10,6)); // 4 GHz
	cpus[4].memory = 8388608; // 8GB
	cpus[4].procCount = 0;

	unsigned long thirdOfBurst = maxBurst/3; // 1/3 of longest runtime
	int i, j, cpuI;
	for (i = 0; i < 50; i++) {
		if (processes[i].burst <= thirdOfBurst) {
			cpuI = (cpus[0].procCount < cpus[1].procCount) ? 0 : 1;
			cpus[cpuI].queue[cpus[cpuI].procCount] = processes[i];
			cpus[cpuI].procCount++;
			reorderQueue(cpus[cpuI].queue, cpus[cpuI].procCount);
		} else if (processes[i].burst <= thirdOfBurst*2) {
			cpuI = (cpus[2].procCount < cpus[3].procCount) ? 2 : 3;
			cpus[cpuI].queue[cpus[cpuI].procCount] = processes[i];
			cpus[cpuI].procCount++;
			reorderQueue(cpus[cpuI].queue, cpus[cpuI].procCount);
		} else if (processes[i].burst <= thirdOfBurst*3) {
			cpus[4].queue[cpus[4].procCount] = processes[i];
			cpus[4].procCount++;
			reorderQueue(cpus[4].queue, cpus[4].procCount);
		}
	}

	// balancing
	// Find largest queue
	int maxQueue = 0, minQueue = 0;
	for (i = 0; i < 5; i++) {
		if (cpus[i].procCount > cpus[maxQueue].procCount)
			maxQueue = i;

		if (cpus[i].procCount < cpus[minQueue].procCount)
			minQueue = i;
	}

	// get # to re-schedule
	int n = cpus[maxQueue].procCount - cpus[minQueue].procCount;

	// reschedule
	for (i = 0; i < n; i++) {
		// get process
		process p;
		p.burst = cpus[maxQueue].queue[cpus[maxQueue].procCount-1].burst;
		p.memory = cpus[maxQueue].queue[cpus[maxQueue].procCount-1].memory;
		//cpus[maxQueue].queue[cpus[maxQueue].procCount-1] = {0};
		cpus[maxQueue].procCount--;

		// place on shortest queue
		cpus[minQueue].queue[cpus[minQueue].procCount] = p;
		cpus[minQueue].procCount++;
		reorderQueue(cpus[minQueue].queue, cpus[minQueue].procCount);

		// Find new shortest queue
		for (j = 0; j < 5; j++)
			if (cpus[j].procCount < cpus[minQueue].procCount)
				minQueue = j;
	}

	unsigned long long turnarounds[5];
	for (i = 0; i < 5; i++) {
		turnarounds[i] = turnaroundTimeForCpu(i);
	}

	// convert cycles to real time
	int max = 0;
	for (i = 0; i < 5; ++i) {
		turnarounds[i] = turnarounds[i] / cpus[i].speed;
		if (turnarounds[i] > turnarounds[max])
			max = i;
	}

	printProcsMulti();
	printf("Total time for problem 3 = %llus\n", turnarounds[max]);
}

void prob4() {
	int i = 0;
	while (i < 50) {
		process p = getNewProcess();

		// TODO: Schedule the new process

		i++;
	}
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));

	generateProcesses();
	prob1();
	prob2();
	prob3();

	return 0;
}