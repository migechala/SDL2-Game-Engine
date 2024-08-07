//
// Created by migecha on 7/17/21.
//

#pragma once

#include <fstream>
#include <string>

#include "imgui.h"

struct LogApp {
  ImGuiTextBuffer buf;
  ImGuiTextFilter filter;
  ImVector<int> lineOffsets;
  bool autoScroll;

  LogApp();

  void clear();

  void addLog(std::string fmt, ...);

  void draw(const char* title, bool* p_open = NULL);
};
enum LOG_VALUES { INFO, WARNING, ERROR };
enum LOG_LEVEL { PRIORITY, MEDIUM, LOW };

namespace Logger {
static std::ofstream file;
static LogApp logApp;
static int ll = LOG_LEVEL::LOW;
void resetLogger();
void writeToFile(const std::string& write);
void writeToWindow(const std::string& write);
void deleteLastLog();
void setLogLevel(LOG_LEVEL level);
void showLogApp(bool* open_app);
void log(const std::string& msg, LOG_VALUES severity, LOG_LEVEL level,
         const char* filename, int line);
};  // namespace Logger

#define LOG_INFO(msg, level) \
  Logger::log(msg, LOG_VALUES::INFO, level, __FILE__, __LINE__);
#define LOG_WARN(msg, level) \
  Logger::log(msg, LOG_VALUES::WARNING, level, __FILE__, __LINE__);
#define LOG_ERR(msg) \
  Logger::log(msg, LOG_VALUES::ERROR, LOG_LEVEL::PRIORITY, __FILE__, __LINE__);
