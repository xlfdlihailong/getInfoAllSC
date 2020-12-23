#ifndef XPLIB_H
#define XPLIB_H

#include "clib.h"
#include <iostream>
//#include <typeinfo>
#include <sstream>
#include <istream>
#include <fstream>
#include <ostream>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <map>
#include <typeinfo>
#include <algorithm>
#include <netdb.h>//用于host等，域名转ip
#include <set>
#include <typeinfo>
#include <iomanip>
#include <thread>
#include <functional>

using namespace std;

//别名
#define lenPack 64*1024 //一包tcp的长度
typedef unsigned char uchar;
typedef unsigned long long ulonglong;
typedef long long longlong;
#define cinit(pdata) memset(pdata,0,sizeof(pdata))//仅适用于此类型: char[xxx]不适用于指针
//全局变量，hlog中要用的，在头文件中extern，cpp中再申请一遍，头文件中不能赋值
extern std::string strPathLog;
extern string strPathConf;
extern bool bexit;
extern int LenPackTCP;
//所有服务器


#define getvarname(var) #var
//日志,支持多线程，标准的好像不支持多线程


#define hlogonenoenter(var) HLOGFunpNoEnter(HLOG_FUN_PREV,#var,var)

template<class T>
int HLOGFunpNoEnter(const char *file, int line, const char *name, const T &t)//配套重载打印函数时不要加&
{
    int iFd;                              /*文件描述符*/
    int iFd1;
    //    std::cout<<"size: "<<sizeof(T)<<std::endl;
    //    char     arrchBuf[655350] = {0};       /*写入参数前的格式信息*/
    string strname(name);
    ostringstream ost;
    ost << t;
    //判断 hlog("xxx")这种仅是字符串的格式
    ostringstream os;
    if (strname == ost.str() || strname == "\"" + ost.str() + "\"")
        os << t;
    else
        os << strname + " :  " << t;
    string str = os.str();

    int len = str.size() + 1024;
    char *arrchBuf = (char *) malloc(len);
    memset(arrchBuf, 0, len);


    char *pchIndex;                        /*当前位置*/
    struct tm struTm;
    time_t tmNow;                            /*当前时间*/
    char pcHLOGPath[1024 + 1];    /*日志路径*/
    strcpy(pcHLOGPath, strPathLog.c_str());
    iFd1 = STDOUT_FILENO;
    pchIndex = strrchr(pcHLOGPath, '.');
    //根据当前日期写入日志文件
    if (pchIndex == NULL) {
        pchIndex = pcHLOGPath + strlen(pcHLOGPath);
    }
    memcpy(arrchBuf, pcHLOGPath, pchIndex - pcHLOGPath);
    time(&tmNow);
    memset(&struTm, 0, sizeof(struct tm));
    localtime_r(&tmNow, &struTm);//线程安全需要，否则多线程不行
    strftime(arrchBuf + strlen(arrchBuf), 20, "%Y-%m-%d", &struTm);
    sprintf(arrchBuf + strlen(arrchBuf), "%s", pchIndex);
    //打开自定义的文件,如果文件打开失败，则退出
    if ((iFd = open(arrchBuf, O_RDWR | O_APPEND | O_CREAT, 0666)) == -1) {
        free(arrchBuf);
        arrchBuf = NULL;
        return -1;
    }


    //    string str=to_string(string(name),t);
    //定义信息说明的格式，并写入缓存
    memset(arrchBuf, 0, sizeof(arrchBuf));

    time(&tmNow);

    memset(&struTm, 0, sizeof(struct tm));
    localtime_r(&tmNow, &struTm);//线程安全
    strftime(arrchBuf, 20, "[%T", &struTm);
    struct timeval tvt;
    gettimeofday(&tvt, NULL);
    sprintf(arrchBuf + strlen(arrchBuf), ".%d", (int) ((double) tvt.tv_usec / 1000));

    string str2(file);
    string pattern = "/";
    string::size_type pos;
    vector<string> result;
    str2 += pattern;
    int size = str2.size();
    for (size_t i = 0; i < size; i++) {
        pos = str2.find(pattern, i);
        if (pos < size) {
            string s = str2.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }


    const char *fileOnly = result[result.size() - 1].c_str();
    sprintf(arrchBuf + strlen(arrchBuf), "]-[tid:%5ld]-[%s:%d]", syscall(__NR_gettid), fileOnly, line);


    sprintf(arrchBuf, "%-20s", arrchBuf);

    //如果信息说明格式未写入成功，则直接写入参数列表，否则按照信息说明格式写入
    if (strlen(arrchBuf) == 0) {
        sprintf(arrchBuf, "%s", str.c_str());
    } else {
        sprintf(arrchBuf + strlen(arrchBuf), " %s", str.c_str());
    }
    write(iFd, arrchBuf, strlen(arrchBuf));
    write(iFd1, arrchBuf, strlen(arrchBuf));
    close(iFd);

    free(arrchBuf);
    arrchBuf = NULL;
    return 0;
}

#define hlogone(var) HLOGFunp(HLOG_FUN_PREV,#var,var)

template<class T>
int HLOGFunp(const char *file, int line, const char *name, const T &t)//配套重载打印函数时不要加&
{
    int iFd;                              /*文件描述符*/
    int iFd1;
    //    std::cout<<"size: "<<sizeof(T)<<std::endl;
    //    char     arrchBuf[655350] = {0};       /*写入参数前的格式信息*/
    string strname(name);
    ostringstream ost;
    ost << t;
    //判断 hlog("xxx")这种仅是字符串的格式
    ostringstream os;
    if (strname == ost.str() || strname == "\"" + ost.str() + "\"")
        os << t;
    else
        os << strname + " :  " << t;
    string str = os.str();
    //    cout<<"str: "<<str<<endl;
    int len = str.size() + 1024;
    char *arrchBuf = (char *) malloc(len);
    memset(arrchBuf, 0, len);


    char *pchIndex;                        /*当前位置*/
    struct tm struTm;
    time_t tmNow;                            /*当前时间*/
    char pcHLOGPath[1024 + 1];    /*日志路径*/
    strcpy(pcHLOGPath, strPathLog.c_str());
    iFd1 = STDOUT_FILENO;
    pchIndex = strrchr(pcHLOGPath, '.');
    //根据当前日期写入日志文件
    if (pchIndex == NULL) {
        pchIndex = pcHLOGPath + strlen(pcHLOGPath);
    }
    memcpy(arrchBuf, pcHLOGPath, pchIndex - pcHLOGPath);
    time(&tmNow);
    memset(&struTm, 0, sizeof(struct tm));
    localtime_r(&tmNow, &struTm);//线程安全需要，否则多线程不行
    strftime(arrchBuf + strlen(arrchBuf), 20, "%Y-%m-%d", &struTm);
    sprintf(arrchBuf + strlen(arrchBuf), "%s", pchIndex);
    //打开自定义的文件,如果文件打开失败，则退出
    if ((iFd = open(arrchBuf, O_RDWR | O_APPEND | O_CREAT, 0666)) == -1) {
        free(arrchBuf);
        arrchBuf = NULL;
        return -1;
    }


    //    string str=to_string(string(name),t);
    //定义信息说明的格式，并写入缓存
    memset(arrchBuf, 0, sizeof(arrchBuf));

    time(&tmNow);

    memset(&struTm, 0, sizeof(struct tm));
    localtime_r(&tmNow, &struTm);//线程安全
    strftime(arrchBuf, 20, "[%T", &struTm);
    struct timeval tvt;
    gettimeofday(&tvt, NULL);
    sprintf(arrchBuf + strlen(arrchBuf), ".%d", (int) ((double) tvt.tv_usec / 1000));

    string str2(file);
    string pattern = "/";
    string::size_type pos;
    vector<string> result;
    str2 += pattern;
    int size = str2.size();
    for (size_t i = 0; i < size; i++) {
        pos = str2.find(pattern, i);
        if (pos < size) {
            string s = str2.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }


    const char *fileOnly = result[result.size() - 1].c_str();
    sprintf(arrchBuf + strlen(arrchBuf), "]-[tid:%5ld]-[%s:%d]", syscall(__NR_gettid), fileOnly, line);


    sprintf(arrchBuf, "%-20s", arrchBuf);
    //加个回车
    str += '\n';
    //    cout<<"str: "<<str;
    //不用原来的方式，因为发现中间夹杂\0时会导致显示不全
    //如果信息说明格式未写入成功，则直接写入参数列表，否则按照信息说明格式写入
    //    if (strlen(arrchBuf) == 0)
    //    {
    //        sprintf(arrchBuf, "%s\n", str.c_str());
    //    }
    //    else
    //    {
    //    sprintf(arrchBuf + strlen(arrchBuf), " %s\n", str.c_str());
    //    }
    //    write(iFd, arrchBuf, strlen(arrchBuf));
    //    write(iFd1, arrchBuf, strlen(arrchBuf));
    //最终长度
    int lenRes = strlen(arrchBuf) + str.size();
    memcpy(arrchBuf + strlen(arrchBuf), str.c_str(), str.size());

    write(iFd, arrchBuf, lenRes);
    write(iFd1, arrchBuf, lenRes);
    close(iFd);

    free(arrchBuf);
    arrchBuf = NULL;
    return 0;
}

//通用时间类型
typedef struct _BASE_DATETIME {
    unsigned short JD;
    unsigned int JS;
} BASE_DATETIME;

typedef struct _DATETIME {
    int year;     //年
    int month;    //月
    int day;      //日
    int hour;     //时
    int min;      //分
    int sec;      //秒
    int msec;     //0.1毫妙
} DateTime;

/*********************************************************
    函数功能：
                检查输入的日期是否正确
    函数参数：
                year   ：年份（>=2000）
                month  ：月份（1～12）
                day    ：日（1～31,各月不同）
    函数返回值：
                  0：日期格式正确
                 -1：日期格式错误
**********************************************************/
static int CheckDate(unsigned int year, unsigned int month, unsigned int day) {
    if (year < 2000 || year > 5000)
        return -1;
    else if (month < 1 || month > 12)
        return -1;
    else if (day < 1 || day > 31)
        return -1;

    switch (month)   //4、6、9、11月每月30天，除2月外其它每月31天
    {
        case 2:
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                if (day > 29) return -1;
            } else if (day > 28) return -1;
            break;
        case 4:
            if (day > 30) return -1;
            break;
        case 6:
            if (day > 30) return -1;
            break;
        case 9:
            if (day > 30) return -1;
            break;
        case 11:
            if (day > 30) return -1;
            break;
    }
    return 0;
}


/*********************************************************
    函数功能：
                计算2000.1.1至输入日期的积日
    函数参数：
                year   ：年份
                month  ：月份（1～12）
                day    ：日（1～31）
    函数返回值：
                >=0：积日
                 -1：日期格式错误
**********************************************************/
static int GetJD(unsigned int year, unsigned int month, unsigned int day) {
    //判断日期取值是否正确
    if (CheckDate(year, month, day) < 0) {
        return -1;
    }
    unsigned int year2 = 2000;
    unsigned int month2 = 1;
    unsigned int day2 = 1;

    int nd, nm, ny; //new_day, new_month, new_year
    int od, om, oy; //old_day, oldmonth, old_year

    nm = (month2 + 9) % 12;
    ny = year2 - nm / 10;
    nd = 365 * ny + ny / 4 - ny / 100 + ny / 400 + (nm * 306 + 5) / 10 + (day2 - 1);

    om = (month + 9) % 12;
    oy = year - om / 10;
    od = 365 * oy + oy / 4 - oy / 100 + oy / 400 + (om * 306 + 5) / 10 + (day - 1);

    return (od - nd) + 1;
}


/*********************************************************
    函数功能：
                计算输入时间的积秒
    函数参数：
                hour ：时（0～23）
                min  ：分（0～59）
                sec  ：秒（0～59）
                msec ：0.1毫秒（0～9999）
    函数返回值：
                >=0：积秒
                 -1：时间格式错误
**********************************************************/
static int GetJS(unsigned short hour, unsigned short min, unsigned short sec, unsigned short msec) {
    if (hour >= 0 && hour < 24) {
        if (min >= 0 && min < 60) {
            if (sec >= 0 && sec < 60) {
                if (msec >= 0 && msec <= 9999)
                    return ((hour * 3600 + min * 60 + sec) * 10000 + msec);
                else
                    return -1;
            } else
                return -1;
        } else
            return -1;
    } else
        return -1;
}


/*********************************************************
    函数功能：
                计算当前日期的积日(自2000.1.1起)
    函数参数：
                无
    函数返回值：
                积日
**********************************************************/
static unsigned short GetCurrentJD() {
    time_t loctime;
    time(&loctime);
    struct tm tm1;
    loctime += 8 * 3600;
    tm1 = *gmtime(&loctime);   //
    return GetJD((tm1.tm_year + 1900), (tm1.tm_mon + 1), tm1.tm_mday);
}


/*********************************************************
    函数功能：
                计算当前时间的积秒(自0时0分0秒起)
    函数参数：
                无
    函数返回值：
                积秒(单位为0.1毫秒)
**********************************************************/
static unsigned int GetCurrentJS() {
    struct timeval a;
    struct timezone b;
    gettimeofday(&a, &b);
    return (((a.tv_sec + 28800) % 86400) * 10000 + a.tv_usec / 100); //28800=8*3600,补时区差8小时
}


/*********************************************************
    函数功能：
                计算当前日期时刻
    函数参数：
                无
    函数返回值：
                pdt：DateTime结构指针
**********************************************************/
static DateTime GetDateTime() {
    struct timeval a;
    struct timezone b;
    struct tm tm1;
    gettimeofday(&a, &b);
    a.tv_sec += 8 * 3600;   //补8小时时差
    tm1 = *gmtime(&a.tv_sec);
    DateTime dt;
    dt.year = tm1.tm_year + 1900;
    dt.month = tm1.tm_mon + 1;
    dt.day = tm1.tm_mday;
    dt.hour = tm1.tm_hour;
    dt.min = tm1.tm_min;
    dt.sec = tm1.tm_sec;
    dt.msec = a.tv_usec / 100;
    return dt;
}

/*********************************************************
    函数功能：
                根据JD、JS计算日期时刻
    函数参数：
                JD 积日(自2000.1.1起)
                JS  天内积秒（0.1毫秒为单位，0～863999999）
                dt：DateTime结构指针，输出值
    函数返回值：
                0 成功
                -1 参数错误
**********************************************************/
static int GetDateTimeFromJDJS(unsigned short JD, unsigned int JS, DateTime &dt) {
    if (JS >= 863999999)//积秒不能大于863999999个单位的0.1毫秒（1秒）
    {
        return -1;
    }
    //计算年月日
    int Jd1, J, N1, L1, L2, L3, Y1, M1, MJd;
    JD -= 1;
    MJd = JD;
    Jd1 = (int) (2451545.0 + MJd);
    J = (int) (Jd1 + 0.5);            // 四舍五入
    // 计算
    N1 = 4 * (J + 68569) / 146097;
    L1 = J + 68569 - (N1 * 146097 + 3) / 4;
    Y1 = 4000 * (L1 + 1) / 1461001;
    L2 = L1 - (1461 * Y1) / 4 + 31;
    M1 = (80 * L2) / 2447;
    dt.day = L2 - (2447 * M1) / 80;
    L3 = M1 / 11;
    dt.month = (int) (M1 + 2 - 12 * L3);
    dt.year = (int) (100 * (N1 - 49) + Y1 + L3);
    //计算时分秒
    dt.msec = (int) (JS % 10000);
    dt.hour = (int) (JS / 36000000);
    dt.min = (int) (((JS / 10000) % 3600) / 60);
    dt.sec = (int) ((JS / 10000) % 3600 % 60);
    return 0;
}


static string GetTimeNhoursLater(int N) {
    string strRet;
    char acTemp[30] = {'\0'};
    unsigned short iJR = GetCurrentJD();
    unsigned int iJS = GetCurrentJS();
    int i10Hours = N * 3600 * 1000 * 10;
    int i24Hours = 24 * 3600 * 1000 * 10;
    if (i24Hours > i10Hours + iJS) {
        iJS += i10Hours;
    } else {
        iJS = i10Hours + iJS - i24Hours;
        iJR += 1;
    }
    DateTime stuDateTime;
    int iRet = GetDateTimeFromJDJS(iJR, iJS, stuDateTime);
    if (iRet == 0) {
        sprintf(acTemp, "'%d-%d-%d %d:%d:%d'",
                stuDateTime.year,
                stuDateTime.month,
                stuDateTime.day,
                stuDateTime.hour,
                stuDateTime.min,
                stuDateTime.sec);
        strRet = string(acTemp);
    }
    return strRet;
}


static BASE_DATETIME StringToBaseDateTime(string strTime) {
    BASE_DATETIME stuDateTime;
    if (strTime == "") {
        stuDateTime.JD = 0;
        stuDateTime.JS = 0;
        return stuDateTime;
    }
    string strYear = strTime.substr(0, 4);
    int iYear = atoi(strYear.c_str());

    string strTemp = strTime.substr(5);
    string::size_type index = strTemp.find_first_of('-');
    string strMonth = strTemp.substr(0, index);
    int iMonth = atoi(strMonth.c_str());

    strTemp = strTemp.substr(index + 1);
    index = strTemp.find_first_of(' ');
    string strDay = strTemp.substr(0, index);
    int iDay = atoi(strDay.c_str());

    strTemp = strTemp.substr(index + 1);
    index = strTemp.find_first_of(':');
    string strHour = strTemp.substr(0, index);
    int iHour = atoi(strHour.c_str());

    strTemp = strTemp.substr(index + 1);
    index = strTemp.find_first_of(':');
    string strMinitue = strTemp.substr(0, index);
    int iMinitue = atoi(strMinitue.c_str());

    strTemp = strTemp.substr(index + 1);
    string strSecond = strTemp;
    int iSecond = atoi(strSecond.c_str());

    stuDateTime.JD = GetJD(iYear, iMonth, iDay);
    stuDateTime.JS = GetJS(iHour, iMinitue, iSecond, 0);

    return stuDateTime;
}


static string GetCurrentTimeString(unsigned short iJR, unsigned int iJS) {
    string strRet;
    DateTime stuDateTime;
    char acTemp[30] = {'\0'};
    int iRet = GetDateTimeFromJDJS(iJR, iJS, stuDateTime);
    if (iRet == 0) {
        sprintf(acTemp, "'%04d-%02d-%02d %02d:%02d:%02d.%03d'",
                stuDateTime.year,
                stuDateTime.month,
                stuDateTime.day,
                stuDateTime.hour,
                stuDateTime.min,
                stuDateTime.sec,
                stuDateTime.msec / 10);
        strRet = string(acTemp);
    }
    return strRet;
}

//只分割一个字符的
//vector<string> common_split(string str, string pattern)
static vector<string> common_split_one(string str, string pattern) {

    string::size_type pos;
    vector<string> result;
    str += pattern;
    string::size_type size = str.size();
    for (string::size_type i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if (pos < size) {
            string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

//可分割多个字符产，并自动去掉空的
static vector<string> common_split(string str, string pattern) {
    //只分割一个字符串的
    //    string::size_type pos;
    //    vector<string> result;
    //    str+=pattern;
    //    int size=str.size();
    //    for(size_t i=0;i<size;i++)
    //    {
    //        pos=str.find(pattern,i);
    //        if(pos<size)
    //        {
    //            string s=str.substr(i,pos-i);
    //            result.push_back(s);
    //            i=pos+pattern.size()-1;
    //        }
    //    }
    //    return result;
    string s = str;
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()) {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while (i != s.size() && flag == 0) {
            flag = 1;
            for (string_size x = 0; x < pattern.size(); ++x)
                if (s[i] == pattern[x]) {
                    ++i;
                    flag = 0;
                    break;
                }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while (j != s.size() && flag == 0) {
            for (string_size x = 0; x < pattern.size(); ++x)
                if (s[j] == pattern[x]) {
                    flag = 1;
                    break;
                }
            if (flag == 0)
                ++j;
        }
        if (i != j) {
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return result;
}

static int pHLOGFunp(const char *file, int line, string str)//配套重载打印函数时不要加&
{
    int iFd;                              /*文件描述符*/
    int iFd1;
    int len = str.size() + 1024;
    char *arrchBuf = (char *) malloc(len);
    memset(arrchBuf, 0, len);
    char *pchIndex;                        /*当前位置*/
    struct tm struTm;
    time_t tmNow;                            /*当前时间*/
    char pcHLOGPath[1024 + 1];    /*日志路径*/
    strcpy(pcHLOGPath, strPathLog.c_str());
    iFd1 = STDOUT_FILENO;
    pchIndex = strrchr(pcHLOGPath, '.');
    //根据当前日期写入日志文件
    if (pchIndex == NULL) {
        pchIndex = pcHLOGPath + strlen(pcHLOGPath);
    }
    memcpy(arrchBuf, pcHLOGPath, pchIndex - pcHLOGPath);
    time(&tmNow);
    memset(&struTm, 0, sizeof(struct tm));
    localtime_r(&tmNow, &struTm);//线程安全需要，否则多线程不行
    strftime(arrchBuf + strlen(arrchBuf), 20, "%Y-%m-%d", &struTm);
    sprintf(arrchBuf + strlen(arrchBuf), "%s", pchIndex);
    //打开自定义的文件,如果文件打开失败，则退出
    if ((iFd = open(arrchBuf, O_RDWR | O_APPEND | O_CREAT, 0666)) == -1) {
        free(arrchBuf);
        arrchBuf = NULL;
        return -1;
    }
    //定义信息说明的格式，并写入缓存
    memset(arrchBuf, 0, sizeof(arrchBuf));
    time(&tmNow);
    memset(&struTm, 0, sizeof(struct tm));
    localtime_r(&tmNow, &struTm);//线程安全
    strftime(arrchBuf, 20, "[%T", &struTm);
    struct timeval tvt;
    gettimeofday(&tvt, NULL);
    sprintf(arrchBuf + strlen(arrchBuf), ".%03d", (int) ((double) tvt.tv_usec / 1000));
    string str2(file);
    string pattern = "/";
    string::size_type pos;
    vector<string> result;
    str2 += pattern;
    string::size_type size = str2.size();
    for (string::size_type i = 0; i < size; i++) {
        pos = str2.find(pattern, i);
        if (pos < size) {
            string s = str2.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    const char *fileOnly = result[result.size() - 1].c_str();
    sprintf(arrchBuf + strlen(arrchBuf), "]-[tid:%5ld]-[%s:%d]", syscall(__NR_gettid), fileOnly, line);
    sprintf(arrchBuf, "%-20s", arrchBuf);
    //    不用原来的方式，因为发现中间夹杂\0时会导致显示不全
    //    //如果信息说明格式未写入成功，则直接写入参数列表，否则按照信息说明格式写入
    //    if (strlen(arrchBuf) == 0)
    //        sprintf(arrchBuf, "%s\n", str.c_str());
    //    else
    //        sprintf(arrchBuf + strlen(arrchBuf), " %s\n", str.c_str());
    //    write(iFd, arrchBuf, strlen(arrchBuf));
    //    write(iFd1, arrchBuf, strlen(arrchBuf));
    str += '\n';
    int lenRes = strlen(arrchBuf) + str.size();
    memcpy(arrchBuf + strlen(arrchBuf), str.c_str(), str.size());
    write(iFd, arrchBuf, lenRes);
    write(iFd1, arrchBuf, lenRes);

    close(iFd);
    free(arrchBuf);
    arrchBuf = NULL;
    return 0;
}

template<typename T>
void printarg(ostream &os, T a) {
    //隔开，防止打印字符串时乱码不能分割，与序列化一个原理
    //但是不能用跟序列化一样的字符1，不然会导致序列化的混乱
    //    if(typeid(a).name()==typeid(char).name())
    //        hlogone("char");
    //    hlogone(typeid(a).name());
    //    hlogone(typeid(char).name());
    string str = (typeid(a).name());
    if (str.find("pstring") != string::npos) {
        ostringstream oss;
        oss << a;
        string strtmp = oss.str();
        string strlast = strtmp.substr(0, strtmp.size() - 1);
        os << strlast << "\2";
    } else
        os << a << "\2";
}

template<class...Args>
void expand(Args... args) {
    ostringstream os;
    int arr[] = {(printarg(os, args), 0)...};
    //或者改进为 std::initializer_list<int>{(printarg(args), 0)...};lumda
    string stres = os.str();
    //    hlogone(stres);
    //这个地方不能用多个字符串分割的，不然结果是空字符串的时候会自动去掉
    vector<string> ls = common_split_one(stres, string("\2"));
    ls.erase(ls.end() - 1);
    //    for(size_t i=0;i<ls.size();i++)
    //    {
    //        hlogone(ls[i]);
    //    }
    const char *file = ls[0].c_str();
    int line = atoi(ls[1].c_str());

    //这个一定是实际参数个数，
    vector<string> listValues;
    //    hlogone(ls.size());
    for (size_t i = 3; i < ls.size(); i++) {
        listValues.push_back(ls[i]);
    }
    //真实参数个数
    //    hlogone(listValues.size());
    //最终结果
    vector<string> listNames;
    //如果真是参数个数是1，则不用分了
    if (listValues.size() == 1) {
        listNames.push_back(ls[2]);
    } else//否则要根据逗号分
    {
        //先按逗号分，后来再整理ls[2]是传进来的参数名字，带逗号的
        vector<string> listNamesOrigin = common_split(ls[2], ",");
        //        hlogone(listNamesOrigin.size());

        //如果name数正好等于value数，那就正好了，不然就要考虑其他情况
        if (listNamesOrigin.size() == listValues.size()) {
            for (size_t i = 0; i < listNamesOrigin.size(); i++) {
                listNames.push_back(listNamesOrigin[i]);
            }
        } else {
            for (size_t i = 0; i < listNamesOrigin.size(); i++) {
                string stri = listNamesOrigin[i];
                int il = 0;
                int ir = 0;
                int ishuangyinhao = 0;
                //算（-）的个数
                for (size_t q = 0; q < stri.size(); q++) {
                    if (stri[q] == '(')
                        il++;
                    if (stri[q] == ')')
                        ir++;
                    if (stri[q] == '\"')
                        ishuangyinhao++;
                }
                int icha = il - ir;
                //        hlogone(icha);

                //如果这一项不包扩括号和引号，则就是变量
                if (icha == 0 && ishuangyinhao == 0) {
                    listNames.push_back(stri);
                    //            hlogone(stri);
                }//优先级是先处理引号
                else if (ishuangyinhao > 0) {
                    string strjoin = stri;
                    //            hlogone(stri);
                    //从这个开始往后数
                    for (size_t j = i + 1; j < listNamesOrigin.size(); j++) {
                        string strj = listNamesOrigin[j];
                        strjoin += "," + strj;
                        for (size_t p = 0; p < strj.size(); p++) {
                            if (strj[p] == '"') {
                                ishuangyinhao--;
                                //                        hlogone(ishuangyinhao);
                            }
                        }
                        //为了防止互相干扰
                        if (((ishuangyinhao == 0))) {
                            //把i指针移到这
                            i = j;
                            break;
                        }
                    }

                    //            hlogone(strjoin);
                    listNames.push_back(strjoin);
                }
                    //只包含（但不包含），则要跟下一个结合，遇到（增加，遇到）减少，直到为0为止算一个
                else if (icha > 0) {
                    string strjoin = stri;
                    //从这个开始往后数
                    for (size_t j = i + 1; j < listNamesOrigin.size(); j++) {
                        string strj = listNamesOrigin[j];
                        strjoin += "," + strj;
                        for (size_t p = 0; p < strj.size(); p++) {
                            if (strj[p] == '(')
                                il++;
                            if (strj[p] == ')')
                                ir++;
                        }
                        //为了防止互相干扰
                        if (((il - ir == 0))) {
                            //把i指针移到这
                            i = j;
                            break;
                        }
                    }
                    listNames.push_back(strjoin);
                }
            }
        }
    }
    //打印
    //        hlogone(listNames.size());
    //        for(size_t i=0;i<listNames.size();i++)
    //        {
    //            hlogone(listNames[i]);
    //        }
    //        hlogone(listValues.size());
    //        for(size_t i=0;i<listValues.size();i++)
    //        {
    //            hlogone(listValues[i]);
    //        }
    //整理了一遍之后，如果发现

    string strval;
    for (size_t i = 0; i < listNames.size(); i++) {
        //在此判断是否是单独字符串,或者是pstring()<<这种的行为
        //                hlogone(listNames[i]);
        //                hlogone(listValues[i]);
        //如果名字第一个字符和最后一个是",则说明是字符串,直接打印value,因为有可能会有这种的xfdsf",,wfew,
        if ((listNames[i][0] == '"' && listNames[i][listNames[i].size() - 1] == '"') ||
            (listNames[i].find("pstring()<<") != string::npos) ||
            (listNames[i].find("pstring() <<") != string::npos)) {
            strval += "[" + listValues[i] + "] ";
        } else
            strval += "[" + listNames[i] + " : " + listValues[i] + "] ";
    }

    strval = strval.substr(0, strval.size() - 1);
    //    hlogone(strval);
    pHLOGFunp(file, line, strval);
}
//支持多参数不同类型的联合打印
//支持函数嵌套带逗号，已处理
//hlog(itest,funtest(funtest2(funtest3(2),2),3,funtest2(6,8)),db,funtest2(2,3),funtest3(3));
// 实现HLOG: string strhost="172.16.12.1";
//hlog(pstring()<<"connect "<<strhost<<" success");
#define hlog(...) expand(HLOG_FUN_PREV,#__VA_ARGS__,__VA_ARGS__)


//判断字符串是否为utf8
static bool common_isStringUTF8(string stro) {
    const char *str = stro.c_str();
    unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr = *str;
    bool bAllAscii = true;
    for (unsigned int i = 0; str[i] != '\0'; ++i) {
        chr = *(str + i);
        //判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
        if (nBytes == 0 && (chr & 0x80) != 0) {
            bAllAscii = false;
        }
        if (nBytes == 0) {
            //如果不是ASCII码,应该是多字节符,计算字节数
            if (chr >= 0x80) {
                if (chr >= 0xFC && chr <= 0xFD) {
                    nBytes = 6;
                } else if (chr >= 0xF8) {
                    nBytes = 5;
                } else if (chr >= 0xF0) {
                    nBytes = 4;
                } else if (chr >= 0xE0) {
                    nBytes = 3;
                } else if (chr >= 0xC0) {
                    nBytes = 2;
                } else {
                    return false;
                }
                nBytes--;
            }
        } else {
            //多字节符的非首字节,应为 10xxxxxx
            if ((chr & 0xC0) != 0x80) {
                return false;
            }
            //减到为零为止
            nBytes--;
        }
    }
    //违返UTF8编码规则
    if (nBytes != 0) {
        return false;
    }
    if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
        return true;
    }
    return true;
}

//递归获取目录下所有文件
static void common_getAllFilesAndPathsRecursion(string path, vector<string> &files, vector<string> &paths) {
    DIR *dir;
    struct dirent *ptr;
    if ((dir = opendir(path.c_str())) == NULL) {
        perror("Open dri error...");
        exit(1);
    }
    while ((ptr = readdir(dir)) != NULL) {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        else if (ptr->d_type == 8)//file
            files.push_back(path + "/" + ptr->d_name);
        else if (ptr->d_type == 10)//link file这个就不要了，不然会导致文件再考一遍,且软连接丢失，没事，因为scp也存在这个 问题，以后注意，尽量不要用软连接
            continue;
        else if (ptr->d_type == 4) {
            //files.push_back(ptr->d_name);//dir
            paths.push_back(path + "/" + ptr->d_name);
            common_getAllFilesAndPathsRecursion(path + "/" + ptr->d_name, files, paths);
        }
    }
    closedir(dir);
}

//序列化方案
template<class T>
static string common_toString(T t)//与类中重载的对应
{
    stringstream ss;
    ss << t;
    return ss.str();
}

template<class T>
static void common_toClass(string str, T &t) {
    stringstream ss(str);
    ss >> t;
}

template<class T>
static T common_toClass(string str) {
    T t;
    bzero(&t, sizeof(T));
//    hlogone(str);
    stringstream ss(str);
//    hlogone(ss.str());
    ss >> t;
    return t;
}


//is跳过n个字符
static void common_skipCountch(istream &is, int len) {
    for (size_t i = 0; i < len; i++) {
        is.get();
    }
}

//is提取字符串,跳过空格,根据[]来判断
static string common_istreamToString(istream &is) {
    string all;
    char ch = is.get();
    while (ch != ']') {
        all.push_back(ch);
        ch = is.get();
    }
    return all;
}

//跳过[之前的所有
static void common_skipDescription(istream &is) {
    while (1) {
        char ch = is.get();
        if (ch == '[') {
            break;
        }
    }
}

//将十六进制的字符串转换成char
static char common_hstrtoch(char s[]) {
    int i;
    int n = 0;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        i = 2;
    } else {
        i = 0;
    }
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'); ++i) {
        if (tolower(s[i]) > '9') {
            n = 16 * n + (10 + tolower(s[i]) - 'a');
        } else {
            n = 16 * n + (tolower(s[i]) - '0');
        }
    }
    return (char) n;
}


// 编码转换
static int
common_code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
    iconv_t cd;
    int rc;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset, from_charset);
    if (cd == 0) return -1;
    if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
    iconv_close(cd);
    return 0;
}

// utf8转为gbk
static string common_string_To_GBK(string str) {
    const char *inbuf = str.c_str();
    int inlen = strlen(inbuf);
    string strRet;
    strRet.resize(inlen * 2 + 2);
    if (common_code_convert("utf-8", "gbk", const_cast<char *>(inbuf), inlen, &strRet[0], strRet.size()))
        return inbuf;
    return strRet;
}

// gbk转为utf8
static string common_string_To_UTF8(string str) {
    const char *inbuf = str.c_str();
    int inlen = strlen(inbuf);
    string strRet;
    strRet.resize(inlen * 2 + 2);
    if (common_code_convert("gbk", "utf-8", const_cast<char *>(inbuf), inlen, &strRet[0], strRet.size()))
        return inbuf;
    return strRet;
}

static int common_isIncludeChinese(string stro)//返回0：无中文，返回1：有中文
{
    const char *str = stro.c_str();
    char c;
    while (1) {
        c = *str++;
        if (c == 0) break;  //如果到字符串尾则说明该字符串没有中文字符
        if (c & 0x80)        //如果字符高位为1且下一字符高位也是1则有中文字符
            if (*str & 0x80) return 1;
    }
    return 0;
}

static string common_getSystem() {
    int judge = system("cls");
    if (judge == 0) {
        return "windows";
    } else {
        return "linux";
    }
}

static string common_toChinese(string str) {
    bool isHaveChinese = common_isIncludeChinese(str);
    //    hlog(isHaveChinese);
    //如果不含有中文,直接返回
    if (!isHaveChinese)
        return (str);

    string strSystem = common_getSystem();
    bool iutf8 = common_isStringUTF8(str);

    //    hlog(strSystem,iutf8);
    //如果是windows,且含有中文,且字符串不是GBK而是UTF8,则要转换成GBK
    if (strSystem == "windows" && iutf8) {
        return common_string_To_GBK(str);
    } else if (strSystem == "linux" && !iutf8) {
        //如果是linux,且含有中文,且不是UTF8,要转成UTF8
        return common_string_To_UTF8(str);
    }
    return (str);
}


template<class T>
class plist : public vector<T> {
public:
    plist() {}

    plist(size_t count) {
        T t;
        for (size_t i = 0; i < count; i++) {
            this->push_back(t);
        }
    }

    plist(size_t count, T t) {
        for (size_t i = 0; i < count; i++) {
            this->push_back(t);
        }
    }

    plist(vector<T> v) {
        for (size_t i = 0; i < v.size(); i++) {
            this->push_back(v[i]);
        }
    }

    void append(T t) {
        this->push_back(t);
    }

    T &getDataStart() {
        return this->at(0);
    }

    T &getDataEnd() {
        return this->at(this->size() - 1);
    }

    bool contains(T t) {
        if (std::find(this->begin(), this->end(), t) != this->end())
            return true;
        else
            return false;
    }

    void sortASC() {
        sort(this->begin(), this->end());
    }

    //    template<typename _Compare>
    //    void sortASC(typename _Compare)
    //    {
    //        sort(this->begin(),this->empty(),_Compare);
    //    }

    void sortDESC() {
        sort(this->rbegin(), this->rend());
    }

    //返回 索引
    int find(T t) {
        typename plist<T>::iterator it = std::find(this->begin(), this->end(), t);
        if (it == this->end()) {
            return -1;
        }
        return it - this->begin();
    }


////strDES是要找的目标,stri是列表内部的循环
//    bool fun_pd(pstring strDES,pstring stri)
//    {
//        if(stri==strDES+"111")
//            return true;
//        return false;
//    }
//    int main(int argc, char *argv[])
//    {
//        plist<pstring> lstr;
//        lstr.append("xlfd");
//        lstr.append("zero");
//        lstr.append("nidaye111");
//        hlog(lstr);
//        hlog(lstr.findfun("nidaye",fun_pd));
////因为find_if只有一个参数,所以要用bind处理转换
//    int findfun(T tfind, bool (*fun_condition)(T t, T tf)) {
//        typename plist<T>::iterator it = std::find_if(this->begin(),
//                                                      this->end(), std::bind(fun_condition, tfind, placeholders::_1));
//        if (it == this->end()) {
//            return -1;
//        }
//        return it - this->begin();
//    }
    /*
     * 该函数覆盖了根据结构体字段查找以及直接查找结构体 T2可以是T
     * class CTest
{
public:
    int id;
    double db;
    pstring str;
    CTest()
    {
        id=3;
        db=2.43;
        str="xlfd";
    }
    CTest(int wid,double wdb,pstring wstr)
    {
        id=wid;
        db=wdb;
        str=wstr;
    }
    friend ostream& operator<<(ostream& os,CTest x)
    {
        os<<x.id<<" "<<x.db<<" "<<x.str;
        return os;
    }
    bool operator==(CTest t2)
    {
        if(this->id==t2.id&&this->db==t2.db&&this->str==t2.str)
            return true;
        return false;
    }
};
bool fun_find(double ifind,CTest t)
{
    if(t.db==ifind)
        return true;
    return false;
}
         pmap<pstring,CTest> mt;
    hlog(mt["xlfd"]);
    plist<CTest> ltest;
    ltest.append(CTest(111,1.11,"aaaa"));
    CTest t2(222,2.22,"bbbbbb");
    ltest.append(t2);
    hlog(ltest);
    hlog(ltest.find(CTest(222,2.22,"bbbbbb")));
    hlog(ltest.findx(2.22,fun_find));
     */
    template<class T2>
    int findfun(T2 tfind, bool(*fun_condition)(T2 tfind, T tinner)) {
        typename plist<T>::iterator it = std::find_if(this->begin(), this->end(),
                                                      std::bind(fun_condition, tfind, placeholders::_1));
        if (it == this->end()) {
            return -1;
        }
        return it - this->begin();
    }


    //去重复,自定义结构体需要重载<和==
    void deleteRepeat() {
        sort(this->begin(), this->end());
        this->erase(unique(this->begin(), this->end()), this->end());
    }

    //erase int pos
    bool deleteByIndex(size_t pos) {
        if (pos >= this->size()) {
            string info = "pos>=this->size()";
            hlog(info);
            return false;
        }
        this->erase(this->begin() + pos);
        return true;
    }

    bool deleteBegin() {
        if (this->size() == 0)
            return false;
        this->erase(this->begin());
        return true;
    }

    bool deleteEnd() {
        if (this->size() == 0)
            return false;
        this->erase(this->end() - 1);
        return true;
    }

    void clear() {
        int len = this->size();
        for (size_t i = 0; i < len; i++)
            this->erase(this->begin());
    }

    //加上这个才能提示
    T &operator[](size_t i) {
        assert(i < this->size());
        return ((T &) (this->at(i)));
    }

    friend ostream &operator<<(ostream &os, plist *xv) {
        os << "len: " << xv->size() << " pvector\n{\n";
        for (size_t i = 0; i < xv->size(); i++) {
            os << xv->at(i) << "\n";
        }
        os << "}";
        return os;
    }

    friend ostream &operator<<(ostream &os, plist xv) {
        os << "len: " << xv.size() << " pvector\n{\n";
        for (size_t i = 0; i < xv.size(); i++) {
            os << xv[i] << "\n";
        }

        os << "}";
        return os;
    }

    //为了支持序列化
    friend istream &operator>>(istream &is, plist &xv) {
        for (size_t i = 0; i < 5; i++)
            is.get();

        size_t len = 0;
        is >> len;
        common_skipCountch(is, 9 + 2);
        for (size_t i = 0; i < len; i++) {
            T t;
            is >> t;

            xv.append(t);
            common_skipCountch(is, 1);//跳过空格
        }
        common_skipCountch(is, 1);
        return is;
    }

    plist &operator<<(const T &data) {
        this->append(data);
        return *this;
    }

    //转为vector
    vector<T> toVector() {
        vector<T> v;
        for (size_t i = 0; i < this->size(); i++) {
            v.push_back(this->at(i));
        }
        return v;
    }


    //     //保存,有序列化之后不需要这种传统方式了,直接用
    //    plib::saveSerialization
    //    plib::loadSerialization
    int save(string path) {
        FILE *fp;
        if ((fp = fopen(path.c_str(), "wb")) == NULL) {
            //            hlog("open file fail");
            string info = "打开文件" + path + "失败";
            hlog(info);
            return -1;
        }
        size_t len = this->size();
        fwrite(&len, sizeof(size_t), 1, fp);
        int i;
        for (i = 0; i < this->size(); i++) {
            fwrite(&(this->at(i)), sizeof(T), 1, fp);
        }
        fclose(fp);
        return 0;
    }

    /*
     * 读取向量
     * vector<test> vr=pvector<TASK_STATUS_STRUCT>::load(path);
     */
    static plist<T> load(string path) {
        plist<T> lr;
        FILE *fp;
        if ((fp = fopen(path.c_str(), "rb")) == NULL) {
            return lr;
        }
        //首先要清空lr，不然一直增长
        lr.clear();
        size_t len = 0;
        fread(&len, sizeof(size_t), 1, fp);
        //        XCLOG_INT(len);
        int i;
        for (i = 0; i < len; i++) {
            void *rd = (void *) malloc(sizeof(T));
            fread(rd, sizeof(T), 1, fp);
            lr.push_back((*(T *) rd));
            free(rd);
            rd = NULL;
        }
        fclose(fp);
        return lr;
    }
};

//1字节的都要用这个
class pbyte {
public:
    unsigned char data;

    pbyte() {

    }

    pbyte(char ch) {
        this->data = ch;
    }

    char toChar() {
        return data;
    }

    friend ostream &operator<<(ostream &os, pbyte *x) {
        //虽然是长度5，但是到流里面是4
        char ac[5];
        memset(ac, 0, sizeof(ac));
        //02能保证一定是长度4，即使是\0
        sprintf(ac, "0x%02X", x->data);
        os << ac;
        return os;
    }

    //只是打印的时候打成0，实际上值还是\0，不然的话打印出问题
    //直接用16进制打印也没问题
    friend ostream &operator<<(ostream &os, pbyte x) {
        //虽然是长度5，但是到流里面是4
        char ac[5];
        memset(ac, 0, sizeof(ac));
        //02能保证一定是长度4，即使是\0
        sprintf(ac, "0x%02X", x.data);
        os << ac;
        return os;
    }

    friend istream &operator>>(istream &is, pbyte &x) {
        char ar[4];
        for (size_t i = 0; i < 4; i++) {
            char ch = is.get();
            ar[i] = ch;
        }
        x.data = common_hstrtoch(ar);

        return is;
    }

    bool operator==(pbyte &temp) {
        if (this->data == temp.data)
            return true;
        return false;
    }

    bool operator==(char dw) {
        if (this->data == dw)
            return true;
        return false;
    }

    pbyte &operator=(const char &dw) {
        this->data = dw;
        return *this;
    }
};

//为了序列化传输数据用的
//两种方式,这里使用的是直接继承的方式,也可以内部成员包装,太low没用
//而且有些函数不用重写了比如size等
class plistbyte : public plist<pbyte> {
public:
    plistbyte() {

    }

    plistbyte(char *data, int len) {
        this->setData(data, len);
    }

    plistbyte(plist<pbyte> ls) {
        this->assign(ls.begin(), ls.end());
    }

    //不能挨个赋值,不然特别慢,这类似memcpy
    void setData(char *data, int len) {
        this->assign(data, data + len);
    }

    //获取char*数据,长度为size
    char *getData() {
        return (char *) (&(*(this->begin())));
    }

    friend ostream &operator<<(ostream &os, plistbyte bs) {
        os << (plist<pbyte>) bs;
        return os;
    }

    friend istream &operator>>(istream &is, plistbyte &bs) {
        is >> (plist<pbyte> &) bs;
        return is;
    }
};

class pstring : public string {
public:
    pstring() {
        this->resize(0);
    }

    //构造函数都用这一个,所以只要改这个就行
    void init(const char *arrchData, int len = 0) {
        if (len == 0)
            this->assign(arrchData);
        else
            this->assign(arrchData, len);
    }

    pstring(string str) {
        this->init(str.c_str(), str.size());
    }

    //是否重载了构造函数，就不需要重载赋值运算符，自己就自动了
    pstring(const char *acstr) {
        this->init(acstr);
    }

    pstring(const char *acstr, int len) {
        this->init(acstr, len);
    }

    //找到所有字符串索引
    plist<int> findIndexAll(pstring strfind) {
        plist<int> li;
        int index = 0;
        while (1) {
            int indexLast = index;
//            hlog(index);
            index = this->find(strfind, indexLast + 1);
//            hlog(index);
            if (index == string::npos)
                break;
            li.append(index);
        }
        return li;
    }

    //获取去掉空格的字符串
    pstring getStringNoSpace() {
        plist<pstring> lstr = this->split(" ");
        pstring stres = "";
        for (int i = 0; i < lstr.size(); i++) {
            stres += lstr[i];
        }
        return stres;
    }

    //分割字符串,支持多字符串分割
    // hlog(pstring("2019-03-12 22:11:45").split("- :"));
    plist<pstring> split(string pattern) {
        pstring stro = *this;
        vector<string> vres = common_split(stro, pattern);
        plist<pstring> pres;
        for (size_t i = 0; i < vres.size(); i++) {
            pres.append(vres[i]);
        }
        return pres;
    }

    plist<pstring> splitOne(string pattern) {
        pstring stro = *this;
        vector<string> vres = common_split_one(stro, pattern);
        plist<pstring> pres;
        for (size_t i = 0; i < vres.size(); i++) {
            pres.append(vres[i]);
        }
        return pres;
    }

    //    "12212"这个字符串的所有"12"都替换成"21"，结果是什么21221一次替换
    pstring &replaceOnce(const string &old_value, const string &new_value) {
        for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
            if ((pos = this->find(old_value, pos)) != string::npos) {
                this->replace(pos, old_value.length(), new_value);
            } else {
                break;
            }
        }
        return *this;
    }

    //    "12212"这个字符串的所有"12"都替换成"21"，结果是什么22211迭代递归替换,也就是替换一次
    //之后发现还有12,继续,直到这个字符串没有12全替换成21为止
    pstring &replace_all(const string &old_value, const string &new_value) {
        while (true) {
            string::size_type pos(0);
            if ((pos = this->find(old_value)) != string::npos) {
                this->replace(pos, old_value.length(), new_value);
            } else { break; }
        }
        return *this;;
    }

    bool contain(string str) {
        if (this->find(str) == string::npos)
            return false;
        else
            return true;
    }

    bool isNum();

    //打印和序列化的时候加这个\1，实际上的值还是纯string
    friend ostream &operator<<(ostream &os, pstring str) {
        os << str.data() << '\0';
        return os;
    }

    friend istream &operator>>(istream &is, pstring &pstr) {
        pstr.clear();
        char ch = is.get();
        while (ch != '\0') {
            pstr.push_back(ch);
            ch = is.get();
        }
        return is;
    }

    template<class T>
    pstring &operator<<(const T &data) {
        stringstream ss;
        if (typeid(T) == typeid(pstring)) {
            //<<pstring时因为有增加\0所以要特殊处理
            pstring stres = *(pstring *) (&data);
            //            hlogone(stres.size());
            //            hlogone(stres[stres.size()-1]);
            //            string stres2=stres;
            //            hlog(stres2.size());
            ss << stres.c_str();
            //            hlogone("is self");
        } else
            ss << data;
        this->append(ss.str());
        return *this;
    }

    //由于加了pstring[]这些东西导致原来的东西没了,要转回来用这个
    string toString() {
        return string(this->data());
    }
};

//有问题，后来再改
class pliststring : public plist<pstring> {
public:
    pliststring() {}

    pliststring(plist<pstring> ls) {
        for (size_t i = 0; i < ls.size(); i++) {
            this->append(ls[i]);
        }
    }

    pstring join(pstring strdem) {
        if (this->size() == 0)
            return "";
        pstring stres;
        for (size_t i = 0; i < this->size() - 1; i++) {
            stres += this->at(i) + strdem;
        }
        stres += this->at(this->size() - 1);
        return stres;
    }

    friend ostream &operator<<(ostream &os, pliststring pl) {
        os << (plist<pstring>) pl;
        return os;
    }

    friend istream &operator>>(istream &is, pliststring &pl) {
        is >> (plist<pstring> &) pl;
        return is;
    }

    pliststring &operator<<(const pstring &str) {
        this->append(str);
        return *this;
    }
};

////C语言写法，基础，需要把原来的函数都写一遍
//class pliststring{
//    plist<pstring> liststr;
//public:
//    pliststring(){}
//    void append(pstring str)
//    {
//        liststr.append(str);
//    }

//    bool contains(pstring str)
//    {
//        return liststr.contains(str);
//    }
//    size_t size()
//    {
//        return liststr.size();
//    }

//    pstring join(pstring strdem)
//    {
//        if(liststr.size()==0)
//            return "";
//        pstring stres;
//        for(size_t i=0;i<liststr.size()-1;i++)
//        {
//            stres=liststr[i]+strdem;
//        }
//        stres+=liststr[liststr.size()-1];
//        return stres;
//    }

//    pstring operator[](int i)
//    {
//        return liststr[i];
//    }

//    friend ostream& operator<<(ostream &os,pliststring pl)
//    {
//        os<<pl.liststr;
//        return os;
//    }
//    friend istream& operator>>(istream &is,pliststring &pl)
//    {
//        is>>pl.liststr;
//        return is;
//    }

//};


template<class T1, class T2>
class pmap : public map<T1, T2> {
public:
    pmap() {}


    friend ostream &operator<<(ostream &os, pmap *xv) {
        os << "len: " << xv->size() << " pmap[";
        for (typename map<T1, T2>::iterator iter = xv->begin(); iter != xv->end(); iter++) {
            os << iter->first << ":" << iter->second << " ";
        }
        os << "]";
        return os;
    }


    friend ostream &operator<<(ostream &os, pmap xv) {
        os << "len: " << xv.size() << " pmap[";
        for (typename map<T1, T2>::iterator iter = xv.begin(); iter != xv.end(); iter++) {
            os << iter->first << ":" << iter->second << " ";
        }
        os << "]";
        return os;
    }

    friend istream &operator>>(istream &is, pmap &p) {
        common_skipCountch(is, 5);
        size_t len;
        is >> len;
        //        hlog(len);
        common_skipCountch(is, 6);
        for (size_t i = 0; i < len; i++) {
            T1 t1;
            T2 t2;
            is >> t1;
            common_skipCountch(is, 1);
            is >> t2;
//            p[t1] = t2;
            p.add(t1, t2);
            common_skipCountch(is, 1);//跳过空格
        }
        common_skipCountch(is, 1);
        return is;
    }

    //typename 迭代申请时必须加这个
    plist<T1> getKeys() {
        plist<T1> listkey;
        for (typename map<T1, T2>::iterator iter = this->begin(); iter != this->end(); iter++)
            listkey.push_back(iter->first);
        return listkey;
    }

    //typename 迭代申请时必须加这个
    plist<T2> getValues() {
        plist<T2> listvalue;
        for (typename map<T1, T2>::iterator iter = this->begin(); iter != this->end(); iter++)
            listvalue.push_back(iter->second);
        return listvalue;
    }

    T1 getKey(size_t i) {
        assert(i < this->size());
        return this->getKeys()[i];
    }

    //引用,可以直接用这个改了,除了m[key]=val之外
    T2 &getValue(size_t i) {
        assert(i < this->size());
//        return this->getValues()[i];
        typename map<T1, T2>::iterator it = this->begin();
        for (int index = 0; index < i; index++) {
            it++;
        }
        return it->second;
    }

    //先看看有没有,如果有就覆盖,如果没有就增加
    void add(T1 key, T2 val) {
        if (this->find(key) == this->end()) {
            this->insert(pair<T1, T2>(key, val));
        } else {
            this->at(key) = val;
        }
    }

    //这个改造是使map[key].可提示,并且没有key时不再插入,插入统一用add
    //便于维护列表
    T2 &operator[](T1 key) {
        assert(this->containsKey(key));
        return (T2 &) (this->at(key));
    }


    bool containsKey(T1 t1) {
//        plist<T1> listkey = this->getKeys();
//        return listkey.contains(t1);
        return this->find(t1) != this->end();
    }

    bool containsValue(T2 t2) {
        plist<T2> listval = this->getValues();
        return listval.contains(t2);
    }

    bool deleteByKey(T1 t) {
        typename map<T1, T2>::iterator iter;
        for (iter = this->begin(); iter != this->end(); iter++) {
            //找到t，删除
            if (iter->first == t) {
//                this->erase(iter);
                break;
            }
        }
        if (iter == this->end()) {
            return false;
        } else {
            this->erase(iter);
            return true;
        }
    }
};

//set有自动排序,去重复的功能
template<class T>
class pset : public set<T> {
public:
    pset() {}

    void append(T t) {
        this->insert(t);
    }

    //加上这个才能提示
    T operator[](int i) {
        T t;
        memset(&t, 0, sizeof(t));
        if (i < 0) {
            hlog("索引小于零", i);
            return t;
        } else if (i > this->size() - 1) {
            hlog("索引大于长度减1", this->size(), i);
            return t;
        }

        int count = 0;
        //linux下需要加typename，不然不识别
        for (typename set<T>::iterator it = this->begin(); it != this->end(); it++) {
            if (count == i) {
                t = *it;
                return t;
                //                return (T&)*it;
            }
            count++;
        }
        return t;
    }

    friend ostream &operator<<(ostream &os, pset *s) {

        os << "len: " << s->size() << " pset[";
        //遍历数据，用迭代器遍历数据
        //linux下需要加typename，不然不识别
        for (typename set<T>::iterator it = s->begin(); it != s->end(); it++) {
            T t = *it;
            os << t << " ";//不能直接用*it,不知道为啥,以后就这样吧
        }

        os << "]";
        //这里用到了set中的元素已经从小到大排好序的性质
        return os;
    }


    friend ostream &operator<<(ostream &os, pset s) {

        os << "len: " << s.size() << " pset[";
        //遍历数据，用迭代器遍历数据
        //linux下需要加typename，不然不识别
        for (typename set<T>::iterator it = s.begin(); it != s.end(); it++) {
            T t = *it;
            os << t << " ";//不能直接用*it,不知道为啥,以后就这样吧
        }

        os << "]";
        //这里用到了set中的元素已经从小到大排好序的性质
        return os;
    }

    friend istream &operator>>(istream &is, pset &p) {
        common_skipCountch(is, 5);
        size_t len;
        is >> len;
        hlog(len);
        common_skipCountch(is, 6);
        for (size_t i = 0; i < len; i++) {
            T t;
            is >> t;
            p.insert(t);
            common_skipCountch(is, 1);
        }
        common_skipCountch(is, 1);
        return is;
    }
};

//只获取当前文件夹下的一层，非递归
class pdir {
public:
    cdir *pcdir;
    bool isValid;

    //容量，用量
    long lTotal;
    long lUse;
    double dbUsePercent;

    string strPath;
    plist<string> listNameAll;
    plist<string> listNameFile;
    plist<string> listNameDir;

    plist<string> listPathAll;
    plist<string> listPathFile;
    plist<string> listPathDir;

    //已经有pdir()了
    pdir(const char *path = "./");

    int init(const char *path = "./");

    //复制构造函数，必须加，不然复制就出错，hlog就错
    pdir(const pdir &pdircp);

    friend ostream &operator<<(ostream &os, pdir dir) {
        if (dir.isValid == false) {
            os << "noValid";
            return os;
        }
        os << "dir path: " << dir.strPath << " total: " << dir.lTotal << " use: " << dir.lUse << " per: "
           << dir.dbUsePercent << "\n{\n";
        os << "################files:##################\n";
        for (size_t i = 0; i < dir.listPathFile.size(); i++) {
            os << dir.listPathFile[i] << endl;
        }

        //        os<<dir.listPathFile[dir.listPathFile.size()-1];
        os << "\n#############dirs:#####################\n";
        for (size_t i = 0; i < dir.listPathDir.size(); i++) {
            os << dir.listPathDir[i] << endl;
        }
        //        os<<dir.listPathDir[dir.listPathDir.size()-1];
        os << "\n#########################################";
        os << endl << "}" << endl;
        return os;
    }

    friend ostream &operator<<(ostream &os, pdir *dir) {
        os << "dir path: " << dir->strPath << " total: " << dir->lTotal << " use: " << dir->lUse << " per: "
           << dir->dbUsePercent << "\n{\n";
        os << "################files:##################\n";
        for (size_t i = 0; i < dir->listPathFile.size(); i++) {
            os << dir->listPathFile.at(i) << endl;
        }

        //        os<<dir->listPathFile[dir->listPathFile.size()-1];
        os << "\n#############dirs:#####################\n";
        for (size_t i = 0; i < dir->listPathDir.size(); i++) {
            os << dir->listPathDir.at(i) << endl;
        }
        //        os<<dir->listPathDir[dir->listPathDir.size()-1];
        os << "\n#########################################";
        os << endl << "}" << endl;
        return os;
    }

    ~pdir();
};


typedef struct STRUCT_HEADER {
    int type;
    int length;
    char arrchType[50];//一般不超过50，类型
} STRUCT_HEADER;

/*
 * 需要自己关闭socket，调用close
 * 考虑多线程的问题,一个界面如果有刷新线程,建议另来一个连接
 * 一个连接干一个事情,server端是来一个连接新建一个线程单独处理不影响
 * client端则要自己注意,一个线程干一个事情,用recvx的时候注意一下,因为recvx用到了全局变量data,不要多个线程收就ok,不然会错乱
 */
class ptcp {
    //接收数据缓冲区，不再用char*，提供专门的转换接口
public:
    ctcp *pCtcp;
    string strCtcptype;// client server 初始是init
    //这三个要考虑多线程同时的问题
    pstring data;
    plist<ptcp *> listclient;
    string type;
    int length;
    string ip;
    int port;
    int sock;
    bool bIsConnecting;//判断是否在重连,防止多线程同时重连
    const char *getData();//接收的数据，用于转换
    pstring getDataToString();//专用于字符串

    void setCtcp(ctcp *ct);

    ctcp *getCtcp();

    //内部删除listclient用
    void deleteClientByIndex(int i);


    //这里返回的是一个新的plist<ptcp *>，相当与复制了一份,只能读取，不能操作，不然出错
    //若想操作应该返回plist<ptcp *>*类型
    plist<ptcp *> getAllClients();

    //通过ip查找ptcp
    ptcp *getPtcpByIp(string ip);

    ptcp();

    //接受连接时用,只会是server //该方法不需要init
    ptcp(int iSockAccept, string ip, int port);

    void init(string ip, int port);

    ptcp(string ip, int port);

    //    int update();//处理初始化失败的问题//默认阻塞3秒超时，一般用于 ftp
    int connect();//若是客户端,则调用该方法
    //针对客户端，断线重连,重试次数，次数之间的间隔,默认参数代表无限重连
    //加了是否在重连判断,因此可以在任何情况下连不上就调用该函数,可以保证
    //同一时间只有一个在重连,重连的标准是发送或者接收失败,只有这两种情况
    int reconnect(int iretrycount = -1, int itimesecond = 1);

    //连接失败后线程重连,不需要提示的话就这样,如果需要提示则在ui中封装
    //void MainWindow::slotThreadConnect()
    //{
    //    if(ptc->connect()<0)
    //    {
    //        sigShowStatusBar("连接服务器失败!重连中......");
    //        ptc->reconnect();
    //    }
    //    sigShowStatusBar("连接服务器成功");
    //}
    //或者做个回调函数
    //这个就包括了连接和线程重连,可以每次发送或者接收失败都调用一次connectx
    //同时reconnect中有判断,可以保证始终只有一个线程在重连
    int connectx() {
        if (this->connect() < 0) {
            hlog(pstring() << "连接" << this->ip << ":" << this->port << "失败,重连中...");
            std::thread(&ptcp::threadReconnect, this).detach();
            return -1;
        }
        hlog(pstring() << "连接" << this->ip << ":" << this->port << "成功!");
        return 1;
    }

    void threadReconnect() {
//        hlog(pstring()<<"连接"<<this->ip<<":"<<this->port<<"失败,重连中...");
//如果返回-的说明已经有其他线程在重连,那么这个重连判定失败sleep1秒
        if (this->reconnect() < 0) {
            sleep(1);
        } else {
            hlog(pstring() << "重新连接" << this->ip << ":" << this->port << "成功!");
        }

    }

    int connect2();//通过域名获取外网ip
    //下面这个构造函数不许要init
    ptcp(ctcp *tcpin);//根据xtcp转换
    //当socket连接之后获取封装对象，仅用与客户端
    ptcp(int sock);//根据sock初始化tcp
    ~ptcp();//因为连接的特殊性，一般手动关闭，所以在此不释放，否则导致多次释放的问题，也不能重载<<

    //也就是关闭连接//使用shutdown，多线程使用socket也都会关闭，如果用close不行
    int close();

    /*    void thread_typestring(ptcp* p)
    //    {
    //        if(p->strtype=="test")
    //        {
    //            pstring pstr=p->getClass<pstring>();
    //            hlog(pstr);
    //        }
    //    }

    //    void thread_typeint(ptcp* p)
    //    {
    //        if(p->getTypeInt()==1111)
    //        {
    //            //        hlog("get");
    //            double dbrecv=*(double*)(p->getData());
    //            hlog(dbrecv);
    //        }
    //    }
    //    //测试客户端
    //    void thread_testtcp(void* para)
    //    {
    //        ptcp tcptest(plib::getIPLocal(),4567);
    //        hlog(tcptest.connect());
    //        sleep(1);
    //    //        hlog(tcptest.sendp(1111,22.334));
    //            hlog(tcptest.sendb("test",pstring("xlfdlihailong@126.com")));

    //    //    char data[5];bzero(data,5);strcpy(data,"xlfd");
    //    //    hlog(tcptest.send(data,4));
    //        sleep(1);
    //        tcptest.close();

    //    }
    //    void thread_selfwithWhile(ptcp* p)
    //    {
    //不用pack的时候这么写
    //        while(1)
    //        {
    //            char data[5];
    //            bzero(data,5);
    //            if(p->recv(data,4)>0)
    //                hlog(data);
    //            else
    //                break;
    //        }
    //    }
    //        ptcp tcp(plib::getIPLocal(),4567);
    //        //测试客户端，本机发送
    //        pthread thtest;thtest.start(thread_testtcp);

    //        tcp.setThreadServer(thread_typestring);
    //    tcp.setThreadServer(thread_typeint,1);
    //    tcp.setThreadServer(thread_selfwithWhile,2);
    //用pack的时候这么写
    //    while(true)
    //    {
    //        if(p->recvx()<0)
    //            break;
    //    。。。
    //}
    //默认用这个来写，因为比较灵活，在业务中有时候需要continue，如连不上数据库的时候
    //server类型,是int类型还是string 0标识string，2是nopack,2要自己处理关闭连接等
    //用2时，还能处理发现连接处理一件事情的情况，如果用0则必须收到包才能处理*/
    int setThreadServer(void (*fun_thread)(ptcp *), int itype = 0);

    //放到线程中去
    int setThreadServerDetach(void (*fun_thread)(ptcp *), int itype = 0);

    //专门为类成员函数准备的
    template<class T>
    int setThreadServerClass(void (T::*funtest)(ptcp *para), T *pthis) {
        int iSock = ctcp_InitTcpSocket();
        if (iSock < 0) {
            hlog("iSock<0");
            ctcp_CloseConnection(iSock);
            return -1;
        }
        int ibind = ctcp_BindSocket(iSock, this->ip.c_str(), this->port);
        if (ibind < 0) {
            hlog("bind fail");
            hlog(strerror(errno));
            ctcp_CloseConnection(iSock);
            return -2;
        }
        this->sock = iSock;

        //不停等待，来一个，创建一个线程
        while (true) {
            int iClientSock = -1;
            int iLength;
            struct sockaddr_in struSin;
            iLength = sizeof(struSin);
            memset(&struSin, 0, iLength);
            iClientSock = accept(this->sock, (struct sockaddr *) &struSin, (socklen_t *) &iLength);
            if (iClientSock < 0)
                continue;
            //            hlog(iClientSock);
            string clientip = string(inet_ntoa(struSin.sin_addr));
            //            hlog(clientip);
            ptcp *ptnew = new ptcp(iClientSock, clientip, this->port);

            //            std::function<void (void)> func = std::bind(funtest,pthis,ptnew);
            //            std::thread thserver(func);

            std::thread thserver(funtest, pthis, ptnew);
            //        //如果不调用,线程完了不自动释放资源
            thserver.detach();

        }
    }

    //专门为类成员函数准备的,直接多线程
    //ps->setTcpServerxClassThread(&MainWindow::slotThreadServer2,this);
    //参数ptcp* p,这个没有封装业务函数,只能自己写while循环
    //    void MainWindow::slotThreadServer2(ptcp *p)
    //    {
    //        while(1)
    //        {
    //            if(p->recvx()<0)
    //                break;
    //            if(p->strtype=="test")
    //            {
    //                int irecv=p->getClass<int>();
    //                hlog(irecv);
    //            }
    //        }
    //    }
    template<class T>
    void setThreadServerClassDetach(void (T::*funtest)(ptcp *para), T *pthis) {
        std::thread thServer(&ptcp::setThreadServerClass<T>, this, funtest, pthis);
        thServer.detach();
    }


    //    map<xptcp*,bool> mapTcpThread;//所有客户端ptcp与线程号的对应关系
    //要map的key使用自定义的值，则必须要重载<，const必须这样，不然不认
    bool operator<(const ptcp &pCtcp) const;

    //不收到指定字节就退出，收多少返回多少，适用于转发,若收20，发46，则收三次20，20，6，若收512k发46则收46
    //=如果实际接收包的长度小于0，说明接收时出现了错误，包括对方关闭了连接，判断时就是小于0就失败
    int recvNoFullPack(char *arrecv, int ilenWantRecv, int timeout = -1);

    //基本接收，发送
    int recv(char *acrecv, int len, int timeout = -1) {
        if (this->strCtcptype == "init") {
            hlog(pstring() << "TCP未初始化,请先指定tcp的类型是客户端还是服务端 " << this->ip << ":" << this->port);
            return -999;
        }
        int ires = ctcpRecvFullPacketTimeout(pCtcp->sock, acrecv, len, timeout);
        //        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
//            hlog(this->strCtcptype);
            if (this->strCtcptype == "client") {
                this->connectx();
            }
        }
        return ires;
    }

    int send(const char *acsend, int len) {
        if (this->strCtcptype == "init") {
            hlog(pstring() << "TCP未初始化,请先指定tcp的类型是客户端还是服务端 " << this->ip << ":" << this->port);
            return -999;
        }
        int ires = ctcpSendFullPacketTimeout(pCtcp->sock, acsend, len, 5);
//        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
            if (this->strCtcptype == "client") {
                this->connectx();
            }
        }
        return ires;
    }

    //不能用下边这个，不然就发送send(buf,sizeof(TASK_STATUS_STRUCT)时默认选择这个，就会导致有问题
    //    int send(T t,int timeout=5)
    template<class T>
    int send(T t) {
        if (this->strCtcptype == "init") {
            hlog(pstring() << "TCP未初始化,请先指定tcp的类型是客户端还是服务端 " << this->ip << ":" << this->port);
            return -999;
        }
        int ires = ctcpSendFullPacketTimeout(pCtcp->sock, &t, sizeof(t), 5);
//        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
            if (this->strCtcptype == "client") {
                this->connectx();
            }
        }
        return ires;
    }

    template<class T>
    int recv(T &t) {
        if (this->strCtcptype == "init") {
            hlog(pstring() << "TCP未初始化,请先指定tcp的类型是客户端还是服务端 " << this->ip << ":" << this->port);
            return -999;
        }
        //        return tcp->recv(tcp,&t,sizeof(t));
        int ires = ctcpRecvFullPacketTimeout(pCtcp->sock, &t, sizeof(t), -1);
//        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
            if (this->strCtcptype == "client") {
                this->connectx();
            }
        }
        return ires;
    }


    int sendzstring(string strtype, string content) {
        return sendx(strtype, content.c_str(), content.size());
    }

    /*序列化方案
     * //send用这个,支持发送类，字符串，不支持结构体
     * 结构体直接用sendx(三参数形式)
     */
    template<class T>
    int sendx(string type, T t) {
        /*如果发的是字符串，则直接发送字符串，不转了，如果不是，则要
        //转成字符串发送，发送的字符串是全的，只有反序列化时才会出现不全
        //的情况，因此发送全用string*/
        if (typeid(t) == typeid(string) || typeid(t) == typeid(pstring)) {
            return this->sendzstring(type, *((string *) (&t)));
        }
        return this->sendzstring(type, common_toString(t));
    }

    //只有小于0才是发送失败，等于0是只发了类型
    int sendx(pstring type, const char *pdata = NULL, int length = 0) {
        //这是为了防止这样的调用:ptcp->sendx("res","xfdsfds")
        if (pdata != NULL && length == 0)
            return sendx(type, pdata, (string(pdata).size()));

        STRUCT_HEADER sHeader;
        memset(&sHeader, 0, sizeof(STRUCT_HEADER));
        //        sHeader.type=type;
        strcpy(sHeader.arrchType, type.c_str());
        sHeader.length = length;
        pstring strall;
        strall.resize(sizeof(STRUCT_HEADER) + length);
        char *pstart = (char *) (&(*(strall.begin())));
        memcpy(pstart, &sHeader, sizeof(STRUCT_HEADER));
        //非空的话要把数据弄上去
        if (pdata != NULL) {
            memcpy(pstart + sizeof(STRUCT_HEADER), pdata, length);
        }
        int iResSend = this->send(strall.c_str(), strall.size());
        if (iResSend != strall.size()) {
            hlog("发送数据失败，未全部发送：", iResSend, strall.size());
            return -5;
        }
        if (iResSend == sizeof(STRUCT_HEADER)) {
            //如果只发了类型，则返回数据长度0
            return 0;
        }
        return sHeader.length;
    }

    //>0表示接收成功==0表示只发类型
    int recvx(int timeout = -1) {
        STRUCT_HEADER sHeader;
        memset(&sHeader, 0, sizeof(STRUCT_HEADER));
        int iresHeader = -1;
        if ((iresHeader = this->recv((char *) &sHeader, sizeof(STRUCT_HEADER), timeout)) < 0) {
            //            hlog("接收包头失败");
            //不要打印，不然会一直打印，因为如果接收不到就一直打
            return iresHeader;
        }

        this->type.assign(sHeader.arrchType);
        //如果只发了类型，则返回数据长度0
        if (sHeader.length == 0)
            return 0;
        if (sHeader.length < 0) {
            return -9;
        }
        this->length = sHeader.length;
        //pstring
        this->data.resize(sHeader.length);
        //    hlog(sHeader.length);
        int iRecvDataRes = this->recv((char *) (&(*(this->data.begin()))), sHeader.length, timeout);
        if (iRecvDataRes < 0) {
            //            hlog("接收数据失败");
            return iRecvDataRes;
        }
        if (iRecvDataRes != sHeader.length) {
            //            hlog("接收到的数据长度与包头中的长度不一致，接收失败");
            return -8;
        }
        return iRecvDataRes;
    }


    template<class T>
    T getClass() {
        //        hlog(typeid(T).name());

        //如果本来就是string，就别转了，不然出问题
        //        if(typeid(T).name()==typeid(string).name())
        //        return strdata;


        return common_toClass<T>(this->data);
    }


    //支持hlog，重写cout,前提是不能自动释放
    friend ostream &operator<<(ostream &os, ptcp tcp) {
        os << "[ip: " << tcp.ip << "   port: " << tcp.port << "  sock: " << tcp.sock << "   type: " << tcp.strCtcptype
           << "]";
        if (tcp.strCtcptype == "server") {
            string allclients;
            if (tcp.listclient.size() > 0) {
                allclients = "  当前连接到本tcp服务的客户端如下:\n{\n";
                if (tcp.getAllClients().size() > 0) {
                    for (size_t i = 0; i < tcp.getAllClients().size() - 1; i++) {
                        allclients += tcp.getAllClients()[i]->ip + " ";
                    }
                    allclients += tcp.getAllClients()[tcp.getAllClients().size() - 1]->ip + "\n}";
                } else
                    allclients += "}";
            } else {
                allclients = " 当前无客户端连接到本tcp服务";
            }
            os << allclients;
        }
        return os;
    }

    //可打印ptcp*的，加一个就可以了！
    //支持hlog，重写cout,前提是不能自动释放
    friend ostream &operator<<(ostream &os, ptcp *tcp) {
        os << "[ip: " << tcp->ip << "   port: " << tcp->port << "  sock: " << tcp->sock << "   type: "
           << tcp->strCtcptype << "]";
        if (tcp->strCtcptype == "server") {
            string allclients;
            if (tcp->listclient.size() > 0) {
                allclients = "  当前连接到本tcp服务的客户端如下:\n{\n";
                if (tcp->getAllClients().size() > 0) {
                    for (size_t i = 0; i < tcp->getAllClients().size() - 1; i++) {
                        allclients += tcp->getAllClients()[i]->ip + " ";
                    }
                    allclients += tcp->getAllClients()[tcp->getAllClients().size() - 1]->ip + "\n}";
                } else
                    allclients += "}";
            } else {
                allclients = " 当前无客户端连接到本tcp服务";
            }
            os << allclients;
        }
        return os;
    }

