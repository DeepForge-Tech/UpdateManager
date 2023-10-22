#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <exception>
// #include <filesystem>
#include <cmath>
#include <concepts>
#include <map>
#include "json/json.h"
#if defined(__linux__) || __APPLE__
#include "Client_UNIX.hpp"
using namespace UNIX;
#elif _WIN32
#include "Client_POSIX.hpp"
using namespace POSIX;
#endif

using namespace std;

class Logger
{
public:
    bool toWrite = false;
    long MAX_SIZE;
    string LogPath;
    const string DebugStr = "☑️[DEBUG]";
    const string ErrorStr = "❌[ERROR]";
    const string InfoStr = "🆗[INFO]";
    const string WarningStr = "⚠️[WARNING]";
    const string SuccessStr = "✅[SUCCESS]";
    map<string,int> LabelSize = {
        {"byte",1},{"kb",1024},{"mb",1048576},{"gb",1073741824}
    };
    Client UDP_CLIENT;

    Logger(const char *PathFile = nullptr,const char *MaxSize = nullptr)
    {
        #if defined(_WIN32)
            /* The line `system("chcp 65001");` is a system command that sets the console code page to UTF-8 encoding. This is necessary to display and handle Unicode characters correctly in the console output. */
            system("chcp 65001");
        #endif
        if (PathFile != nullptr)
        {
            if (MaxSize != nullptr)
            {
                convertSize(MaxSize);
            }
            toWrite = true;
        }
    }
    void Debug(string LogText);
    void Error(string LogText);
    void Info(string LogText);
    void Warning(string LogText);
    void Success(string LogText);
    void SendError(string Architecture,string Channel,string OS_NAME,string FunctionName,string LogText);
    void WriteFile(string filename, string value)
    {
        if (toWrite == true)
        {
            fstream file(filename, ios::in | ios::binary | ios::out);
            file.seekg(0, ios::end);
            long file_size = file.tellg();
            if (file_size > MAX_SIZE)
            {
                file.close();
                ofstream new_file(filename,ios::out | ios::binary | ios::trunc);
                new_file.write(value.c_str(),sizeof(string));
                new_file.close();
            }
            else 
            {
                file.write(value.c_str(),sizeof(string));
                // file << value << endl;
                file.close();
            }
            cout << file_size << endl;
        }
    }

private:
    /* The `convertSize` function is used to convert a given size string into bytes. */
    void convertSize(string size)
    {
        size = to_lower(size);
        // Converting to bytes
        for (const auto &element:LabelSize)
        {
            if (size.find(element.first) != string::npos)
            {
                size = replaceAll(size,element.first,"");
                /* The line `MAX_SIZE = stol(size) * element.second;` is calculating the maximum size in bytes based on the given size string and the corresponding label size. */
                MAX_SIZE = stol(size) * element.second;
                break;
            }
        }
    }
    string replaceAll(string str, const string &from, const string &to)
    {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }
    /* The `GetTime()` function is used to get the current time and format it as a string. */
    string GetTime()
    {
        /* The line `time_t now = time(0);` is used to get the current time in seconds since the Unix epoch (January 1, 1970). The `time(0)` function returns the current time as a `time_t` object, which is then assigned to the variable `now`. */
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        /* The line `tstruct = *localtime(&now);` is assigning the value of the `tm` structure returned by the `localtime` function to the variable `tstruct`. */
        tstruct = *localtime(&now);
        /* The `strftime` function is used to format the time and date values into a string. In this case, `strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct)` is formatting the current time and date into the string `buf` using the specified format. */
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        return buf;
    }
    // Function of make string to lower
    string to_lower(string sentence)
    {
        string new_sentence = "";
        /* The code snippet `for (int i = 0; i < sentence.length(); i++)` is a for loop that iterates over each character in the string `sentence`. */
        for (int i = 0; i < sentence.length(); i++)
        {
            char ch = sentence[i];
            ch = tolower(ch);
            new_sentence += ch;
        }
        return new_sentence;
    }
};