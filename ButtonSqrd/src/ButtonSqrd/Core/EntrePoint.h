#pragma once

#ifdef BTN_PLATFORM_WINDOWS

#include <iostream>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include<filesystem>

extern BtnSqd::Application* BtnSqd::CreateApp();

namespace fs = std::filesystem;

std::string GetCTime() {
    auto now = std::chrono::system_clock::now();
    auto timeTimeT = std::chrono::system_clock::to_time_t(now);
    auto timeLocal = *std::localtime(&timeTimeT);

    std::ostringstream timeStream;
    timeStream << std::put_time(&timeLocal, "%Y-%m-%d_%H-%M-%S");
    return timeStream.str();
}

void LogExceptionToFile(const std::exception& errorInstance, const std::string& contextMessage) {
    std::string directoryPath = "./Crashes";
    std::string path = directoryPath + "/crashlog_" + GetCTime() + ".txt";

    try {
        if (!fs::exists(directoryPath)) {
            fs::create_directories(directoryPath);
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to create directory: " << e.what() << '\n';
        return;
    }

    std::ofstream errorLog(path, std::ios::app);
    if (errorLog.is_open()) {
        errorLog << "[" << GetCTime() << "] [ERROR] " << contextMessage << " -> " << errorInstance.what() << "\n";
    }
    else {
        std::cerr << "Failed to open crashlog at path: " << path << "\n";
    }
}

int main(int argc, char** argv) {
	auto app = BtnSqd::CreateApp();
	try {
		app->RunApp();
		delete app;
	}
	catch(const std::exception& e){
        LogExceptionToFile(e,"");
	}
}
#endif