private:

};

//网上的，如果哪天xpmutex不管用了试试这个
class pmutex2 {
public:
    pmutex2() {
        mutex = PTHREAD_MUTEX_INITIALIZER;
    }

    ~pmutex2() {}

    void lock() {
        pthread_mutex_lock(&mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex);
    }

private:
    pthread_mutex_t mutex;
};

//根据C分装的线程锁
class pmutex {
    cmutex *mut;
public:
    pmutex();

    ~pmutex();

    int lock();

    int unlock();



    ////从此处开始是测试线程锁代码
    //int i=9;
    //pmutex mut;
    //void thread_mutex(void* para)
    //{
    //    while(1)
    //    {
    //        mut.lock();
    //        if (i != 0)
    //        {
    //            i = i -1;
    //            hlog(i);
    //        }
    //        else {
    //            hlog("no more tickets");
    //            exit(0);
    //            mut.unlock();
    //        }
    //        mut.unlock();
    //    }
    //}
    //void testMutext()
    //{
    //    pthread pth;
    //    pth.start(thread_mutex,NULL);
    //    pthread pth2;
    //    pth2.start(thread_mutex,NULL);
    //    pthread pth3;
    //    pth3.start(thread_mutex,NULL);
    //    pthread pth4;
    //    pth4.start(thread_mutex,NULL);
    //    pthread pth5;
    //    pth5.start(thread_mutex,NULL);

