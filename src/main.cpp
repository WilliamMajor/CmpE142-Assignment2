#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <fstream>
#include "Jobs.h"
#include <algorithm>
#include <sstream>
#include <string>



using namespace std;

bool importjobs(string argFileLocation, Jobs * JobsArr);
string replaceSpaceTab(string rawInput);
void parseline(string procInput, Jobs& newjob);
void FIFO(Jobs * JobsArr);
void SJF(Jobs * JobsArr);
void BJF(Jobs * JobsArr);
void bubbleSortAT(Jobs * JobsArr) ;
void bubbleSortSJF(Jobs * JobsArr, int length, int start);
void bubbleSortBJF(Jobs * JobsArr, int length, int start);
int waitingLine(Jobs * JobsArr, int start, int current_time, int queue_length);

//I do not like using this global var, but for right now its what is working
int jobcount;

int main(int argc, char *argv[]) {

	string filelocation;
	Jobs* JobsArr = new Jobs[100];
	
	
	cout<<"Enter location of input file: ";
	cin>>filelocation;

	(importjobs(filelocation, JobsArr))? cout <<"jobs imported"<<endl << endl : cout << "failed impoting jobs";

	//FIFO(JobsArr);

	//SJF(JobsArr);

	BJF(JobsArr);
	


	return 0;
}
//Functions to set up the data we are working with
bool importjobs(string argFileLocation, Jobs * JobsArr)
{
	ifstream jobsfile(argFileLocation);
	int lineCount;
	string temp[1000];
	int idx = 0;
	//jobsfile.open(argFileLocation);
	if (jobsfile.fail())
	{
		cout <<"error opening file";
		return -1;
	}

	cout <<"importing jobs..." << endl;

	//start by counting the number of jobs we will be creating/populating
	while (!jobsfile.eof())
	{
		getline(jobsfile, temp[idx]);
		lineCount++;
		idx++;
	}
	jobcount = lineCount -1;

	//parse and fill jobs with data
	for (int i = 0; i <=jobcount; i++)
	{
		parseline(temp[i], JobsArr[i]);
	}

	
	return true;	
}

string replaceSpaceTab(string rawInput)
{
	bool fstf = false;
	string commastring = rawInput;
	//replace the fist 
	for(char& c : commastring)
	{
		if((c == ' ' && !fstf) || (c == '	' && !fstf))
		{
			c = '\n';
			fstf = true;
		}
		else if((c == ' ') || (c == '	'))
		{
			//do nothing we will handle these next
		}
		else
		{
			fstf = false;
		}
		
	}
	commastring.erase(remove(commastring.begin(), commastring.end(), ' '), commastring.end());
	commastring.erase(remove(commastring.begin(), commastring.end(), '	'), commastring.end());


	return commastring;
}

void parseline(string Input, Jobs& newjob)
{
	string csvInput = replaceSpaceTab(Input);
	int idx = 0;
	int jobdata[3];
	stringstream input(csvInput);
	string data[3];
	while(getline(input , data[idx]))
	{
		jobdata[idx] = stoi(data[idx]);
		idx++;
	}
	newjob.setJID(jobdata[0]);
	newjob.setAT(jobdata[1]);
	newjob.setDur(jobdata[2]);
}

//Schedualing Algorithms
void FIFO(Jobs * JobsArr)
{
	int current_time = 0;
	
	//sort jobs based on their arrival time
	bubbleSortAT(JobsArr);
	for(int idx = 0; idx < jobcount; idx++)
	{
		//cover the case were a job arrives and no job is running currently
		if(JobsArr[idx].getAT() >= current_time)
		{
			JobsArr[idx].setStartT(JobsArr[idx].getAT());
			JobsArr[idx].setResponceT(0);
			JobsArr[idx].setFinishT(JobsArr[idx].getStartT() + JobsArr[idx].getDur());
			JobsArr[idx].setTotalT(JobsArr[idx].getFinishT() - JobsArr[idx].getAT());
			current_time = JobsArr[idx].getFinishT();
		}
		else
		{
			JobsArr[idx].setStartT(current_time);
			JobsArr[idx].setResponceT(JobsArr[idx].getStartT() - JobsArr[idx].getAT());
			JobsArr[idx].setFinishT(JobsArr[idx].getStartT() + JobsArr[idx].getDur());
			JobsArr[idx].setTotalT(JobsArr[idx].getFinishT() - JobsArr[idx].getAT());
			current_time = JobsArr[idx].getFinishT();
		}
		cout << JobsArr[idx].getJID() << " " << JobsArr[idx].getAT() << " " << JobsArr[idx].getDur() << " " 
			 << JobsArr[idx].getStartT() << " " << JobsArr[idx].getFinishT() << " " << JobsArr[idx].getTotalT() 
			 << " " << JobsArr[idx].getResponceT() << endl;
	}

	
}

