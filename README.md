# CppWF
C++ 기반 웹 서버를 구축하기 위한 라이브러리

## 프로젝트 구조

```
CppWF/
├── include/                  # 라이브러리 공개 헤더 파일 모음
│   └── cppwf/                # 네임스페이스 및 프로젝트 최상위 디렉터리
│       ├── http/             # HTTP 통신 관련 클래스 및 구조체 (Request, Response 등)
│       │   ├── request.hpp   # HTTP 요청 클래스
│       │   └── response.hpp  # HTTP 응답 클래스
│       ├── router/           # URL 라우팅 및 핸들러 처리 관련 클래스
│       │   └── router.hpp    # URL 라우터
│       ├── server/           # HTTPS 서버 구동 및 로거, 연결 관리 등 메인 서버 로직
│       │   ├── logger.hpp    # 로그 출력 함수 선언
│       │   └── server.hpp    # HTTPS 서버 클래스
│       └── ssl/              # OpenSSL 래핑 및 SSL 컨텍스트/연결 관리
│           ├── ssl_connection.hpp # TLS/SSL 연결 세션 관리
│           └── ssl_context.hpp # OpenSSL 컨텍스트 래퍼
├── src/                      # 헤더에 대응되는 구현(소스) 파일 모음
│   ├── http/                 # http 모듈 구현 파일
│   │   └── response.cpp
│   ├── router/               # router 모듈 구현 파일
│   │   └── router.cpp
│   ├── server/               # server 모듈 구현 파일
│   │   ├── logger.cpp
│   │   └── server.cpp
│   └── ssl/                  # ssl 모듈 구현 파일
│       ├── ssl_connection.cpp
│       └── ssl_context.cpp
├── tests/                    # 단위 테스트(Unit Tests) 및 통합 테스트 코드
├── examples/                 # 사용 예제 및 데모 코드
│   └── hello_world.cpp
├── docs/                     # 프로젝트 문서화 자료 (폴더 구조 명세서 등)
│   └── README.md
├── cmake/                    # CMake 모듈
├── CMakeLists.txt            # 빌드 설정 파일
├── .gitignore                # Git 버전 관리에서 제외할 파일 목록
├── LICENSE                   # 프로젝트 라이선스 (MIT)
├── NOTICE                    # 서드파티 저작권 및 공지사항 (OpenSSL)
├── THIRD_PARTY_LICENSES.md   # 서드파티 라이선스 전문 및 고지 사항
└── README.md                 # 프로젝트 개요 및 빌드, 기본 설명서
```

## 빌드 방법

```bash
mkdir build && cd build
cmake ..
make
```



### OpenSSL

CppWF는 TLS/SSL 기능을 위해 **OpenSSL 3.x** 이상을 필요로 합니다.

> [!NOTE]
> **macOS** 및 일부 Linux 배포판에는 OpenSSL이 기본으로 설치되어 있을 수 있습니다.
> 설치 전에 먼저 아래 명령어로 설치 여부와 버전을 확인하세요.

#### 설치 확인

```bash
openssl version
```

출력 예시:
```
OpenSSL 3.4.1 11 Feb 2025 (Library: OpenSSL 3.4.1 11 Feb 2025)
```

버전이 `3.x` 이상이라면 별도 설치 없이 사용할 수 있습니다.

#### 운영체제별 설치 방법

**macOS** (Homebrew)
```bash
brew install openssl@3
```

> [!IMPORTANT]
> macOS의 경우 시스템 기본 OpenSSL이 오래된 버전(LibreSSL)일 수 있습니다.
> `openssl version` 출력이 `LibreSSL`로 표시된다면 Homebrew로 OpenSSL 3.x를 별도 설치하는 것을 권장합니다.

**Ubuntu / Debian**
```bash
sudo apt update && sudo apt install libssl-dev
```

**Fedora / RHEL / CentOS**
```bash
sudo dnf install openssl-devel
```

**Windows** (vcpkg)
```bash
vcpkg install openssl
```

---

## 라이선스

이 프로젝트는 **MIT 라이선스** 하에 배포됩니다 — 자세한 내용은 [LICENSE](LICENSE) 파일을 참고하세요.

### 서드파티 라이브러리

| 라이브러리 | 라이선스 | 버전 |
|-----------|---------|------|
| [OpenSSL](https://www.openssl.org) | Apache 2.0 | 3.x |

이 프로젝트는 OpenSSL 프로젝트가 OpenSSL 툴킷(https://www.openssl.org/) 사용을 위해 개발한 소프트웨어를 포함하고 있습니다.

서드파티 라이선스 전문 및 저작권 고지는 [NOTICE](NOTICE) 및 [THIRD_PARTY_LICENSES.md](THIRD_PARTY_LICENSES.md) 파일을 참고하세요.