    //    sleep(1);
    //}
};


extern pmutex mut;


typedef struct xparacpp {
    bool isFinish;
    void *para;

    void (*fun_thread)(void *para);
} xparacpp;

//void* fun_thread_xpackcppjoin(void* para);
//void* fun_thread_xpackcpp(void* para);
//根据C封装的线程函数,已经测试过内存泄漏
class pthread {
    xparacpp xp;

public:
    pthread_t tid;

    bool isFinish() {
        return xp.isFinish;
    }

    pthread() {
        xp.isFinish = false;
    }

    //在while循环中需要设置取消点
    //pthread_testcancel();//主动设置取消点
    //返回0表示成功
    int kill() {
        return pthread_cancel(this->tid);
    }

    //不等带完成
    int start(void (*fun_thread)(void *), void *para = NULL);

    //等带完成
    int startjoin(void (*fun_thread)(void *), void *para = NULL);

    bool operator<(const pthread &th) const {
        return this->tid < th.tid;
    }

    //测试
    //    //如果是函数内部定义的变量，进入线程后会释放，就没了
    //    int igobal=232;
    //    void fun_pthread(void* para)
    //    {
    //        int *ir=(int*)para;
    //        for(size_t i=0;i<3;i++)
    //        {
    //            hlog(*ir);
    //            sleep(1);
    //        }
    //    }
    //    void fun_checktest(void*  para)
    //    {
    //        pthread* pt=(pthread*) para;
    //        while(1)
    //        {
    //            bool finish=pt->isFinish();
    //            hlog(finish);
    //            if(finish)
    //                break;
    //        }
    //        hlog("finish");
    //    }
    //    void testpthread()
    //    {
    //        pthread th;
    //        th.start(fun_pthread,&igobal);
    //        //检测是否线程完成
    //        pthread thtest;
    //        thtest.start(fun_checktest,&th);
    //        sleep(5);
    //    }

