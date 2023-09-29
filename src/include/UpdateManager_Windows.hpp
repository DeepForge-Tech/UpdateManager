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
#include <Windows.h>
#pragma comment(lib, "urlmon.lib")
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../DatabaseConnect.cpp"
#include <map>
#include "json/json.h"
#include <fstream>
#include "zipper/unzipper.h"
#include "Logger.cpp"

using namespace std;
using namespace DB;
using namespace Json;
using namespace zipper;
using namespace Logger;

namespace Windows
{   
    // int type
    int result;
    // init class
    MainLogger logger(true, "logs/DeepForgeToolset.log");
    Database database;
    Value AppInformation;
    // string type
    string Architecture;
    string Answer;
    const string OrganizationFolder = "C:\\ProgramData\\DeepForge";
    const string ApplicationFolder = "C:\\ProgramData\\DeepForge\\DeepForge-Toolset";
    const string TempFolder = ApplicationFolder + "\\Temp";
    const string DB_URL = "https://github.com/DeepForge-Technology/DeepForge-Toolset/releases/download/InstallerUtils/Versions.db";
    std::filesystem::path ProjectDir = std::filesystem::current_path().generic_string();
    string DB_PATH = TempFolder + "\\Versions.db";
    string NameVersionTable = "WindowsVersions";

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
            ImportAppInformation();
            if (filesystem::exists(DB_PATH) == false)
            {
                MakeDirectory(TempFolder);
                Download(DB_URL,TempFolder);
            }
            database.open(&DB_PATH);
        }
        void InstallLatestRelease(string version);
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
            catch (exception error)
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

        void ImportAppInformation()
        {
            try
            {
                ifstream f("./AppInformation.json");
                // File open check
                if (f.is_open())
                {
                    // Dictionary entry with translation
                    f >> AppInformation;
                    f.close();
                }
            }
            catch (exception &error)
            {
                // Error output
                cout << error.what() << endl;
                logger.Error("❌ Error of import AppInformation.json");
            }
        }

        /* The 'MakeDirectory' function is used to create a directory (folder) in the file system.*/
        void MakeDirectory(string dir)
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
                        filesystem::create_directory(fullPath);
                    }
                }
                else
                {
                    fullPath = currentDir + "\\";
                }
                dir.erase(0, pos + delimiter.length());
            }
            fullPath = fullPath + "\\" + dir;
            if (filesystem::exists(fullPath) == false)
            {
                filesystem::create_directory(fullPath);
            }
        }
        /*The 'UnpackArchive' function takes two parameters: 'path_from' and 'path_to'.*/
        int UnpackArchive(string path_from, string path_to)
        {
            Unzipper unzipper(path_from);
            unzipper.extract(path_to);
            unzipper.close();
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