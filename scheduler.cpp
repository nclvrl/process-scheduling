#include <string>
#include <iostream> 
#include <deque>
#include <algorithm>
#include <queue>
#include <iomanip>
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
    bool queueStatus = false;

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

struct CompareRemainingTime {
    bool operator()(Process* a, Process* b) {
        if (a->remainingTime != b->remainingTime)
            return a->remainingTime > b->remainingTime;
        if (a->arrivalTime != b->arrivalTime)
            return a->arrivalTime > b->arrivalTime;
        return a->index > b->index;
    }
};

struct CompareNiceLevel {
    bool operator()(Process* a, Process* b) {
        if (a->niceLevel != b->niceLevel)
            return a->niceLevel > b->niceLevel;
        if (a->arrivalTime != b->arrivalTime)
            return a->arrivalTime > b->arrivalTime;
        return a->index > b->index;
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

    sort(processes, processes + n, [](Process* a, Process* b) {
        return a->index < b->index;
    });

    int totalBurstTime = 0;
    for (int i = 0; i < n; i++) totalBurstTime += processes[i]->burstTime;

    double cpuUtilization = (double)totalBurstTime / totalTime * 100;
    double throughput = (double)n / totalTime;

    cout << "Total time elapsed: " << totalTime << "ns" << endl;
    cout << "Total CPU burst time: " << totalBurstTime << "ns" << endl;
    cout << "CPU Utilization: " << (int)cpuUtilization << "%" << endl;
    cout << "Throughput: " << setprecision(16) << throughput << " processes/ns" << endl;

    cout << setprecision(2) << "Waiting times:" << endl;
    double totalWaiting = 0;
    for (int i = 0; i < n; i++) {
        cout << " Process " << processes[i]->index << ": " << processes[i]->waitingTime << "ns" << endl;
        totalWaiting += processes[i]->waitingTime;
    }
    cout << defaultfloat << "Average waiting time: " << setprecision(16) << totalWaiting / n << "ns" << endl;

    cout <<  setprecision(4) << "Turnaround times:" << endl;
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
    cout << numTest << " " << testCase.algorithmName << endl;
    int n = testCase.processCount;
    Process** processes = testCase.processList;

    sort(processes, processes + n, [](Process* a, Process* b) {
        if (a->arrivalTime != b->arrivalTime) return a->arrivalTime < b->arrivalTime;
        else return a->index < b->index;
    });

    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        Process* process = processes[i];
        if (currentTime < process->arrivalTime)
            currentTime = process->arrivalTime;
        process->startTime = currentTime;
        cout << currentTime << " " << processes[i]->index << " " << processes[i]->burstTime << "X" << endl;
        process->completionTime = currentTime + process->burstTime;

        currentTime += process->burstTime;
    }
    int totalTime = currentTime;
    calculateMetrics(processes, n);
    printResults(testCase, totalTime);
}

void simulateSJF(int numTest, TestCase& testCase) {
    int n = testCase.processCount;
    Process** processes = testCase.processList;
    int completedProcesses;
    sort(processes, processes + n, [](Process* a, Process* b) {
        if (a->arrivalTime != b->arrivalTime) return a->arrivalTime < b->arrivalTime;
        else return a->index < b->index;
    });
    int totalTime = 0;
    cout << numTest << " " << testCase.algorithmName << endl;
    while(completedProcesses < n) {
        int minimumBurst = 9999;   
        int shortestProcess = -1;
        for(int i=0; i < n; i++) {
            if(processes[i]->arrivalTime <= totalTime && processes[i]->burstTime < minimumBurst && processes[i]->remainingTime > 0) {
                minimumBurst = processes[i]->burstTime;
                shortestProcess = i;
            }
        }
        if(shortestProcess == -1) {
            totalTime++;
            continue;
        }
        processes[shortestProcess]->startTime=totalTime;
        cout << totalTime << " " << processes[shortestProcess]->index << " " << processes[shortestProcess]->burstTime << "X" << endl;
        totalTime += processes[shortestProcess]->burstTime;
        processes[shortestProcess]->remainingTime = 0;
        processes[shortestProcess]->completionTime = totalTime;
        completedProcesses++;
    }
    calculateMetrics(processes, n);
    printResults(testCase, totalTime);
}

