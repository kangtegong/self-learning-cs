## dtruss가 실행이 안될 경우

아래와 같은 메세지와 함께 dtruss가 실행되지 않을 경우가 있습니다.

```
dtrace: system integrity protection is on, some features will not be available
dtrace: failed to initialize dtrace: DTrace requires additional privileges
```

복구 모드로 macOS 재부팅 후 터미널에서 아래와 같이 명령어를 입력한 뒤 재부팅해주세요.

```
$ csrutil disable
```

그럼 정상적으로 dtruss 명령어가 동작하는 것을 볼 수 있습니다.

참고로 아래의 메세지는 보안상의 이유로 dtrace(dtruss)실행이 의도적으로 안되게 보안 기능이 설정되어 있는 의미이고

```
dtrace: system integrity protection is on, some features will not be available
dtrace: failed to initialize dtrace: DTrace requires additional privileges
```

아래의 명령어는 해당 기능을 끄겠다는 의미입니다. 

```
$ csrutil disable
```

그렇기에 dtruss 실습이 끝났다면 다시 복구 모드에서 아래 명령어로 해당 기능을 켜주는 것을 권장합니다.

```
$ csrutil enable
```
