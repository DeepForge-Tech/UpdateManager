// Импортирование заголовочного файла
#include "Logger.hpp"

using namespace std;

namespace Logger {
    class MainLogger
    {
        public:
            string GetTime() {
                try {
                    time_t     now = time(0);
                    struct tm  tstruct;
                    char       buf[80];
                    tstruct = *localtime(&now);
                    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
                    return buf;
                }
                catch (exception& error) {
                    cout << error.what() << endl;
                }
            }

            void Debug(const char *sentence) {
                string time = GetTime();
                string FileText = "\x1B[36m" + DebugStr + "::[" + time + "]" + ":::" + sentence + "\033[0m";
                string text = DebugStr + "::[" + time + "]" + ":::" + sentence;
                if (toFile == true) {
                    WriteFile(NameLogFile,text);
                }
                else {
                    cout << text <<endl;
                }
            }

            void Info(const char *sentence) {
                string time = GetTime();
                string text = InfoStr + "::[" + time + "]" + ":::" + sentence;
                if (toFile == true) {
                    WriteFile(NameLogFile,text);
                }
                else {
                    cout << text <<endl;
                }
            }

            void Error(const char *sentence) {
                string time = GetTime();
                string FileText = "\x1B[31m" + ErrorStr + "::[" + time + "]" + ":::" + sentence + "\033[0m";
                string text = ErrorStr + "::[" + time + "]" + ":::" + sentence;
                if (toFile == true) {
                    WriteFile(NameLogFile,text);
                }
                else {
                    cout << text << endl;
                }
            }
            
            void Warning(const char *sentence) {
                string time = GetTime();
                string FileText = "\x1B[33m" + WarningStr + "::[" + time + "]" + ":::" + sentence + "\033[0m";
                string text = WarningStr + "::[" + time + "]" + ":::" + sentence;
                if (toFile == true) {
                    WriteFile(NameLogFile,text);
                }
                else {
                    cout << text << endl;
                }
            }

            void Success(const char *sentence) {
                string time = GetTime();
                string FileText = "\x1B[32m" + SuccessStr + "::[" + time + "]" + ":::" + sentence + "\033[0m";
                string text = SuccessStr + "::[" + time + "]" + ":::" + sentence;
                if (toFile == true) {
                    WriteFile(NameLogFile,text);
                }
                else {
                    cout << text << endl;
                }
            }

            // void PrintRED(auto sentence) {
            //     cout << "\x1B[31m" << sentence << "\033[0m" << endl;
            // }

            // void PrintGREEN(auto sentence) {
            //     cout << "\x1B[32m" << sentence << "\033[0m" << endl;
            // }

            // void PrintYELLOW(auto sentence) {
            //     cout << "\x1B[33m" << sentence << "\033[0m" << endl;
            // }

            MainLogger(bool log_to_file,string filename) {
                toFile = log_to_file;
                NameLogFile = filename;
            }
            
            ~MainLogger() {}
        
    private:
            // Function for write logs to file
            void WriteFile(string filename,string sentence) {
                try {
                    if (filesystem::exists("logs") == false) {
                        filesystem::create_directory("logs");
                    }
                    file.open(filename,ofstream::app);
                    // get_file_size.open(filename, ios::in | ios::binary);
                    // get_file_size.seekg(0,ios::end);
                    // get_file_size.close();
                    file << sentence + "\n";
                    file.close();
                }
                catch(exception& error) {
                    cout << error.what() << endl;
                }
            }
    };
}