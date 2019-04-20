#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <fstream>
#include "Jobs.h"
#include <algorithm>
#include <sstream>
#include <string>
#include <unistd.h>



using namespace std;

bool importjobs(string argFileLocation, Jobs * JobsArr);
string replaceSpaceTab(string rawInput);
void parseline(string procInput, Jobs& newjob);
void FIFO(Jobs * JobsArr);
void SJF(Jobs * JobsArr);
void BJF(Jobs * JobsArr);
void STCF(Jobs * JobsArr);
void RR(Jobs * JobsArr);
void bubbleSortAT(Jobs * JobsArr) ;
void bubbleSortSJF(Jobs * JobsArr, int length, int start);
void bubbleSortBJF(Jobs * JobsArr, int length, int start);
void bubbleSortSTCF(Jobs * JobsArr, int length, int start);
int waitingLine(Jobs * JobsArr, int start, int current_time, int queue_length);
int waitingLineSTCF(Jobs * JobsArr, int start, int current_time, int queue_length);

int jobcount;

int main(int argc, char *argv[]) {

	string filelocation;
	int choice = 0;
	bool failed;
	bool badchoice;


	cout<<"Enter location of input file: ";
	cin>>filelocation;
	while (1)
	{
		failed = true;
		badchoice = false;
		Jobs* JobsArr = new Jobs[100];
		while(failed)
		{
			(importjobs(filelocation, JobsArr)) ? failed = false : failed = true;
			if(failed)
			{
				cout << "Bad file location please try again"<< endl << endl;
				cout << "Enter location of input file: ";
				cin >> filelocation;
			}
		}

		cout <<"1. FIFO " << endl << "2. SJF " << endl << "3. BJF" << endl << "4. STCF" << endl <<"5. RR" << endl << "6 Exit" << endl << endl;
		cout << "What algorithm would you like to run?  ";
		cin >> choice;

		while(badchoice)
		{
			((choice >= 1) && (choice <= 5)) ? badchoice = false: badchoice = true;

			if(badchoice)
			{
				cout << choice << " is not in the specified range" << endl << endl;
				cout <<"1. FIFO " << endl << "2. SJF " << endl << "3. BJF" << endl << "4. STCF" << endl <<"5. RR" << endl << "6. Exit" << endl << endl;
				cout << "What algorithm would you like to run?  ";
				cout << endl;
				cin >> choice;

			}
			
		}

		switch (choice)
		{
			case 1:
				FIFO(JobsArr);
				break;
			case 2:
				SJF(JobsArr);
				break;
			case 3:
				BJF(JobsArr);
				break;
			case 4:
				STCF(JobsArr);
				break;
			case 5:
				RR(JobsArr);
				break;
			case 6: 
				return -1;
		}
		jobcount = 0;
		delete [] JobsArr;
	}
	
	return 0;
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
		cout << "Job ID: " <<JobsArr[idx].getJID() << endl << "Arrival Time: " << JobsArr[idx].getAT() << endl << "Job Durration: " << JobsArr[idx].getDur() << endl 
		 	 << "Start Time: " << JobsArr[idx].getStartT() << endl << "Finish Time: " << JobsArr[idx].getFinishT() << endl << "Total Time: " << JobsArr[idx].getTotalT() 
		 	 << endl << "Responce Time: " << JobsArr[idx].getResponceT() << endl << endl;
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
		

		cout << "Job ID: " <<JobsArr[count].getJID() << endl << "Arrival Time: " << JobsArr[count].getAT() << endl << "Job Durration: " << JobsArr[count].getDur() << endl 
		 	 << "Start Time: " << JobsArr[count].getStartT() << endl << "Finish Time: " << JobsArr[count].getFinishT() << endl << "Total Time: " << JobsArr[count].getTotalT() 
		 	 << endl << "Responce Time: " << JobsArr[count].getResponceT() << endl << endl;
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
		

		cout << "Job ID: " <<JobsArr[count].getJID() << endl << "Arrival Time: " << JobsArr[count].getAT() << endl << "Job Durration: " << JobsArr[count].getDur() << endl 
		 	 << "Start Time: " << JobsArr[count].getStartT() << endl << "Finish Time: " << JobsArr[count].getFinishT() << endl << "Total Time: " << JobsArr[count].getTotalT() 
		 	 << endl << "Responce Time: " << JobsArr[count].getResponceT() << endl << endl;
		count++;
		if(queue_length > 0)
		{
			queue_length--;
		}
	}
	
}

