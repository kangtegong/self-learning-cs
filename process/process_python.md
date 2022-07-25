## Python으로 프로세스 다루기

### 프로세스 만들기

가장 먼저 아래와 같은 간단한 소스 코드를 봅시다.

```py
print('hello, os')
```

이 코드는 단순히 ‘hello, os’라는 문자열을 출력하는 코드입니다. 이 소스 코드를 실행하면 ‘hello, os’라는 문자열을 출력하는 프로세스가 생성됩니다. ‘hello, os’를 출력하는 이 프로세스 또한 PID를 가지고 있겠죠? 한 번 확인해 볼까요? 아래와 같이 코드를 입력해 보세요.

```python
01 import os
02 
03 print('hello os')
04 print('my pid is', os.getpid())
```

실행 결과

```
hello, os
my pid is 17924
```

앞서 `print('hello os')` 코드에 01줄과 04줄을 추가했습니다. 01줄은 04줄의 os.getpid()를 사용하기 위해 넣은 코드입니다. 파이썬에서는 `os.getpid()`로 프로세스의 PID를 확인할 수 있습니다. 소스 코드를 실행하면 ‘my pid is PID값’ 형태로 출력됩니다.

PID값은 운영체제가 그때그때 부여하는 값이기 때문에 여러분이 실제로 실행했을 때는 위와 다른 PID값이 출력될 수 있습니다. 그리고 여러 번 반복적으로 실행했을 때 다른 PID 값이 출력될 수 있습니다. 이번에는 자식 프로세스를 생성해 볼까요? 아래와 같이 작성해 보세요.

```python
01 from multiprocessing import Process
02 import os
03 
04 def foo():
05     print('child process', os.getpid())
06     print('my parent is', os.getppid())
07   
08 if __name__ == '__main__':
09     print('parent process', os.getpid())
10     child = Process(target=foo).start()
```

> 실행 결과

```
parent process 6488
child process 27724
my parent is 6488
```

코드가 조금 복잡한가요? 조금만 주의를 기울인다면 전혀 복잡하지 않습니다. 위 소스 코드는 08줄(if __name__ == '__main__':) 아래 두 줄을 실행하는 코드입니다. 

```
print('parent process', os.getpid())
child = Process(target=foo).start()
```

09줄 코드는 ‘parent process + PID값’ 형태로 출력하는 코드이고, 10줄 코드는 foo라는 함수를 실행하는 자식 프로세스를 만들고 실행하는 코드입니다. 그리고 foo 함수는 아래 두 줄(05줄과 06줄)의 코드를 실행합니다. 

```
print('child process', os.getpid())
print('my parent is', os.getppid())
```

05줄 코드는 ‘parent process + PID값’ 형태로 출력하는 코드이고, 06줄 코드는 ‘my parent is + 부모 프로세스의 PID 값(PPID)’ 형태로 출력하는 코드입니다. 

> 출력되는 PID 값은 다를 수 있습니다.

여기서 주목할 점은 파이썬 코드 내에서 PID가 27724인 자식 프로세스를 생성했다는 점입니다. 자식 프로세스가 부모 프로세스의 PID(PPID)를 출력했을 때 자식 프로세스를 생성한 프로세스의 PID(위 예시의 경우 6488)가 나온다는 것을 알 수 있습니다.

### 동일한 작업을 하는 프로세스 만들기

자식 프로세스는 얼마든지 여러 개 만들 수 있습니다. 아래 예시를 볼까요?

```python
01 from multiprocessing import Process
02 import os
03 
04 def foo():
05     print('hello, os')
06   
07 if __name__ == '__main__':
08     child1 = Process(target=foo).start()
09     child2 = Process(target=foo).start()
10     child3 = Process(target=foo).start()
```

> 실행 결과

```
hello, os
hello, os
hello, os
```

위 코드를 실행하면 07줄(`if __name__ == '__main__':`) 아래 세 줄(08~10줄)이 실행됩니다. 이 세 줄 코드는 모두 foo라는 동일한 함수를 실행하는 프로세스입니다. 그리고 foo은 'hello, os'를 출력하는 함수입니다.

```python
child1 = Process(target=foo).start()
child2 = Process(target=foo).start()
child3 = Process(target=foo).start()
```

즉, 위와 같이 동일한 작업(foo 함수 실행)을 수행하는 여러 자식 프로세스를 생성할 수도 있습니다. 

### 각기 다른 작업을 하는 프로세스 생성하기

마지막으로 각기 다른 작업을 하는 자식 프로세스들도 생성해 봅시다. 아래 코드를 볼까요? child1은 foo를, child2는 bar를, child3은 baz 함수를 실행합니다. 

```python
01 from multiprocessing import Process
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
14     child1 = Process(target=foo).start()
15     child2 = Process(target=bar).start()
16     child3 = Process(target=baz).start()
```

> 실행 결과

```
This is foo
This is bar
This is baz
```

위 파이썬 프로그램이 워드 프로세서 프로그램이었다고 가정해 보세요. 그리고 foo 함수를 사용자로부터 입력받는 기능, bar 함수를 사용자가 입력한 값을 화면에 출력하는 기능, baz 함수를 사용자가 입력한 값의 맞춤법을 검사하는 기능이었다고 가정해 보세요. 프로세스를 이해하고 능수능란하게 다룰 수 있다면 각기 다른 작업을 동시에 수행하는 소스 코드를 작성할 수 있겠죠?