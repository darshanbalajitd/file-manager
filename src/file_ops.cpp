#include "../include/file_ops.h"
#include<iostream>
#include<iomanip>
#include<filesystem>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<algorithm>
namespace fs=std::filesystem;
void listDirectory(const fs::path& path){
    std::cout<<"\n contents of:"<<path<<"\n\n";
    for(const auto& entry: fs::directory_iterator(path)){
        std::string name=entry.path().filename().string();
        if(fs::is_directory(entry.status())){
            std::cout<<"[DIR]\033[33m"<<name<<"\n\033[0m";
        }else if(fs::is_regular_file(entry.status())){
            auto size=fs::file_size(entry.path());
            std::cout<<"[FILE]\033[33m"<<std::setw(20)<<std::left<<name<<"size:"<<size<<"bytes\n\033[0m";
        }else{
            std::cout<<"[OTHER]\033[33m"<<name<<"\n\033[0m";
        }
    }
}
void previewFile(const fs::path& filePath, const std::string& customApp = "") {
    try {
        if (!fs::exists(filePath)) {
            std::cout << "\033[31mFile does not exist.\033[0m\n";
            return;
        }
        if (!customApp.empty()) {
#ifdef _WIN32
            std::string command = customApp + " \"" + filePath.string() + "\"";
            system(command.c_str());
#else
            std::string command = customApp + " \"" + filePath.string() + "\" &";
            system(command.c_str());
#endif
            return;
        }
#ifdef _WIN32
        std::string command = "start \"\" \"" + filePath.string() + "\"";
#else
        std::string command = "xdg-open \"" + filePath.string() + "\"";
#endif
        system(command.c_str());

    } catch (const std::exception& e) {
        std::cout << "\033[31mError opening file: \033[0m" << e.what() << "\n";
    }
}
void deleteFile(const fs::path& filePath){
    if(fs::remove(filePath)){
        std::cout<<"deleted"<<filePath.filename().string()<<"\n";
    }
    else std::cout<<"\033[31mfile cannot be deleted\033[0m\n";
}
void copyFile(const fs::path& source, const fs::path& destination) {
    try {
        if (!fs::exists(source) || !fs::is_regular_file(source)) {
            std::cout << "\033[31mSource file does not exist or is not a regular file:\033[0m " << source << "\n";
            return;
        }
        fs::path finalDest = destination;
        if (fs::exists(destination) && fs::is_directory(destination)) {
            finalDest = destination / source.filename();
        } else {
            fs::path parent = finalDest.parent_path();
            if (!parent.empty() && !fs::exists(parent)) {
                std::error_code ec;
                fs::create_directories(parent, ec);
                if (ec) {
                    std::cout << "\033[31mWarning: could not create directories for destination:\033[0m " << ec.message() << "\n";
                }
            }
        }
        std::ifstream in(source, std::ios::binary);
        if (!in) {
            std::cout << "\033[31mFailed to open source for reading: \033[0m" << source << "\n";
            return;
        }
        std::ofstream out(finalDest, std::ios::binary | std::ios::trunc);
        if (!out) {
            std::cout << "\033[31mFailed to open destination for writing: \033[0m" << finalDest << "\n";
            return;
        }
        constexpr std::size_t BUFFER_SIZE = 1 << 20; 
        std::vector<char> buffer(BUFFER_SIZE);
        while (in) {
            in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
            std::streamsize bytes = in.gcount();
            if (bytes > 0) out.write(buffer.data(), bytes);
        }
        out.flush();
        std::cout << "\033[36mCopied " << source.filename().string() << " to \033[0m" << finalDest.string() << "\n";

    } catch (const std::exception& e) {
        std::cout << "\033[31mCopy failed: \033[0m" << e.what() << "\n";
    }
}
void listSorted(const fs::path& path, const std::string& criteria) {
    try {
        std::vector<fs::directory_entry> entries{fs::directory_iterator(path), fs::directory_iterator()};
        if (criteria == "size") {
            std::sort(entries.begin(), entries.end(),
                [](const fs::directory_entry& a, const fs::directory_entry& b) {
                    auto sizeA = a.is_regular_file() ? fs::file_size(a) : 0;
                    auto sizeB = b.is_regular_file() ? fs::file_size(b) : 0;
                    return sizeA < sizeB;
                });
        } else if (criteria == "date") {
            std::sort(entries.begin(), entries.end(),
                [](const fs::directory_entry& a, const fs::directory_entry& b) {
                    return fs::last_write_time(a) < fs::last_write_time(b);
                });
        } else {
            std::cout << "\033[31mInvalid criteria! Use 'size' or 'date'.\033[0m\n";
            return;
        }
        for (const auto& e : entries) {
            std::cout << e.path().filename().string();
            if (e.is_directory())
                std::cout << "/";
            std::cout << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "\033[31mSort error: \033[0m" << e.what() << "\n";
    }
}

void searchFile(const fs::path& path, const std::string& keyword) {
    try {
        std::string lowerKeyword = keyword;
        std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            std::string name = entry.path().filename().string();
            std::string lowerName = name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            if (lowerName.find(lowerKeyword) != std::string::npos) {
                std::cout << entry.path().string() << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cout << "\033[31mSearch error: \033[0m" << e.what() << "\n";
    }
}
