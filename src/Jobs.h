#ifndef JOBS_H
#define JOBS_H

#include <iostream>
using namespace std;

class Jobs
{
    public:
        Jobs();
        Jobs(int JID, int AT, int Dur);
        virtual ~Jobs();
        int getJID() const;
        int getAT() const;
        int getDur() const;
        int getStartT() const;
        int getFinishT() const;
        int getTotalT() const;
        int getResponceT() const;
        void setJID(int argJID);
        void setAT(int argAT);
        void setDur(int argDur);
        void setStartT(int argStartT);
        void setFinishT(int argFinishT);
        void setTotalT(int TotalT);
        void setResponceT(int ResponceT);
    private:
        int JID;
        int AT;
        int Dur;
        int StartT;
        int FinishT;
        int TotalT;
        int ResponceT;
};
#endif