#include<Windows.h>
#include<iostream>
#include<string>
#include <fstream>
#include <io.h>
#include <time.h>
#include <stdlib.h>
#include <cstring>
using namespace std;

// void HashDB::setFileHash(std::string fileID, std::string fileDir, std::string fileName,
//     std::string hashValue, std::string joinTime, std::string lastTime, std::string fileSize) 

//传入参数为 文件id 文件路径 文件哈希值     
//功能：根据文件路径解析出其他信息，存入数据库条目
void fileInfoToDB (string fileID, string fileDir, string hashValue) {
    string fileName;
    string joinTime;
    string lastTime;
    string fileSize;
    //根据路径获取 文件名
    string flag = "\\";
    string::size_type position = fileDir.rfind(flag);
    fileName = fileDir.substr(++position);
    //获取当前时间作为加入系统时间
    char *pszCurrTime = (char*)malloc(sizeof(char)*20);
    memset(pszCurrTime, 0, sizeof(char)*20);
    time_t now;
    time(&now);
    strftime(pszCurrTime, 20 , "%Y/%m/%d %H:%M:%S", localtime(&now));
    joinTime = pszCurrTime;
    free(pszCurrTime);
    //根据路径获取 文件上一次修改时间
    TCHAR szWriteTime[30];
    HANDLE hFile = CreateFile(fileDir.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    LPSTR lpszLastWriteTime = szWriteTime;
    FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC1, stLocal1;
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
		cout << "error!" << endl;
	}
    FileTimeToSystemTime(&ftWrite, &stUTC1);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC1, &stLocal1);
	wsprintf(lpszLastWriteTime, "%02d/%02d/%02d %02d:%02d:%02d",
		stLocal1.wYear, stLocal1.wMonth, stLocal1.wDay,
		stLocal1.wHour, stLocal1.wMinute, stLocal1.wSecond);
    lastTime = ((char*)szWriteTime);
    //根据路径获取 文件大小 
    int size = GetFileSize(hFile, NULL);
    fileSize = to_string(size);
    std::cout << fileID << std::endl;
    std::cout << fileDir << std::endl;
    std::cout << fileName << std::endl;
    std::cout << hashValue << std::endl;
    std::cout << joinTime << std::endl;
    std::cout << lastTime << std::endl;
    std::cout << fileSize << std::endl;
}

int main(int argc, char const *argv[])
{
    fileInfoToDB("1", "E:\\VSCODE\\C++\\SM3Hash\\HashDB\\fileInfo.txt", "333");
    return 0;
}
