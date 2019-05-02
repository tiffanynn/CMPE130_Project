//============================================================================
// Name        : scheduler.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <bits/stdc++.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>
#include "Jobs.hpp"
using namespace std;
/*
need to output:
start time, finish time, total time elapsed, response time
*/
void FIFO(Jobs *jobsArry, int numberOfJobs);
void SJF(Jobs *jobsArry, int numberOfJobs);
void BJF(Jobs *jobsArry, int numberOfJobs);
void STCF(Jobs *jobsArry, int numberOfJobs);
void RR(Jobs *jobsArry, int numberOfJobs);
int FIFO_partition(Jobs *jobsArry, int low, int high);
int SJF_partition(Jobs *jobsArry, int low, int high);
int BJF_partition(Jobs *jobsArry, int low, int high);
void FIFO_quickSort(Jobs *jobsArry, int low, int high);
void SJF_quickSort(Jobs *jobsArry, int low, int high);
void BJF_quickSort(Jobs *jobsArry, int low, int high);
int findNumJobs();
int max(int val1, int val2);
int knapsack(int beta, int weight[], int value[], int number);
void outputJobs(Jobs *jobsArry, int numberOfJobs);
int getMaxIndexOfCurrentAvailableJobs(Jobs *jobsArry, int currentTimeStamp, int numberOfJobs);
int main() {
    int numberOfJobsFound = 0;
    ifstream in_stream;
    in_stream.open("jobs.dat");

    numberOfJobsFound = findNumJobs();

    Jobs *jobsArry = new Jobs[numberOfJobsFound];

    for(int i = 0; i < numberOfJobsFound; i++)
    {
        in_stream >> jobsArry[i];

    }
    cout << "FIFO: ";
    FIFO(jobsArry, numberOfJobsFound);
    cout << endl;
    cout << "SJF: ";
    SJF(jobsArry, numberOfJobsFound);
    cout << endl;
    cout << "BJF: ";
    BJF(jobsArry, numberOfJobsFound);
    cout << endl;
    cout << "STCF: ";
    STCF(jobsArry, numberOfJobsFound);
        cout << endl;
    cout << "RR: ";
    RR(jobsArry, numberOfJobsFound);
    cout << endl;
    int bursttime = 0;
    int beta = 0;
    int duration[numberOfJobsFound];
    int priority[numberOfJobsFound];
    for(int i = 0; i < numberOfJobsFound; i++){
       	jobsArry[i].setjobpriority(i+1);
       	priority[i] = i+1;
    }
    for(int i = 0; i < numberOfJobsFound; i++){
       	bursttime = jobsArry[i].getDuration() + bursttime;
       	duration[i] = jobsArry[i].getRemainingTime();
    }
    while(bursttime > 0){
       	beta = bursttime/ numberOfJobsFound;
       	knapsack(beta, duration, priority, numberOfJobsFound);
       	for(int i = 0; i < numberOfJobsFound; i++){
       		if(jobsArry[i].getRemainingTime() != jobsArry[i].getDuration() || jobsArry[i].getjobpriority() < 0){
       			jobsArry[i].setjobpriority(jobsArry[i].getjobpriority()*-1);
       			priority[i] = jobsArry[i].getjobpriority() * -1;
       		}
       	}
       	bursttime = 0;
       	for(int i = 0; i < numberOfJobsFound; i++){
       		bursttime = jobsArry[i].getRemainingTime() + bursttime;
       	}
    }
    return 0;
}
//*NOTE: this FIFO function also sorts the array of jobs in increasing order of arrival times
void FIFO(Jobs *jobsArry, int numberOfJobs){//no preemption
    int k = 0;
    Jobs temp;
    int currentTimeStamp = 0;//time stamp starts at zero

    //sorting the jobs array by increasing order of arrival times using quicksort
    FIFO_quickSort(jobsArry, 0, numberOfJobs - 1);

    /*
    need to set the following:
    start time, finish time, total time elapsed, response time
    */
    for(k = 0; k < numberOfJobs; k++){
        if(jobsArry[k].getArrival() > currentTimeStamp)//if there are no jobs to schedule
        {
            currentTimeStamp = jobsArry[k].getArrival();//adjust the current time stamp to skip to the next job's arrival time
        }
        jobsArry[k].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
        jobsArry[k].setFinishTime((jobsArry[k].getDuration() + currentTimeStamp));
        currentTimeStamp = jobsArry[k].getFinishTime();
    }
    outputJobs(jobsArry, numberOfJobs);
}
void SJF(Jobs *jobsArry, int numberOfJobs){//no preemption

    Jobs key;
    int k, currentTimeStamp = 0;//time stamp starts at zero

    //at this point (assuming FIFO already ran), the jobs are sorted in increasing order of arrival times

    //run the first job to get a starting time stamp (assuming no 2 jobs arrive at the same time)
    if(jobsArry[0].getArrival() > currentTimeStamp)//if there are no jobs to schedule
    {
        currentTimeStamp = jobsArry[0].getArrival();//adjust the current time stamp to skip to the next job's arrival time
    }
    jobsArry[0].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
    jobsArry[0].setFinishTime((jobsArry[0].getDuration() + currentTimeStamp));
    currentTimeStamp = jobsArry[0].getFinishTime();

    for(k = 1; k < numberOfJobs; k++){
        /*
        run quicksort (based on increasing duration time) on the array from the job's index up to the job with the highest
        arrival time within (<=) the currentTimeStamp
        */
		SJF_quickSort(jobsArry, k, getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs));
        // for (i = (k + 1); i <= getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs); i++) {
            // key = jobsArry[i];
            // j = i - 1;

            // /* Move elements of arr[0..i-1], that are
              // greater than key, to one position ahead
              // of their current position */
            // while (j >= k && jobsArry[j].getDuration() > key.getDuration()) {
                // jobsArry[j + 1] = jobsArry[j];
                // j = j - 1;
            // }
            // jobsArry[j + 1] = key;
        // }
        //set the appropriate values
        if(jobsArry[k].getArrival() > currentTimeStamp)//if there are no jobs to schedule
        {
            currentTimeStamp = jobsArry[k].getArrival();//adjust the current time stamp to skip to the next job's arrival time
        }
        jobsArry[k].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
        jobsArry[k].setFinishTime((jobsArry[k].getDuration() + currentTimeStamp));
        currentTimeStamp = jobsArry[k].getFinishTime();
    }
    outputJobs(jobsArry, numberOfJobs);
    /*
    Need to account for arrival times--if a long job arrives earlier than a short job,
    then the long job will run first (this is because we are not required to have preemption
    in this implementation of SJF). Thus, the jobs whose arrival times are within the current
    time stamp should be sorted by increasing order of duration times.
    */
}
void BJF(Jobs *jobsArry, int numberOfJobs){//no preemption

    Jobs key, temp;
    int k, currentTimeStamp = 0;//time stamp starts at zero

    //running quicksort just to sort the jobs by increasing order of arrival times
    FIFO_quickSort(jobsArry, 0, numberOfJobs - 1);

    //run the first job to get a starting time stamp (assuming no 2 jobs arrive at the same time)
    if(jobsArry[0].getArrival() > currentTimeStamp)//if there are no jobs to schedule
    {
        currentTimeStamp = jobsArry[0].getArrival();//adjust the current time stamp to skip to the next job's arrival time
    }
    jobsArry[0].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
    jobsArry[0].setFinishTime((jobsArry[0].getDuration() + currentTimeStamp));
    currentTimeStamp = jobsArry[0].getFinishTime();

    for(k = 1; k < numberOfJobs; k++){
        /*
        run quicksort (based on decreasing duration time) on the array from the job's index up to the job with the highest
        arrival time within (<=) the currentTimeStamp
        */
        BJF_quickSort(jobsArry, k, getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs));
        //set the appropriate values
        if(jobsArry[k].getArrival() > currentTimeStamp)//if there are no jobs to schedule
        {
            currentTimeStamp = jobsArry[k].getArrival();//adjust the current time stamp to skip to the next job's arrival time
        }
        jobsArry[k].setStartTime(currentTimeStamp);//start time is the current time stamp because FIFO (array is sorted)
        jobsArry[k].setFinishTime((jobsArry[k].getDuration() + currentTimeStamp));
        currentTimeStamp = jobsArry[k].getFinishTime();
    }
    outputJobs(jobsArry, numberOfJobs);
}

