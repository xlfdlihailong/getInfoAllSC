#include "pmysql.h"

bool pmysql::connected() {
    if (mysql_conn != NULL)
        return true;
    else
        return false;
}

//网上查找发现localhost与127.0.0.1是不同的，localhost是通过UNIX socket连接的，而127.0.0.1是通过TCP/IP连接的
//centos系用127.0.0.1多线程没问题,ubuntu就有问题..看情况吧
bool pmysql::connect(string host, string user, string pwd, string db, int port) {
//    hlog(host);
    mysql_conn = mysql_init(NULL);
    if (!mysql_conn) {
        HLOG("mysql init failed");
        return false;
    }


    mysql_conn = mysql_real_connect(mysql_conn, host.c_str(), user.c_str(),
                                    pwd.c_str(), db.c_str(), port, NULL, 0);
    //连接失败的话是空
    if (mysql_conn == NULL) {
        hlog(host);
        hlog(pstring() << "连接" << host << "-" << user << "-" << pwd << "-" << db << "失败");
        hlog(mysql_error(mysql_conn));
        return false;
    }

    if (mysql_conn) {
        //linux下要设置utf8读取
        mysql_query(mysql_conn, "SET NAMES UTF8");
        //        HLOG("Connection mysql success!");
        return true;
    } else {
        HLOG("Connect mysql failed");
        return false;
    }
}

pmysql::pmysql() {
    mysql_conn = NULL;
    mysql_res = NULL;
}

pmysql::~pmysql() {
    if (mysql_res != NULL)
        mysql_free_result(mysql_res);
    mysql_close(mysql_conn);
}

/*
 * 返回一个字符串列表的列表，一行是一个QStringlist
 * 若返回的list长度为0，说明有错误
 */
plist<plist<pstring> > pmysql::query(string strsql) {
    plist<plist<pstring> > list;

    MYSQL_ROW sql_row;
    int res = mysql_query(mysql_conn, strsql.c_str());//查询
    if (!res) {
        mysql_res = mysql_store_result(mysql_conn);//保存查询到的数据到result
        if (mysql_res) {
            int j;
//            printf("the result number is %lu\n ",(unsigned long)mysql_num_rows(mysql_res));
            j = mysql_num_fields(mysql_res);
            while (sql_row = mysql_fetch_row(mysql_res))//获取具体的数据
            {
                //这是一行
                //                QStringList row;
                plist<pstring> rowtmp;
                for (int i = 0; i < j; i++) {
                    //                    row<<xqlib::toQString(sql_row[i]);
                    rowtmp.push_back(sql_row[i] == NULL ? string("") : string(sql_row[i]));
                }
                //                list<<rowtmp;
                list.push_back(rowtmp);
            }


        }
    } else
        HLOG("mysql query failed");
    return list;
}


/*
 * 返回一个字符串列表的列表，一行是一个QStringlist
 * 若返回的list长度为0，说明有错误
 */
plist<pmap<pstring, pstring> > pmysql::querymap(string strsql) {
    plist<plist<pstring> > list;
    plist<pmap<pstring, pstring> > lmap;
    MYSQL_ROW sql_row;
    MYSQL_FIELD *sql_field;
    int res = mysql_query(mysql_conn, strsql.c_str());//查询/

    if (!res) {
        mysql_res = mysql_store_result(mysql_conn);//保存查询到的数据到result
        if (mysql_res) {
            int j;
//            printf("the result number is %lu\n ",(unsigned long)mysql_num_rows(mysql_res));
            j = mysql_num_fields(mysql_res);
            while (sql_row = mysql_fetch_row(mysql_res))//获取具体的数据
            {
                //这是一行
                //                QStringList row;
                plist<pstring> rowtmp;
                for (int i = 0; i < j; i++) {
                    //                    row<<xqlib::toQString(sql_row[i]);
                    rowtmp.push_back(sql_row[i] == NULL ? string("") : string(sql_row[i]));
                }
                //                list<<rowtmp;
                list.push_back(rowtmp);

            }
            plist<pstring> listname;
            while (sql_field = mysql_fetch_field(mysql_res)) {
                listname.append(pstring(sql_field->name));
            }

            for (int i = 0; i < list.size(); i++) {
                pmap<pstring, pstring> mapi;
                plist<pstring> listvalue = list[i];
                for (int i = 0; i < listname.size(); i++) {
//                    mapi[listname[i]]=listvalue[i];
                    mapi.add(listname[i], listvalue[i]);
                }
                lmap.push_back(mapi);
            }
        }
    } else
        HLOG("mysql query failed");
    return lmap;
}

