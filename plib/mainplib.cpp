#include "pmysql.h"
#include "pudp.h"
#include "ptrans.h"

void thread_test() {
    pudp udp;
    udp.init(3333);
    //    hlog(udp.bind());
    hlog(udp.bind("224.0.0.3"));
    while (1) {
        hlog(udp.recvx());
        //        if(udp.recvx()<0)
        //           break;
        //        hlog(udp.type);
        udp.setHostDES("224.0.0.4");
        //        udp.setHostDES(plib::getIPLocal());
        hlog(udp.sendx("xlfd", 55.555));
    }
    hlog("finish");
}

void thread_test2() {
    pudp udptest;
    udptest.init(3333);
    hlog(udptest.bind("224.0.0.3"));
    while (1) {
        hlog(udptest.recvx());
        //        hlog(udptest.type);
        //        hlog(udptest.getClass<double>());
    }
}

void testpudp() {
    hlog(plib::killProcessAllByNameSelfExceptSelf());
    std::thread(thread_test).detach();
    std::thread(thread_test2).detach();

    plib::sleep(1000);
    pudp udp;
    udp.init(3333);

    udp.bind("224.0.0.4");
    //    hlog(udp.bind());
    udp.setHostDES("224.0.0.3");
    //    udp.setHostDES(plib::getIPLocal());
    hlog(udp.sendx("test", ("zzzzz")));

    while (1) {
        hlog(udp.recvx());
        plib::sleep(1000);
    }
}

class INFO_SOURCE {
public:
    int id;//包id，根据该id在DEST中查找对应包
    char arrchIP[30];//该包的IP，用于区分包
    char arrchTimeStamp[30];//发送时间，计算delay用
};

void thread_recv() {
    pstring strIplocal = "172.16.11.6";
    pudp udprecv;
    udprecv.init(1111, strIplocal);
    udprecv.bind();
    while (1) {
        if (udprecv.recvx() < 0)
            break;
        pliststring lrecv = udprecv.getClass<pliststring>();
        hlog(lrecv);
    }
}

class ctest {
public:
    pstring name;
    int id;
};

int main(int argc, char *argv[]) {
    plib::killProcessAllByNameSelfExceptSelf();
    testpudp();
//    hlog("xxxxxxxxxxx");
//    char ch = 0x11;
//    plib::showBin(ch);
//    ptime tm("2020-12-02 12:12:12");
//    hlog(tm);
//    std::thread thtest(thread_recv);
//    thtest.detach();
//    pstring strIplocal="172.16.11.6";
//    plib::sleep(1000);
//    pudp udpsend;
//    udpsend.init(1111,strIplocal);
//    pliststring lres;
//    lres.append("xlfd");
//    lres.append("zero");
//    udpsend.sendx("test",lres);
//    plib::sleep(3000);
//    while (1) {
//        plib::sleep(1000);
//    }
    return 0;
}
