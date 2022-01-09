#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
class HashDB {
public:
    MYSQL mysql;
    HashDB() {//构造函数中登录MySQL：mysql_init()，mysql_real_connect()
        try {
            if (!mysql_init(&mysql)) {
                throw std::string("MySQL init is error!");
            }
            if (!mysql_real_connect(&mysql, "localhost", "root", "root", "hashdb", 3306, NULL, 0)) {
                throw std::string("MySQL connect is error!");
            }

        }
        catch (std::string& error_msg) {

            std::cout << error_msg << std::endl;
        }
        catch (...) {

            std::cout << "MySQL operation is error!" << std::endl;
        }

    }
    ~HashDB() {//析构函数中断开MySQL连接
        mysql_close(&mysql);
    }

    //接口 1 ： 将文件信息 以及 其哈希值条目保存到数据库中
    //接口 2 ： 根据文件名 取出 保存的哈希值
    //接口 3 ： 列出当前数据库中所有的条目

    void setFileHash(std::string fileID, std::string fileDir, std::string fileName,
        std::string hashValue, std::string joinTime, std::string lastTime, std::string fileSize);

    std::string getFileHash(std::string fileName);
    bool getAll();
};

// eg：INSERT INTO `hashdb`.`table1` (`文件id`, `文件路径`, `文件名`, `校验的哈希值`, `加入系统时间`, `上次修改时间`, `文件大小`) 
//     VALUES ('002', 'c', '文件2', 'asd', '123', '567', '12');
void HashDB::setFileHash(std::string fileID, std::string fileDir, std::string fileName,
    std::string hashValue, std::string joinTime, std::string lastTime, std::string fileSize) {

    mysql_query(&mysql, "SET NAMES GBK"); //设置编码格式
    
    std::ostringstream buffer;
    buffer << "INSERT INTO `hashdb`.`table1` " <<
        "(`文件id`, `文件路径`, `文件名`, `校验的哈希值`, `加入系统时间`, `上次修改时间`, `文件大小`)"
        << " VALUES ('" << fileID << "', '" << fileDir << "', '"
        << fileName << "', '" << hashValue << "', '" << joinTime << "', '" << lastTime << "', '" << fileSize << "');";
    std::string cmd = buffer.str();
    if (mysql_query(&mysql, cmd.c_str())) {//失败返回 1 
        std::cout << "当前条目插入失败！可能已经存在相同的文件id" << std::endl;
        // std::string err_string = mysql_error(&mysql);
        // if (err_string.empty())
        // {
        //     throw std::string("MySQL query is error!");
        // }
        // else
        // {
        //     throw err_string;
        // }
    }
}

// eg：select filehash from table0 where filename = '文件1'
std::string HashDB::getFileHash(std::string fileName) {
    mysql_query(&mysql, "SET NAMES GBK"); //设置编码格式
    std::string cmd = "select 校验的哈希值 from table1 where 文件名 = '";
    cmd.append(fileName);
    cmd.append("';");
    if (mysql_query(&mysql, cmd.c_str())) {

        std::string err_string = mysql_error(&mysql);
        if (err_string.empty())
        {
            throw std::string("MySQL query is error!");
        }
        else
        {
            throw err_string;
        }
    }
    MYSQL_RES* result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result); //获取整条数据内容

    std::string temp = row[0];
    mysql_free_result(result);
    return temp;

}

bool HashDB::getAll() {
    mysql_query(&mysql, "SET NAMES GBK"); //设置编码格式
    try
    {
        //MySQL命令
        std::string sql = "SELECT * from table1;";
        if (mysql_query(&mysql, sql.c_str()))
        {
            std::string err_string = mysql_error(&mysql);
            if (err_string.empty())
            {
                throw std::string("MySQL query is error!");
            }
            else
            {
                throw err_string;
            }
        }
        std::cout << "MySQL语句 : " << sql << std::endl;
        MYSQL_RES* result = mysql_store_result(&mysql);
        if (!result)
        {
            throw std::string("MySQL not result!");
        }
        std::cout << std::setiosflags(std::ios::left) << std::setw(20);


        //获取字段数量
        int num_fields = mysql_num_fields(result);
        if (0 == num_fields)
        {
            throw std::string("MySQL fields number is 0!");
        }
        //获取字段名
        MYSQL_FIELD* fields = mysql_fetch_fields(result);
        if (!fields)
        {
            throw std::string("MySQL fields fetch is error!");
        }

        for (int i = 0; i < num_fields; i++)
        {
            //std::cout << fields[i].name << "\t";
            std::cout << std::setiosflags(std::ios::left) << std::setw(14) << fields[i].name;
        }
        std::cout << std::endl;

        while (MYSQL_ROW row = mysql_fetch_row(result)) //获取整条数据内容
        {
            for (int i = 0; i < num_fields; i++)
            {
                if (NULL == row[i])
                {
                    std::cout << std::setiosflags(std::ios::left) << std::setw(14) << "NULL";
                }
                else
                {
                    std::cout << std::setiosflags(std::ios::left) << std::setw(14) << row[i];
                }
            }
            std::cout << std::endl;
        }

        mysql_free_result(result);

        std::cout << "MySQL is OK." << std::endl;

    }
    catch (std::string& error_msg)
    {
        std::cout << error_msg << std::endl;
    }
    catch (...)
    {
        std::cout << "MySQL operation is error!" << std::endl;
    }

}

//main 函数仅用于测试
int main() {
    HashDB SM3DB;

    SM3DB.setFileHash("1000", "d", "文件3", "xyz", "231", "wer", "eee");      //设置一个已经存在的条目

    std::string str = SM3DB.getFileHash("文件2");//得到 文件2 的哈希值字符串
    std::cout << str << std::endl;
    SM3DB.getAll();                         //打印整张数据表的内容
    return 0;
}