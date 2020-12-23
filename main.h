//
// Created by Administrator on 2020/9/3.
//

#ifndef GETINFOALLSC_MAIN_H
#define GETINFOALLSC_MAIN_H

#include "plib/plib.h"

void funExit(int id);

void funPipe(int id);

bool xr(pstring host);

bool xp(pstring host);

bool xs(pstring host);

bool xk(pstring host);

pstring getUsageCPU(pstring host);

pstring getUsageMem(pstring host);

#endif //GETINFOALLSC_MAIN_H
