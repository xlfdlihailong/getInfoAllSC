#ifndef XPMYSQL_H
#define XPMYSQL_H
#include "plib.h"
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
class pmysql
{
private:
    string host;
    string user;
    string pwd;
    string db;
    int port;

    MYSQL *mysql_conn;
    MYSQL_RES *mysql_res;
public:
    bool connected();
    //网上查找发现localhost与127.0.0.1是不同的，localhost是通过UNIX socket连接的，而127.0.0.1是通过TCP/IP连接的
//centos系用127.0.0.1多线程没问题,ubuntu就有问题..看情况吧
    bool connect(string host,string user,string pwd,string db,int port=3306);
    pmysql();
    ~pmysql();//关闭连接，所以正确用法是局部变量关闭
    plist< plist<pstring> > query(string strsql);
    plist< pmap<pstring,pstring> > querymap(string strsql);
    int query(string strsql, vector<vector<string> > &list);
    //hlog(sql.insert({"data","name", "timestart", "finish",strName,strTimeStart,"未完成"}));第一个参数时表名字
    bool insert(std::initializer_list<std::string> il);
    bool process(string strsql);
    //图片的
    bool update_img(string strTableName, string FieldName, string strWhereField, string strWhereValue,
                   char* acFrom, unsigned long ulLenIMG);

};

#endif // XPMYSQL_H
