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
	int i;
	for (i = 0; i < 50; i++) {
		printf("p%d = burst: %d memory: %d\n", i, processes[i].burst, processes[i].memory);
	}
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

}

void prob2() {
	cpus[0].speed = 3 * (int)(pow(10,6));
	cpus[0].memory = 2097152; // 2GB
	cpus[1].speed = 3 * (int)(pow(10,6));
	cpus[1].memory = 2097152; // 2GB
	cpus[2].speed = 3 * (int)(pow(10,6));
	cpus[2].memory = 4194304; // 4GB
	cpus[3].speed = 3 * (int)(pow(10,6));
	cpus[3].memory = 4194304; // 4GB
	cpus[4].speed = 3 * (int)(pow(10,6));
	cpus[4].memory = 8388608; // 8GB
}

void prob3() {
	cpus[0].speed = 2 * (int)(pow(10,6)); // 2 GHz
	cpus[0].memory = 8388608; // 8GB
	cpus[1].speed = 2 * (int)(pow(10,6)); // 2 GHz
	cpus[1].memory = 8388608; // 8GB
	cpus[2].speed = 3 * (int)(pow(10,6)); // 3 GHz
	cpus[2].memory = 8388608; // 8GB
	cpus[3].speed = 3 * (int)(pow(10,6)); // 3 GHz
	cpus[3].memory = 8388608; // 8GB
	cpus[4].speed = 4 * (int)(pow(10,6)); // 4 GHz
	cpus[4].memory = 8388608; // 8GB
}

void prob4() {
	int i = 0;
	while (i < 50) {
		processes p = getNewProcess();

		// TODO: Schedule the new process

		i++;
	}
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));

	generateProcesses();
	printProcs();

	return 0;
}