void simulateSRTF(int numTest, TestCase& testCase) {
    cout << numTest << " " << testCase.algorithmName << endl;

    // Setup local view
    Process** sortedView = new Process*[testCase.processCount];
    for (int i = 0; i < testCase.processCount; i++) {
        sortedView[i] = testCase.processList[i];
    }

    // Sort by arrival time, then by index
    sort(sortedView, sortedView + testCase.processCount, [](Process* a, Process* b) {
        if (a->arrivalTime != b->arrivalTime) return a->arrivalTime < b->arrivalTime;
        return a->index < b->index;
    });

    // Priority queue to manage ready processes, ordered by shortest remaining time then index
    priority_queue<Process*, vector<Process*>, CompareRemainingTime> readyQueue;
    int totalTime = 0;
    int nextArrivalIndex = 0;
    int completedProcesses = 0;

    // Tracker variables to stitch continuous bursts interrupted by preemption checks
    Process* lastRunningProcess = nullptr;
    int burstStartTime = 0;

    // Main simulation loop: runs until all processes are completed
    while (completedProcesses < testCase.processCount) {

        // Add newly arrived processes to the ready queue
        while (nextArrivalIndex < testCase.processCount && sortedView[nextArrivalIndex]->arrivalTime <= totalTime) {
            readyQueue.push(sortedView[nextArrivalIndex]);
            nextArrivalIndex++;
        }

        // If no processes are ready, jump forward in time to the next arrival
        if (readyQueue.empty()) {
            if (nextArrivalIndex < testCase.processCount) {
                // If we were running something and CPU becomes idle, print end time of process nbjh.
                if (lastRunningProcess != nullptr) {
                    int duration = totalTime - burstStartTime;
                    cout << burstStartTime << " " << lastRunningProcess->index << " " << duration << endl;
                    lastRunningProcess = nullptr;
                }
                totalTime = sortedView[nextArrivalIndex]->arrivalTime;
                continue; 
            }
            break;
        }

        Process* runningProcess = readyQueue.top();

        // Determine if we switched to a different process than last time
        if (runningProcess != lastRunningProcess) {
            // If something was running before, print its burst duration
            if (lastRunningProcess != nullptr) {
                int duration = totalTime - burstStartTime;
                cout << burstStartTime << " " << lastRunningProcess->index << " " << duration << endl;
            }
            
            burstStartTime = totalTime;
            lastRunningProcess = runningProcess;

            // If this is the first time this process is running, set its start time and response time
            if (runningProcess->startTime == -1) {
                runningProcess->startTime = totalTime;
                runningProcess->responseTime = totalTime - runningProcess->arrivalTime;
            }
        }

        // Calculate time until the next event where either current  process finishes or a new process arrives that could preempt it
        int timeToFinish = runningProcess->remainingTime;
        bool arrivalsLeft = (nextArrivalIndex < testCase.processCount);
        // If there are no more arrivals, just run to completion. Otherwise, check if the next arrival happens before we finish.
        int timeToNextArrival = arrivalsLeft ? (sortedView[nextArrivalIndex]->arrivalTime - totalTime) : (timeToFinish + 1);

        // If the current process finishes before the next arrival, we can run it to completion. Otherwise, run until next arrival and then re-evaluate the queue.
        if (timeToFinish <= timeToNextArrival) {
            // Current process finishes
            int duration = (totalTime + timeToFinish) - burstStartTime;
            cout << burstStartTime << " " << runningProcess->index << " " << duration << "X" << endl;

            totalTime += timeToFinish;
            runningProcess->remainingTime = 0;
            runningProcess->completionTime = totalTime;
            
            // Compute metrics for the completed process
            runningProcess->turnaroundTime = runningProcess->completionTime - runningProcess->arrivalTime;
            runningProcess->waitingTime = runningProcess->turnaroundTime - runningProcess->burstTime;
            
            readyQueue.pop();
            completedProcesses++;
            lastRunningProcess = nullptr; // CPU can now pick someone else
        } 
        else {
            // An arrival happens, pause to check the queue
            totalTime += timeToNextArrival;
            runningProcess->remainingTime -= timeToNextArrival;
            
            // The next iteration of the while loop will add the new process and potentially preempt according to Priority Scheduling rules
        }
    }

    printResults(testCase, totalTime);

    delete[] sortedView;
    sortedView = nullptr;
}


