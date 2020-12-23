#include "main.h"

//所有信息
CSGLAll csglAll;

void thread_server(ptcp *p) {
    if (p->type == "stopSC") {
        pstring strip = p->getDataToString();
        hlog(strip);
        bool bres = xk(strip);
        hlog(bres);
        hlog(p->sendx(p->type, bres));
    }
    if (p->type == "startSC") {
        pstring strip = p->getDataToString();
        hlog(strip);
        bool bres = xs(strip);
        hlog(bres);
        hlog(p->sendx(p->type, bres));
    }
    if (p->type == "restartSC") {
        pstring strip = p->getDataToString();
        hlog(strip);
        bool bres = xr(strip);
        hlog(bres);
        hlog(p->sendx(p->type, bres));
    }
    if (p->type == "getDiskUsage") {
        //包括了根目录使用和总控状态
//        hlog(csglAll.mapInfo);
        (p->sendx(p->type, csglAll.mapInfo));
    }
    if (p->type == "gettask") {
        pstring strip = p->getDataToString();
        hlog(strip);
        ArrstruTask resTasks = csglAll.getStation(strip).getTasks();
//        ArrstruTask resTasks;
        hlog(sizeof(ArrstruTask));
        hlog(sizeof(TASK_STATUS_STRUCT), sizeof(ROAD_STATUS_STRUCT));
        hlog(resTasks.length);
        plist<TASK_STATUS_STRUCT> listTask;
        for (int i = 0; i < resTasks.length; i++)
            listTask.append(resTasks.tasks[i]);
        hlog(p->sendx(p->type, listTask));

//        hlog(p->sendx(p->type, (char *) &resTasks, sizeof(resTasks)));
    }
    if (p->type == "getconf") {
        pstring strip = p->getDataToString();
        hlog(strip);
        pmap<pstring, pstring> mres = csglAll.getStation(strip).getConfStation();
        hlog(mres);

        (p->sendx(p->type, mres));
    }
    if (p->type == "addconf") {
        pliststring lconf = p->getClass<pliststring>();
        hlog(lconf);
        pstring key = lconf[0];
        pstring val = lconf[1];
        pstring ip = lconf[2];
        bool bres = csglAll.getStation(ip).addConf(key, val);
        hlog(bres);
        hlog(p->sendx(p->type, bres));
    }
    if (p->type == "setconf") {
        pliststring lconf = p->getClass<pliststring>();
        hlog(lconf);
        pstring key = lconf[0];
        pstring val = lconf[1];
        pstring ip = lconf[2];
        bool bres = csglAll.getStation(ip).setConf(key, val);
        hlog(bres);
        hlog(p->sendx(p->type, bres));
    }
    if (p->type == "delconf") {
        pliststring ldel = p->getClass<pliststring>();
        hlog(ldel);
        pstring key = ldel[0];
        pstring ip = ldel[1];
        bool bres = csglAll.getStation(ip).delConf(key);
        hlog(bres);
        hlog(p->sendx(p->type, bres));
    }
}

void thread_getDiskUsageRootAndStateSC(pstring host) {
    while (1) {
        pliststring lres;
        pstring strDiskUsage = plib::getRemoteRootDiskUsage(host);
        lres.append(strDiskUsage);
        pstring strStateSC;
        bool bRunning = xp(host);
        if (bRunning)
            strStateSC = "运行";
        else
            strStateSC = "停止";
        lres.append(strStateSC);

        pstring strCpuSC=getUsageCPU(host);
        lres.append(strCpuSC);

        pstring strMemSC=getUsageMem(host);
        lres.append(strMemSC);

        mut.lock();
        csglAll.mapInfo[host] = lres;
        mut.unlock();
        plib::sleep(1);
    }
}

pstring getUsageCPU(pstring host) {
    pstring cpu=plib::xsh(host,"ps aux|grep SCService|grep -v grep");
//    hlog(cpu);
    pliststring lres=cpu.split(" ");
//    hlog(lres);
    if(lres.size()>3)
        cpu=lres[2]+"%";
    else
        cpu="获取失败";
//    hlog(cpu);
    return cpu;
}
pstring getUsageMem(pstring host) {
    pstring cpu=plib::xsh(host,"ps aux|grep SCService|grep -v grep");
//    hlog(cpu);
    pliststring lres=cpu.split(" ");
//    hlog(lres);
    if(lres.size()>3)
        cpu=lres[3]+"%";
    else
        cpu="获取失败";
//    hlog(cpu);
    return cpu;
}
int main() {
    hlog("程序已重新启动");
    hlog(plib::killProcessAllByNameSelfExceptSelf());
    plib::setSignals(funExit, funPipe);
    plib::setPathConf("/opt/sc/csgl/SCService/sc.conf");


    //先连接
    csglAll.connectAll();
    ptcp tcpServer("172.16.11.6", 9999);
    tcpServer.setThreadServerDetach(thread_server);

    //获取各个服务器的根目录比和总控状态,因为轮询慢
    //所以每个服务器一个线程
    pliststring mhostAll = (csglAll.gethostAll());
    for (int i = 0; i < mhostAll.size(); i++) {
        pstring host = mhostAll[i];
        std::thread thi(thread_getDiskUsageRootAndStateSC, host);
        thi.detach();
    }
    while (1) {
        plib::sleep(1000);
    }

    return 0;
}

void funExit(int id) {
//    exit(0);
}

void funPipe(int id) {

}

bool xr(pstring host) {
    hlog(host);
    pstring tidold = plib::xsh(host, "pgrep SCService;pkill -9 scProtect;pkill -9 SCService;/opt/sc/csgl/SCService/SCService;/opt/sc/csgl/SCService/scProtect");
    hlog(tidold);
    pstring tidnew = plib::xsh(host, "pgrep SCService");
    hlog(tidnew);
    if (tidnew == "") {
        hlog("tidnew == \"\"");
        return false;
    }
    if (tidnew == tidold) {
        hlog("tidnew==tidold");
        return false;
    }
    return true;
}

bool xp(pstring host) {
    pstring tidnew = plib::xsh(host, "pgrep SCService");
    if (tidnew == "")
        return false;
    return true;
}

bool xs(pstring host) {
    pstring tid = plib::xsh(host,
                            "/opt/sc/csgl/SCService/SCService&&pgrep SCService&&/opt/sc/csgl/SCService/scProtect");

    if (tid == "")
        return false;
    return true;
}

bool xk(pstring host) {
    pstring tid = plib::xsh(host, "pkill -9 scProtect&&pkill -9 SCService&&pgrep SCService");
    if (tid == "")
        return true;
    return false;
}