    //    void testpthreadjoin()
    //    {
    //        pthread th;
    //        hlog(th.isFinish());
    //        th.startjoin(fun_pthread,&igobal);
    //        hlog(th.isFinish());
    //    }
};
//测试线程条件等待
//int xcount = 0;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
////该函数增加count数值
//void * creator(void * arg)
//{
//    cout << "creator add lock" << endl;
//    pthread_mutex_lock(&mutex);
//    xcount ++;
//    cout << "in creator count is : " << xcount << endl;
//    //条件满足时发送信号
//    if(xcount > 0)
//    {
//        pthread_cond_signal(&cond);
//    }
//    cout << "creator release lock" << endl;
//    pthread_mutex_unlock(&mutex);
//    return NULL;
//}
////该函数减少count数值
//void * consumer(void * arg)
//{
//    cout << "consumer add lock" << endl;
//    pthread_mutex_lock(&mutex);
//    //当条件不满足时等待
//    if(xcount <= 0)
//    {
//        cout << "begin wait" << endl;
//        pthread_cond_wait(&cond,&mutex);
//        cout << "end wait" << endl;
//    }
//    xcount --;
//    cout << "in consumer count is " << xcount << endl;
//    pthread_mutex_unlock(&mutex);
//    cout << "consumer release lock" << endl;
//    return NULL;
//}


//int main()
//{
//    //两个线程，一个生产者线程一个消费者线程
//    pthread_t createthread,consumethread;
//    pthread_create(&consumethread, NULL, consumer, NULL);
//    sleep(2);
//    pthread_create(&createthread, NULL, creator, NULL);
//    //主进程等待两个线程结束
//    pthread_join(createthread, NULL);
//    pthread_join(consumethread, NULL);
//    return 0;
//}









//根据C封装的线程函数
class pthreadpackc {
    cthread *pth;
public:
    pthreadpackc();

    //不等带完成
    int start(void (*fun_thread)(void *), void *para = NULL);

    //等待完成
    int startjoin(void (*fun_thread)(void *), void *para = NULL);

    bool operator<(const pthreadpackc &th) const;
};


//template <class T>
//class classPara
//{
//public:
//    classPara();
//    T t;
//    void (*fun_thread)(T t);
//};

//template <class T>
//void *fun_thread_packcpp(void* para)
//{
//    pthread_detach(pthread_self());
//    classPara<T> * xp=(classPara<T> *)para;
//    xp->fun_thread(xp->t);
//    return NULL;
//}

//template <class T>
//class pthreadcpp
//{
//    classPara<T> para;
//    pthread_t tid;
//public:
//    pthreadcpp(void (*fun_thread)(T t),T t)
//    {
//        this->para.fun_thread=fun_thread;
//        this->para.t=t;
//        pthread_create(&tid,NULL,fun_thread_packcpp,(void*)(&(this->para)));
//    }
//};


//读取，保存，追加等功能
class pfile {
    cfile *pf;
public:
    //获取文件所有内容
    char *dataAll;
    //文件长度
    ulonglong sizeFile;
    string strtype;
    string strpath;

    pfile(string strFullPath, typexfile type);

    ~pfile();

    //正常fread是读到0就是末尾了，这个封装成<0就是读不出来了
    int load(char *data, int len);

    //<0代表错误
    int save(const char *data, int len);

    // 读取到pstirng中,如果size==0表示读不到了
    //直接操作pstring的指针
    pstring load(int len) {
        pstring strdata;
        strdata.resize(len);
        int iread = this->load((char *) (&(*(strdata.begin()))), len);
        if (iread < 0) {
            return pstring();
        }
        strdata.resize(iread);
        return strdata;
    }

    int loadAll();

    bool isOpen() {
        if (pf == NULL)
            return false;
        return true;
    }

    int move(unsigned long long pos);

    unsigned long long size();

    template<class T>
    int load(T &t) {
        return load((char *) &t, sizeof(t));
    }

    template<class T>
    int save(T t) {
        if (typeid(t) == typeid(string) || typeid(t) == typeid(pstring)) {
            string str = *((string *) (&t));
            return this->save(str.c_str(), str.size());
        }
        return save((char *) &t, sizeof(t));
    }

    friend ostream &operator<<(ostream &os, pfile f) {
        os << "path: " << f.strpath << "  type: " << f.strtype;
        return os;
    }

    friend ostream &operator<<(ostream &os, pfile *pf) {
        os << "path: " << pf->strpath << "  type: " << pf->strtype;
        return os;
    }

    int close();
};

class pspan {
    double dbspan;
public:
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    int msec;

    double getSecs() {
        return dbspan;
    }


    void init() {
        year = 0;
        month = 0;
        day = 0;
        hour = 0;
        min = 0;
        sec = 0;
    }

    pspan() {
        init();
    }

    pspan(double db) {
        init();
        this->dbspan = db;
        process();
    }

    void process() {
        //用360天作为一年,因为要用月的倍数
        this->year = (int) dbspan / 31104000;
        this->month = (int) dbspan % 31104000 / 2592000;
        this->day = (int) dbspan % 2592000 / 86400;
        this->hour = (int) dbspan % 86400 / 3600;
        this->min = (int) dbspan % 3600 / 60;
        this->sec = (int) dbspan % 60;
        this->msec = (int) (1000 * (dbspan - (int) dbspan));
        //        hlog(year,month,day,hour,min,sec,msec);
    }

    friend ostream &operator<<(ostream &os, pspan p) {
        if (p.year > 0)
            os << setw(4) << p.year << "年";
        if (p.month > 0)
            os << setw(2) << p.month << "月";
        if (p.day > 0)
            os << setw(2) << p.day << "天";
        if (p.hour > 0)
            os << setw(2) << p.hour << "小时";
        if (p.min > 0)
            os << setw(2) << p.min << "分钟";
        if (p.sec > 0)
            os << setw(2) << p.sec << "秒";
        if (p.msec > 0)
            os << setw(3) << p.msec << "毫秒";
        return os;
    }
};

