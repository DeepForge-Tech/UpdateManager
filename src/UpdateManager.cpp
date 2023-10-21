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
// Checking the name of the operating system and importing the necessary libraries for this system
#if defined(__linux__)
#include "include/UpdateManager_Linux.hpp"
using namespace Linux;
#elif __APPLE__
#include "include/UpdateManager_macOS.hpp"
using namespace macOS;
#elif _WIN32
#include "include/UpdateManager_Windows.hpp"
using namespace Windows;
#endif

Update App;

void Update::InstallLatestRelease(string version)
{
    string NewApplication_Url;
    string name;
    string filename;
    string ArchivePath;
    string file_path;
    MakeDirectory(TempFolder);
    result = Download(DB_URL, TempFolder);
    switch (result)
    {
    case 200:
        NewApplication_Url = database.GetApplicationURL(NameVersionTable, "stable\\latest",Architecture == "amd64" ? "Url" : "Url_arm64", Architecture, version);
        result = Download(NewApplication_Url, TempFolder);
        if (result == 200)
        {
            // Scan ApplicationFolder and directorins in this folder,if name of dir not == "Temp"
            for (const auto &entry : filesystem::directory_iterator(ApplicationFolder))
            {
                if (entry.path() != TempFolder)
                    filesystem::remove_all(entry.path());
            }
            name = (NewApplication_Url.substr(NewApplication_Url.find_last_of("/")));
            ArchivePath = TempFolder + "/" + name.replace(name.find("/"), 1, "");
            UnpackArchive(ArchivePath,ApplicationFolder);
            file_path = ApplicationFolder + "\\DeepForgeToolset";
            if (filesystem::exists(file_path) == false) CreateSymlink("DeepForgeToolset", file_path);
        }
        break;
    case 502:
        throw domain_error("Error downloading archive - 502");
    }
}

void Update::CheckNewVersion()
{
    try
    {
        string currentVersion = AppInformation["DeepForgeToolset_Version"].asString();
        string LatestVersion = database.GetLatestVersion(NameVersionTable,"stable\\latest","Version", Architecture);
        if (LatestVersion != "" && currentVersion != "" && stof(LatestVersion) > stof(currentVersion)) InstallLatestRelease(LatestVersion);
    }
    catch (exception &error)
    {
        logger.SendError(Architecture,"Empty",OS_NAME,"CheckNewVersion",error.what());
    }
}

int main()
{
    App.CheckNewVersion();
    system("pause");
    return 0;
}