void simulatePriority(int numTest, TestCase& testCase) {
    cout << numTest << " " << testCase.algorithmName << endl;
    // Setup local view
    Process** sortedView = new Process*[testCase.processCount];
    for (int i = 0; i < testCase.processCount; i++) {
        sortedView[i] = testCase.processList[i];
    }

    // Sort by arrival time, then by index
    sort(sortedView, sortedView + testCase.processCount, [](Process* a, Process* b) {
        if (a->arrivalTime != b->arrivalTime) return a->arrivalTime < b->arrivalTime;
        return a->index < b->index;
    });

    // Priority queue to manage ready processes, ordered by nice level then index
    priority_queue<Process*, vector<Process*>, CompareNiceLevel> readyQueue;
    int totalTime = 0;
    int nextArrivalIndex = 0;
    int completedProcesses = 0;

    // Tracker variables to stitch continuous bursts interrupted by preemption checks
    Process* lastRunningProcess = nullptr;
    int burstStartTime = 0;

    // Main simulation loop: runs until all processes are completed
    while (completedProcesses < testCase.processCount) {

        // Add newly arrived processes to the ready queue
        while (nextArrivalIndex < testCase.processCount && sortedView[nextArrivalIndex]->arrivalTime <= totalTime) {
            readyQueue.push(sortedView[nextArrivalIndex]);
            nextArrivalIndex++;
        }

        // If no processes are ready, jump forward in time to the next arrival
        if (readyQueue.empty()) {
            if (nextArrivalIndex < testCase.processCount) {
                // If we were running something and CPU becomes idle, print end time of process nbjh.
                if (lastRunningProcess != nullptr) {
                    int duration = totalTime - burstStartTime;
                    cout << burstStartTime << " " << lastRunningProcess->index << " " << duration << endl;
                    lastRunningProcess = nullptr;
                }
                totalTime = sortedView[nextArrivalIndex]->arrivalTime;
                continue; 
            }
            break;
        }

        Process* runningProcess = readyQueue.top();

        // Determine if we switched to a different process than last time
        if (runningProcess != lastRunningProcess) {
            // If something was running before, print its burst duration
            if (lastRunningProcess != nullptr) {
                int duration = totalTime - burstStartTime;
                cout << burstStartTime << " " << lastRunningProcess->index << " " << duration << endl;
            }
            
            burstStartTime = totalTime;
            lastRunningProcess = runningProcess;

            // If this is the first time this process is running, set its start time and response time
            if (runningProcess->startTime == -1) {
                runningProcess->startTime = totalTime;
                runningProcess->responseTime = totalTime - runningProcess->arrivalTime;
            }
        }

        // Calculate time until the next event where either current  process finishes or a new process arrives that could preempt it
        int timeToFinish = runningProcess->remainingTime;
        bool arrivalsLeft = (nextArrivalIndex < testCase.processCount);
        // If there are no more arrivals, just run to completion. Otherwise, check if the next arrival happens before we finish.
        int timeToNextArrival = arrivalsLeft ? (sortedView[nextArrivalIndex]->arrivalTime - totalTime) : (timeToFinish + 1);

        // If the current process finishes before the next arrival, we can run it to completion. Otherwise, run until  next arrival and then re-evaluate the queue.
        if (timeToFinish <= timeToNextArrival) {
            // Current process finishes
            int duration = (totalTime + timeToFinish) - burstStartTime;
            cout << burstStartTime << " " << runningProcess->index << " " << duration << "X" << endl;

            totalTime += timeToFinish;
            runningProcess->remainingTime = 0;
            runningProcess->completionTime = totalTime;
            
            // Compute metrics for the completed process
            runningProcess->turnaroundTime = runningProcess->completionTime - runningProcess->arrivalTime;
            runningProcess->waitingTime = runningProcess->turnaroundTime - runningProcess->burstTime;
            
            readyQueue.pop();
            completedProcesses++;
            lastRunningProcess = nullptr; // CPU can now pick someone else
        } 
        else {
            // An arrival happens, pause to check the queue
            totalTime += timeToNextArrival;
            runningProcess->remainingTime -= timeToNextArrival;
            
            // The next iteration of the while loop will add the new process and potentially preempt according to SRTF rules
        }
    }

    printResults(testCase, totalTime);

    delete[] sortedView;
    sortedView = nullptr;
}

void simulateRR(int numTest, TestCase& testCase) {
    int n = testCase.processCount;
    Process** processes = testCase.processList;
    int timeQuantum = testCase.timeQuantum;
    int completedProcesses = 0;
    int totalTime = 0;
    sort(processes, processes + n, [](Process* a, Process* b) {
        if (a->arrivalTime != b->arrivalTime) return a->arrivalTime < b->arrivalTime;
        else return a->index < b->index;
    });
    cout << numTest << " " << testCase.algorithmName << endl;
    deque<Process*> readyQueue;
    // TODO:
    // add processes to ready queue once they arrive
     while(completedProcesses < n) {
        for(int i=0; i < n; i++) {
            if(processes[i]->queueStatus == false && processes[i]->remainingTime > 0 && processes[i]-> arrivalTime <= totalTime) {
                if(processes[i]->startTime == -1) {
                    processes[i]->startTime = totalTime;
                }
                readyQueue.push_back(processes[i]);
                processes[i]->queueStatus = true;
            }
        }
        if(readyQueue.empty()) {
            totalTime++;
            continue;
        }
        Process* currentProcess = readyQueue.front();
        readyQueue.pop_front();
        if(currentProcess->remainingTime > timeQuantum) {
            cout << totalTime << " " << currentProcess->index << " " << timeQuantum << endl;
            totalTime += timeQuantum;
            currentProcess->remainingTime -= timeQuantum;
        }
        else if(currentProcess->remainingTime <= timeQuantum) {
            cout << totalTime << " " << currentProcess->index << " " << currentProcess->remainingTime << "X" << endl;
            totalTime += currentProcess->remainingTime;
            currentProcess->completionTime = totalTime;
            currentProcess->remainingTime = 0;
            completedProcesses++;
        }
        for(int i=0; i < n; i++) {
            if(processes[i]->queueStatus == false && processes[i]->remainingTime > 0 && processes[i]-> arrivalTime <= totalTime) {
                if(processes[i]->startTime == -1) {
                    processes[i]->startTime = totalTime;
                }
                readyQueue.push_front(processes[i]);
                processes[i]->queueStatus = true;
            }
        }
        if(currentProcess->remainingTime > 0) {
            readyQueue.push_back(currentProcess);
        }
     }
    calculateMetrics(processes, n);
    printResults(testCase, totalTime);
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