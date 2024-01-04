/*  The MIT License (MIT)
    ============================================================================

    ██████╗ ███████╗███████╗██████╗ ███████╗ ██████╗ ██████╗  ██████╗ ███████╗
    ██╔══██╗██╔════╝██╔════╝██╔══██╗██╔════╝██╔═══██╗██╔══██╗██╔════╝ ██╔════╝
    ██║  ██║█████╗  █████╗  ██████╔╝█████╗  ██║   ██║██████╔╝██║  ███╗█████╗
    ██║  ██║██╔══╝  ██╔══╝  ██╔═══╝ ██╔══╝  ██║   ██║██╔══██╗██║   ██║██╔══╝
    ██████╔╝███████╗███████╗██║     ██║     ╚██████╔╝██║  ██║╚██████╔╝███████╗
    ╚═════╝ ╚══════╝╚══════╝╚═╝     ╚═╝      ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝

    ████████╗ ██████╗  ██████╗ ██╗     ███████╗███████╗████████╗
    ╚══██╔══╝██╔═══██╗██╔═══██╗██║     ██╔════╝██╔════╝╚══██╔══╝
       ██║   ██║   ██║██║   ██║██║     ███████╗█████╗     ██║
       ██║   ██║   ██║██║   ██║██║     ╚════██║██╔══╝     ██║
       ██║   ╚██████╔╝╚██████╔╝███████╗███████║███████╗   ██║
       ╚═╝    ╚═════╝  ╚═════╝ ╚══════╝╚══════╝╚══════╝   ╚═╝

    ============================================================================
    Copyright (c) 2023 DeepForge Technology
    ============================================================================
    Organization: DeepForge Technology
    ============================================================================
    Author: Blackflame576
    ============================================================================
    Created: 4 Juny 2023
    ============================================================================
*/
#include <filesystem>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../DatabaseConnect.hpp"
#include <map>
#include "json/json.h"
#include <fstream>
#include <curl/curl.h>
#include "Logger.cpp"
#include <string>
#include <vector>
#include "zip/zip.h"
#include <cstring>

/* The `#define` directive is used to define a constant value in C++. In this case, `OS_NAME` is defined as the string "Linux", and `NameVersionTable` is defined as the string "LinuxVersions". These constants can be used throughout the code to represent the operating system name and the name of the version table for Linux. */
#define OS_NAME "Linux"

using namespace std;
using namespace DB;

namespace Linux
{
    // Function for write data from curl
    size_t WriteData(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        size_t WriteProcess = fwrite(ptr, size, nmemb, stream);
        return WriteProcess;
    }
    // int type
    int result;
    // string type
    string Architecture;
    string Answer;
    const string OrganizationFolder = "/usr/bin/DeepForge";
    const string ApplicationFolder = OrganizationFolder + "/UpdateManager";
    const string TempFolder = ApplicationFolder + "/Temp";
    const string DB_URL = "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db";
    string ProjectDir = std::filesystem::current_path().generic_string();
    string DB_PATH = TempFolder + "/Versions.db";
    const string LogPath = ProjectDir + "/logs/DeepForgeToolset.log";
    // init classes
    Logger logger(LogPath.c_str(), "10mb");
    Database database;
    Json::Value AppInformation;
    // Main class
    class Update
    {
    public:
        Update()
        {
            GetArchitectureOS();
            MakeDirectory(TempFolder);
            Download(DB_URL, TempFolder);
            database.open(&DB_PATH);
        }
        void InstallLatestRelease(string name,string AppVersionTable,string version);
        void CheckNewVersion();

    private:
        int Download(string url, string dir)
        {
            string name = (url.substr(url.find_last_of("/")));
            string filename = dir + "/" + name.replace(name.find("/"), 1, "");
            FILE *file = fopen(filename.c_str(), "wb");
            CURL *curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, true);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteData);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            CURLcode response = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(file);
            return 200;
        }

        void CreateSymlink(string nameSymlink, string filePath)
        {
            char *UserFolder = getenv("HOME");
            string symlinkPath = string(UserFolder) + "/Desktop/" + nameSymlink;
            if (filesystem::exists(symlinkPath) == false)
                // CreateHardLinkA(symlinkPath.c_str(), filePath.c_str(), NULL);
                filesystem::create_hard_link(filePath, symlinkPath);
        }

        /*The `MakeDirectory` function is responsible for creating a directory (folder) in the file system.*/
        void MakeDirectory(string dir)
        {
            try
            {
                string currentDir;
                string fullPath = "";
                string delimiter = "/";
                size_t pos = 0;
                while ((pos = dir.find(delimiter)) != string::npos)
                {
                    currentDir = dir.substr(0, pos);
                    if (fullPath != "")
                    {
                        fullPath = fullPath + "/" + currentDir;
                        if (filesystem::exists(fullPath) == false)
                        {
                            filesystem::create_directory(fullPath);
                        }
                    }
                    else
                    {
                        fullPath = "/" + currentDir;
                    }
                    dir.erase(0, pos + delimiter.length());
                }
                fullPath = fullPath + "/" + dir;
                if (filesystem::exists(fullPath) == false)
                {
                    filesystem::create_directory(fullPath);
                }
            }
            catch (exception &error)
            {
                logger.SendError(Architecture, "Empty", OS_NAME, "MakeDirectory", error.what());
            }
        }
        /*The 'UnpackArchive' function takes two parameters: 'path_from' and 'path_to'.*/
        void UnpackArchive(string path_from, string path_to)
        {
            try
            {
                MakeDirectory(path_to);
                int err;
                struct zip *zip = zip_open(path_from.c_str(), ZIP_RDONLY, &err);
                if (zip == nullptr)
                {
                    string ErrorText = "Cannot open zip archive: " + path_from;
                    throw runtime_error(ErrorText);
                }

                int num_entries = zip_get_num_entries(zip, 0);
                for (int i = 0; i < num_entries; ++i)
                {
                    zip_stat_t zip_stat;
                    zip_stat_init(&zip_stat);
                    int err = zip_stat_index(zip, i, 0, &zip_stat);
                    if (err != 0)
                    {
                        zip_close(zip);
                    }

                    string file_name = zip_stat.name;
                    string full_path = path_to + "/" + file_name;
                    filesystem::path file_dir(full_path);
                    MakeDirectory(file_dir.remove_filename().string());

                    struct zip_file *zip_file = zip_fopen_index(zip, i, 0);
                    if (zip_file == nullptr)
                    {
                        string ErrorText = "Cannot open file in zip archive: " + file_name;
                        zip_close(zip);
                        throw runtime_error(ErrorText);
                    }

                    if (filesystem::is_directory(full_path) == false)
                    {
                        ofstream out_file(full_path, ios::binary);
                        if (!out_file.is_open())
                        {
                            string ErrorText = "Cannot open file for writing: " + full_path;
                            zip_fclose(zip_file);
                            zip_close(zip);
                            throw runtime_error(ErrorText);
                        }
                        vector<char> buffer(zip_stat.size);
                        zip_fread(zip_file, buffer.data(), buffer.size());
                        out_file.write(buffer.data(), buffer.size());
                        out_file.close();
                    }

                    zip_fclose(zip_file);
                }

                zip_close(zip);
            }
            catch (exception &error)
            {
                logger.SendError(Architecture, "Empty", OS_NAME, "UnpackArchive()", error.what());
                cerr << "❌ " << error.what() << endl;
            }
        }
        // Method for getting architecture of OS
        void GetArchitectureOS()
        {
#if defined(__x86_64__)
            Architecture = "amd64";
#elif __arm__
            Architecture = "arm64";
#endif
        }
    };
}