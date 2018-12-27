/*
    Operating Systems - Assignment 1
    Pre-Emptive Shortest Job First Scheduling

    Student:
    Name: Osman Araz
    No: 16011020
    Delivery Date: 24.12.2019
*/

/*
    Input examples:
    input #1: 7 p1 8 1 p2 5 1 p3 2 7 p4 4 3 p5 2 8 p6 4 2 p7 3 5
    input #2: 4 p1 0 8 p2 1 4 p3 2 9 p4 3 5
    input #3: 4 p1 1 6 p2 1 8 p3 2 7 p4 3 3
    Just copy one of the inputs above and past that to the executed program screen and hit enter.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>

struct Job
{
    std::string name;
    double arrival, burst, burstBeginning, waiting;

    // constructors:
    Job() = default;
    Job(const std::string &n, int arrv, int brst): name(n), arrival(arrv), burst(brst), burstBeginning(brst), waiting(0) {}
};

void newJobComes(std::vector <Job> &jobs, int N, int first, int last);
void processJob(std::vector <Job> &jobs, int N, int &first, int last);
void printSchedule(const std::vector <Job> &jobs, int N);
int main()
{
    int N;
    std::cout << "Enter the total number of jobs: ";
    std::cin >> N;
    std::vector <Job> jobs(N);

    for (int i = 0; i < N; ++i) // getting the jobs from the user
    {
        std::string p_name;
        double p_arrival, p_burst;
        std::cout << "\np_name of the job #" << i+1 << ": ";
        std::cin >> p_name;
        std::cout << "p_arrival of the job #" << i+1 << ": ";
        std::cin >> p_arrival;
        std::cout << "p_burst of the job #" << i+1 << ": ";
        std::cin >> p_burst;

        jobs[i] = Job(p_name, p_arrival, p_burst);
    }

    // sorting the jobs according to their arrival times
    std::sort(jobs.begin(), jobs.end(), [] (const Job &job1, const Job &job2) { return job1.arrival < job2.arrival; });
    
    int first = 0, last = 0;  // the range of active jobs (last is not inclusive)
    // an active job means that the current time passed the arrival time of the job  
    // and the job hasn't finished yet

    // processing the jobs till the time become to the longest arrival time 
    for (int t = 0; t <= jobs[N-1].arrival; ++t) // t represents time
    {
        // when the current time has become to the arrival time of a job/jobs
        // that/those job/jobs become active
        for (int i = last; i < N && jobs[i].arrival == t; ++i)
            // incrementing the last, which represents the upper bound (not inclusive) of active jobs
            // and repositioning the new job according to its burst time in a function
            newJobComes(jobs, N, first, ++last);

        if (first < last)   // if there are active job/jobs, process the first one of them
            processJob(jobs, N, first, last);
    }

    while (first < last)   // processing remaining active jobs
        processJob(jobs, N, first, last);

    // sorting the jobs according to their arrival times, to display results to the user
    std::sort(jobs.begin(), jobs.end(), [] (const Job &job1, const Job &job2) { return job1.arrival < job2.arrival; });

    printSchedule(jobs, N);

    return 0;
}

/*
    A function which repositions a job in the job list according to its burst time.
    It works like insertion part of insertion sort.
*/
void newJobComes(std::vector <Job> &jobs, int N, int first, int last)
{
    Job newJob = jobs[last-1];
    int j = last-2;

    while (j >= first && newJob.burst < jobs[j].burst)
    {
        jobs[j+1] = jobs[j];
        --j;
    }

    jobs[j+1] = newJob;
}

/*
    A function which processes a job (decrementing its burst time) and incrementing 
    waiting times of the other active jobs. (The "first" variable sent with its reference
    because it may change in the function.)
*/
void processJob(std::vector <Job> &jobs, int N, int &first, int last)
{
    for (int i = first+1; i < last; ++i)
        ++jobs[i].waiting;

    if (--jobs[first].burst <= 0)
        ++first;
}

/*
    A function which calculates the total waiting time of all jobs.
*/
double totalWaitingTime(const std::vector <Job> &jobs, int N)
{
    double sum = 0.0;

    for (int i = 0; i < N; ++i)
        sum += jobs[i].waiting;

    return sum;
}

/*
    A function which prints the generated pre-emptive SJF schedule table.
*/
void printSchedule(const std::vector <Job> &jobs, int N)
{
    std::cout << "\n\nJob Name   Arrival Time   Burst Time   Waiting Time\n" << std::endl;

    for (int i = 0; i < N; ++i)
    {
        std::cout << std::setw(5) << jobs[i].name << std::setw(12) << jobs[i].arrival;
        std::cout << std::setw(14) << jobs[i].burstBeginning << std::setw(14) << jobs[i].waiting << std::endl;
    }

    double total = totalWaitingTime(jobs, N);
    std::cout << "\nTotal waiting time: " << total << std::endl;
    std::cout << "Average waiting time: " << total/N << std::endl;
}