int pmysql::query(string strsql, vector<vector<string> > &list) {
    MYSQL_ROW sql_row;
    int res = mysql_query(mysql_conn, strsql.c_str());//查询
    if (!res) {
        mysql_res = mysql_store_result(mysql_conn);//保存查询到的数据到result
        if (mysql_res) {
            int j;
            printf("the result number is %lu\n ", (unsigned long) mysql_num_rows(mysql_res));
            j = mysql_num_fields(mysql_res);
            while (sql_row = mysql_fetch_row(mysql_res))//获取具体的数据
            {
                //这是一行
                //                QStringList row;
                vector<string> rowtmp;
                for (int i = 0; i < j; i++) {
                    //                    row<<xqlib::toQString(sql_row[i]);
                    rowtmp.push_back(sql_row[i] == NULL ? string("") : string(sql_row[i]));
                }
                //                list<<rowtmp;
                list.push_back(rowtmp);
            }
        }
    } else {
        HLOG("mysql query failed");
        return -1;
    }
    return 0;
}

bool pmysql::insert(std::initializer_list<std::string> il) {
    if (il.size() % 2 == 0) {
        hlog("参数个数应为奇数");
        return false;
    }
    if (il.size() < 3) {
        hlog("参数个数应大于3");
        return false;
    }
    pstring strNameTable;
    pstring strkey = "(";
    pstring strval = " values(";
    int count = 0;
//    for ( string sone : il )
    for (auto it = il.begin(); it != il.end(); it++) {
        pstring sone = *it;
        hlog(sone);
        if (count == 0)
            strNameTable = sone;

        else if (count > 0 && count < il.size() / 2 + 1)
            strkey += sone + ",";
        else
            strval += "\"" + sone + "\",";
        count++;
    }
    strkey[strkey.size() - 1] = ')';
    strval[strval.size() - 1] = ')';

    hlog(strkey, strval);

    pstring strCmdSql = "insert into " + strNameTable + strkey + strval;
    hlog(strCmdSql);
    return process(strCmdSql);
}

bool pmysql::process(string strsql) {
    //    int resi=mysql_query(mysql_conn,xqlib::toCharArray(strsql));
    int resi = mysql_query(mysql_conn, strsql.c_str());
    //    HLOG_INT(resi);
    if (resi) {
        HLOG("error! process failed!");
        HLOG("Insert error %d: %s\n", mysql_errno(mysql_conn), mysql_error(mysql_conn));
        return false;
    }
    return true;
}


bool pmysql::update_img(string strTableName, string strFieldName, string strWhereField, string strWhereValue,
                        char *acFrom, unsigned long ulLenIMG) {
    char *end;
    //动态分配，不然系统不支持这么大的
    char *sql = HMALLOC(char, 20 * 1024 * 1024);
    memset(sql, 0, sizeof(sql));
    sprintf(sql, "UPDATE %s set %s=", strTableName.c_str(), strFieldName.c_str());
    end = sql + strlen(sql);
    *end++ = '\'';
    end += mysql_real_escape_string(this->mysql_conn, end, acFrom, ulLenIMG);
    *end++ = '\'';
    char where[1024];
    memset(where, 0, sizeof(where));

    sprintf(where, " WHERE %s='%s'", strWhereField.c_str(), strWhereValue.c_str());
    strcat(end, where);
    end += strlen(where);

    printf("end - sql: %d\n", (unsigned int) (end - sql));
    if (mysql_real_query(this->mysql_conn, sql, (unsigned int) (end - sql))) {
        hlog("error");
        return false;
    }
    free(sql);
    return true;
}
