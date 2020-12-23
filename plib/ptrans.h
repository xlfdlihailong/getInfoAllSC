#ifndef PTRANS_H
#define PTRANS_H
#include "plib.h"
class ptrans
{
public:
    pstring strhost;
    int iport;
    ptcp* pts;
    ptrans(pstring strhost="192.168.1.9",int iport=5555);
    void threadRecv(ptcp* p)
    {
        while(1)
        {
            if(p->recvx()<0)
                break;
            hlog(p->type);
        }
    }
    void setThreadServer()
    {
        pts=new ptcp(strhost,iport);
        pts->setThreadServerClassDetach(&ptrans::threadRecv,this);
    }
};

#endif // PTRANS_H
