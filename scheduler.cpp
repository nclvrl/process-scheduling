#include <string>
#include <iostream> 
using namespace std;

struct Process {
    int index;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int niceLevel;
    int startTime = -1;
    int completionTime = 0;
    int waitingTime = 0;
    int turnaroundTime = 0;
    int responseTime = 0;

    Process(int index, int arrivalTime, int burstTime, int niceLevel) {
        this->index = index;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
        this->remainingTime = burstTime;
        this->niceLevel = niceLevel;
    }
};

struct TestCase {
    string algorithmName;
    int timeQuantum;
    Process** processList;
    int processCount;

    TestCase(int processCount, string algorithmName) {
        this->algorithmName = algorithmName;
        this->timeQuantum = 0;
        this->processCount = processCount;
        processList = new Process*[processCount];
    }

    void addProcess(int index, Process* process) {
        processList[index] = process;
    }

    ~TestCase() {
        for (int i = 0; i < processCount; i++) {
            delete processList[i];
        }
        delete[] processList;
    }
};

void calculateMetrics(Process* processes[], int n) {
    for (int i = 0; i < n; i++) {
        Process* process = processes[i];

        if (process->startTime == -1) process->startTime = process->completionTime - process->burstTime;

        process->turnaroundTime = process->completionTime - process->arrivalTime;
        process->waitingTime = process->turnaroundTime - process->burstTime;
        process->responseTime = process->startTime - process->arrivalTime;
    }
}

void printResults(TestCase& testCase, int totalTime) {
    int n = testCase.processCount;
    Process** processes = testCase.processList;

    cout << n << " " << testCase.algorithmName << endl;

    int totalBurstTime = 0;
    for (int i = 0; i < n; i++) totalBurstTime += processes[i]->burstTime;

    double cpuUtilization = (double)totalBurstTime / totalTime * 100;
    double throughput = (double)n / totalTime;

    cout << "Total time elapsed: " << totalTime << "ns" << endl;
    cout << "Total CPU burst time: " << totalBurstTime << "ns" << endl;
    cout << "CPU Utilization: " << (int)cpuUtilization << "%" << endl;
    cout << "Throughput: " << throughput << " processes/ns" << endl;

    cout << "Waiting times:" << endl;
    double totalWaiting = 0;
    for (int i = 0; i < n; i++) {
        cout << " Process " << processes[i]->index << ": " << processes[i]->waitingTime << "ns" << endl;
        totalWaiting += processes[i]->waitingTime;
    }
    cout << "Average waiting time: " << totalWaiting / n << "ns" << endl;

    cout << "Turnaround times:" << endl;
    double totalTurnaround = 0;
    for (int i = 0; i < n; i++) {
        cout << " Process " << processes[i]->index << ": " << processes[i]->turnaroundTime << "ns" << endl;
        totalTurnaround += processes[i]->turnaroundTime;
    }
    cout << "Average turnaround time: " << totalTurnaround / n << "ns" << endl;

    cout << "Response times:" << endl;
    double totalResponse = 0;
    for (int i = 0; i < n; i++) {
        cout << " Process " << processes[i]->index << ": " << processes[i]->responseTime << "ns" << endl;
        totalResponse += processes[i]->responseTime;
    }
    cout << "Average response time: " << totalResponse / n << "ns" << endl;
}

void simulateFCFS(int numTest, TestCase& testCase) {
    // TODO: implement FCFS logic
}

void simulateSJF(int numTest, TestCase& testCase) {
    // TODO: implement SJF logic
    int n = testCase.processCount;
    Process** processes = testCase.processList;
    int completedProcesses;
    bool swapped;
    for(int i=0; i < n - 1; i++) {
        swapped = false;
        for(int j=0; j < n - i - 1; j++) {
            if(processes[j]->arrivalTime > processes[j+1]->arrivalTime) {
                Process* tempProcess = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = tempProcess;
                swapped = true;
            }
            else if(processes[j]->arrivalTime == processes[j+1]->arrivalTime) {
                if(processes[j]->burstTime > processes[j+1]->burstTime) {
                    Process* tempProcess = processes[j];
                    processes[j] = processes[j+1];
                    processes[j+1] = tempProcess;
                    swapped = true;
                }
                else if(processes[j]->index > processes[j+1]->index) {
                    Process* tempProcess = processes[j];
                    processes[j] = processes[j+1];
                    processes[j+1] = tempProcess;
                    swapped = true;
                }
            }
        }
        if(!swapped) {
            break;
        }
    }
    // TODO:
    // Assuming processes are sorted by FCFS standards, run first process
    // after process is done, look at ready queue
    // compare processes in ready queue with burst times
    //     pick process with lowest

    
    int totalTime;
    int minimumBurst = 9999;
    if(totalTime < processes[0]->arrivalTime) {
            totalTime = processes[0]->arrivalTime;
        }
    processes[0]->startTime=totalTime;
    while(completedProcesses < n) {
        for(int i=0; i < n; i++) {
        if(processes[i]->arrivalTime <= totalTime && processes[i]->burstTime < minimumBurst && processes[i]->remainingTime > 0) {
            minimumBurst = processes[i]->burstTime;
        }

    }

    }


    // TODO: printouts
    cout << numTest << " SJF";
    for(int i=0; i < n; i++) {
        if(totalTime < processes[i]->arrivalTime) {
            totalTime = processes[i]->arrivalTime;
        }
        processes[i]->startTime=totalTime;
        cout << totalTime << " " << processes[i]->index << " " << processes[i]->burstTime << "X" << endl;
        totalTime += processes[i]->burstTime;

    }
    calculateMetrics(processes, n);
    printResults(testCase, totalTime);
}

void simulateSRTF(int numTest, TestCase& testCase) {
    // TODO: implement SRTF logic
}

void simulatePriority(int numTest, TestCase& testCase) {
    // TODO: implement Priority scheduling logic
}

void simulateRR(int numTest, TestCase& testCase) {
    // TODO: implement Round Robin logic
}

int main() {
    int numTestCase;
    cin >> numTestCase;

    for (int i = 0; i < numTestCase; i++) {
        int processCount;
        string algorithmName;

        cin >> processCount >> algorithmName;

        int timeQuantum = 0;
        if (algorithmName == "RR") cin >> timeQuantum;

        TestCase testCase(processCount, algorithmName);
        testCase.timeQuantum = timeQuantum;

        for (int j = 0; j < processCount; j++) {
            int arrivalTime, burstTime, niceLevel;
            cin >> arrivalTime >> burstTime >> niceLevel;

            Process* process = new Process(j + 1, arrivalTime, burstTime, niceLevel);
            testCase.addProcess(j, process);
        }

        if (algorithmName == "FCFS") simulateFCFS(i + 1, testCase);
        else if (algorithmName == "SJF") simulateSJF(i + 1, testCase);
        else if (algorithmName == "SRTF") simulateSRTF(i + 1, testCase);
        else if (algorithmName == "P") simulatePriority(i + 1, testCase);
        else if (algorithmName == "RR") simulateRR(i + 1, testCase);
    }
    
    return 0;
}