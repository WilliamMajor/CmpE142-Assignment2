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
void bubbleSort(Jobs * JobsArr) ;
//I do not like using this global var, but for right now its what is working
int jobcount;

int main(int argc, char *argv[]) {

	string filelocation;
	Jobs* JobsArr = new Jobs[100];
	
	
	cout<<"Enter location of input file: ";
	cin>>filelocation;

	(importjobs(filelocation, JobsArr))? cout <<"jobs imported"<<endl : cout << "failed impoting jobs";

	FIFO(JobsArr);



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
	bubbleSort(JobsArr);
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

void bubbleSort(Jobs * JobsArr) 
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
           Temp.setAT(JobsArr[j].getAT());
		   Temp.setJID(JobsArr[j].getJID());
		   Temp.setDur(JobsArr[j].getDur());

		   JobsArr[j].setAT(JobsArr[j+1].getAT());
		   JobsArr[j].setJID(JobsArr[j+1].getJID());
		   JobsArr[j].setDur(JobsArr[j+1].getDur());

		   JobsArr[j+1].setAT(Temp.getAT());
		   JobsArr[j+1].setJID(Temp.getJID());
		   JobsArr[j+1].setDur(Temp.getDur());

           swapped = true; 
        } 
     } 
  
     // IF no two elements were swapped by inner loop, then break 
     if (swapped == false) 
        break; 
   } 
} 