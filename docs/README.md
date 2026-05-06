# CppWF 폴더 구조 및 역할 명세

이 문서는 CppWF 프로젝트의 디렉터리 및 주요 파일들의 역할과 목적을 명세합니다.

## 전체 프로젝트 구조

```
CppWF/
├── include/                  # 라이브러리 공개 헤더 파일 모음
│   └── cppwf/                # 네임스페이스 및 프로젝트 최상위 디렉터리
│       ├── http/             # HTTP 통신 관련 클래스 및 구조체 (Request, Response 등)
│       ├── router/           # URL 라우팅 및 핸들러 처리 관련 클래스
│       ├── server/           # HTTPS 서버 구동 및 로거, 연결 관리 등 메인 서버 로직
│       └── ssl/              # OpenSSL 래핑 및 SSL 컨텍스트/연결 관리
├── src/                      # 헤더에 대응되는 구현(소스) 파일 모음
│   ├── http/                 # http 모듈 구현 파일
│   ├── router/               # router 모듈 구현 파일
│   ├── server/               # server 모듈 구현 파일
│   └── ssl/                  # ssl 모듈 구현 파일
├── tests/                    # 단위 테스트(Unit Tests) 및 통합 테스트 코드
├── examples/                 # 사용 예제 및 데모 코드
├── docs/                     # 프로젝트 문서화 자료 (이 파일 등)
├── cmake/                    # CMake 모듈 (Custom Find 모듈 등 필요한 경우 사용)
├── CMakeLists.txt            # CMake 빌드 설정 파일
├── .gitignore                # Git 버전 관리에서 제외할 파일 목록
├── LICENSE                   # 프로젝트 라이선스 (MIT)
├── NOTICE                    # 서드파티 저작권 및 공지사항 (OpenSSL)
├── THIRD_PARTY_LICENSES.md   # 서드파티 라이선스 전문 및 고지 사항
└── README.md                 # 프로젝트 개요 및 빌드, 기본 설명서
```

---

## 각 모듈 및 패키지의 역할

### 1. `include/cppwf/` 및 `src/` (코어 라이브러리)

프로젝트의 핵심 라이브러리 코드는 `include/` (선언부)와 `src/` (구현부)로 나뉘어 있습니다. 
사용자는 `include/cppwf/...` 경로를 통해 필요한 헤더를 포함할 수 있습니다.

#### `http` 하위 모듈
HTTP 프로토콜의 메세지를 표현하고 파싱 및 구성하는 역할을 합니다.
* **`request.hpp/cpp`**: 클라이언트로부터 수신한 HTTP 요청을 분석하여 메서드(Method), 경로(Path), 헤더(Headers), 바디(Body) 데이터 등을 저장하는 구조체/클래스입니다.
* **`response.hpp/cpp`**: 클라이언트에게 전송할 HTTP 응답을 구성합니다. 상태 코드(Status Code), 응답 헤더, 응답 본문 등을 설정하는 기능을 제공합니다.

#### `ssl` 하위 모듈
OpenSSL의 복잡한 C API를 C++ 객체지향 스타일로 캡슐화합니다.
* **`ssl_context.hpp/cpp`**: `SSL_CTX` 구조체를 관리하며 인증서(Certificate)와 개인키(Private Key) 로딩 및 초기 설정을 담당합니다.
* **`ssl_connection.hpp/cpp`**: 단일 클라이언트와의 TLS/SSL 연결 세션을 관리합니다. `SSL` 객체를 생성하고 핸드셰이크(Handshake), 암호화된 데이터 송수신(`read`/`write`), 자원 해제 등의 기능을 담당합니다.

#### `router` 하위 모듈
요청된 URL 경로 및 HTTP 메서드에 따라 적절한 처리 함수(핸들러)로 연결해주는 역할을 합니다.
* **`router.hpp/cpp`**: GET, POST 등의 HTTP 메서드와 엔드포인트 URL 경로에 따라 사용자가 등록한 콜백 함수를 매핑하고(`get()`, `post()` 등), 클라이언트의 요청이 들어왔을 때 해당하는 콜백 함수로 디스패치(`dispatch()`) 합니다.

#### `server` 하위 모듈
실제 서버 소켓을 열고 클라이언트의 연결을 수락 및 처리하며, 프로젝트의 진입점 역할을 합니다.
* **`server.hpp/cpp`**: 메인 `Server` 클래스로, TCP 소켓 생성/바인드/리슨 과정을 처리하고 클라이언트의 접속을 받습니다(`accept`). 이후 `SslConnection`을 통해 핸드셰이크를 진행하고 `Router`에 요청을 위임하여 최종 응답을 반환합니다.
* **`logger.hpp/cpp`**: 서버 내부 동작이나 오류를 로깅하는 헬퍼 함수입니다. `Server::set_log_file()` 설정을 통해 터미널(표준 출력) 또는 특정 파일로 로그의 출력 방향을 제어할 수 있게 도와줍니다.

### 2. 프로젝트 외곽 요소

* **`examples/`**: CppWF 프레임워크를 어떻게 사용하는지 보여주는 샘플 코드가 들어있습니다. (예: `hello_world.cpp` - 기본 HTTPS 서버 구동 및 엔드포인트 생성 예제)
* **`tests/`**: 프레임워크의 각 모듈이 정상적으로 동작하는지 검증하기 위한 테스트 코드가 위치하는 디렉터리입니다.
* **`docs/`**: 현재 읽고 계신 파일 구조 명세서와 같은 개발/사용 문서가 들어갑니다.
* **`cmake/` 및 `CMakeLists.txt`**: C++ 빌드 시스템 도구인 CMake 관련 설정 파일들입니다. 소스 파일을 엮어서 정적 라이브러리(`libcppwf.a`)를 빌드하고, OpenSSL 라이브러리를 동적/정적 링킹하며, 테스트 및 예제 실행 파일을 빌드하는 룰이 정의되어 있습니다.
* **`LICENSE` / `NOTICE` / `THIRD_PARTY_LICENSES.md`**: 본 프로젝트는 MIT 라이선스를 따르며, 코어 기능으로 OpenSSL(Apache 2.0)을 활용하므로 서드파티 관련 저작권 고지 및 라이선스 조건을 명시하는 파일들입니다.