//相减精确到ms,单位是秒
class ptime {
    xctime time;

    void init();

    ptime(xctime time);

public:
    //获取当前时间
    ptime();

    //重新获取当前时间
    void setNowTime();

    ptime(int year, int mon, int day, int hour, int min, int sec, int ms);

    //支持2020-12-02 12:12:12.232 和 2020-12-02 12:12:12 两种长度的,23 19 后续支持不带分隔符的
    ptime(string strTime);

    //支持2020-12-02 12:12:12.232 和 2020-12-02 12:12:12 两种长度的,23 19 后续支持不带分隔符的
    ptime(const char *acTime);

    //20200925213541562
    static ptime getTimeFromStringNoSplit(pstring str) {
        //先判断长度
        if (str.size() != 17) {
            hlog("长度不对");
            return ptime();
        }
        pstring year = str.substr(0, 4);
        pstring mon = str.substr(4, 2);
        pstring day = str.substr(6, 2);
        pstring hour = str.substr(8, 2);
        pstring min = str.substr(10, 2);
        pstring sec = str.substr(12, 2);
        pstring ms = str.substr(14, 3);
        return ptime(stoi(year), stoi(mon), stoi(day), stoi(hour), stoi(min),
                     stoi(sec), stoi(ms));
    }

    //20200916111111
    static pstring getTimeFullNowNoSplitNoMs();

    //20200916111111999
    static pstring getTimeFullNowNoSplit();

    //2020-09-16 00:44:27
    static pstring getTimeFullNoMsNow();

    //00:44:27
    static pstring getTimeNoMsNow();

    //00:44:27.854
    static pstring getTimeNow();

    //2020-09-16 00:44:27.854
    static pstring getTimeFullNow();

    //2020-09-16
    static pstring getDateNow();

    static double getDiff(clock_t t1, clock_t t2);

    static ptime getTimeFromSeconds(int64_t secs);

    //此函数会返回从公元 1970 年1 月1 日的UTC 时间从0 时0 分0 秒算起到现在所经过的秒数
    static int64_t getSecondsFrom1970();

    //2000-01-01 00:00:00.000
    static int64_t getSecondsFromString(string strTime);

    //#############################军队相关 自己写的，先不用，用plib里的积日积秒##########################
    //这是积秒，军队项目要求从2000开始，用4字节存储--当我没说，积秒不是这样的
    static int getSecondsFrom2000();

    //获取2000开始的日期时间，要加上1970到2000的差才行
    static ptime getTimeFromSeconds2000(int secs);

    //#######################################没用
    //这是真的积秒,本包数据发送北京时间相对本日0时的积秒，单位是0.1ms
    static unsigned int getJS();

    //从积秒获取时间HH:MM:SS
    static string getTimeFromJS(int js);

    //这是积日，军队项目要求从2000开始，用2字节存储
    static unsigned short getJD();

    //这一堆是获取积日
    //IsLeap函数判断一个年份是否为闰年，方法如下:
    static bool isLeap(int year);

    //上面的StringToDate函数用于取出日期中的年月日并判断日期是否合法
    //从字符中最得年月日 规定日期的格式是yyyy-mm-dd
    static bool StringToDate(string date, int &year, int &month, int &day);

    //DayInYear能根据给定的日期，求出它在该年的第几天，代码如下
    static int getDayInYear(int year, int month, int day);

    //获取两个日期之间的天数
    static int getDaysBetween2Date(string date1, string date2);

    unsigned short year();

    unsigned char mon();

    unsigned char day();

    unsigned char hour();

    unsigned char min();

    unsigned char sec();

    unsigned short msec();

    string toString() {
        char acTime[20];
        clib_getStringFromXtime(this->time, acTime);
        return string(acTime);
    }

    string toStringYYYYMMDD() {
        pstring strall = this->toString();
        return strall.substr(0, 10);
    }

    //支持hlog，重写cout,前提是不能自动释放
    friend ostream &operator<<(ostream &os, ptime time) {
        os << time.toString();
        return os;
    }

    friend istream &operator>>(istream &is, ptime &data) {
        string str;
        for (size_t i = 0; i < 23; i++)
            str.push_back(is.get());
        xctime ct = clib_getTimeFromString(str.c_str());
        data.time = ct;
        return is;
    }

    double operator-(const ptime &time2) {
        return clib_getDiffBetweenXtime(time2.time, this->time);
    }

    ptime operator+(const int64_t secs) {
        return ptime(clib_getTimeAdd(this->time, secs));
    }

    //相减精确到ms 单位是秒 tmnow-tmLastB : 6.00748
    ptime operator-(const int64_t secs) {
        return ptime(clib_getTimeSub(this->time, secs));
    }

    //比较
    bool operator<(const ptime &tm) const {
        if (clib_getDiffBetweenXtime(tm.time, this->time) < 0)
            return true;
        else
            return false;
    }

    bool operator==(const ptime &tm) const {
        if (clib_getDiffBetweenXtime(tm.time, this->time) == 0)
            return true;
        else
            return false;
    }
};

class protect {
public:
    ptime tdes;
    ptime tlast;
    bool valid;

    friend ostream &operator<<(ostream &os, protect p) {
        os << p.tdes << " " << p.tlast << " " << p.valid;
        return os;
    }

    friend istream &operator>>(istream &is, protect &p) {
        is >> p.tdes;
        is.get();
        is >> p.tlast;
        is.get();
        is >> p.valid;
        return is;
    }
};

//通用函数
class plib {
public:
    plib();

    //打印8位二进制
    static void showBin(char byData) {
        int n0, n1, n2, n3, n4, n5, n6, n7;
        n0 = (byData & 0x01) == 0x01 ? 1 : 0;
        n1 = (byData & 0x02) == 0x02 ? 1 : 0;
        n2 = (byData & 0x04) == 0x04 ? 1 : 0;
        n3 = (byData & 0x08) == 0x08 ? 1 : 0;
        n4 = (byData & 0x10) == 0x10 ? 1 : 0;
        n5 = (byData & 0x20) == 0x20 ? 1 : 0;
        n6 = (byData & 0x40) == 0x40 ? 1 : 0;
        n7 = (byData & 0x80) == 0x80 ? 1 : 0;
        HLOG("[bin:%d%d%d%d%d%d%d%d]", n0, n1, n2, n3, n4, n5, n6, n7);
    }

    //杀死除自己之外的进程,适用于更新,调试
    static bool killProcessAllByNameSelfExceptSelf() {
        plist<long> lother = plib::getPidAllByNameSelfExceptSelf();
        //挨个杀死
        for (int i = 0; i < lother.size(); i++) {
            long pidi = lother[i];
            plib::killProcessByPid(pidi);
        }
        plist<long> lres = plib::getPidAllByNameSelfExceptSelf();
        if (lres.size() == 0)
            return true;
        return false;
    }

    static void killProcessByPid(long pid) {
        pstring cmd = "kill -9 " + plib::toString(pid);
//        hlog(cmd);
        plib::getShellOutput(cmd);
    }

    //获取除自己之外的同名进程pid
    static plist<long> getPidAllByNameSelfExceptSelf() {
        plist<long> lall = plib::getPidAllByNameSelf();
        long lpidSelf = plib::getPid();
        int ifind = lall.find(lpidSelf);
//        hlog(ifind);
        if (ifind < 0) {
            hlog("未找到本进程名对应的pid,未知错误");
            return lall;
        }
        (lall.deleteByIndex(ifind));
        return lall;
    }

    //获取自己进程名的所有pid
    static plist<long> getPidAllByNameSelf() {
        return plib::getPidAllByName(plib::getNameProcessSelf());
    }

    //获取自身程序进程名
    static pstring getNameProcessSelf() {
        return getNameProcessByPid(plib::getPid());
    }

    //根据pid获取进程名
    static pstring getNameProcessByPid(pid_t pid) {
        pstring strPathFull = getPathFullProcessbyPid(pid);
        return plib::getFileNameFromFullPath(strPathFull);
    }

    //根据pid获取进程全路径
    static pstring getPathFullProcessbyPid(pid_t pid) {
        char name[65535];
        cinit(name);
        FILE *fptr;
        bool bret = false;
        char cmd[65535] = {'\0'};
        sprintf(cmd, "readlink /proc/%d/exe", pid);
        if ((fptr = popen(cmd, "r")) != NULL) {
            if (fgets(name, 65535, fptr) != NULL) {
//                printf("the path name is %s.\n",name);
                bret = true;
            }
        }
        pclose(fptr);
        if (bret) {
            //最后多一个\n,去掉
            name[strlen(name) - 1] = '\0';
            return name;
        } else
            return "";
    }

    //根据进程名获取pid
    static plist<long> getPidAllByName(pstring strNameProcess) {
        pstring strAllDes = (plib::getShellOutput(
                "ps -ef|grep " + strNameProcess + "|grep -v grep|awk '{print $2\" \"$9}'"));
//        hlog(strAllDes);
        pliststring lsplit = strAllDes.split("\n");
//        hlog(lsplit);
        plist<long> lres;
        for (int i = 0; i < lsplit.size(); i++) {
            lres.append(stol(lsplit[i]));
        }
        return lres;
    }

    //获取当前积秒
    static unsigned int getCurrentJS() {
        return GetCurrentJS();
    }

    //根据积日积秒获取时间
    static int getDateTimeFromJDJS(unsigned short JD, unsigned int JS, DateTime &dt) {
        return GetDateTimeFromJDJS(JD, JS, dt);
    }

    static string getStringTimeFromJDJS(unsigned short JD, unsigned int JS) {
        DateTime dtres;
        if (plib::getDateTimeFromJDJS(JD, JS, dtres) < 0)
            return "";
        char data[50];
        cinit(data);
        sprintf(data, "%d-%02d-%02d %02d:%02d:%02d", dtres.year, dtres.month, dtres.day,
                dtres.hour, dtres.min, dtres.sec);
        //        hlog(data);
        return data;
    }

    static string getStringTimeWithoutDateFromJS(unsigned int JS) {
        pstring strTimeFull = plib::getStringTimeFromJDJS(plib::getCurrentJD(), JS);
        return strTimeFull.split(" ")[1];
    }

    //获取当前积日
    static unsigned short getCurrentJD() {
        return GetCurrentJD();
    }

    //将0x类的16进制字符串转为int,主要是转mid
    static int toHex(const char *ac0xHex) {
        if (ac0xHex == NULL)
            return -1;
        if (strlen(ac0xHex) < 2)
            return -2;
        //先去掉0x
        char acNakeNum[20];
        bzero(acNakeNum, sizeof(acNakeNum));
        memcpy(acNakeNum, ac0xHex + 2, strlen(ac0xHex) - 2);
        //        HLOG_STRING(acNakeNum);
        //之后再改
        int i, t;
        int sum = 0;
        for (i = 0; acNakeNum[i]; i++) {
            if (acNakeNum[i] <= '9')
                t = acNakeNum[i] - '0';
            else
                t = acNakeNum[i] - 'a' + 10;
            sum = sum * 16 + t;
        }
        //        HLOG_INT(sum);
        return sum;
    }

    //0xFF 一个字节
    static string toStringHex1Byte(int hex) {
        char data[20];
        cinit(data);
        //unsigned char 为了防止出现一堆ff
        sprintf(data, "0x%02x", (unsigned char) hex);
        return data;
    }

    //0x0023 一个字符是两字节
    static string toStringHex2Byte(int hex) {
        char data[20];
        cinit(data);
        sprintf(data, "0x%04x", (unsigned short) hex);
        return data;
    }

    //0x00000023 是四字节 两个十六进制是一个字节
    static string toStringHex4Byte(int hex) {
        char data[20];
        cinit(data);
        sprintf(data, "0x%08x", (unsigned int) hex);
        return data;
    }

    static pmap<pstring, pmap<pstring, pstring> > getAllServerCSGL() {
        pmap<pstring, pmap<pstring, pstring> > mall;
        //增加各系统host
        pmap<pstring, pstring> mapgf;
        mapgf["KSC"] = "172.16.141.5";
        mapgf["MYC"] = "172.16.77.6";
        mapgf["SYC"] = "172.16.171.5";
        mapgf["DTC"] = "172.16.11.3";
        mapgf["KR1"] = "130.242.60.241";
        mapgf["YD1"] = "172.16.112.2";
        mapgf["YE2"] = "172.17.2.2";
        mall["高分"] = mapgf;
        pmap<pstring, pstring> mapxd;
        mapxd["KSC"] = "172.16.142.2";
        mapxd["MYC"] = "172.16.78.2";
        mapxd["SYC"] = "172.16.172.1";
        mapxd["DTC"] = "172.16.12.3";

        mall["先导"] = mapxd;
        pmap<pstring, pstring> mapz3;
        mapz3["KSC"] = "172.16.143.1";
        mapz3["MYC"] = "172.16.80.1";
        mapz3["MYC"] = "172.16.80.2";
        mapz3["SYC"] = "172.16.173.1";
        mapz3["DTC"] = "172.16.13.2";
        mall["资三"] = mapz3;
        pmap<pstring, pstring> mapgf4;
        mapgf4["DTC"] = "172.16.2.110";
        mapgf4["MYC"] = "172.16.2.111";
        mapgf4["QBJ"] = "172.16.2.104";
        mall["高分四号"] = mapgf4;
        pmap<pstring, pstring> mapkj;
        mapkj["KSC"] = "172.16.144.2";
        mapkj["MYC"] = "172.16.81.2";
        mapkj["DTC"] = "172.16.14.2";
        mapkj["SYC"] = "172.16.174.2";
        mapkj["YD1"] = "172.16.112.1";
        mapkj["KR1"] = "130.242.60.243";
        mall["空基"] = (mapkj);
        pmap<pstring, pstring> mapfenfa;
        mapfenfa["DTC"] = "172.16.2.93";
        mapfenfa["ZYD"] = "172.16.13.1";
        mapfenfa["VCRE1"] = "172.16.14.9";
        mapfenfa["GF4D"] = "172.16.2.110";//会与高分4dtc重复,不加了
        mapfenfa["GFD"] = "172.16.11.16";
        mapfenfa["DTC1"] = "172.16.2.94";
        mapfenfa["MDJ"] = "172.16.2.102";
        mall["传输管理控制"] = (mapfenfa);
        return mall;
    }

    static pstring toChinese(string str) {
        return common_toChinese(str);
    }

    /*首先，汉字是双字节的编码格式！
      汉字的第一字节：是从0xB0   开始编码
      汉字的第二字节：是从0xA1   开始编码
      有了以上的了解，就不难识别出是中文，还是其他的编码！
       程序中只需要判断（   第一字节>=0xB0   &&   第二字节>=0xA1   ）就好！
       */
    static int isIncludeChinese(string str)//返回0：无中文，返回1：有中文
    {
        return common_isIncludeChinese(str);
    }

    static pstring getSystem() {
        return common_getSystem();
    }

    //    static string toGBK(const char* str)
    //    {
    //        return Common_UTF8toGBK(str);
    //    }

    static string toUTF8(const char *str) {
        return common_string_To_UTF8(str);
    }

    //判断字符串是否为utf8
    static bool isUTF8(const char *str) {
        return common_isStringUTF8(str);
    }

    //这个地方是为了与windows保持统一,因为linux下不用转
    static string toGBK(const char *str) {
        return common_string_To_GBK(str);
    }

    /*将大写字母转换成小写字母*/
    static int tolower(int c) {
        if (c >= 'A' && c <= 'Z') {
            return c + 'a' - 'A';
        } else {
            return c;
        }
    }

