## C/C++로 스레드 다루기

### 코드로 스레드 만들기

우선 아래의 코드를 실행해 보겠습니다. ‘process id is PID’ 형태로 출력하는 코드입니다.

이는 [C/C++로 프로세스 다루기]에서 한 번 살펴본 적이 있었죠?

```c
01 #include <stdio.h>
02 #include <unistd.h>
03 
04 int main()
05 {
06    printf("process id is %d", getpid());
07    return 0;
08 }
```

> 실행 결과

```
process id is 60090
```

당연하게도 위 소스 코드를 실행하면 프로세스가 생성됩니다. 

그럼 이 프로세스에 새로운 스레드를 만들어 보겠습니다. 아래와 같이 입력한 뒤 실행해 보세요.

````c
 1 #include <stdio.h>
 2 #include <unistd.h>
 3 #include <pthread.h>
 4
 5 void * foo() {
 6   printf("process id is %d\n", getpid());
 7   return NULL;
 8 }
 9
10 int main()
11 {
12     pthread_t thread1;
13     pthread_create(&thread1, NULL, foo, NULL);
14     pthread_join(thread1, NULL);
15
16     return 0;
17 }
````

> 실행 결과

```
process id is 60090
```

프로세스 ID는 다를 수 있습니다. 

위 코드에서 thread를 만드는 코드는 12줄, 13줄, 14줄입니다. 

```
12     pthread_t thread1;														// thread1 이라는 이름으로 pthread_t 형 변수 선언
13     pthread_create(&thread1, NULL, foo, NULL); 	// thread1은 foo를 실행토록 thread 생성
14     pthread_join(thread1, NULL); 								// thread1 실행
```

>참고
>
>pthread_create : https://man7.org/linux/man-pages/man3/pthread_create.3.html
>
>pthread_join: https://man7.org/linux/man-pages/man3/pthread_join.3.html

`thread1` 스레드는 "process id is PID"를 출력하는 foo 함수를 실행합니다. 

스레드의 고유한 식별자를 확인하는 방법에는 다양한 방법이 있지만, 대표적인 방법 중 하나로 `pthread_self()`를 이용하는 방법이 있습니다.

06줄에서 thread_id 변수에 pthread_self() 의 결과를 반환받고, 이를 08줄에서 출력한 결과를 보세요.

```c
 01 #include <stdio.h>
 02 #include <unistd.h>
 03 #include <pthread.h>
 04
 05 void * foo() {
 06   long thread_id = (long int) pthread_self();
 07   printf("process id is %d\n", getpid());
 08   printf("this is thread %ld\n", thread_id);
 09   return NULL;
 10 }
 11
 12 int main()
 13 {
 14     pthread_t thread1;
 15     pthread_create(&thread1, NULL, foo, NULL);
 16     pthread_join(thread1, NULL);
 17
 18     return 0;
 19 }
```

> 실행 결과

```
process id is 60820
this is thread 6090387456
```

프로세스 ID와 스레드 ID 숫자는 다를 수 있습니다. 



### 동일한 작업을 하는 스레드 생성하기

이번에는 위 코드를 조금 변형하여 여러 개의 스레드를 만들어보겠습니다. 아래와 같이 작성해보세요. thread1, thread2, thread3을 만들고, 이 모두가 동일한 foo를 실행토록 하는 예제입니다. 

참고로, 아래 예제 코드에서는 코드를 최대한 단순한 형태로 보이기 위해 아래와 같이 작성하였지만, 배열과 반복문으로 main 함수를 구성해도 무방합니다.

```c
 01 #include <stdio.h>
 02 #include <unistd.h>
 03 #include <pthread.h>
 04
 05 void * foo() {
 06   long thread_id = (long int) pthread_self();
 07   printf("process id is %d\n", getpid());
 08   printf("this is thread %ld\n", thread_id);
 09   return NULL;
 10 }
 11
 12 int main()
 13 {
 14     pthread_t thread1;
 15     pthread_t thread2;
 16     pthread_t thread3;
 17
 18     pthread_create(&thread1, NULL, foo, NULL);
 19     pthread_create(&thread2, NULL, foo, NULL);
 20     pthread_create(&thread3, NULL, foo, NULL);
 21
 22     pthread_join(thread1, NULL);
 23     pthread_join(thread2, NULL);
 24     pthread_join(thread3, NULL);
 25
 26     return 0;
 27 }
```



> 실행 결과

```
process id is 60932
this is thread 6095646720
process id is 60932
this is thread 6096220160
process id is 60932
this is thread 6096793600
```

여기서 여러분이 주목해야 할 점은 세 개의 스레드 모두 각기 다른 스레드지만, 이들은 모두 동일한 프로세스를 공유하고 있다는 점입니다. 

그래서 스레드 ID는 모두 다르게 출력되고, 프로세스 ID는 모두 동일하게 출력이 되는 것이지요.



### 각기 다른 작업을 하는 스레드 생성하기

당연하게도 각기 다른 스레드는 다른 작업을 수행토록 코드를 짤 수도 있습니다. 아래와 같이 말이지요.

```c
 01 #include <stdio.h>
 02 #include <unistd.h>
 03 #include <pthread.h>
 04
 05 void * foo() {
 06   printf("foo executed\n");
 07   return NULL;
 08 }
 09
 10 void * bar() {
 11   printf("bar executed\n");
 12   return NULL;
 13 }
 14
 15 void * baz() {
 16   printf("baz executed\n");
 17   return NULL;
 18 }
 19
 20 int main()
 21 {
 22     pthread_t thread1;
 23     pthread_t thread2;
 24     pthread_t thread3;
 25
 26     pthread_create(&thread1, NULL, foo, NULL);
 27     pthread_create(&thread2, NULL, bar, NULL);
 28     pthread_create(&thread3, NULL, baz, NULL);
 29
 30     pthread_join(thread1, NULL);
 31     pthread_join(thread2, NULL);
 32     pthread_join(thread3, NULL);
 33
 34     return 0;
 35 }
```

> 실행 결과

```
foo executed
bar executed
baz executed
```

이처럼 하나의 프로세스 내에 여러 개의 실행 흐름을 만들어낼 수 있습니다. 