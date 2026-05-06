#include "cppwf/server/logger.hpp"

#include <fstream>
#include <iostream>

namespace cppwf {
namespace server {

void log(const std::string& msg, const std::string& filename) {
    if (filename.empty()) {
        // 파일명이 없으면 터미널(표준 출력)에 출력
        std::cout << msg << "\n";
    } else {
        // 파일명이 있으면 해당 파일에 이어쓰기
        std::ofstream ofs(filename, std::ios_base::app);
        if (!ofs.good()) {
            std::cerr << "[logger] 파일 열기 실패: " << filename << "\n";
            return;
        }
        ofs << msg << "\n";
    }
}

} // namespace server
} // namespace cppwf
