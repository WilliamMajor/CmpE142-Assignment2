#include "Jobs.h"
#include <iostream>


using namespace std;

//Constructors
Jobs::Jobs()
{
    JID         = 0;
    AT          = 0;
    Dur         = 0;
    StartT      = 0;
    FinishT     = 0;
    TotalT      = 0;
    ResponceT   = 0;
    RemainT     = 0;
    waiting     = false;
    started     = false;
    finished    = false;
}

Jobs::~Jobs()
{
}

Jobs::Jobs(int argJID, int argAT, int argDur)
{
    JID = argJID;
    AT  = argAT;
    Dur = argDur;
}

//Getter functions
int Jobs::getJID() const
{
    return JID;
}

int Jobs::getAT() const
{
    return AT;
}

int Jobs::getDur() const
{
    return Dur;
}

int Jobs::getStartT() const
{
    return StartT;
}

int Jobs::getFinishT() const
{
    return FinishT;
}

int Jobs::getTotalT() const
{
    return TotalT;
}

int Jobs::getResponceT() const
{
    return ResponceT;
}

int Jobs::getRemainT() const
{
    return RemainT;
}

//Setter Functions
void Jobs::setJID(int argJID)
{
    JID = argJID;
}

void Jobs::setAT(int argAT)
{
    AT = argAT;
}

void Jobs::setDur(int argDur)
{
    Dur = argDur;
}

void Jobs::setStartT(int argStartT)
{
    StartT = argStartT;
}

void Jobs::setFinishT(int argFinishT)
{
    FinishT = argFinishT;
}

void Jobs::setTotalT(int argTotalT)
{
    TotalT = argTotalT;
}

void Jobs::setResponceT(int argResponceT)
{
    ResponceT = argResponceT;
}

void Jobs::setRemainT(int argRemainT)
{
    RemainT = argRemainT;
}

