# CppWF
C++을 위한 웹 프레임워크

## 의존성

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
