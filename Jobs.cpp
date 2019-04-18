#include "Jobs.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;
#include <iostream>
Jobs::Jobs(){ arrival = 0, completion = 0, ID = 0, duration = 0, startTime = 0, finishTime = 0, totalTimeElapsed = 0, responseTime = 0, remainingTime = 0; }
int Jobs::getArrival(){ return arrival; }
int Jobs::getID(){ return ID; }
int Jobs::getDuration(){ return duration; }
int Jobs::getRemainingTime(){ return remainingTime; }
void Jobs::takeTimeSliceAway(int timeToTakeAway){
	remainingTime = remainingTime - timeToTakeAway;//we chose 5 as the quanta of time that a job is alloted in the RR scheduler
}
void Jobs::operator =(const Jobs &job)
{
	ID = job.ID;
	arrival = job.arrival;
	duration = job.duration;
}
istream &operator >>(istream &in_stream, Jobs &job){
	in_stream >> job.ID;
	in_stream >> job.arrival;
	in_stream >> job.duration;
	job.remainingTime = job.duration;
	return in_stream;
}
ostream &operator <<(ostream &out_stream, const Jobs &job){
	out_stream << job.ID << "\t" << job.arrival << "\t" << job.duration << "\n";
	return out_stream;
}
Jobs::~Jobs(){}
int Jobs::getStartTime(){ return startTime; }
int Jobs::getFinishTime(){ return finishTime; }
int Jobs::getTotalTimeElapsed(){ return totalTimeElapsed; }
int Jobs::getResponseTime(){ return responseTime; }
void Jobs::setStartTime(int start){ startTime = start; }
void Jobs::setFinishTime(int finish){ finishTime = finish; }
void Jobs::setTotalTimeElapsed(int total){ totalTimeElapsed = total; }
void Jobs::setResponseTime(int response){ responseTime = response; }