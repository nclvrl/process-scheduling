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

void simulateFCFS(int numTest, TestCase& testCase) {
    // TODO: implement FCFS logic
}

void simulateSJF(int numTest, TestCase& testCase) {
    // TODO: implement SJF logic
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
        if (algorithmName == "RR") {
            cin >> timeQuantum;
        }

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