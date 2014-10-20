#include <stdio.h>
#include <math.h>
#include <time.h>

typedef struct {
	unsigned int burst;
	unsigned int memory;
} process;

typedef struct {
	unsigned int speed;
	unsigned int memory;
	process queue[50];
	int procCount;
} CPU;

process processes[50];
CPU cpus[5];

void generateProcesses() {
	int i;
	for (i = 0; i < 50; i++) {
		process p;
		p.memory = (rand() % (8388608 - 250)) + 250; // .25MB - 8GB
		p.burst = (rand() % (int)((50*pow(10,12)) - (10*pow(10,6)))) + (10*pow(10,6));
		processes[i] = p;
	}
}

process getNewProcess() {
	process p;
	p.memory = (rand() % (8388608 - 250)) + 250; // .25MB - 8GB
	p.burst = (rand() % (int)((50*pow(10,12)) - (10*pow(10,6)))) + (10*pow(10,6));
	return p;
}

void printProcs() {
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 5; j++) {
			printf("m: %7d b: %10d\t", cpus[j].queue[i].memory, cpus[j].queue[i].burst);
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

unsigned int getTurnaroundTime() {
	unsigned int turnaround = 0, sum = 0, i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 10; j++) { // 10 process per CPU
			sum += cpus[i].queue[j].burst;
		}
		if (sum > turnaround)
			turnaround = sum;
	}

	return turnaround;
}

void prob1() { // All CPU's identical
	cpus[0].speed = 3 * (int)(pow(10,6));
	cpus[0].memory = 8388608; // 8GB
	cpus[1].speed = 3 * (int)(pow(10,6));
	cpus[1].memory = 8388608; // 8GB
	cpus[2].speed = 3 * (int)(pow(10,6));
	cpus[2].memory = 8388608; // 8GB
	cpus[3].speed = 3 * (int)(pow(10,6));
	cpus[3].memory = 8388608; // 8GB
	cpus[4].speed = 3 * (int)(pow(10,6));
	cpus[4].memory = 8388608; // 8GB

	// SJF
	int i, queuePos = 0;
	for (i = 0; i < 50; i++) {
		cpus[i % 5].queue[queuePos] = processes[i];
		reorderQueue(cpus[i % 5].queue, (i % 10)+1);

		if (i % 5 == 0 && i > 0) {
			queuePos++;
			printf("queuePos: %d", queuePos);
		}
	}

	// Get turnaround time in cycles
	unsigned int turnaround = getTurnaroundTime();
	printProcs();
	printf("Total time for problem 1 = %ds\n", turnaround / cpus[0].speed);

}

void prob2() {
	cpus[0].speed = 3 * (int)(pow(10,6));
	cpus[0].memory = 2097152; // 2GB
	cpus[0].procCount = 0;
	cpus[1].speed = 3 * (int)(pow(10,6));
	cpus[1].memory = 2097152; // 2GB
	cpus[1].procCount = 0;
	cpus[2].speed = 3 * (int)(pow(10,6));
	cpus[2].memory = 4194304; // 4GB
	cpus[2].procCount = 0;
	cpus[3].speed = 3 * (int)(pow(10,6));
	cpus[3].memory = 4194304; // 4GB
	cpus[3].procCount = 0;
	cpus[4].speed = 3 * (int)(pow(10,6));
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

	unsigned int turnaround = getTurnaroundTime();

	printf("Total time for problem 2 = %ds\n", turnaround / cpus[0].speed);

}

void prob3() {
	cpus[0].speed = 2 * (int)(pow(10,6)); // 2 GHz
	cpus[0].memory = 8388608; // 8GB
	cpus[0].procCount = 0;
	cpus[1].speed = 2 * (int)(pow(10,6)); // 2 GHz
	cpus[1].memory = 8388608; // 8GB
	cpus[1].procCount = 0;
	cpus[2].speed = 3 * (int)(pow(10,6)); // 3 GHz
	cpus[2].memory = 8388608; // 8GB
	cpus[2].procCount = 0;
	cpus[3].speed = 3 * (int)(pow(10,6)); // 3 GHz
	cpus[3].memory = 8388608; // 8GB
	cpus[3].procCount = 0;
	cpus[4].speed = 4 * (int)(pow(10,6)); // 4 GHz
	cpus[4].memory = 8388608; // 8GB
	cpus[4].procCount = 0;

	int i, cpuI;
	for (i = 0; i < 50; i++) {

	}
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

	return 0;
}