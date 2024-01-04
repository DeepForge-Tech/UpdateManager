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
#include <conio.h>
#include <winsock2.h>
#include <Windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "../DatabaseConnect.hpp"
#include <map>
#include <fstream>
#include <cctype>
#include "Logger.cpp"
#include <fstream>
#include <urlmon.h>
#include "json/json.h"
#pragma comment(lib, "urlmon.lib")

/* The `#define` directive is used to define a constant value in C++. In this case, `OS_NAME` is defined as "Windows" and `NameVersionTable` is defined as "WindowsVersions". These constants can be used throughout the code to represent the operating system name and the name of the version table in the database. */
#define OS_NAME "Windows"

using namespace std;
using namespace DB;
using namespace Json;

namespace Windows
{   
    // int type
    int result;
    // init class
    Logger logger("./logs/DeepForgeToolset.log", "10mb");
    Database database;
    Value AppInformation;
    // string type
    string Architecture;
    string Answer;
    const string OrganizationFolder = "C:\\ProgramData\\DeepForge";
    const string ApplicationFolder = "C:\\ProgramData\\DeepForge\\UpdateManager";
    const string TempFolder = ApplicationFolder + "\\Temp";
    const string DB_URL = "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db";
    filesystem::path ProjectDir = filesystem::current_path().generic_string();
    string DB_PATH = TempFolder + "\\Versions.db";

    class WriteData : public IBindStatusCallback
    {
    public:
        HRESULT __stdcall QueryInterface(const IID &, void **)
        {
            return E_NOINTERFACE;
        }
        ULONG STDMETHODCALLTYPE AddRef(void)
        {
            return 1;
        }
        ULONG STDMETHODCALLTYPE Release(void)
        {
            return 1;
        }
        HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, IBinding *pib)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE GetPriority(LONG *pnPriority)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved)
        {
            return S_OK;
        }
        virtual HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, LPCWSTR szError)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
        {
            return E_NOTIMPL;
        }
        virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID riid, IUnknown *punk)
        {
            return E_NOTIMPL;
        }

        virtual HRESULT __stdcall OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
        {
            return S_OK;
        }
    };
    // Main class
    class Update
    {
    public:
        Update()
        {
            GetArchitectureOS();
            MakeDirectory(TempFolder);
            Download(DB_URL,TempFolder);
            database.open(&DB_PATH);
        }
        void InstallLatestRelease(string name,string AppVersionTable,string version);
        void CheckNewVersion();

    private:
        int Download(string url, string dir)
        {
            try
            {
                WriteData writer;
                string name = (url.substr(url.find_last_of("/")));
                string filename = dir + "/" + name.replace(name.find("/"), 1, "");
                HRESULT Download = URLDownloadToFile(NULL, url.c_str(), filename.c_str(), 0, static_cast<IBindStatusCallback *>(&writer));
                return 200;
            }
            catch (exception& error)
            {
                return 502;
            }
        }

        void CreateSymlink(string nameSymlink, string filePath)
        {
            nameSymlink = nameSymlink + ".exe";
            filePath = filePath + ".exe";
            char *UserFolder = getenv("USERPROFILE");
            string symlinkPath = string(UserFolder) + "\\Desktop\\" + nameSymlink;
            if (filesystem::exists(symlinkPath) == false)
                CreateHardLinkA(symlinkPath.c_str(), filePath.c_str(), NULL);
        }

        /* The 'MakeDirectory' function is used to create a directory (folder) in the file system.*/
        void MakeDirectory(string dir)
        {
            try
            {
                string currentDir;
                string fullPath = "";
                string delimiter = "\\";
                size_t pos = 0;
                while ((pos = dir.find(delimiter)) != string::npos)
                {
                    currentDir = dir.substr(0, pos);
                    if (fullPath != "")
                    {
                        fullPath = fullPath + "\\" + currentDir;
                        if (filesystem::exists(fullPath) == false)
                        {
                            CreateDirectoryA(fullPath.c_str(), NULL);
                        }
                    }
                    else
                    {
                        fullPath = currentDir + "\\";
                    }
                    dir.erase(0, pos + delimiter.length());
                }
                if (fullPath != "")
                {
                    fullPath = fullPath + "\\" + dir;
                }
                else
                {
                    fullPath = dir + "\\";
                }
                if (filesystem::exists(fullPath) == false)
                {
                    CreateDirectoryA(fullPath.c_str(), NULL);
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
            string unpack_command = "tar -xf" + path_from + " --directory " + path_to;
            system(unpack_command.c_str());
        }
        // Method for getting architecture of OS
        void GetArchitectureOS()
        {
            #if defined(_M_AMD64)
                Architecture = "amd64";
            #elif _M_ARM64
                Architecture = "arm64";
            #endif
        }
    };
}