    //将十六进制的字符串转换成整数
    static int htoi(char s[]) {
        int i;
        int n = 0;
        if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            i = 2;
        } else {
            i = 0;
        }
        for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'); ++i) {
            if (tolower(s[i]) > '9') {
                n = 16 * n + (10 + tolower(s[i]) - 'a');
            } else {
                n = 16 * n + (tolower(s[i]) - '0');
            }
        }
        return n;
    }

    //将十六进制的字符串转换成char
    static char hstrtoch(char s[]) {
        return common_hstrtoch(s);
    }

    static void sleep(int ms) {
        usleep(ms * 1000);
    }



    //如果没斜杠,加上,有就不动
    static pstring getPathWithXiegang(pstring path) {
        if (path[path.size() - 1] != '/')
            path = path + "/";
        return path;
    }

    static pstring getConfStringRemoteHost(pstring host, pstring path, pstring key, pstring pwd = "scfwq_325") {
        pstring cmd = "cat " + path + "|grep " + key + "=";
        //        hlog(cmd);
        pstring stres = xsh(host, cmd, pwd);

        if (stres.contain("\n")) {
            hlog("警告：找到多个key值,需要去掉重复", stres);
            stres = stres.split("\n")[0];
            hlog(stres);
            return stres;
        }
        //找不到的话是空
        //        hlog(stres);
        if (stres != "") {
            pliststring listres = stres.split("=");
            stres = listres[listres.size() - 1];
        }

        return stres;
    }
    static pstring getConfStringRemoteHostBridge(pstring host, pstring path, pstring key, pstring pwd = "scfwq_325",
                                                 pstring hostbridge = "172.16.11.6", pstring pwdbridge = "scfwq_325") {
        pstring cmd = "cat " + path + "|grep " + key + "=";
        //        hlog(cmd);
        pstring stres = xshbridge(host, cmd, pwd, hostbridge, pwdbridge);

        if (stres.contain("\n")) {
            hlog("警告：找到多个key值,需要去掉重复", stres);
            stres = stres.split("\n")[0];
            hlog(stres);
            return stres;
        }
        //找不到的话是空
        //        hlog(stres);
        if (stres != "") {
            pliststring listres = stres.split("=");
            stres = listres[listres.size() - 1];
        }

        return stres;
    }
    //删除所有空行
    static pstring delConfSpaceLineRemoteHost(pstring host, pstring path, pstring pwd = "scfwq_325") {


        //        pstring cmdGetCountSpaceLine="grep \"^$\" "+path+"| wc -l";
        //        pstring cmd= "sed -i '/^\s*$/d' "+path+";"+cmdGetCountSpaceLine;
        pstring cmd = "sed -i '/^\s*$/d' " + path;
        cmd = "path=" + path +
              ";if [ ! -f $path ]; then echo \"找不到配置文件$path,删除失败\";" +
              "else " + cmd + ";echo \"删除空行成功\";fi";

        pstring stres = plib::xsh(host, cmd, pwd);
        hlog(stres);
        return stres;

    }

    //删除文件中包含某个关键字开头的所有行
    static pstring
    delConfStringContainKeyRemoteHost(pstring host, pstring path, pstring key, pstring pwd = "scfwq_325") {
        pstring cmd = "source=" + key + ";path=" + path +
                      ";lcount=`sed -n \"/$source=/=\" $path`;if [ ! -f $path ]; then echo \"找不到配置文件$path,删除失败\";" +
                      "else if [ \"$lcount\" == \"\" ]; then echo \"配置文件中没有相同配置项$source,删除失败\";" +
                      "else " + "sed -i '/^" + key + "=/d' " + path + ";echo \"删除所有配置项$source成功\";fi;fi";


        //        pstring cmd="sed -i '/^"+key+"=/d' "+path;
        //        plib::getShellOutputXsh(host,pwd,cmd);
        pstring stres = plib::xsh(host, cmd, pwd);
        hlog(stres);
        return stres;
    }

    //远程增加配置项
    static pstring
    addConfStringRemoteHost(pstring host, pstring path, pstring key, pstring value, pstring pwd = "scfwq_325") {
        pstring cmd = "source=" + key + ";target=" + value + ";path=" + path +
                      ";lcount=`sed -n \"/$source=/=\" $path`;if [ ! -f $path ]; then echo \"找不到配置文件$path,添加失败\";" +
                      "else if [ \"$lcount\" != \"\" ]; then echo \"配置文件中已经有相同配置项$source,添加失败\";" +
                      "else echo \"$source=$target\" >> $path;echo \"配置项$source=$target添加成功\";fi;fi";
        hlog(cmd);
        pstring stres = plib::xsh(host, cmd, pwd);
        hlog(stres);
        return stres;
    }

    //远程修改配置项的值，如果文件不存在或者key找不到则失败,先备份
    static pstring
    setConfStringRemoteHost(pstring host, pstring path, pstring key, pstring value, pstring pwd = "scfwq_325") {
        pstring cmdbk = "cp -f " + path + " " + path + ".bk" + plib::getTimeNow().substr(0, 10);
        hlog(cmdbk);
        hlog(plib::xsh(host, cmdbk, pwd));
        //全都用；分割远程命令，不能用换行
        //首先查找key所在的行数，如果照不到，则不替换，
        //如果找到多个，则不替换，报错
        //如果找到一个，则替换
        pstring cmd = "source=" + key + ";target=" + value + ";path=" + path +
                      ";lcount=`sed -n \"/^$source=/=\" $path`;flcount=`echo \"$lcount\"|wc -l`;if [ \"$lcount\" = \"\" ]; then echo \"修改配置项失败，未找到配置项\"$source;" +
                      "else if [ $flcount -gt 1 ];then echo \"找到多个配置项$source，请检查配置文件，替换失败\";else sed -i \"${lcount}c $source=$target\" $path;" +
                      "echo \"替换成功，配置项\"`cat $path|grep $source=`;fi;fi";
        hlog(cmd);
        pstring stres = plib::xsh(host, cmd, pwd);
        hlog(stres);
        return stres;
    }

    //桥接修改配置文件,直接shell会因为双引号单引号嵌套的问题无法使用，所以必须先写个程序
    //可以本地修改的，目前用的是shell，以后要改成c++的setconf程序更完善
    static pstring
    setConfStringRemoteHostBridge(pstring host, pstring pathFull, pstring key, pstring value, pstring pwd = "scfwq_325",
                                  pstring hostbridge = "172.16.11.6", pstring pwdbridge = "scfwq_325") {
        //先把xsetconf考过去,
        plib::xcpUploadBridge(host, "xsetconf", "/usr/bin");
        hlog("已将设置脚本拷贝过去");

        pstring cmdupdate = "xsetconf " + pathFull + " " + key + " " + value;
        hlog(cmdupdate);

        return plib::xshbridge(host, cmdupdate);

    }


    //打开终端并登录到新服务器上去
    static void xshNewTerminal(pstring strip, pstring pwd) {
        pstring strcmd = "gnome-terminal -x xsh " + pwd + " " + strip;
        hlog(strcmd);
        plib::getShellOutput(strcmd);
    }

    static string xsh(string strip, string cmd, string strpwd = "scfwq_325") {
        string info = "xsh " + strpwd + " " + strip + " '" + cmd + "'";
//        hlog(info);
        return getShellOutput(info);
    }

    //通过中转服务器hlog(plib::getShellOutputXshBridge("130.242.60.241","172.16.11.6","scfwq_325","df -h"));
    static string
    xshbridge(string stripDest, string cmd, string strpwd = "scfwq_325", string stripBridge = "172.16.11.6",
              string strpwdBridge = "scfwq_325") {
        //都用‘’有问题再改回来
        string info =
                "xsh " + strpwdBridge + " " + stripBridge + " '" + "xsh " + strpwd + " " + stripDest + " \"" + cmd +
                "\"'";
        //        string info="xsh "+strpwd+" "+stripBridge+" '"+"xsh "+strpwd+" "+stripDest+" \'"+cmd+"\''";
        hlog(info);
        return getShellOutput(info);
    }

    static bool xupdate(pstring ipRemote, pstring pathFullProgram, pstring pathRemote) {
        //先杀掉远程的,再考过去,再启动远程的

        //先获取文件名,与远程路径组成远程全路径再拷贝
        pstring strNameProgramOnly = plib::getFileNameFromFullPath(pathFullProgram);
        plib::xsh(ipRemote, "killall -9 " + strNameProgramOnly);
        hlog(strNameProgramOnly, pathFullProgram);
        //把输入的路径加斜杠
        pstring strPathFullRemote = plib::getPathWithXiegang(pathRemote) + strNameProgramOnly;
        hlog(strPathFullRemote);
        plib::xcpUpload(ipRemote, pathFullProgram, pathRemote);
        //启动
        plib::xsh(ipRemote, strPathFullRemote);
        return true;
    }

    //支持文件和文件夹
    static string xcpDownload(string ipRemote, string filelocal, string pathRemote, string strpwd = "scfwq_325") {
        string info = "xcp " + strpwd + " \"" + ipRemote + ":" + pathRemote + " " + filelocal + "\"";
        hlog(info);
        return getShellOutput(info);
    }

    //支持文件和文件夹
    static string xcpUpload(string ipRemote, string filelocal, string pathRemote, string strpwd = "scfwq_325") {
        string info = "xcp " + strpwd + " \"" + filelocal + " " + ipRemote + ":" + pathRemote + "\"";
        hlog(info);
        return getShellOutput(info);
    }

    //支持文件和文件夹
    static string
    xcpUploadBridge(string ipDest, string filelocalFullPath, string pathRemote, string strpwd = "scfwq_325",
                    string ipBridge = "172.16.11.6", string pwdbridge = "scfwq_325") {
        // /root/Desktop/xsh
        //        hlog(filelocalFullPath);
        //考到11.6根目录上去
        plib::xcpUpload(ipBridge, filelocalFullPath, "/", pwdbridge);
        //获取文件名，不带路径的，这个时候如果有多个文件，则需要先根据空格分开再处理
        pstring stresFull;
        pliststring listFullName = pstring(filelocalFullPath).split(" ");
        for (size_t i = 0; i < listFullName.size(); i++) {
            string filelocalNameOnly = plib::getFileNameFromFullPath(listFullName[i]);
            //        hlog(filelocalNameOnly);
            //获取11.6上的文件全路径，也就是/+文件名
            string pathFullBridge = "/" + filelocalNameOnly;
            hlog(pathFullBridge);
            stresFull += pathFullBridge + " ";
        }
        hlog(stresFull);
        //最后在11.6上执行xcp
        string cmdBridge = "xcp " + strpwd + " \"" + stresFull + " " + ipDest + ":" + pathRemote + "\"";
        hlog(cmdBridge);

        return plib::xsh(ipBridge, cmdBridge);
    }

    //关闭集群的总控服务,strhost指从哪关,一般集群第一台就可以
    static void closeSCServiceJQ(plist<pstring> listhost, int index = 0) {
        //        hlog(listhost[0]);
        //        string strcmd="xsh scfwq_325 "+listhost[index]+" '/usr/sbin/clusvcadm -s SCService'";
        //        hlog(strcmd);
        //        plib::getShellOutput(strcmd);
        plib::xsh(listhost[index], "/usr/sbin/clusvcadm -s SCService");
    }

    //关闭集群的总控服务,strhost指从哪关,一般集群第三台就可以
    static void startSCServiceJQ(plist<pstring> listhost, int index = 0) {
        //        hlog(listhost[0]);
        //        string strcmd="xsh scfwq_325 "+listhost[index]+" '/usr/sbin/clusvcadm -e SCService'";
        //        hlog(strcmd);
        //        plib::getShellOutput(strcmd);

        plib::xsh(listhost[index], "/usr/sbin/clusvcadm -e SCService");
    }

    //更新集群
    static void updateSCServiceJQ(plist<pstring> lstr) {
        //先关闭，再拷贝，再打开
        plib::closeSCServiceJQ(lstr, 0);
        for (size_t i = 0; i < lstr.size(); i++) {
            plib::xcpUpload(lstr[i], "../SCService", "/opt/sc/csgl/SCService");
        }
        plib::startSCServiceJQ(lstr, 0);
    }

    //先备份队列和总控再更新
    static void updateOneSingle(pstring host) {
        ptime tnow;
        //根据配置里面的队列路径，备份队列,路径在队列路径
        pstring strData = plib::getConfStringRemoteHost(host, "/opt/sc/csgl/SCService/sc.conf", "SHARE_STORE_PATH");
        hlog(strData);
        pstring strDataPathFull = strData + "task.dat";
        hlog(strDataPathFull);
        pstring strDataForbk = strDataPathFull + tnow.toStringYYYYMMDD();
        hlog(strDataForbk);
        pstring cmdbkdat = "cp " + strDataPathFull + " " + strDataForbk;
        hlog(cmdbkdat);

        //备份总控
        pstring cmdbkzk =
                "cp /opt/sc/csgl/SCService/SCService /opt/sc/csgl/SCService/SCService" + tnow.toStringYYYYMMDD();
        hlog(cmdbkzk);
        pstring cmdall = "xk;" + cmdbkdat + ";" + cmdbkzk;
        hlog(cmdall);

        //先杀死再备份程序和队列，最后更新程序
        plib::xsh(host, cmdall);
        //        plib::xsh(host,"scfwq_325","xk");
        plib::xcpUpload(host, "../SCService", "/opt/sc/csgl/SCService");
        plib::xsh(host, "xs");
    }

    //先备份队列和总控再更新
    static void updateOneSingleBridge(pstring host) {
        ptime tnow;
        //根据配置里面的队列路径，备份队列,路径在队列路径
        pstring strData = plib::getConfStringRemoteHostBridge(host, "/opt/sc/csgl/SCService/sc.conf",
                                                              "SHARE_STORE_PATH");
        hlog(strData);
        pstring strDataPathFull = strData + "task.dat";
        hlog(strDataPathFull);
        pstring strDataForbk = strDataPathFull + tnow.toStringYYYYMMDD();
        hlog(strDataForbk);
        pstring cmdbkdat = "cp " + strDataPathFull + " " + strDataForbk;
        hlog(cmdbkdat);

        //备份总控
        pstring cmdbkzk =
                "cp /opt/sc/csgl/SCService/SCService /opt/sc/csgl/SCService/SCService" + tnow.toStringYYYYMMDD();
        hlog(cmdbkzk);
        pstring cmdall = "xk;" + cmdbkdat + ";" + cmdbkzk;
        hlog(cmdall);


        plib::xshbridge(host, cmdall);
        plib::xcpUploadBridge(host, "../SCService", "/opt/sc/csgl/SCService");
        plib::xshbridge(host, "xs");
    }

    //远程修改配置文件
    static void updateConfSCRemote(pstring host, pstring path, pstring key, pstring value, pstring pwd = "scfwq_325") {
        //先备份再修改
        plib::setConfStringRemoteHost(host, path, key, value, pwd);
        //重启总控
        pstring cmdrestart = "xk;xs";
        plib::xsh(host, cmdrestart, pwd);
    }

    //远程修改配置文件
    static string
    updateConfSCRemoteBridge(pstring host, pstring path, pstring key, pstring value, pstring pwd = "scfwq_325",
                             pstring hostbridge = "172.16.11.6", pstring pwdbridge = "scfwq_325") {
        //先备份再修改
        plib::setConfStringRemoteHostBridge(host, path, key, value, pwd, hostbridge, pwdbridge);
        //重启总控
        pstring cmdrestart = "xk;xs";
        return plib::xshbridge(host, cmdrestart, pwd, hostbridge, pwdbridge);
    }


    //获取外网ip
    static string getIPWaiwang() {
        return plib::getShellOutput("curl ifconfig.me");
    }

    //获取远程根目录使用大小
    static string getRemoteRootDiskUsage(string strhost, string pwd = "scfwq_325") {
        //        string cmd="xsh scfwq_325 "+strhost+" \"df -h |grep -w '/'\"";
        //                hlog(cmd);
        //        pstring pers=(plib::getShellOutput(cmd));

        pstring pers = plib::xsh(strhost, "df -h |grep -w '/'", pwd);
        //        hlog(pers);
        plist<pstring> listp = pers.split(" ");
        //        hlog(listp);
        string strfind;
        for (size_t i = 0; i < listp.size(); i++) {
            if (listp[i].find('%') != string::npos) {
                strfind = listp[i];
                break;
            }
        }
        return strfind;
//        string stres = strhost + " 根目录占用百分比： " + strfind;
//        return stres;
    }

    //获取远程根目录使用大小
    static string
    getUsageRemoteRootPathBridge(string strhost, string pwd = "scfwq_325", string strbridge = "172.16.11.6",
                                 string pwdbridge = "scfwq_325") {
        //        string cmd="xsh scfwq_325 "+strhost+" \"df -h |grep -w '/'\"";
        //        //        hlog(cmd);
        //        pstring pers=(plib::getShellOutput(cmd));

        pstring pers = xshbridge(strhost, "df -h |grep -w '/'", pwd, strbridge, pwdbridge);



        //        hlog(pers);
        plist<pstring> listp = pers.split(" ");
        //        hlog(listp);
        string strfind;
        for (size_t i = 0; i < listp.size(); i++) {
            if (listp[i].find('%') != string::npos) {
                strfind = listp[i];
                break;
            }
        }
        string stres = strhost + " 根目录占用百分比： " + strfind;
        return stres;
    }

    //获取进程id
    static long getPid() {
        return getpid();
    }

    //获取线程id
    static long getTid() {
        return syscall(__NR_gettid);
    }

    static bool deleteFile(const string path) {
        int ires = remove(path.c_str());
        //        hlog(ires);
        if (ires == 0)
            return 1;
        return 0;
    }


    //    static int  TraverseDir_Num(const char* strVideoDir)//返回该文件夹的文件个数 //会导致编译文件夹一场，查找原因
    //    {
    //        //遍历目录
    //        static  int num = 0;
    //        DIR*		dp;
    //        struct dirent *entry;
    //        struct stat statbuf;
    //        dp = opendir(strVideoDir);
    //        if(!dp)
    //        {
    //            //            LLERROR("无法打开目录:%s", strVideoDir);
    //            hlog("open fail");
    //            return -1;
    //        }
    //        chdir(strVideoDir);
    //        while((entry = readdir(dp)) != NULL)
    //        {
    //            lstat(entry->d_name, &statbuf);
    //            if(S_ISDIR(statbuf.st_mode))
    //            {
    //                if(!strcmp(".",entry->d_name) || !strcmp("..",entry->d_name))
    //                {
    //                    continue;
    //                }
    //                char		strNewDir[256];
    //                sprintf(strNewDir, "%s/%s", strVideoDir, entry->d_name);
    //                TraverseDir_Num(strNewDir);
    //            }
    //            else
    //            {
    //                num += 1;
    //            }

    //        };

    //        chdir("..");
    //        closedir(dp);
    //        return num;
    //    }

    static string getContentsAllFromFile(const string path) {
        ifstream is(path.c_str());
        if (!is) {
            hlog("open file fail");
            return "";
        }
        std::stringstream buffer;
        buffer << is.rdbuf();
        string contents(buffer.str());
        //        hlog(contents);
        is.close();
        return contents;
        //       pfile fload(path, typeload);
        //       hlog(fload.sizeFile);
        //       int ires=-1;
        //       if ((ires=fload.loadAll()) < 0) {
        //           return "";
        //       }
        //       hlog(ires);
        //       pstring stres(fload.dataAll);
        //       free(fload.dataAll);
        //       hlog(stres);
        //       return stres;
    }

    //文件中是这样的形式
    //2018-12-15 11:23:00 2018-12-15 11:23:00
    //目标时间，上一次的时间
    static bool checkone() {
        string strpath = "/root/test.dat";
        //如果找不到文件,则false
        if (!plib::isExsistFile(strpath)) {
            hlog("找不到license文件，程序退出");
            hlog(strpath);
            return false;
        }
        //此处增加一个检查文件格式是否正确的操作，否则格式不正确，序列化无法读取卡在这，其实没啥用
        plist<string> listall = plib::getAllLinesFromFile(strpath);
        string strall = listall[0];
        hlog(listall.size());
        hlog(strall.size());
        if (strall.size() != 41 || listall.size() > 1 || strall[19] != ' ' || strall[39] != ' ') {
            hlog("文件行数或者长度不对");
            hlog(strall);
            return false;
        }
        string str1 = strall.substr(0, 19);
        string str2 = strall.substr(20, 39);
        hlog(str1);
        hlog(str2);


        protect pload = plib::loadSerialization<protect>(strpath);
        hlog(pload);

        if (!pload.valid) {
            hlog("程序已判定过期或者作弊，无法使用，程序退出");
            return false;
        }
        //        double dbModify=pload.tdes-pload.tlast;
        //        hlog(dbModify);
        //        if(dbModify!=0)
        //        {
        //            hlog("文件被修改了");
        //            hlog(strpath);
        //            return false;
        //        }


        ptime timeDes = pload.tdes;
        ptime timeNow;//当前时间
        //正常情况下当前时间肯定比读取的上次时间大，如果小了，说明用户想作弊，改了当前时间改小了
        if (timeNow - pload.tlast < 0) {
            string info =
                    "当前时间" + plib::toString(timeNow) + "比文件中保存的上次时间" + plib::toString(pload.tlast) + "小，用户作弊，程序退出";
            hlog(info);

            //存一下
            pload.tlast = timeNow;//当前时间作为上一次的时间
            pload.valid = false;
            plib::saveSerialization(strpath, pload);
            return false;
        }

        //存一下
        pload.tlast = timeNow;//当前时间作为上一次的时间
        plib::saveSerialization(strpath, pload);

        //        hlog(timeDes);
        //        hlog(timeNow);
        //如果超时了
        double dbres = timeNow - timeDes;
        hlog(dbres);
        if (dbres > 0) {
            string strinfo = "已超过软件试用期,当前时间为" + plib::toString(timeNow) + ",到期时间是" + plib::toString(timeDes);
            hlog(strinfo);
            //存一下
            pload.tlast = timeNow;//当前时间作为上一次的时间
            pload.valid = false;
            plib::saveSerialization(strpath, pload);
            return false;
        }


        return true;
        //如果当前时间超过目标时间,则false
    }

    static void threadall(void *para) {
        bool bres = checkone();
        while (bres) {
            bres = (checkone());
            sleep(5);
        }
        hlog("程序退出");
        exit(0);
    }

    //判断软件是否过期
    static void checkExpire() {
        pthreadpackc th;
        th.start(threadall);
    }

    //判断字符串是否是个正常日期YYYY-MM-DD HH:MM:SS
    static bool isDateTime(const string strdt) {
        //还要检查除了空格-还有冒号之外的全是数字
        for (size_t i = 0; i < strdt.size(); i++) {
            if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16)
                continue;
            int c = strdt[i];
            if (!isdigit(c))
                return false;
        }
        if (strdt.size() != 19 || strdt[4] != '-' || strdt[7] != '-' || strdt[10] != ' '
            || strdt[13] != ':' || strdt[16] != ':')
            return false;
        plist<pstring> lists = pstring(strdt).split(" ");
        //        hlog(lists);
        pstring date = lists[0];
        pstring time = lists[1];

        plist<pstring> listdate = date.split("-");
        //        hlog(listdate);
        int year = atoi(listdate[0].c_str());
        hlog(year);
        int mon = atoi(listdate[1].c_str());
        hlog(mon);
        int day = atoi(listdate[2].c_str());
        hlog(day);

        plist<pstring> listtime = time.split(":");
        //        hlog(listtime);
        int hour = atoi(listtime[0].c_str());
        int min = atoi(listtime[1].c_str());
        int sec = atoi(listtime[2].c_str());

        hlog(hour);
        hlog(min);
        hlog(sec);

        if (year < 0 || mon < 1 || mon > 12 || day < 1 || day > 31 || hour < 0 || hour > 23 || min < 0 || min > 59 ||
            sec < 0 || sec > 59)
            return false;


        return true;
    }

    //递归获取当前文件夹下所有的文件全路径和文件夹全路径以及path下的相对路径
    //第一个是所有文件全路径列表，第二个是所有目录全路径列表，第三个是所有文件名相对路径列表，第四个是所有目录相对路径列表
    static plist<plist<pstring> > getAllFilesAndPathsRecurision(const string path);

    //最好兼容windows//获取全路径中的路径
    static string getPathFromFullPath(string strFullPath);

    //获取全路径中的文件名
    static string getFileNameFromFullPath(string strFullPath);

    //由于 /root/Desktop/xlfdrecv/new file这种的无法识别，所以必须加双引号
    static string getmd5(string path);

    static string pwd();

    //plib::setPathConf("/opt/sc/csgl/SCService/sc.conf");
    static int setPathConf(string strnewpath);

    //plib::setPathLog("/opt/sc/csgl/SCService/log/SCService.log");
    static int initLog(string strnewpath);

    /*
     * 类变量不要直接在声明中赋值,clion和本函数暂不支持
     * 复制从class开始到字段结束,获取类名等
     * char 类的话,数据不能只是一个\0
     * 支持char数组,char类型,bool类型,支持int数组,结构体,结构体数组
     * 类型名已经支持unsigned long long这类的,不用统一改成ulonglong此类了,自动解析
     * char数组或者结构体数组里面必须是确定的数字,不支持宏定义,因为要涉及到字符串转变量名,等支持了再改
     */
    static void generateISAndOS(pstring str) {
//        hlog(str);
        //根据行分割先
        //去掉空行
        pliststring lstrall = str.split("\n{");
//        hlog(lstrall);
        //结果
        pstring strNameClass;
        pliststring lstr;//纯字段名的列表
        pliststring ltype;//每个类型,包括带空格的比如 unsigned long
        //去掉public,获取classname,第一行一定是name,单独处理,这是为了防止
        //变量名字里有class
        strNameClass = lstrall[0].split(": ")[1];
        hlog(strNameClass);
        //第二行一定是public,直接略过,如果不是public无法当接口
        for (int i = 2; i < lstrall.size(); i++) {
            pstring stri = lstrall[i];
//            hlog(stri);
            //如果前两个字符是//,则是注释,跳过
            pstring strNoSpace = stri.getStringNoSpace();
            if (strNoSpace.substr(0, 2) == "//")
                continue;
            lstr.append(stri);
        }
        //此处是分割一行,这里根据;以及空格分割
        //pmap<int ,pstring> mtest; //这是测试复合型map有可能这样的,要根据;先分,取出前边
        //这样就能解决unsigned long long lltest这种前边带空格的
        for (size_t i = 0; i < lstr.size(); i++) {
            pstring pi = lstr[i];
//            hlog(pi);
            pliststring listres = pi.splitOne(";");
            //listres[0]就是pmap<int ,pstring> mtest,不带分号,现在就是再根据空格分割,最后一个就是要的变量名
//            hlog(listres[0]);
            pliststring listres2 = listres[0].split(" ");
//            hlog(listres2);
            pstring strResVal = listres2[listres2.size() - 1];
//            hlog(strResVal);
            listres2.deleteEnd();
            pstring strtype = listres2.join(" ");
//            hlog(strtype);
            ltype.append(strtype);
            lstr[i] = strResVal;
//            hlog(listres2.contains("char"));
//            hlog(lstr[i]);
        }
        //现在是纯字段名的列表

        //现在开始组ostream字符串
        string stros = "";
        for (size_t i = 0; i < lstr.size(); i++) {
            //            string pi=lstr[i];
            //            stros+="<<\""+pi+": \"<<x."+pi+"<<\" \"";
            string pi = lstr[i];
            //这是变量名
//                        hlog(pi);
            //要支持data[333]这种的要在此处判断,去掉括号后边的东西
            string pval = pi;
//                        hlog(pval);
            int ifind = pval.find('[');

            pval = pval.substr(0, ifind);
//                        hlog(pval);
//            pstring strOneos = "<<\"" + pi + ": \"<<x." + pval + "<<\" \"";
            pstring strOneos;
//            hlog(strOneos);
            //一个os 有[]的数组
            if (ifind != string::npos) {
                //如果是char数组,特殊处理
                if (ltype[i].contain("char")) {
                    //获取数组长度再进行
                    pliststring listForshuzu = pstring(pi).split("[]");
//                hlog(listForshuzu);
                    //后边那个就是数组长度
                    pstring strName = listForshuzu[0];
//                hlog(strName);
                    int len = atoi(listForshuzu[1].c_str());
                    pstring strtype = ltype[i];
//                hlog(strtype);
//                hlog(len);
                    strOneos = "os<<\"" + pi + ": \"" + ";os.write(x." + pval + "," + plib::toString(len) + "*sizeof(" +
                               strtype + "));os<<\" \";";
//                    hlog(strOneos);
                } else {
                    //其他数组要挨个存
                    //获取数组长度再进行
                    pliststring listForshuzu = pstring(pi).split("[]");
//                hlog(listForshuzu);
                    //后边那个就是数组长度
                    pstring strName = listForshuzu[0];
                    hlog(strName);
                    int len = atoi(listForshuzu[1].c_str());
                    pstring strtype = ltype[i];
//                hlog(strtype);
//                hlog(len);

                    strOneos = "os<<\"" + pi + ": \";for(int i=0;i<" + plib::toString(len) + ";i++){os<<x." +
                               strName + "[i]<<\" \";}os<<\" \";";
//                    hlog(strOneos);
                }
            } else {
                strOneos = "os<<\"" + pi + ": \"<<x." + pval + "<<\" \";";
                hlog(strOneos);
            }
            stros += strOneos;
//            hlog(stros);
        }
        stros += ";";
        //再加上函数体等
        stros = " friend ostream& operator<<(ostream& os," + strNameClass + " x){" + stros + "return os;}";

//        hlog("\n"+stros+"\n");
        //现在开始组istream字符串
        string stris = "";
        for (size_t i = 0; i < lstr.size(); i++) {
            string pi = lstr[i];
//            hlog(pi);
            pstring pval = pi;
//            hlog(pval);
            int ifind = pval.find('[');
//            pval = pval.substr(0, ifind);
//            hlog(pval);
//            hlog(ifind);
            pstring strOneis;
            if (ifind == string::npos) {
//                hlog("找不到 ");
                strOneis = string("plib::skipCountch(is,") + plib::toString(pi.size() + 2) + ");is>>x." + pval + ";";
//                hlog(strOneis);
            } else {//找到了的话,说明是数组
                //获取数组长度再进行
                //如果是char数组,特殊处理
                if (ltype[i].contain("char")) {
                    pliststring listForshuzu = pval.split("[]");
//                hlog(listForshuzu);
                    //后边那个就是数组长度
                    pstring strName = listForshuzu[0];
//                hlog(strName);
                    int len = atoi(listForshuzu[1].c_str());

                    // 关于数组的单独处理
                    pstring strshuzu = "is.read((char*)x." + strName + "," + plib::toString(len) + ");";
//                hlog(strshuzu);
                    strOneis = string("plib::skipCountch(is,") + plib::toString(pi.size() + 2) + ");" + strshuzu;

                } else {
                    //其他数组要挨个取
                    pliststring listForshuzu = pval.split("[]");
//                hlog(listForshuzu);
                    //后边那个就是数组长度
                    pstring strName = listForshuzu[0];
//                hlog(strName);
                    int len = atoi(listForshuzu[1].c_str());

                    // 关于数组的单独处理
                    pstring strshuzu = "is.read((char*)x." + strName + "," + plib::toString(len) + ");";
//                hlog(strshuzu);
                    strOneis = string("plib::skipCountch(is,") + plib::toString(pi.size() + 2) + ");";
                    strOneis += "for(int i=0;i<" + plib::toString(len) + ";i++){is>>x." +
                                strName + "[i];is.get();}";
                    hlog(strOneis);
                }
            }
            //一个is
//            hlog(strOneis);
            stris += strOneis;
            //最后去掉空格，不然有字符串的话会失败
            stris += "is.get();";
        }
        //再加上函数体等 istream
        stris = " friend istream& operator>>(istream& is," + strNameClass + " &x){" + stris + "return is;}";
        //统一打印
        pstring code = "\n" + stros + stris + "\n";
        hlog(code);
    }

    //is跳过n个字符
    static void skipCountch(istream &is, int len) {
        return common_skipCountch(is, len);
    }

    //is提取字符串,跳过空格,根据[]来判断
    static string istreamToString(istream &is) {
        return common_istreamToString(is);
    }

    //跳过[之前的所有
    static void skipDescription(istream &is) {
        return common_skipDescription(is);
    }

    //序列化方案
    template<class T>
    static string toString(T t)//与类中重载的对应
    {
        return common_toString(t);
    }

    template<class T>
    static T toClass(string str) {
        return common_toClass<T>(str);
    }


    //结构体
    //    friend ostream& operator<<(ostream& os,ROAD_STATUS_STRUCT r)
    //    {
    //        os.write((char*)&r,sizeof(ROAD_STATUS_STRUCT));
    //        return os;
    //    }
    //    friend istream& operator>>(istream& is,ROAD_STATUS_STRUCT &r)
    //    {
    //        is.read((char*)&r,sizeof(ROAD_STATUS_STRUCT));
    //        return is;
    //    }
    template<class T>
    static void toClass(string str, T &t) {
        return common_toClass(str, t);
    }

    //istream的时候跳过几个字符，一般是跳过名字
    static void skip(istream &is, int len) {
        for (size_t i = 0; i < len; i++)
            is.get();
    }

    //是否包含字母
    static bool isContaiAlpha(string str);

    static string hostToIp(const char *url);

    //路径加双引号，不然出错
    static plist<long> getDiskUseInfo(const char *arrchPath);

    static bool createFile(const char *arrchFullPath);

    static void mkdirp(string path) {
        return clib_mkdirp(path.c_str());
    }

    static bool remove(const char *path);

    //有live或者time=，认为ping通
    //获取是否能ping通 1秒超时后边参数是指定源ip
    static bool ping(pstring strip, pstring strIpSrc = "") {
        string cmd;
        if (strIpSrc == "") {
            //这样才能超时
            cmd = "ping " + strip + " -c 1 -W 1";
        } else {
            cmd = "ping " + strip + " -c 1 -W 1 -I " + strIpSrc;
        }
        //         hlog(cmd);
        string stres = plib::getShellOutput(cmd);
        if ((stres.find("live") != string::npos) || (stres.find("time=") != string::npos))
            return true;
        return false;
    }

    //有live或者time=，认为ping通
    //获取是否能ping通 1秒超时后边参数是指定源ip
    static bool ping3(pstring strip, pstring strIpSrc = "") {
        string cmd;
        if (strIpSrc == "") {
            //这样才能超时
            cmd = "ping " + strip + " -c 3 -W 1";
        } else {
            cmd = "ping " + strip + " -c 3 -W 1 -I " + strIpSrc;
        }
        hlog(cmd);
        string stres = plib::getShellOutput(cmd);
        if ((stres.find("live") != string::npos) || (stres.find("time=") != string::npos))
            return true;
        return false;
    }

    static void setSignals(void (*funExit)(int id), void(*funPipe)(int id));

    //查看进程是否存在
    static bool isExsistProcess(string strNameProcess);

    //守护进程,一旦调用就代表使用日志了
    static int daemonize();

    //发送邮件,需要预先配置mail服务器
    static string mail(string addr, string title, string content);

    /*读取配置文件
     * [section]
     * key=value
     * 默认是当前目录的xlfd.conf，支持中文
     * 配置文件请按格式来,否则不保证函数正确
     * section后边不要加注释,配置项后边可以加注释,可以有空行,key后要紧跟=不要有空格,key前边也不要有空格
     */
    static string getConfString(string section, string key);

    //删除配置
    static bool delConfString(string section, string key);

    //根据section和key增加配置,已有的不加,么有section加section
    static bool addConfString(string section, string key, string value);

    //更改配置
    static bool setConfString(string section, string key, string value);

    //获取本机地址,利用shell //新的算法，根据网络名分组，可判断是否没网等，去除vmnet lo等,没网返回空
    static pstring getIPLocal();

    //获取shell执行结果 更改无限制
    static string getShellOutput(string shell);

    static string exec(const char *cmd) {
        FILE *pipe = popen(cmd, "r");
        if (!pipe) return "";
        char buffer[2048];
        std::string result = "";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        return result;
    }

    //会出问题，以后解决
    //    static string getShellOutput(char* shell);
    //监控日志函数模板，检测到keys中任一个在日志文件paths列表中任一个新行中出现时进行报警等操作 多对多
    static void monitorLogMultiFileMultiConstrain(plist<pstring> paths, plist<pstring> keys);

    static void monitorLogs(pliststring paths, pliststring keys) {
        monitorLogMultiFileMultiConstrain(paths, keys);
    }

    //一对一
    static void monitorLogOneFileOneConstrain(string path, string key);

    //一对多
    static void monitorLogOneFileMultiConstrain(string path, plist<string> keys);

    //获取path文件中含有strsub的行个数，不要带空格
    static int getLineCountContains(string path, string strsub);

    //获取path文件中含有strsub的最后一行,不要带空格
    static pstring getLineLastContains(string path, string strsub);

    //追加写入文件
    static bool saveStringAppend(string path, string content);

    //由keyFind字符串位置后插入文本,暂时先整个覆盖
    static bool insertStringToFile(string path, string content, string keyFind);

    //覆盖写入文件,如果不存在，则创建
    static bool saveString(string path, string content);

    //覆盖写入文件,如果不存在，则创建，参数为vector<string>
    static bool saveLines(string path, plist<string> keys);

    //获取文件中所有行,读取的时候要去掉换行浮
    static plist<string> getAllLinesFromFile(string path);


    //按行读取，用string，不能设置1字节对齐，否则不行
    static void readLinesFromFile();

    //获取文件大小ong 和 longlong在64位下都是8位范围是-9223372036854775807到9223372036854775807，足够
    static long long getFileSize(string strFullPath);

    //y-m-d h:m:s 获取当前时间,不带毫秒的
    static string getTimeNowNoMs() {
        pstring strTimeAll = plib::getTimeNow();
        return strTimeAll.substr(0, strTimeAll.size() - 4);
    }

    //y-m-d h:m:s.毫秒 获取当前时间
    static string getTimeNow();

    //ymdhms获取无分割符时间
    static string getTimeNowNoSplit();

    static pstring getTimeNowNoSplitNoMs() {
        return pstring(getTimeNowNoSplit()).split(".")[0];
    }

    //ymd,只获取日期 2016-10-19
    static string getDateNow();

    static string getTimeHHMMSS();

    //转大端，这是float的，int用htohl即可
    static float reverse(const float inFloat) {
        float retVal;
        char *floatToConvert = (char *) &inFloat;
        char *returnFloat = (char *) &retVal;

        // swap the bytes into a temporary buffer
        returnFloat[0] = floatToConvert[3];
        returnFloat[1] = floatToConvert[2];
        returnFloat[2] = floatToConvert[1];
        returnFloat[3] = floatToConvert[0];

        return retVal;
    }

    //转大端，这是float的，int用htohl即可
    static double reverse(const double inFloat) {
        double retVal;
        char *floatToConvert = (char *) &inFloat;
        char *returnFloat = (char *) &retVal;

        // swap the bytes into a temporary buffer
        returnFloat[0] = floatToConvert[7];
        returnFloat[1] = floatToConvert[6];
        returnFloat[2] = floatToConvert[5];
        returnFloat[3] = floatToConvert[4];
        returnFloat[4] = floatToConvert[3];
        returnFloat[5] = floatToConvert[2];
        returnFloat[6] = floatToConvert[1];
        returnFloat[7] = floatToConvert[0];

        return retVal;
    }

    static int reverse(const int i) {
        return ntohl(i);
    }

    //判断文件是否存在
    static bool isExsistFile(string strpath);

    //判断文件夹是否存在
    static bool isExsistDir(string strpath);

    //判断文件或文件夹是否存在,通用
    static bool isExsist(string path) {
        if (access(path.c_str(), 0) < 0)
            return false;
        return true;
    }

    //获取路径类型
    static string getPathType(string path) {
        //如果该目录或路径不存在，则返回空字符串
        if (access(path.c_str(), 0) < 0)
            return "noaccess";
        struct stat s_buf;
        /*获取文件信息，把信息放到s_buf中*/
        stat(path.c_str(), &s_buf);
        if (S_ISDIR(s_buf.st_mode))
            return "dir";
        if (S_ISREG(s_buf.st_mode))
            return "file";
    }

    static bool isDirEmpty(string path) {
        pdir dir(path.c_str());
        //        hlog(dir.listNameAll.size());
        //        hlog(dir.listPathAll.size());
        if (dir.listNameAll.size() + dir.listPathAll.size() == 0)
            return true;
        return false;
    }

    //分割字符串普通
    static vector<string> split(string str, string pattern);

    template<class T>
    static void saveSerialization(string path, T t) {
        ofstream outFile;


        //This overload's new in C++11, which means you need to pass -std=c++11 in the build command.
        //In C++03, we used to write this:
        //打开文件
        outFile.open(path.c_str());

        outFile << t;
        //关闭文件
        outFile.close();
    }

    template<class T>
    static T loadSerialization(string path) {
        T t;
        ifstream fin(path.c_str());
        if (fin.is_open()) {
            fin >> t;
            fin.close();
        } else {
            hlog(pstring() << "未找到序列化文件" << path);
        }
        return t;
    }
};


