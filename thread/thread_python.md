## Python으로 스레드 다루기

### 코드로 스레드 만들기

우선 아래의 코드를 실행해 보겠습니다. ‘process id + PID’ 형태로 출력하는 코드입니다.

```python
import os

print('process id', os.getpid())
```

위 소스 코드를 실행하면 프로세스가 생성됩니다. 

그럼 이 프로세스에 새로운 스레드를 만들어 보겠습니다. 아래와 같이 입력한 뒤 실행해 보세요.

````python
01 import threading
02 import os
03 
04 def foo():
05     print('thread id', threading.get_native_id())
06     print('process id', os.getpid())
07 
08 if __name__ == '__main__':
09     print('process id', os.getpid())
10     thread = threading.Thread(target=foo).start()
````

> 실행 결과

```
process id 26264
thread id 2248
process id 26264
```

스레드를 만드는 코드는 열번째 줄입니다. foo라는 함수를 실행하는 스레드를 만들고 실행하라는 의미이지요. 

```python
thread = threading.Thread(target=foo).start()
```

foo 함수는 ‘thread id + 스레드 ID’ 형태로 출력하는 첫 번째 코드와 ‘process id + PID’ 형태로 출력하는 두 번째 코드로 이루어져 있습니다. 

```py
print('thread id', threading.get_native_id())
print('process id', os.getpid())
```

> 참고로 스레드 ID를 출력하는 파이썬 코드는 threading.get_native_id()입니다.

위 소스 코드를 직접 실행하면 프로세스 ID와 스레드 ID 숫자는 다를 수 있습니다. 

여기서 중요한 점은 프로세스 내에서 ID가 2248인 스레드를 만들어냈다는 점입니다. 

### 동일한 작업을 하는 스레드 생성하기

위 소스 코드를 아래와 같이 조금 변형해 보겠습니다. 

```py
01 import threading
02 import os
03 
04 def foo():
05     print('thread id', threading.get_native_id())
06     print('process id', os.getpid())
07 
08 if __name__ == '__main__':
09     print('process id', os.getpid())
10     thread1 = threading.Thread(target=foo).start()
11     thread2 = threading.Thread(target=foo).start()
12     thread3 = threading.Thread(target=foo).start()
```

> 실행 결과

```
process id 11912
thread id 20044
process id 11912
thread id 26056
process id 11912
thread id 21764
process id 11912
```

foo라는 함수를 실행하는 세 개의 스레드(thread1, thread2, thread3)를 만들었습니다. 

여기서 여러분이 주목해야 할 점은 세 개의 스레드 모두 각기 다른 스레드지만, 이들은 모두 동일한 프로세스를 공유하고 있다는 점입니다. 

그래서 스레드 ID는 모두 다르게 출력되고, 프로세스 ID는 모두 동일하게 출력이 되는 것입니다.

### 각기 다른 작업을 하는 스레드 생성하기

```python
01 import threading
02 import os
03 
04 def foo():
05     print('This is foo')
06  
07 def bar():
08     print('This is bar')
09  
10 def baz():
11     print('This is baz')
12 
13 if __name__ == '__main__':
14     thread1 = threading.Thread(target=foo).start()
15     thread2 = threading.Thread(target=bar).start()
16     thread3 = threading.Thread(target=baz).start()
```

> 실행 결과

```
This is foo
This is bar
This is baz
```

이처럼 하나의 프로세스 내에 여러 개의 실행 흐름을 만들어낼 수 있습니다. 