#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int creationTime;
    int duration;
    int waitingTime;
    int priority;
    int turnaroundTime;
    int id;
} Process;

typedef struct {
    Process *processes;
    int size;
    int numProcess;
} ProcessArray;

void initArray(ProcessArray *a, size_t initialSize) {
    a->processes = malloc(initialSize * sizeof(Process));
    a->numProcess = 0;
    a->size = initialSize;
}

void insertArray(ProcessArray *a, Process element) {
    if (a->numProcess == a->size) {
        a->size *= 2;
        a->processes = realloc(a->processes, a->size * sizeof(Process));
    }
    a->processes[a->numProcess++] = element;
}

void freeArray(ProcessArray *a) {
    free(a->processes);
    a->processes = NULL;
    a->numProcess = a->size = 0;
}

ProcessArray processArray;

void readFile() {
    char line[10], *file;
    int i = 1;
    FILE *input;

    initArray(&processArray, 1);

    // Abre arquivo para leitura
    input = fopen("input.txt", "rt");
    if (input == NULL) {
        printf("Arquivo nao pode ser aberto!\n");
        return;
    }

    // Insere dados do arquivo nos vetores
    while (!feof(input)) {
        file = fgets(line, 10, input);
        if (file) {
            Process process;
            process.turnaroundTime = 0;
            process.creationTime = (int)(file[0]) - 48;
            process.duration = (int)file[2] - 48;
            process.priority = (int)file[4] - 48;
            process.id = i;
            insertArray(&processArray, process);
        }
        i++;
    }

    fclose(input);
}

void selectionSort() {
    int i, j, min;
    Process *pMin;
    // Ordem crescente de acordo com prioridade

    Process *p = &processArray.processes[0];  // Endereço do processo p
    min = 0;
    for (j = 1; j < processArray.numProcess; j++)
        if (processArray.processes[j].creationTime < processArray.processes[min].creationTime)
            min = j;
        else if (processArray.processes[j].creationTime == processArray.processes[min].creationTime)
            if (processArray.processes[j].priority > processArray.processes[min].priority)
                min = j;
    pMin = &processArray.processes[min];  // Endereço do processo min
    Process aux;
    aux = *p;               // aux recebe valor do processo p
    *p = *pMin;             // Endereço de p recebe valor do processo min
    *pMin = aux;            // Endereço min recebe valor do processo aux
    pMin->waitingTime = 0;  // Tempo de espera para primeiro processo é nulo
    pMin->turnaroundTime = pMin->duration + pMin->waitingTime + pMin->creationTime;

    for (i = 1; i < processArray.numProcess; i++) {
        Process *ant = &processArray.processes[i - 1];
        int next = i;
        for (j = i; j < processArray.numProcess; j++) {
            if (processArray.processes[j].creationTime <= ant->turnaroundTime) {
                if (processArray.processes[j].priority > processArray.processes[next].priority)
                    next = j;
            }
        }
        Process *p = &processArray.processes[i];
        if (next != i) {
            Process *p2 = &processArray.processes[next];
            aux = *p;  // aux recebe valor do processo p
            *p = *p2;  // Endereço de p recebe valor do processo min
            *p2 = aux;
        }
        p->waitingTime = ant->turnaroundTime - p->creationTime;
        if (p->waitingTime < 0)
            p->waitingTime = 0;
        p->turnaroundTime = p->duration + p->waitingTime + p->creationTime;
    }
}

int averageWaitingTime() {
    float total = 0;
    int i, j;
    Process *processes = processArray.processes;

    // Soma das durações para cálculo do tempo médio de espera
    for (i = 1; i < processArray.numProcess; i++)
        total += processes[i].waitingTime;

    return total / (float)processArray.numProcess;
}

int averageTurnaroundTime() {
    float total = 0;
    int i;
    Process *processes = processArray.processes;

    // Soma do tempo de crição,durações e tempo de espera para cálculo do tempo médio de vida
    for (i = 0; i < processArray.numProcess; i++)
        total += processes[i].turnaroundTime;

    return total / (float)processArray.numProcess;
}

void writeFile(float average_waiting_time, float average_turnaround_time) {
    FILE *output;
    int i, j, tempMax;
    tempMax = 0;
    // Abre arquivo de saída para escrita
    output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Arquivo nao pode ser aberto!\n");
        return;
    }
    for (i = 0; i < processArray.numProcess; i++) {
        int pos = i;
        for (j = i + 1; j < processArray.numProcess; j++) {
            if (processArray.processes[j].id < processArray.processes[pos].id) {
                pos = j;
            }
        }
        Process *p = &processArray.processes[i];  // Endereço do processo p
        Process *p2 = &processArray.processes[pos];
        Process aux;
        aux = *p;  // aux recebe valor do processo p
        *p = *p2;  // Endereço de p recebe valor do processo min
        *p2 = aux;
    }

    fprintf(output, "Tempo\t");
    for (i = 0; i < processArray.numProcess; i++) {
        fprintf(output, "\tP%d", processArray.processes[i].id);
        if (tempMax < processArray.processes[i].turnaroundTime)
            tempMax = processArray.processes[i].turnaroundTime;
    }
    fprintf(output, "\n");
    int timer = 1;

    while (timer <= tempMax) {
        fprintf(output, "%2.d-%2.d\t", timer - 1, timer);
        for (i = 0; i < processArray.numProcess; i++) {
            Process p = processArray.processes[i];
            fprintf(output, "\t");
            if (p.creationTime <= timer && (p.waitingTime + p.creationTime) >= timer)
                fprintf(output, "--");
            else if ((p.creationTime <= timer) && ((p.waitingTime + p.creationTime) < timer) && (timer <= p.turnaroundTime))
                fprintf(output, "##");
            else
                fprintf(output, "\t");
        }
        fprintf(output, "\n");
        timer++;
    }

    fprintf(output, "\nTempo medio de vida = %.2f", average_turnaround_time);
    fprintf(output, "\nTempo medio de espera = %.2f", average_waiting_time);
    fprintf(output, "\nNumero de trocas de contexto = %d\n", processArray.numProcess - 1);

    fclose(output);
}

int main() {
    float average_waiting_time, average_turnaround_time;
    int i;

    // Abre e lê as informações no arquivo de entrada
    readFile();

    // Coloca em ordem crescente os processos
    selectionSort();

    // Calcula o tempo médio de espera
    average_waiting_time = averageWaitingTime();

    // Calcula o tempo médio de vida
    average_turnaround_time = averageTurnaroundTime();

    // Escreve os resultados no arquivo de saída
    writeFile(average_waiting_time, average_turnaround_time);

    return 0;
}