class ParaOne {
public:
    ulonglong ullstart;
    ulonglong ullend;
    int icurrent;


    int irealsize;
    //下边是公共的
    ulonglong ullFileSize;//总大小
    int count;//分成几块

    bool isFinish;

    char strip[20];
    char strfile[512];
    int iport;

    friend ostream &operator<<(ostream &os, ParaOne po) {
        os << "icurrent: " << po.icurrent << " ullstart: " << po.ullstart << " ullend: " << po.ullend << " sizeAll: "
           << po.ullFileSize << " count: " << po.count
           << " realsize: " << po.irealsize << " isFinish: " << po.isFinish << " ip: " << po.strip << " port: "
           << po.iport << " file: " << po.strfile;
    }
};


//回执结构体
class preply {
public:
    preply() {
        this->bres = 0;
        strinfo = "";
    }

    preply(bool bres, pstring strinfo) {
        this->bres = bres;
        this->strinfo = strinfo;
    }

    void set(bool bres, pstring strinfo) {
        this->bres = bres;
        this->strinfo = strinfo;
    }

    bool bres;
    pstring strinfo;

    friend ostream &operator<<(ostream &os, preply x) {
        os << "bres: " << x.bres << " " << "strinfo: " << x.strinfo << " ";
        return os;
    }

    friend istream &operator>>(istream &is, preply &x) {
        plib::skipCountch(is, 6);
        is >> x.bres;
        is.get();
        plib::skipCountch(is, 9);
        is >> x.strinfo;
        is.get();
        return is;
    }
};

void printString(pstring str);


#define COMMENTS_LEN               128                          /*描述信息长度*/
#define FMT_TIME_LEN               20                           /*时间格式长度:yyyy-MM-dd hh:mm:ss*/
#define FILENAME_LEN               64                           /*文件名长度*/
#define RAW_FILENAME_LEN           256                          /*RawDataFile文件名长度*/
#define TASK_ID_LEN                42                           /*任务流水号长度*/
#define JOB_ID_LEN                 27                          /*作业任务编号长度*/
#define TASKARRAY_NUM_LEN          18                           /*任务单流水号长度*/
#define ROAD_ID_LEN                (JOB_ID_LEN + 6)             /*路标识长度*/
#define STAR_LEN                   20                           /*卫星名称长度*/
#define TASK_MODE_LEN              9                            /*任务作业方式长度*/
#define DEST_LEN                   6                            /*目的节点长度*/
#define SCHEID_LEN                 13                           //原始计划编号
#define MAX_ROAD_NUM               8                            /*最大路信息个数*/
#define MAX_RAWFILE_NUM           8                           /*最大原始数据传输文件个数*/
#define HTYPEGETCONF 91111111//查询conf
#define HTYPEADDCONF 91111112
#define HTYPEDELCONF 91111113
#define HTYPESETCONF 91111114
#define HTYPE_GETTASKS 0x9115//获取任务信息
#pragma pack(1)

//以下是获取传输所有服务器信息类
class TASK_HEAD {
public:
    int type;
    int length;

    TASK_HEAD() {
        this->length = 0;
    }
};

class TASK_ADDCONF {
public:
    char arrchkey[50];
    char arrchval[50];

    TASK_ADDCONF() {
        bzero(arrchkey, sizeof(arrchkey));
        bzero(arrchval, sizeof(arrchval));
    }
};

//传输通道信息
class ROAD_STATUS_STRUCT {
public:
    char arrchRoadNumber[33];            /*传输通道标识*/
    char arrchTaskSerialNumber[42];      /*任务流水号*/
    char arrchRoadFileName[64];         /*本路传输文件名*/
    int iTransferRate;                           /*传输通道实际带宽*/
    short shFinishPercent;                         /*传输通道完成百分比*/
    unsigned long long ulTransferedData;                        /*传输数据量,数据类型需要修改成unsigned long*/
    char arrchServerIP[20];             /*传输服务器IP*/
    char arrchRealStartTime[20];        /*传输实际开始时间*/
    char arrchRealEndTime[20];          /*传输实际完成时间*/
    int iTransfersTime;                          /*传输时间*/
    int iCompressRatio;                          /*传输压缩比*/
    short shRoadState;                             /*传输通道状态：参见任务状态*/
    char arrchSystem[100];                        /*当前通道的系统GF,KJ,Z3*/
    char arrchTaskInfo[256];         /*通道的备注及说明*/
    short shLink;                                  //哪条链路
    char arrchTimeLast[30];                       //上一次更新时间
    char DTCIP[30];

    bool operator<(const ROAD_STATUS_STRUCT &road) const {
        //        if(strcmp(arrchRoadNumber,road.arrchRoadNumber)<0)
        string strall = string(arrchTaskSerialNumber) + string(arrchRoadNumber);
        string strallalien = string(road.arrchTaskSerialNumber) + string(road.arrchRoadNumber);
        if (strall < strallalien)
            return true;
        return false;
    }

    friend ostream &operator<<(ostream &os, ROAD_STATUS_STRUCT x) {
        os << "arrchRoadNumber[33]: ";
        os.write(x.arrchRoadNumber, 33 * sizeof(char));
        os << " ";
        os << "arrchTaskSerialNumber[42]: ";
        os.write(x.arrchTaskSerialNumber, 42 * sizeof(char));
        os << " ";
        os << "arrchRoadFileName[64]: ";
        os.write(x.arrchRoadFileName, 64 * sizeof(char));
        os << " ";
        os << "iTransferRate: " << x.iTransferRate << " ";
        os << "shFinishPercent: " << x.shFinishPercent << " ";
        os << "ulTransferedData: " << x.ulTransferedData << " ";
        os << "arrchServerIP[20]: ";
        os.write(x.arrchServerIP, 20 * sizeof(char));
        os << " ";
        os << "arrchRealStartTime[20]: ";
        os.write(x.arrchRealStartTime, 20 * sizeof(char));
        os << " ";
        os << "arrchRealEndTime[20]: ";
        os.write(x.arrchRealEndTime, 20 * sizeof(char));
        os << " ";
        os << "iTransfersTime: " << x.iTransfersTime << " ";
        os << "iCompressRatio: " << x.iCompressRatio << " ";
        os << "shRoadState: " << x.shRoadState << " ";
        os << "arrchSystem[100]: ";
        os.write(x.arrchSystem, 100 * sizeof(char));
        os << " ";
        os << "arrchTaskInfo[256]: ";
        os.write(x.arrchTaskInfo, 256 * sizeof(char));
        os << " ";
        os << "shLink: " << x.shLink << " ";
        os << "arrchTimeLast[30]: ";
        os.write(x.arrchTimeLast, 30 * sizeof(char));
        os << " ";
        os << "DTCIP[30]: ";
        os.write(x.DTCIP, 30 * sizeof(char));
        os << " ";;
        return os;
    }

