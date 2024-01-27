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
// Checking the name of the operating system and importing the necessary libraries for this system
#if defined(__linux__)
#include "UpdateManager_Linux.hpp"
using namespace Linux;
#elif __APPLE__
#include "UpdateManager_macOS.hpp"
using namespace macOS;
#elif _WIN32
#include "UpdateManager_Windows.hpp"
using namespace Windows;
#endif

Update App;

void Update::InstallLatestRelease(string name,string AppVersionTable,string version)
{
    string NewApplication_Url;
    string filename;
    string ArchivePath;
    string ArchiveName;
    string file_path;
    string NewApplicationFolder = OrganizationFolder + "/" + name;
    MakeDirectory(TempFolder);
    MakeDirectory(NewApplicationFolder);
    Download(DB_URL, TempFolder);
    NewApplication_Url = database.GetApplicationURL(AppVersionTable, "stable", Architecture == "amd64" ? "Url" : "Url_arm64", Architecture, version);
    Download(NewApplication_Url, TempFolder);
    // Scan NewApplicationFolder and directorins in this folder,if name of dir not == "Temp"
    for (const auto &entry : filesystem::directory_iterator(NewApplicationFolder))
    {
        filesystem::remove_all(entry.path());
    }
    ArchiveName = (NewApplication_Url.substr(NewApplication_Url.find_last_of("/")));
    ArchivePath = TempFolder + "/" + ArchiveName.replace(ArchiveName.find("/"), 1, "");
    UnpackArchive(ArchivePath, NewApplicationFolder);
    file_path = NewApplicationFolder + "/" + name;
    if (filesystem::exists(file_path) == false)
        CreateSymlink(name, file_path);
}

void Update::CheckNewVersion()
{
    try
    {
        string pathFile = ApplicationFolder + "/AppInformation.db";
        Database AppInformationDB;
        AppInformationDB.open(&pathFile);
        map<string,string> AllApplications = AppInformationDB.GetAllValuesFromDB("Applications","NameTable");
        for(const auto &element : AllApplications)
        {
            cout << element.first << endl;
            string currentVersion = AppInformationDB.GetValueFromDB("Applications",element.first,"Version");

            // string currentVersion = AppInformation["DeepForgeToolset_Version"].asString();
            string LatestVersion = database.GetLatestVersion(element.second, "stable", "Version", Architecture);
            cout << currentVersion << endl;
            cout << LatestVersion << endl;
            if (LatestVersion != "" && currentVersion != "" && stof(LatestVersion) > stof(currentVersion))
                InstallLatestRelease(element.first,element.second,LatestVersion);
        }
    }
    catch (exception& error)
    {
        logger.SendError(Architecture, "Empty", OS_NAME, "CheckNewVersion", error.what());
    }
}

int main()
{
    App.CheckNewVersion();
    return 0;
}