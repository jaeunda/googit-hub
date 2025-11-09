# `googit-hub`
홍 박사님이 git을 사용하지 말라고 하셔서... 직접 만든 Google Drive 백업 커맨드
## Install
```
$ git clone https://github.com/jaeunda/googit-hub.git
$ cd googit-hub && make install
$ rm -rf googit-hub
```
## Run
```
~$ mkdir project_dir && cd project_dir
~/project_dir$ googit init 1 20232372 다
~/project_dir$ ls -a
. .. .googit xv6-public
~/project_dir$ googit push
```
## Command
### `googit init`
```
$ googit init
Usage: googit init <project_num> <student_id> <class_num>
$ googit init 1 20232372 다
```

### `googit push`
```
$ googit push
$ googit push -m "push message"
```
- `<timestamp>_<message>`: 구글 드라이브에 저장

### `googit run`
```
$ googit run
$ googit run --no-clean
```
- `xv6-public` 원본에 수정사항을 덮어씌워 임시 테스트 환경 생성
- `make` && `make qemu` 실행

### `googit zip`
```
$ googit zip
```
- 강의계획서에 제시된 파일 이름 규칙에 의거하여 압축 파일 생성 및 백업


```
$ googit zip --revert
	Version reverted to 1.
```
- `--revert`: 버전 번호 되돌리기

### `googit test`
```
$ googit test
$ googit test -f "filename"
$ googit test --no-clean -f "filename"
```
- 최신 버전의 제출 파일의 압축을 해제하여 `googit run` 실행
- `-f` 옵션 사용 시 제출 파일 지정 가능

### (X) `googit restore`
```
$ googit restore

Select a backup to restore:
[1] 2025-09-01_23-15-00: Implemented basic syscall structure
[2] 2025-09-01_21-55-03: Added user program for testing
...
Enter a number to restore (or 'q' to quit): 2

WARNING: This will overwrite your local 'output_dir' with the backup from 2025-09-01_21-55-03. Are you sure? (y/n): y 

Restoring from 2025-09-01_21-55-03... 
Restore complete.
```
- 선택한 시점의 소스코드로 복원
- 못합니다
