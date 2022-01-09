#include<Windows.h>
#include<iostream>
#include<string>
#include <fstream>
#include <io.h>
using namespace std;

BOOL GetFileTime(HANDLE hFile, LPSTR lpszLastWriteTime)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC1, stLocal1;

	// 获取 FileTime
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
		cout << "error!" << endl;
		return FALSE;
	}
	//转换: FileTime --> LocalTime
	FileTimeToSystemTime(&ftWrite, &stUTC1);

	SystemTimeToTzSpecificLocalTime(NULL, &stUTC1, &stLocal1);

	//获取最后修改时间时间
	wsprintf(lpszLastWriteTime, "%02d/%02d/%02d %02d:%02d",
		stLocal1.wYear, stLocal1.wMonth, stLocal1.wDay,
		stLocal1.wHour, stLocal1.wMinute);
	return TRUE;
}

// std::string TCHAR2STRING(TCHAR *STR)
// {
// 	int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);
// 	char* chRtn = new char[iLen*sizeof(char)];
// 	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
// 	std::string str(chRtn);
// 	delete chRtn;
// 	return str;
// }

string GetFilename(string fullpathname) {
    string flag = "\\";
    string::size_type position = fullpathname.rfind(flag);
    return fullpathname.substr(++position);    
}


int main() {
	HANDLE hFile;
	TCHAR szWriteTime[30];
    string fileFullName = "E:\\VSCODE\\C++\\SM3Hash\\HashDB\\fileInfo.txt";
	hFile = CreateFile(fileFullName.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    string fileName(GetFilename(fileFullName));
    cout << fileName << endl;
    
    GetFileTime(hFile, szWriteTime);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "error!" << endl;
		return -1;
	}
	int size = GetFileSize(hFile, NULL);
    string lastWriteTime((char*)szWriteTime);
    string fileSize = to_string(size);
    
	//cout << "Create Time:" << szWriteTime << strlen((char*)szWriteTime) << endl;
    cout << "Create Time:" << lastWriteTime << endl;
	cout << "File Size:" << fileSize <<" Bytes"<< endl;
	CloseHandle(hFile);
	return 0;
}
