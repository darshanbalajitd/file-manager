#include<iostream>
#include<string>
#include<filesystem>
#include<cstdlib>
#include "../include/file_ops.h"
namespace fs= std::filesystem;
int main(){
    fs:: path desktopPath =fs::path(std::getenv("USERPROFILE"));
    std::cout<<"trying to access folders...";
    std::cout << "USERPROFILE: " << std::getenv("USERPROFILE") << "\n";
    if(!fs::exists(desktopPath)){
        std::cerr<<"error no desktop path found\n";
        return 1;
    }
    fs::path currentPath=desktopPath;
    std::cout<<"######################################  \033[34mCLI FILE MANAGER\033[0m  ######################################"<<std::endl;
    std::cout<<"\033[33mcommands are as follow\n\033[32m*ls-\033[36mlists all directories\n\033[32m*cd<folder>-\033[36mopens folder\n\033[32m*back-\033[36mgo back to parent folder\n";
    std::cout<<"\033[32mdelete-\033[36mdeletes file\n";
    std::cout<<"\033[32m*open-\033[36mopens file>>  example-open report.pdf   -> opens in default viewer\n\t                   open report.txt notepad -> opens with Notepad\n\t                   open main.cpp code      -> opens in VS Code\n";
    std::cout<<"\033[32m*sort \033[36m<size/date>\n\033[32m*copy \033[36m<source> <destination>\n\033[32m*exit-\033[36mexit from file manager\033[0m";
    std::string command;
    while(true){
        std::cout<<"\n["<<currentPath<<"]>\033[35m";
        std::getline(std::cin,command);
        std::cout<<"\033[0m";
        if(command=="exit") break;
        else if (command=="ls")
        {
            listDirectory(currentPath);
        }else if(command=="back"){
            if(currentPath.has_parent_path()){
                currentPath=currentPath.parent_path();
            }else std::cout<<"already at root directory";
        }
        else if(command.rfind("cd",0)==0){
            std::string folder=command.substr(3);
            fs::path newPath=currentPath/folder;

            if(fs::exists(newPath)&& fs::is_directory(newPath)) currentPath=newPath;
            else std::cout<<"no directory found";

        }else if (command.rfind("open", 0) == 0) {
        std::istringstream iss(command);
        std::string cmd, fileName, appName;
        iss >> cmd >> fileName >> appName; 

        if (fileName.empty()) {
        std::cout << "\033[36mUsage: open <filename> [app]\033[0m\n";
        } else {
        fs::path filePath = currentPath / fileName;
        previewFile(filePath, appName); 
        }
        }else if(command.rfind("delete",0)==0){
            deleteFile(currentPath/command.substr(7));
        }else if (command.rfind("copy ", 0) == 0) {
        std::istringstream iss(command);
        std::string cmd, src, dst;
        iss >> cmd >> src >> dst;
        if (src.empty() || dst.empty()) {
        std::cout << "\033[36mUsage: copy <source> <destination>\033[0m\n";
            } else {
        copyFile(currentPath / src, currentPath / dst);
        }
        }
        else if (command.rfind("sort ", 0) == 0)
            listSorted(currentPath, command.substr(5));
        else if (command.rfind("search ", 0) == 0)
            searchFile(currentPath, command.substr(7));
        else std::cout<<"\033[31munknown command\033[0m";
        }
    std::cout<<"\033[31mexiting file manager\033[0m";
    return 0;
}