void STCF(Jobs * JobsArr)
{
	int current_time = 0;
	int queue_length = 0;
	int count = 0;
	bool first = true;
	
	//sort jobs based on their arrival time
	bubbleSortAT(JobsArr);

	//set the remaining time for each job
	for(int idx = 0; idx < jobcount; idx++)
	{
		JobsArr[idx].setRemainT(JobsArr[idx].getDur());
	}
	//advance to the first job
	current_time = JobsArr[0].getAT();

	while((queue_length > 0 ) || first == true)
	{
		first = false;
		queue_length = waitingLineSTCF(JobsArr, count, current_time, queue_length);
		bubbleSortSTCF(JobsArr, queue_length, count);
		if(JobsArr[count].started)
		{
			JobsArr[count].setRemainT(JobsArr[count].getRemainT() - 1);
			current_time++;

			//Check to see if the job is finished
			if(JobsArr[count].getRemainT() == 0)
			{
				JobsArr[count].setFinishT(current_time);
				JobsArr[count].setResponceT(JobsArr[count].getStartT() - JobsArr[count].getAT());
				JobsArr[count].setTotalT(JobsArr[count].getFinishT() - JobsArr[count].getAT());
				queue_length--;
				count++;
			}
		}
		else
		{
			JobsArr[count].setStartT(current_time);
			JobsArr[count].started = true;
			JobsArr[count].setRemainT(JobsArr[count].getRemainT() - 1);
			current_time++;

			//Check to see if the job is finished
			if(JobsArr[count].getRemainT() == 0)
			{
				JobsArr[count].setFinishT(current_time);
				JobsArr[count].setResponceT(JobsArr[count].getStartT() - JobsArr[count].getAT());
				JobsArr[count].setTotalT(JobsArr[count].getFinishT() - JobsArr[count].getAT());
				queue_length--;
				count++;
			}

		}
		


	}

	for (int idx = 0; idx < jobcount; idx++)
	{
		cout << "Job ID: " <<JobsArr[idx].getJID() << endl << "Arrival Time: " << JobsArr[idx].getAT() << endl << "Job Durration: " << JobsArr[idx].getDur() << endl 
		 	 << "Start Time: " << JobsArr[idx].getStartT() << endl << "Finish Time: " << JobsArr[idx].getFinishT() << endl << "Total Time: " << JobsArr[idx].getTotalT() 
		 	 << endl << "Responce Time: " << JobsArr[idx].getResponceT() << endl << endl;
	}



}

void RR(Jobs * JobsArr)
{
	int current_time = 0;
	int queue_length = 0;
	int count = 0;
	bool first = true;
	int finished = 0;
	Jobs * finishedJobs = new Jobs[jobcount];

	//sort jobs based on their arrival time
	bubbleSortAT(JobsArr);

	//set the remaining time for each job
	for(int idx = 0; idx < jobcount; idx++)
	{
		JobsArr[idx].setRemainT(JobsArr[idx].getDur());
	}
	//advance to the first job
	current_time = JobsArr[0].getAT();

	while((queue_length > 0 ) || first == true)
	{
		first = false;
		queue_length = waitingLineSTCF(JobsArr, count, current_time, queue_length);
		if(JobsArr[count].started)
		{
			JobsArr[count].setRemainT(JobsArr[count].getRemainT() - 1);
			current_time++;
			if(JobsArr[count].getRemainT() == 0)
			{
				JobsArr[count].setFinishT(current_time);
				JobsArr[count].setResponceT(JobsArr[count].getStartT() - JobsArr[count].getAT());
				JobsArr[count].setTotalT(JobsArr[count].getFinishT() - JobsArr[count].getAT());
				JobsArr[count].finished = true;
				finishedJobs[finished] = JobsArr[count];
				finished++;

				for(int i = count; i <= queue_length; i++)
				{
					JobsArr[i] = JobsArr[i + 1];
				}
				queue_length--;
				
			}
			(count >= queue_length - 1) ? count = 0 : count++; 
		}
		else
		{
			JobsArr[count].setStartT(current_time);
			JobsArr[count].started = true;
			JobsArr[count].setRemainT(JobsArr[count].getRemainT() - 1);
			current_time++;
			if(JobsArr[count].getRemainT() == 0)
			{
				JobsArr[count].setFinishT(current_time);
				JobsArr[count].setResponceT(JobsArr[count].getStartT() - JobsArr[count].getAT());
				JobsArr[count].setTotalT(JobsArr[count].getFinishT() - JobsArr[count].getAT());
				finishedJobs[finished] = JobsArr[count];
				JobsArr[count].finished = true;
				finished++;
				for(int i = count; i = queue_length; i++)
				{
					JobsArr[i] = JobsArr[i + 1];
				}
				queue_length--;
				
			}
			
			(count >= queue_length - 1) ? count = 0 : count++; 
		}
		
	}
	for (int idx = 0; idx < jobcount; idx++)
	{
	cout << "Job ID: " <<finishedJobs[idx].getJID() << endl << "Arrival Time: " << finishedJobs[idx].getAT() << endl << "Job Durration: " << finishedJobs[idx].getDur() << endl 
		 << "Start Time: " << finishedJobs[idx].getStartT() << endl << "Finish Time: " << finishedJobs[idx].getFinishT() << endl << "Total Time: " << finishedJobs[idx].getTotalT() 
		 << endl << "Responce Time: " << finishedJobs[idx].getResponceT() << endl << endl;
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

int waitingLineSTCF(Jobs * JobsArr, int start, int current_time, int queue_length)
{
	for(int idx = 0; idx < jobcount; idx++)
	{
		//cout << current_time <<endl;
			if((JobsArr[idx].getAT() == current_time))
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

void bubbleSortSTCF(Jobs * JobsArr, int length, int start)
{
	int i, j; 
   Jobs Temp;
   bool swapped; 

   for (i = 0; i < length-1; i++) 
   { 
     swapped = false; 
     for (j = start; j < start + length - i - 1; j++) 
     { 
        if (JobsArr[j].getRemainT() > JobsArr[j+1].getRemainT()) 
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

bool importjobs(string argFileLocation, Jobs * JobsArr)
{
	ifstream jobsfile(argFileLocation);
	int lineCount = 0;
	string temp[1000];
	int idx = 0;
	//jobsfile.open(argFileLocation);
	if (jobsfile.fail())
	{
		cout <<"error opening file";
		return false;
	}

	cout << endl <<"importing jobs..." << endl << endl;

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
