#pragma once

#include <string>

namespace cppwf {
namespace server {

/**
 * @brief 로그 출력 함수
 *
 * @param msg      출력할 로그 메시지
 * @param filename 저장할 파일 이름.
 *                 비어 있으면 표준 출력(터미널)에 출력하고,
 *                 값이 있으면 해당 파일에 이어쓰기(append)합니다.
 */
void log(const std::string& msg, const std::string& filename = "");

} // namespace server
} // namespace cppwf