void SJF(Jobs * JobsArr)
{
	int current_time = 0;
	int queue_length = 0;
	int count = 0;
	
	//sort jobs based on their arrival time
	bubbleSortAT(JobsArr);

	current_time = JobsArr[0].getAT();

	while (count < jobcount)
	{
		//establish who is in the queue
		queue_length = waitingLine(JobsArr, count, current_time, queue_length);

		if(queue_length > 0)
		{
			bubbleSortSJF(JobsArr, queue_length, count);
		}

		//clear the next job in the queue
		JobsArr[count].setStartT(current_time);
		JobsArr[count].setResponceT(JobsArr[count].getStartT() - JobsArr[count].getAT());
		JobsArr[count].setFinishT(JobsArr[count].getStartT() + JobsArr[count].getDur());
		JobsArr[count].setTotalT(JobsArr[count].getFinishT() - JobsArr[count].getAT());
		current_time = JobsArr[count].getFinishT();

		//cout << "count: " << count << " current_time: " << current_time << endl;
		

		cout << JobsArr[count].getJID() << " " << JobsArr[count].getAT() << " " << JobsArr[count].getDur() << " " 
			 << JobsArr[count].getStartT() << " " << JobsArr[count].getFinishT() << " " << JobsArr[count].getTotalT() 
			 << " " << JobsArr[count].getResponceT() << endl << endl;
		count++;
		if(queue_length > 0)
		{
			queue_length--;
		}
	}
	
}
void BJF(Jobs * JobsArr)
{
	int current_time = 0;
	int queue_length = 0;
	int count = 0;
	
	//sort jobs based on their arrival time
	bubbleSortAT(JobsArr);

	current_time = JobsArr[0].getAT();

	while (count < jobcount)
	{
		//establish who is in the queue
		queue_length = waitingLine(JobsArr, count, current_time, queue_length);

		if(queue_length > 0)
		{
			bubbleSortBJF(JobsArr, queue_length, count);
		}

		//clear the next job in the queue
		JobsArr[count].setStartT(current_time);
		JobsArr[count].setResponceT(JobsArr[count].getStartT() - JobsArr[count].getAT());
		JobsArr[count].setFinishT(JobsArr[count].getStartT() + JobsArr[count].getDur());
		JobsArr[count].setTotalT(JobsArr[count].getFinishT() - JobsArr[count].getAT());
		current_time = JobsArr[count].getFinishT();

		//cout << "count: " << count << " current_time: " << current_time << endl;
		

		cout << JobsArr[count].getJID() << " " << JobsArr[count].getAT() << " " << JobsArr[count].getDur() << " " 
			 << JobsArr[count].getStartT() << " " << JobsArr[count].getFinishT() << " " << JobsArr[count].getTotalT() 
			 << " " << JobsArr[count].getResponceT() << endl << endl;
		count++;
		if(queue_length > 0)
		{
			queue_length--;
		}
	}
	
}

///////////Utilities/////////////

int waitingLine(Jobs * JobsArr, int start, int current_time, int queue_length)
{
	for(int idx = 0; idx < jobcount; idx++)
	{
		//cout << current_time <<endl;
			if((JobsArr[idx].getAT() <= current_time))
			{
				if(JobsArr[idx].waiting == false)
				{
					JobsArr[idx].waiting = true;
					queue_length++;
				}
				
			}
			
	}
	return queue_length;
}
void bubbleSortAT(Jobs * JobsArr) 
{ 
   int i, j; 
   Jobs Temp;
   bool swapped; 
   for (i = 0; i < jobcount-1; i++) 
   { 
     swapped = false; 
     for (j = 0; j < jobcount-i-1; j++) 
     { 
        if (JobsArr[j].getAT() > JobsArr[j+1].getAT()) 
        { 

		   Temp = JobsArr[j];
		   JobsArr[j] = JobsArr[j + 1];
		   JobsArr[j + 1] = Temp;

           swapped = true; 
        } 
     } 
  
     // IF no two elements were swapped by inner loop, then break 
     if (swapped == false) 
        break; 
   } 
} 

void bubbleSortSJF(Jobs * JobsArr, int length, int start) 
{ 
   int i, j; 
   Jobs Temp;
   bool swapped; 

   for (i = 0; i < length-1; i++) 
   { 
     swapped = false; 
     for (j = start; j < start + length - i - 1; j++) 
     { 
        if (JobsArr[j].getDur() > JobsArr[j+1].getDur()) 
        { 
			Temp = JobsArr[j];
			JobsArr[j] = JobsArr[j + 1];
			JobsArr[j + 1] = Temp;
           	swapped = true; 
        } 
     } 
  
     // IF no two elements were swapped by inner loop, then break 
     if (swapped == false) 
        break; 
   } 
} 

void bubbleSortBJF(Jobs * JobsArr, int length, int start) 
{ 
   int i, j; 
   Jobs Temp;
   bool swapped; 

   for (i = 0; i < length-1; i++) 
   { 
     swapped = false; 
     for (j = start; j < start + length - i - 1; j++) 
     { 
        if (JobsArr[j].getDur() < JobsArr[j+1].getDur()) 
        { 
			Temp = JobsArr[j];
			JobsArr[j] = JobsArr[j + 1];
			JobsArr[j + 1] = Temp;
           	swapped = true; 
        } 
     } 
  
     // IF no two elements were swapped by inner loop, then break 
     if (swapped == false) 
        break; 
   } 
} 