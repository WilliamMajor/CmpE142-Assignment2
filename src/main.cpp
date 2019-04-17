#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <fstream>
#include "Jobs.h"
#include <algorithm>
#include <sstream>
#include <string>



using namespace std;

bool importjobs(string argFileLocation);
string replaceSpaceTab(string rawInput);
void parseline(string procInput, Jobs& newjob);



int main(int argc, char *argv[]) {

	string filelocation;
	
	
	cout<<"Enter location of input file: ";
	cin>>filelocation;

	(importjobs(filelocation))? cout <<"jobs imported"<<endl : cout << "failed impoting jobs";
;



	return 0;
}

bool importjobs(string argFileLocation)
{
	fstream jobsfile;
	int lineCount;
	string temp[1000];
	int idx = 0;
	jobsfile.open(argFileLocation);
	if (jobsfile.fail())
	{
		cout <<"error opening file";
		return -1;
	}

	cout <<"importing jobs..." << endl;

	//start by counting the number of jobs we will be creating/populating
	while (getline(jobsfile, temp[idx]))
	{
		lineCount++;
		idx++;
		
	}
	//creat as many job objects as we have imported jobs
	Jobs* JobsArr = new Jobs[lineCount];
	//parse and fill jobs with data
	for (int i = 0; i <=lineCount; i++)
	{
		parseline(temp[i], JobsArr[i]);
	}
	
	cout << JobsArr[0].getJID() << endl;

	
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
			c = ',';
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
	if(getline(input , data[idx], ','))
	{
		jobdata[idx] = stoi(data[idx]);
		idx++;
	}
	newjob.setJID(jobdata[0]);
	newjob.setAT(jobdata[1]);
	newjob.setDur(jobdata[2]);
}