void STCF(Jobs *jobsArry, int numberOfJobs){

	Jobs temp;

	//sorting the jobs array by increasing order of arrival times using quicksort
    FIFO_quickSort(jobsArry, 0, numberOfJobs - 1);

	int numberOfCompletedJobs = 0;
	int timer = 0, shortestJobTimeRemaining = INT_MAX;
	int shortestJob = 0;
	bool shortestJobFound = false;

	while(numberOfCompletedJobs != numberOfJobs)
	{
		for(int j = 0; j < numberOfJobs; j++){
			/* Three potential cases:
			1.If job is less than or equivalent to the current time the timer has passed
			2.The remaining time of the job is less than the current job's remaining job time
			3.The remaing time of the job is less than 0 */
			if((jobsArry[j].getArrival() <= timer) && (jobsArry[j].getRemainingTime() < shortestJobTimeRemaining) && (jobsArry[j].getRemainingTime() > 0)) {
				shortestJobTimeRemaining = jobsArry[j].getRemainingTime();
				shortestJob = j;
                shortestJobFound = true;
			}
		}
        //if job has not started before and all jobs were checked in the jobsArry

		if(shortestJobFound == false) {//if shortest was not found keep timer going
			timer++;
			continue;
		}

        if(!jobsArry[shortestJob].getjobStarted()){

			jobsArry[shortestJob].setStartTime(timer);
			jobsArry[shortestJob].setjobStarted(true);

		}

		// Reduce job's remaining time by one
		jobsArry[shortestJob].setRemainingTime(jobsArry[shortestJob].getRemainingTime()-1);

		// Update shortestJobTimeRemaining
		shortestJobTimeRemaining = jobsArry[shortestJob].getRemainingTime();

		// If a job is completed
		if(jobsArry[shortestJob].getRemainingTime() == 0) {

			// Increment numberOfCompletedJobs
			numberOfCompletedJobs++;
			shortestJobFound = false;

			// Set finish time of current job
			jobsArry[shortestJob].setFinishTime(timer+1);
		}
        if(shortestJobTimeRemaining == 0) shortestJobTimeRemaining = INT_MAX;
		// Increment timer
		timer++;
    }

	outputJobs(jobsArry, numberOfJobs);

}

