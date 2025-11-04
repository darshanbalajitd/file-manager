#ifndef FILE_OPS_H
#define FILE_OPS_H
#include <filesystem>
#include<string>
namespace fs=std::filesystem;
void listDirectory(const fs::path& path);
void previewFile(const fs::path& filePath,const std::string& customApp);
void deleteFile(const fs::path& filePath);
void copyFile(const fs::path& source,const fs::path& destination);
void listSorted(const fs::path& path,const std::string& criteria);
void searchFile(const fs::path& path,const std::string& keyword);
#endif
