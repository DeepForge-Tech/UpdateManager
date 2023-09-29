#ifndef LOGGER_HPP_
#define LOGGER_HPP_
// Импортирование библиотек
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime> 
#include <exception>
#include <filesystem>

// Переменные
bool toFile = true;
ofstream file;
string NameLogFile = "logs/Log.log";
ifstream get_file_size;
char limit_value;
const string DebugStr = "☑️[DEBUG]";
const string ErrorStr = "❌[ERROR]";
const string InfoStr = "🆗[INFO]";
const string WarningStr = "⚠️[WARNING]";
const string SuccessStr = "✅[SUCCESS]";
// string LoggerDir = std::filesystem::current_path();
#endif