void RR(Jobs *jobsArry, int numberOfJobs){
    for(int f = 0; f < numberOfJobs; f++){
        jobsArry[f].setRemainingTime(jobsArry[f].getDuration());//resetting remaining times for all jobs
    }
    int numberOfJobsDone = 0;
    Jobs temp;
    int currentTimeStamp = 0;//time stamp starts at zero
    bool jobAlreadyStarted[numberOfJobs], alreadyCheckedIfJobDone[numberOfJobs];

    //running quicksort (for simplicity--should be improved) just to sort the jobs by increasing order of arrival times
    FIFO_quickSort(jobsArry, 0, numberOfJobs - 1);
    //initialize the bool arrays to false
	for(int k = 0; k < numberOfJobs; k++){
		jobAlreadyStarted[k] = false;
		alreadyCheckedIfJobDone[k] = false;
	}

    /*
    jobs whose arrival time is within the current time stamp need to be time sliced into predetermined quanta of time
    and swapped in a 'round robin' fashion
     */
    while(numberOfJobsDone != numberOfJobs){//while all jobs are not done
        /*
        run through all jobs currently available (all jobs whose arrival time is within (<=) the currentTimeStamp)
         */
        for (int i = 0; i <= getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs); i++) {
            if(jobsArry[i].getRemainingTime() <= 0){//the 'i' job is done (duration has expired)
                if(alreadyCheckedIfJobDone[i] == true){//you already updated numberOfJobsDone and set finish time so do nothing
                }
                else{//you have not updated numberOfJobsDone so set finish time, increment numberOfJobsDone, and update alreadyCheckedIfJobsDone
                    alreadyCheckedIfJobDone[i] = true;
                    numberOfJobsDone = numberOfJobsDone + 1;
                    jobsArry[i].setFinishTime(currentTimeStamp);
                }
            }
            else{//the 'i' job is not done (duration has not expired) so take a time slice from this job and update current time stamp
                if(jobsArry[i].getArrival() > currentTimeStamp){//if there are no jobs to schedule
                    currentTimeStamp = jobsArry[i].getArrival();//adjust the current time stamp to skip to the next job's arrival time
                }
                if(jobAlreadyStarted[i] == true){//job already started so don't need to set the start time
                    if(jobsArry[i].getRemainingTime() >= 1){
                        jobsArry[i].takeTimeSliceAway(1);//we chose 1 as the quanta of time that a job is alloted in the RR scheduler
                        currentTimeStamp = currentTimeStamp + 1;//increase currentTimeStamp by one time interval
                    }
                    else{//time is less than 1 so just take it all away
                        currentTimeStamp = currentTimeStamp + jobsArry[i].getRemainingTime();//increase currentTimeStamp by remainingTime
                        jobsArry[i].takeTimeSliceAway(jobsArry[i].getRemainingTime());
                    }
                }
                else{//this is first time job starts, so set start time
                    jobAlreadyStarted[i] = true;
                    jobsArry[i].setStartTime(currentTimeStamp);
                    if(jobsArry[i].getRemainingTime() >= 1){
                        jobsArry[i].takeTimeSliceAway(1);//we chose 1 as the quanta of time that a job is alloted in the RR scheduler
                        currentTimeStamp = currentTimeStamp + 1;//increase currentTimeStamp by one time interval
                    }
                    else{//time is less than 5 so just take it all away
                        currentTimeStamp = currentTimeStamp + jobsArry[i].getRemainingTime();//increase currentTimeStamp by one time interval
                        jobsArry[i].takeTimeSliceAway(jobsArry[i].getRemainingTime());
                    }
                }
            }
        }

        //fixing the currentTimeStamp if all jobs within the currentTimeStamp are done
        if(numberOfJobsDone == (getMaxIndexOfCurrentAvailableJobs(jobsArry, currentTimeStamp, numberOfJobs) + 1))//if all the jobs within the currentTimeStamp are done
        {
            currentTimeStamp = jobsArry[numberOfJobsDone].getArrival();//adjust the current time stamp to skip to the next job's arrival time
        }
    }
    outputJobs(jobsArry, numberOfJobs);
}
void outputJobs(Jobs *jobsArry, int numberOfJobs){//print jobs results
    cout << "\t\tStart Time" << "\tFinish Time" << "\tTotal Time Elapsed" << "\tResponse Time\n";
    cout << "_____________________________________________________________________________________\n";
    for(int a = 0; a < numberOfJobs; a++){
        cout << "Job ID " << jobsArry[a].getID() << ": "
                << "\t" << jobsArry[a].getStartTime() << "\t"
                << "\t" << jobsArry[a].getFinishTime() << "\t"
                << "\t" << (jobsArry[a].getFinishTime() - jobsArry[a].getArrival()) << "\t"
                << "\t\t" << (jobsArry[a].getStartTime() - jobsArry[a].getArrival()) << endl;
    }
}
int findNumJobs()
{
    int numberOfJobsFound = 0, counter = 0;
    int next;
    ifstream in_stream;
    in_stream.open("jobs.dat");
    in_stream >> next;

    while(!in_stream.eof())
    {
        if((counter % 3) == 0)
        {
            numberOfJobsFound++;
        }
        in_stream >> next;
        counter++;
    }
    in_stream.close();
    return numberOfJobsFound;
}
int getMaxIndexOfCurrentAvailableJobs(Jobs *jobsArry, int currentTimeStamp, int numberOfJobs){//if the current job's arrival time is <= currentTimeStamp
    int index = 0;
    /*
    note that this function assumes you have already sorted the jobsArry in increasing order
    of job arrival times
    */

    for(int i = 1; i < numberOfJobs; i++){
        if(jobsArry[i].getArrival() <= currentTimeStamp){
            index = i;
        }
    }
    return index;

}
int FIFO_partition(Jobs *jobsArry, int low, int high)
{
	Jobs pivot = jobsArry[high]; // pivot
	int i = (low - 1); // Index of smaller element
	Jobs temp;

	for (int j = low; j <= high - 1; j++)
	{
		/* If current element is smaller than or
		equal to pivot  */
		if (jobsArry[j].getArrival() <= pivot.getArrival())
		{
			i++; // increment index of smaller element
			temp = jobsArry[j];/* swap */
			jobsArry[j] = jobsArry[i];
			jobsArry[i] = temp;
		}
	}
	temp = jobsArry[high];/* swap */
	jobsArry[high] = jobsArry[i + 1];
	jobsArry[i + 1] = temp;
	return (i + 1);
}
void FIFO_quickSort(Jobs *jobsArry, int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, jobsArry[p] is now
		at right place */
		int pi = FIFO_partition(jobsArry, low, high);

		/* Separately sort elements before
		partition and after partition */
		FIFO_quickSort(jobsArry, low, pi - 1);
		FIFO_quickSort(jobsArry, pi + 1, high);
	}
}
int SJF_partition(Jobs *jobsArry, int low, int high)
{
	Jobs pivot = jobsArry[high]; // pivot
	int i = (low - 1); // Index of smaller element
	Jobs temp;

	for (int j = low; j <= high - 1; j++)
	{
		/* If current element is smaller than or
		equal to pivot  */
		if (jobsArry[j].getDuration() <= pivot.getDuration())
		{
			i++; // increment index of smaller element
			temp = jobsArry[j];/* swap */
			jobsArry[j] = jobsArry[i];
			jobsArry[i] = temp;
		}
	}
	temp = jobsArry[high];/* swap */
	jobsArry[high] = jobsArry[i + 1];
	jobsArry[i + 1] = temp;
	return (i + 1);
}
void SJF_quickSort(Jobs *jobsArry, int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, jobsArry[p] is now
		at right place */
		int pi = SJF_partition(jobsArry, low, high);

		/* Separately sort elements before
		partition and after partition */
		SJF_quickSort(jobsArry, low, pi - 1);
		SJF_quickSort(jobsArry, pi + 1, high);
	}
}
int BJF_partition(Jobs *jobsArry, int low, int high)
{
	Jobs pivot = jobsArry[high]; // pivot
	int i = (low - 1); // Index of smaller element
	Jobs temp;

	for (int j = low; j <= high - 1; j++)
	{
		/* If current element is smaller than or
		equal to pivot  */
		if (jobsArry[j].getDuration() >= pivot.getDuration())
		{
			i++; // increment index of smaller element
			temp = jobsArry[j];/* swap */
			jobsArry[j] = jobsArry[i];
			jobsArry[i] = temp;
		}
	}
	temp = jobsArry[high];/* swap */
	jobsArry[high] = jobsArry[i + 1];
	jobsArry[i + 1] = temp;
	return (i + 1);
}
void BJF_quickSort(Jobs *jobsArry, int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, jobsArry[p] is now
		at right place */
		int pi = BJF_partition(jobsArry, low, high);

		/* Separately sort elements before
		partition and after partition */
		BJF_quickSort(jobsArry, low, pi - 1);
		BJF_quickSort(jobsArry, pi + 1, high);
	}
}
int max(int val1, int val2){
	if(val1 > val2){
		return val1;
	}
	else{
		return val2;
	}
}
int knapsack(int beta, int weight[], int value[], int number){
	int K[number+1][beta+1];
	for(int i = 0; i <= number; i++){
		for(int j = 0; j <= beta; j++){
			if(i == 0 || j == 0){
				K[i][j] = 0;
			}
			else if(weight[i-1] <= beta){
				K[i][j] = max(value[i-1]+K[i-1][beta-weight[i-1]], K[i-1][beta]);
			}
			else{
				K[i][j] = K[i-1][j];
			}
		}
	}
	return K[number][beta];
}
