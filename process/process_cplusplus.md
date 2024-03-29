## C/C++로 프로세스 다루기

### 프로세스 만들기

가장 먼저 아래와 같은 간단한 소스 코드를 봅시다.

```c
#include <stdio.h>

int main()
{
    printf("hello, os\n");

    return 0;
}
```

이 코드는 단순히 ‘hello, os’라는 문자열을 출력하는 코드입니다. 이 소스 코드를 실행하면 ‘hello, os’라는 문자열을 출력하는 프로세스가 생성됩니다. ‘hello, os’를 출력하는 이 프로세스 또한 PID를 가지고 있겠죠? 한 번 확인해 볼까요? 아래와 같이 코드를 입력해 보세요.

```c
01 #include <stdio.h>
02 #include <unistd.h>
03 
04 int main()
05 {
06    printf("hello, os\n");
07    printf("my pid is %d", getpid());
08    return 0;
09 }
```

실행 결과

```
hello, os
my pid is 53389
```

앞선 코드에 02줄과 07줄을 추가했습니다. 02줄은 07줄의 getpid()를 사용하기 위해 넣은 코드입니다. C/C++에서는 `getpid()`로 프로세스의 PID를 확인할 수 있습니다. 소스 코드를 실행하면 ‘my pid is PID값’ 형태로 출력됩니다.

PID값은 운영체제가 그때그때 부여하는 값이기 때문에 여러분이 실제로 실행했을 때는 위와 다른 PID값이 출력될 수 있습니다. 그리고, 여러 번 반복적으로 실행했을 때 다른 PID 값이 출력될 수 있습니다. 이번에는 자식 프로세스를 생성해 볼까요? 아래와 같이 작성해 보세요.

```c
01 #include <stdio.h>
02 #include <unistd.h>
03 
04 int main()
05 {
07    printf("parent pid is %d\n", getpid());
08
09    if (fork() == 0) {
11        printf("child pid is %d\n", getpid());
12    }
13
14    return 0;
15 }
```

> 실행 결과

```
parent pid is 53559
child pid is 53560
```

> 출력되는 PID 값은 다를 수 있습니다.

`fork()`를 통해 자식 프로세스를 생성했습니다. 참고로 `fork()`는 자식 프로세스를 생성하고, `fork()`의 결과로 반환되는 값이 0인 프로세스는 자식 프로세스, `fork()`의 결과로 반환되는 값이 `자식프로세스의 `PID 인 프로세스는 부모 프로세스입니다. 그렇기에 09줄에서 fork의 결과가 0일 경우 해당 프로세스를 자식 프로세스로 간주할 수 있는 것이지요.



### 동일한 작업을 하는 프로세스 만들기

위 예시에서 중요한 점은 fork가 된 순간 동일한 코드를 실행하는 자식 프로세스가 생성되었다는 점입니다. 아래 예시를 볼까요?

위 예시 코드에서 14줄을 추가한 코드입니다.

```c
01 #include <stdio.h>
02 #include <unistd.h>
03 
04 int main()
05 {
07    printf("parent pid is %d\n", getpid());
08
09    if (fork() == 0) {
11        printf("child pid is %d\n", getpid());
12    }
13
14  printf("executed!\n");
15
16    return 0;
17 }
```

> 실행 결과

```
parent pid is 53796
executed!
child pid is 53797
executed!
```

'executed!' 를 출력하는 14줄이 두 번 실행되었음을 알 수 있습니다. 

이를 간단히 응용하면 아래와 같이 동일한 작업(foo 함수 실행)을 수행토록 자식 프로세스를 생성할 수도 있겠죠?

```c
#include <stdio.h>
#include <unistd.h>

void foo() {
  printf("execute foo\n");
}

int main()
{
   if (fork() == 0) {
      printf("child pid is %d\n", getpid());
      foo();
   }
   else {
     printf("parent pid is %d\n", getpid());
     foo();
   }

   return 0;
}
```

> 실행 결과

```
parent pid is 59558
execute foo
child pid is 59559
execute foo
```

그리고 자식 프로세스는 얼마든지 또 다른 자식 프로세스를 생성할 수도 있고, 부모 프로세스 또한 또다른 자식 프로세스를 생성할 수 있습니다.

```c
01 #include <stdio.h>
02 #include <unistd.h>
03 
04 void foo() {
05   printf("execute foo\n");
06 }
07
08 int main()
09 {
10    if (fork() == 0) {
11      if (fork() == 0) {
12         // 11줄에서 fork된 또 다른 child 프로세스
13         printf("child of child pid is %d\n", getpid());
14         foo();
15       }
16       else {
17         // 10줄에서 fork된 child 프로세스
18         printf("child pid is %d\n", getpid());
19         foo();
20       }
21    }
22    else {
23      if(fork() == 0) {
24  				// 23줄에서 fork된 child 프로세스
25        printf("child pid is %d\n", getpid());
26        foo();
27      }
28      else {
29  			// parent 프로세스
30        printf("parent pid is %d\n", getpid());
31        foo();
32      }
33    }
34 
35    return 0;
36 }
```

> 실행 결과

```
parent pid is 59632
execute foo
child pid is 59634
execute foo
child pid is 59633
execute foo
child of child pid is 59635
execute foo
```



### 각기 다른 작업을 하는 프로세스 생성하기

위에서는 모든 프로세스들이 execute foo 라는 문자열을 출력하는 동일한 작업(foo 함수 실행)을 수행했습니다.

이를 조금만 응용해보면 아래와 같이 각기 다른 작업을 하는 프로세스를 생성할 수 있겠죠?

```c
01 #include <stdio.h>
02 #include <unistd.h>
03
04 void foo() {
05   printf("execute foo\n");
06 }
07
08 void bar() {
09   printf("execute bar\n");
10 }
11
12 void baz() {
13   printf("execute baz\n");
14 }
15 
16 int main()
17 {
18    if (fork() == 0) {
19       if (fork() == 0) {
20         foo();
21       }
22       else {
23         bar();
24       }
25    }
26    else {
27      baz();
28    }
29 
30    return 0;
31 }
```

> 실행 결과

```
execute baz
execute bar
execute foo
```

위 프로그램이 워드 프로세서 프로그램이었다고 가정해 보세요. 그리고 foo 함수를 사용자로부터 입력받는 기능, bar 함수를 사용자가 입력한 값을 화면에 출력하는 기능, baz 함수를 사용자가 입력한 값의 맞춤법을 검사하는 기능이었다고 가정해 보세요. 프로세스를 이해하고 능수능란하게 다룰 수 있다면 각기 다른 작업을 동시에 수행하는 소스 코드를 작성할 수 있겠죠?
