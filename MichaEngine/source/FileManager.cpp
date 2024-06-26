#include "FileManager.h"

#include <dirent.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "Logger.h"

FileManager *FileManager::instance = nullptr;

FileManager *FileManager::getInstance() {
  if (instance == nullptr) {
    instance = new FileManager();
  }
  return instance;
}

int FileManager::getFileCountInDirectory(const std::string &path) {
  DIR *dp;
  int i = 0;
  struct dirent *ep;
  dp = opendir(path.c_str());

  if (dp != nullptr) {
    while ((ep = readdir(dp))) i++;

    (void)closedir(dp);
  } else {
    perror("Couldn't open the directory");
    return -1;
  }

  return i;
}

std::vector<std::string> FileManager::getFiles(const std::string &path) {
  std::vector<std::string> files;
  struct dirent *entry;
  DIR *dir = opendir(path.c_str());

  if (dir == nullptr) {
    return files;
  }
  while ((entry = readdir(dir)) != nullptr) {
    files.push_back(entry->d_name);
  }
  closedir(dir);
  return files;
}

void FileManager::removeCharacters(std::string &str, char c) {
  str.erase(std::remove(str.begin(), str.end(), c), str.end());
}

std::string FileManager::getSettingsFromJson(std::string path, std::string tree,
                                             std::string child) {
  char START = '{', END = '}', NEXT = ',', VALUE = ':';
  std::vector<std::string> fileContents;
  std::string ret = ":(";

  std::ifstream json;
  json.open(path);
  std::string line;
  if (json.is_open()) {
    while (getline(json, line)) {
      // Write all content of json file to vector fileContents
      fileContents.push_back(line);
    }
  } else {
    LOG_ERR("Failed to open config file " + path);
  }
  json.close();

  bool continueLoops = true;
  for (int i = 0; i < fileContents.size() && continueLoops; ++i) {
    // Loop through each value in fileContents to find tree string
    std::string line = fileContents[i];
    if (line.find(tree) != std::string::npos) {
      int skip = i;
      // Get skip from index i to the first opening brace ( { )
      for (; fileContents[skip].find(START) == std::string::npos &&
             skip < fileContents.size();
           ++skip);

      // Get closest closing brace after skip ( } )
      for (int j = skip; fileContents[j].find(END) == std::string::npos &&
                         j < fileContents.size();
           ++j) {
        if (fileContents[j].find("\"" + child + "\"") != std::string::npos) {
          std::string line = fileContents[j];
          size_t pos = line.find(child);
          size_t Ipos = 1;

          for (; Ipos + pos < line.size() && line[Ipos + pos] != '"'; ++Ipos);

          line = line.substr(Ipos + pos + 2);
          // remove " and ,
          removeCharacters(line, '"');
          removeCharacters(line, ',');
          continueLoops = false;
          ret = line;
          break;
        }
      }
    }
  }
  return ret;
}

FileManager::~FileManager() { delete instance; }