    friend istream &operator>>(istream &is, ROAD_STATUS_STRUCT &x) {
        plib::skipCountch(is, 21);
        is.read((char *) x.arrchRoadNumber, 33);
        is.get();
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchTaskSerialNumber, 42);
        is.get();
        plib::skipCountch(is, 23);
        is.read((char *) x.arrchRoadFileName, 64);
        is.get();
        plib::skipCountch(is, 15);
        is >> x.iTransferRate;
        is.get();
        plib::skipCountch(is, 17);
        is >> x.shFinishPercent;
        is.get();
        plib::skipCountch(is, 18);
        is >> x.ulTransferedData;
        is.get();
        plib::skipCountch(is, 19);
        is.read((char *) x.arrchServerIP, 20);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchRealStartTime, 20);
        is.get();
        plib::skipCountch(is, 22);
        is.read((char *) x.arrchRealEndTime, 20);
        is.get();
        plib::skipCountch(is, 16);
        is >> x.iTransfersTime;
        is.get();
        plib::skipCountch(is, 16);
        is >> x.iCompressRatio;
        is.get();
        plib::skipCountch(is, 13);
        is >> x.shRoadState;
        is.get();
        plib::skipCountch(is, 18);
        is.read((char *) x.arrchSystem, 100);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchTaskInfo, 256);
        is.get();
        plib::skipCountch(is, 8);
        is >> x.shLink;
        is.get();
        plib::skipCountch(is, 19);
        is.read((char *) x.arrchTimeLast, 30);
        is.get();
        plib::skipCountch(is, 11);
        is.read((char *) x.DTCIP, 30);
        is.get();
        return is;
    }

};


/*任务状态数据结构*/
class TASK_STATUS_STRUCT {
public:
    char arrchTaskSerialNumber[42];      /*任务流水号*/
    char arrchTaskArraySerialNumber[18]; //任务单流水号
    char arrchJobTaskID[27];              /*作业任务编号*/
    short shTaskPriority;                          //数据传输级别(0 一般 1较高 2 最高)
    short shInexecutingState;                      /*任务状态*/
    char arrchScheID[13];                 //原始计划编号
    char arrchTaskMode[9];            /*作业方式*/
    char arrchDataFileName[256];     /*原始数据文件名(T51时出现)*/
    char arrchSatelliteName[20];            /*卫星名称*/
    char arrchDataSource[6];               /*数据源地址*/
    char arrchDataDestination[6];          /*数据传输目的节点*/
    char arrchForeseeStartTime[20];     /*传输预计开始时间*/
    char arrchForeseeEndTime[20];       /*传输预计开始时间*/
    short shDestNum;                               //改过了，多目的地个数 %%非实时传输状态(3：上传OK  2：上传数据文件  1：上传DESC  0：未开始)
    short iFileExist;                              //数据文件是否存在(0：存在 1：不存在)
    char arrchFailReason[128];           //失败原因(用于判断T51源的标志和填写失败原因)
    int iRoadNumber;                             /*传输通道信息个数*/
    ROAD_STATUS_STRUCT arrstruRoadStatus[8];      /*传输通道信息*/
    short shSendDESC;                              //是否发送DESC文件()
    short shWhichLink;                             //哪个链路，0表示第一个，1表示第二个
    char arrchTaskInfo[128];             //通过第一个字节转int判断T51是否从接收站重传，已封装如下函数


    //是否从DTC开始重传
    bool isRetransFromDTC() {
        char arrchTmp[4];
        memset(arrchTmp, 0, sizeof(arrchTmp));
        memcpy(arrchTmp, this->arrchTaskInfo, 1);
        int iRet = atoi(arrchTmp);
        //1 从DTC 0从接受站
        if (iRet == 1)
            return true;
        return false;
    }

    bool operator<(const TASK_STATUS_STRUCT &t2) const {
        if (strcmp(arrchTaskSerialNumber, t2.arrchTaskSerialNumber) < 0)
            return true;
        return false;
    }

    friend ostream &operator<<(ostream &os, TASK_STATUS_STRUCT x) {
        os << "arrchTaskSerialNumber[42]: ";
        os.write(x.arrchTaskSerialNumber, 42 * sizeof(char));
        os << " ";
        os << "arrchTaskArraySerialNumber[18]: ";
        os.write(x.arrchTaskArraySerialNumber, 18 * sizeof(char));
        os << " ";
        os << "arrchJobTaskID[27]: ";
        os.write(x.arrchJobTaskID, 27 * sizeof(char));
        os << " ";
        os << "shTaskPriority: " << x.shTaskPriority << " ";
        os << "shInexecutingState: " << x.shInexecutingState << " ";
        os << "arrchScheID[13]: ";
        os.write(x.arrchScheID, 13 * sizeof(char));
        os << " ";
        os << "arrchTaskMode[9]: ";
        os.write(x.arrchTaskMode, 9 * sizeof(char));
        os << " ";
        os << "arrchDataFileName[256]: ";
        os.write(x.arrchDataFileName, 256 * sizeof(char));
        os << " ";
        os << "arrchSatelliteName[20]: ";
        os.write(x.arrchSatelliteName, 20 * sizeof(char));
        os << " ";
        os << "arrchDataSource[6]: ";
        os.write(x.arrchDataSource, 6 * sizeof(char));
        os << " ";
        os << "arrchDataDestination[6]: ";
        os.write(x.arrchDataDestination, 6 * sizeof(char));
        os << " ";
        os << "arrchForeseeStartTime[20]: ";
        os.write(x.arrchForeseeStartTime, 20 * sizeof(char));
        os << " ";
        os << "arrchForeseeEndTime[20]: ";
        os.write(x.arrchForeseeEndTime, 20 * sizeof(char));
        os << " ";
        os << "shDestNum: " << x.shDestNum << " ";
        os << "iFileExist: " << x.iFileExist << " ";
        os << "arrchFailReason[128]: ";
        os.write(x.arrchFailReason, 128 * sizeof(char));
        os << " ";
        os << "iRoadNumber: " << x.iRoadNumber << " ";
        os << "arrstruRoadStatus[8]: ";
        for (int i = 0; i < 8; i++) { os << x.arrstruRoadStatus[i] << " "; }
        os << " ";
        os << "shSendDESC: " << x.shSendDESC << " ";
        os << "shWhichLink: " << x.shWhichLink << " ";
        os << "arrchTaskInfo[128]: ";
        os.write(x.arrchTaskInfo, 128 * sizeof(char));
        os << " ";;
        return os;
    }

    friend istream &operator>>(istream &is, TASK_STATUS_STRUCT &x) {
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchTaskSerialNumber, 42);
        is.get();
        plib::skipCountch(is, 32);
        is.read((char *) x.arrchTaskArraySerialNumber, 18);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchJobTaskID, 27);
        is.get();
        plib::skipCountch(is, 16);
        is >> x.shTaskPriority;
        is.get();
        plib::skipCountch(is, 20);
        is >> x.shInexecutingState;
        is.get();
        plib::skipCountch(is, 17);
        is.read((char *) x.arrchScheID, 13);
        is.get();
        plib::skipCountch(is, 18);
        is.read((char *) x.arrchTaskMode, 9);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchDataFileName, 256);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchSatelliteName, 20);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchDataSource, 6);
        is.get();
        plib::skipCountch(is, 25);
        is.read((char *) x.arrchDataDestination, 6);
        is.get();
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchForeseeStartTime, 20);
        is.get();
        plib::skipCountch(is, 25);
        is.read((char *) x.arrchForeseeEndTime, 20);
        is.get();
        plib::skipCountch(is, 11);
        is >> x.shDestNum;
        is.get();
        plib::skipCountch(is, 12);
        is >> x.iFileExist;
        is.get();
        plib::skipCountch(is, 22);
        is.read((char *) x.arrchFailReason, 128);
        is.get();
        plib::skipCountch(is, 13);
        is >> x.iRoadNumber;
        is.get();
        plib::skipCountch(is, 22);
        for (int i = 0; i < 8; i++) {
            is >> x.arrstruRoadStatus[i];
            is.get();
        }
        is.get();
        plib::skipCountch(is, 12);
        is >> x.shSendDESC;
        is.get();
        plib::skipCountch(is, 13);
        is >> x.shWhichLink;
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchTaskInfo, 128);
        is.get();
        return is;
    }

};


class ArrstruTask {
public:
    int length;
    TASK_STATUS_STRUCT tasks[400];

    ArrstruTask() {
        this->length = 0;
        for (int i = 0; i < 400; i++) {
            bzero(&tasks[i], sizeof(TASK_STATUS_STRUCT));
        }
    }
};

//获取配置文件端口8888
class CSGLStation {
public:
    pstring strName;//站点名称
    pstring strip;//站点ip
    pmap<pstring, pstring> mapconf;//该站点所有配置
    pstring strPathFullConf;//配置文件路径

    ptcp *tcpc;

    CSGLStation() {
        strPathFullConf = "/opt/sc/csgl/SCService/sc.conf";
    }

    CSGLStation(pstring name, pstring strip, pstring pathconf = "/opt/sc/csgl/SCService/sc.conf") {
        this->strName = name;
        this->strip = strip;
        this->strPathFullConf = pathconf;
        this->tcpc = new ptcp(strip, 8888);
    }

    pmap<pstring, pstring> getConfMap() {
        return mapconf;
    }

    void connect() {
        (this->tcpc->connectx());
    }

    bool delConf(pstring key) {
        hlog(key);
        TASK_HEAD head;
        head.type = HTYPEDELCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv(bRes) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    bool setConf(pstring key, pstring val) {
        hlog(key, val);
        TASK_HEAD head;
        head.type = HTYPESETCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        strcpy(struAddConf.arrchval, val.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv(bRes) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    bool addConf(pstring key, pstring val) {
        hlog(key, val);
        TASK_HEAD head;
        head.type = HTYPEADDCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        strcpy(struAddConf.arrchval, val.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv(bRes) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    ArrstruTask getTasks() {
        ArrstruTask arrstruTaskRes;
        TASK_HEAD head;
        head.type = HTYPE_GETTASKS;
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return arrstruTaskRes;
        }
        int ires = -1;
        if ((ires = tcpc->recv(arrstruTaskRes)) < 0) {
            plib::sleep(1000);
            return arrstruTaskRes;
        }
        hlog(ires);
        return arrstruTaskRes;
    }

    //获取配置到map中,用tcp形式,需要server配合
    pmap<pstring, pstring> getConfStation() {
        //先清空,不然一直都有
        this->mapconf.clear();
        //获取配置文件各个行,用xsh返回太慢,改成tcp获取
        pstring strContentConf = getConfUseTcp();
//        pstring strContentConf=plib::xsh(this->strip,"cat "+this->strPathFullConf);
//        hlog(strContentConf.contain("\0"));
//        hlog(strContentConf.size(),strContentConf.find("\0"));
//按行分割,顺便去掉空行
        pliststring listConf = strContentConf.split("\n");

//        hlog(listConf);
        //去掉带#的
        pliststring listConfNoComment;
        for (int i = 0; i < listConf.size(); i++) {
            if (!listConf[i].contain("#"))
                listConfNoComment.append(listConf[i]);
        }
//        hlog(listConfNoComment);
        //然后可以逐条解析然后赋值给map了
        for (int i = 0; i < listConfNoComment.size(); i++) {
            pstring strConfi = listConfNoComment[i];//这就是一条配置A=B
            pliststring listConfi = strConfi.split("=");
//            hlog(listConfi);
//要加个判断,因为还有[basic]这样的就一个,要略过,不然崩溃
            if (listConfi.size() == 2) {
                //                mapconf[listConfi[0]] = listConfi[1];
//                hlog(listConfi);
//                mut.lock();
                mapconf.add(listConfi[0], listConfi[1]);
//                mapconf[listConfi[0]] = listConfi[1];
//                mut.unlock();
            }
        }
        return mapconf;
//        hlog(mapconf);
//        pstring strtest=plib::toString(mapconf);
//        hlog(strtest,strtest.size(),strtest.find("\0"),string::npos);
//        hlog(plib::toClass<pmap<pstring,pstring>>(strtest));
    }

    friend ostream &operator<<(ostream &os, CSGLStation x) {
        os << "strName: " << x.strName << " " << "strip: " << x.strip << " " << "mapconf: " << x.mapconf << " "
           << "strPathFullConf: " << x.strPathFullConf << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLStation &x) {
        plib::skipCountch(is, 9);
        is >> x.strName;
        is.get();
        plib::skipCountch(is, 7);
        is >> x.strip;
        is.get();
        plib::skipCountch(is, 9);
        is >> x.mapconf;
        is.get();
        plib::skipCountch(is, 17);
        is >> x.strPathFullConf;
        is.get();
        return is;
    }

private:

    //要有服务端,超时1秒
    pstring getConfUseTcp() {
        //获取配置文件
        TASK_HEAD head;
        head.type = HTYPEGETCONF;
        head.length = 0;
        if (tcpc->send(head) < 0) {
//            hlog("send head fail");
            plib::sleep(1000);
            return "";
        }
        //等待接收配置文件
//    先接包头
        TASK_HEAD pheadRecv;
        if (tcpc->recv((char *) &pheadRecv, sizeof(TASK_HEAD), 3) < 0) {
            hlog("recv head fail");
//            plib::sleep(1000);
            return "";
        }
//        hlog(pheadRecv.length);
        char *pdataRecv = HMALLOC(char, pheadRecv.length);
        if (tcpc->recv(pdataRecv, pheadRecv.length, 3) < 0) {
//            hlog("recv data fail");
            return "";
        }
        return (pdataRecv);
    }
};

class CSGLSystem {
public:
    pstring strName;    //系统名称
    pmap<pstring, CSGLStation> mapStation;//各个站点
    CSGLSystem() {

    }

    CSGLSystem(pstring name, pmap<pstring, CSGLStation> mapStation) {
        this->strName = name;
        this->mapStation = mapStation;
    }

    CSGLStation getStationByName(pstring strNameStation) {
        return mapStation[strNameStation];
    }

    void connectAllStation() {
        for (int i = 0; i < mapStation.size(); i++) {
            mapStation.getValue(i).connect();
        }
    }

    //当前所有系统站获取配置函数
    void getConfSystem() {
        for (int i = 0; i < mapStation.size(); i++) {
            mapStation.getValue(i).getConfStation();
        }
    }

    pstring getStationNameByIndex(int index) {
        return mapStation.getKey(index);
    }

    CSGLStation &getStationByIndex(int index) {
        return mapStation.getValue(index);
    }

    size_t size() {
        return mapStation.size();
    }

    friend ostream &operator<<(ostream &os, CSGLSystem x) {
        os << "strName: " << x.strName << " " << "mapStation: " << x.mapStation << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLSystem &x) {
        plib::skipCountch(is, 9);
        is >> x.strName;
        is.get();
        plib::skipCountch(is, 12);
        is >> x.mapStation;
        is.get();
        return is;
    }

};

class CSGLAll {
public:
    pmap<pstring, CSGLSystem> mapSystem;//各个系统
    //先根据host确定个数,后边更新,是为了提高速度,后边是信息
    //先是根目录使用比,第二个是总控运行状态
    pmap<pstring, pliststring> mapInfo;

    CSGLAll() {
        pmap<pstring, CSGLStation> mapStationGF;
        mapStationGF.add("DTC", CSGLStation("DTC", "172.16.11.3"));
        mapStationGF.add("KSC", CSGLStation("KSC", "172.16.141.5"));
        mapStationGF.add("MYC", CSGLStation("MYC", "172.16.77.6"));
        mapStationGF.add("SYC", CSGLStation("SYC", "172.16.171.5"));
        mapStationGF.add("KR1", CSGLStation("KR1", "130.242.60.241"));
        mapStationGF.add("YD1", CSGLStation("YD1", "172.16.112.2"));
        mapStationGF.add("YE2", CSGLStation("YE2", "172.17.2.2"));
        CSGLSystem systemGF("高分系统", mapStationGF);

        this->mapSystem.add("高分系统",systemGF);
        pmap<pstring, CSGLStation> mapStationXD;
        mapStationXD.add("YMY", CSGLStation("YMY", "172.16.78.2"));
        mapStationXD.add("YKS", CSGLStation("YKS", "172.16.172.1"));
        mapStationXD.add("YSY", CSGLStation("YSY", "172.16.142.1"));
        mapStationXD.add("DTC", CSGLStation("DTC", "172.16.12.3"));
        CSGLSystem systemXD("先导系统", mapStationXD);
        this->mapSystem.add("先导测试系统", systemXD);

        pmap<pstring, CSGLStation> mapStationKJ;
        mapStationKJ.add("DTC", CSGLStation("DTC", "172.16.14.2"));
        mapStationKJ.add("KSC", CSGLStation("KSC", "172.16.144.2"));
        mapStationKJ.add("MYC", CSGLStation("MYC", "172.16.81.2"));
        mapStationKJ.add("SYC", CSGLStation("SYC", "172.16.174.2"));
        mapStationKJ.add("KR1", CSGLStation("KR1", "130.242.60.243"));
        mapStationKJ.add("YD1", CSGLStation("YD1", "172.16.112.1"));
        mapStationKJ.add("DTCZ3", CSGLStation("DTCZ3", "172.16.13.2"));
        mapStationKJ.add("SYCZ3", CSGLStation("SYCZ3", "172.16.173.1"));
        mapStationKJ.add("KSCZ3", CSGLStation("KSCZ3", "172.16.143.1"));
        mapStationKJ.add("MYCZ3", CSGLStation("MYCZ3", "172.16.80.1"));
        mapStationKJ.add("YE2Z3", CSGLStation("YE2Z3", "172.17.2.1"));
        CSGLSystem systemKJ("空基系统", mapStationKJ);

        this->mapSystem.add("空基系统", systemKJ);

        pliststring listhost = this->gethostAll();
//        hlog(listhost);
        for (int i = 0; i < listhost.size(); i++) {
            this->mapInfo.add(listhost[i], pliststring());
        }
        hlog(this->mapInfo);
    }

    pliststring getSystemAndStationByHost(pstring ip) {
        for (int i = 0; i < this->size(); i++) {
            CSGLSystem sys = this->getSystemByIndex(i);
            for (int j = 0; j < sys.size(); j++) {
                CSGLStation stat = sys.getStationByIndex(j);
                if (stat.strip == ip) {
                    return pliststring() << sys.strName << stat.strName;
                }
            }
        }
        return pliststring();
    }

    pliststring gethostAll() {
        pliststring lmall;
        for (int i = 0; i < this->size(); i++) {
            CSGLSystem sys = this->getSystemByIndex(i);
            for (int j = 0; j < sys.size(); j++) {
                CSGLStation station = sys.getStationByIndex(j);
                pstring strip = station.strip;
                lmall.append(strip);
            }
        }
        return lmall;
    }

    CSGLStation getStation(pstring strNameSystem, pstring strNameStation) {
        return mapSystem[strNameSystem].getStationByName(strNameStation);
    }

    CSGLStation &getStation(pstring strIpStation) {
        for (int i = 0; i < mapSystem.size(); i++) {
            CSGLSystem &systemi = this->getSystemByIndex(i);
            for (int j = 0; j < systemi.size(); j++) {
                CSGLStation &stationj = systemi.getStationByIndex(j);
                if (stationj.strip == strIpStation)
                    return stationj;
            }
        }
    }

    CSGLSystem getSystemByName(pstring strNameSystem) {
        return mapSystem[strNameSystem];
    }

    pstring getSystemNameByIndex(int index) {
        return mapSystem.getKey(index);
    }

    CSGLSystem &getSystemByIndex(int index) {
        return mapSystem.getValue(index);
    }

    size_t size() {
        return mapSystem.size();
    }

    void connectAll() {
        for (int i = 0; i < mapSystem.size(); i++) {
            mapSystem.getValue(i).connectAllStation();
        }

    }

    void getConfAll() {
        for (int i = 0; i < mapSystem.size(); i++) {
            mapSystem.getValue(i).getConfSystem();
        }
    }

    friend ostream &operator<<(ostream &os, CSGLAll x) {
        os << "mapSystem: " << x.mapSystem << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLAll &x) {
        plib::skipCountch(is, 11);
        is >> x.mapSystem;
        is.get();
        return is;
    }

};

#pragma pack()
#endif // xlfd_H
