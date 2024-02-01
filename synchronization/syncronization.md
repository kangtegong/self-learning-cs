## 코드로 보는 동기화

### 뮤텍스 락

아래의 코드를 보세요. 아래의 코드는 스레드를 네 개(정확하게는 04줄의 `NUM_THREADS`만큼) 생성하고, 각각의 스레드가 `foo` 함수(08줄~14줄)를 실행하는 코드입니다. 아래의 코드가 전혀 이해가 안간다면 [이 문서](https://github.com/kangtegong/self-learning-cs/blob/main/thread/thread_cplusplus.md#cc%EB%A1%9C-%EC%8A%A4%EB%A0%88%EB%93%9C-%EB%8B%A4%EB%A3%A8%EA%B8%B0)를 먼저 읽고 오시길 권합니다.

`foo`함수는 1000번씩 반복하며 `shared`라는 변수를 1씩 증가시키는 로직이 포함되어 있습니다(10줄~12줄).

그리고 06줄에 선언되어 있는 `shared` 변수는 모든 스레드가 공통적으로 공유하는 공유 변수이지요.

```c
 01 #include <stdio.h>
 02 #include <pthread.h>
 03
 04 #define NUM_THREADS 4
 05
 06 int shared = 0;
 07
 08 void *foo()
 09 {
 10     for (int i = 0; i < 10000; ++i) {
 11         shared += 1;
 12     }
 13     return NULL;
 14 }
 15
 16 int main()
 17 {
 18     pthread_t threads[NUM_THREADS];
 19
 20     for (int i = 0; i < NUM_THREADS; ++i) {
 21         pthread_create(&threads[i], NULL, foo, NULL);
 22     }
 23
 24     for (int i = 0; i < NUM_THREADS; ++i) {
 25         pthread_join(threads[i], NULL);
 26     }
 27
 28     printf("final result is %d\n", shared);
 29
 30     return 0;
 31 }
```



위 코드를 실행하면 어떻게 될까요?

모든 스레드가 1씩 10000번 반복하여 `shared`를 증가시키니, 결과적으로 `shared`변수에는 40000이 저장될 것이라 기대할 수 있습니다.

하지만 이를 실행하면 전혀 엉뚱한 결과를 얻게 됩니다.

뿐만 아니라 반복하여 실행할 때마다 다른 결과를 얻게 되지요.

```
$ ./a.out				// 위 코드를 컴파일한 뒤 실행 파일 실행
final result is 18767
$ ./a.out				// 다시 실행
final result is 24226
```



이는 코드가 동기화되지 않았기에 생긴 문제입니다.

이를 해결하는 코드를 알아보기 전에, 우선 임계구역이 어디인지부터 알아보죠. 임계 구역이 어디일까요?

foo 함수, 즉 아래의 코드가 바로 임계구역입니다. 이 영역을 한 번에 하나의 스레드만 실행토록 하면 되겠지요.

```
for (int i = 0; i < 10000; ++i) {
    shared += 1;
}
```



그럼 코드를 통해 mutex를 알아봅시다.

간단합니다. 위 코드에서 05줄, 11줄, 15줄을 아래와 같이 추가하면 됩니다.

```c
 01 #include <stdio.h>
 02 #include <pthread.h>
 03
 04 #define NUM_THREADS 4
 05 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 06
 07 int shared = 0;
 08
 09 void *foo()
 10 {
 11     pthread_mutex_lock(&mutex);
 12     for (int i = 0; i < 10000; ++i) {
 13         shared += 1;
 14     }
 15     pthread_mutex_unlock(&mutex);
 16     return NULL;
 17 }
 18
 19 int main()
 20 {
 21     pthread_t threads[NUM_THREADS];
 22
 23     for (int i = 0; i < NUM_THREADS; ++i) {
 24         pthread_create(&threads[i], NULL, foo, NULL);
 25     }
 26
 27     for (int i = 0; i < NUM_THREADS; ++i) {
 28         pthread_join(threads[i], NULL);
 29     }
 30
 31     printf("final result is %d\n", shared);
 32
 33     return 0;
 34 }
```



우선 05줄은 mutex 라는 변수를 (비유하자면 mutex라는 이름의 자물쇠를) PTHREAD_MUTEX_INITIALIZER로 초기화하는 코드입니다.

mutex를 사용하기 전에 변수를 선언하고, 초기화하는 과정입니다. 

```
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```



그리고 나서 임계 구역 앞뒤로 mutex lock, unlock을 하면 됩니다.

`pthread_mutex_lock(&mutex);`가 바로 `mutex` 변수를 잠그는 코드, `pthread_mutex_unlock(&mutex);`이 `mutex` 변수의 잠금을 해제하는 코드입니다. 각 스레드는 임계구역을 진입하기 위해 mutex 변수를 확보해야 하고, 확보하지 못했다면 임계구역 앞에서 대기합니다. 임계구역을 실행하게 되면 mutex 변수로 설정된 mutex 잠금을 해제하게 되지요.

이제 위 코드를 실행해봅시다. 몇 번을 반복하여 실행해도 40000이 잘 나오는 것을 확인할 수 있습니다.

```
$ ./a.out				
final result is 40000
$ ./a.out				
final result is 40000
```



### 세마포

그럼 이번에는 세마포를 소스 코드로 확인해볼까요? 이번에는 언어를 바꿔서 파이썬의 세마포를 확인해보겠습니다. 

**여러 프로그래밍 언어가 책에서 설명한 동기화 도구를 제공한다**는 점을 보이기 위함이 이번 심화 자료의 목적이긴 하지만,

어떤 언어로 동기화 도구를 학습하든 크게 상관은 없습니다. 용례가 크게 다르질 않아서요 ^^;

세마포를 사용하지 않았을 때의 예제입니다. num 이라는 공유 변수를 1씩 100000번 증가시키는 스레드 두 개를 만들고 무작정 실행시켜보았습니다.

```python
  1 from threading import Thread
  2
  3 num = 0
  4
  5 def foo():
  6     for _ in range(100000):
  7         global num
  8         num += 1
  9
 10 if __name__ == '__main__':
 11     t1 = Thread(target=foo)
 12     t2 = Thread(target=foo)
 13
 14     t1.start()
 15     t2.start()
 16
 17     print(num)
```



> 실행 결과

```
$ python3 sem.py
134412
$ python3 sem.py
134689
$ python3 sem.py
139497
$ python3 sem.py
196654
```

위와 같은 결과는 상호 배제를 위한 동기화가 이루어지지 않았기 때문입니다.

이는 아래와 같이 세마포를 이용해 해결할 수 있습니다.

세마포를 사용할 수 있도록 첫번째 줄에서 import 해주고, 세마포를 `sem`이라는 이름으로 정의해줍니다. 

괄호 안에 들어가는 숫자 1은 공유 자원의 숫자입니다. 아무것도 기입하지 않을 시 1로 초기화되지만, 이해를 돕기 위해 1을 명시했습니다.

그리고 공유 자원에 접근하는 임계 영역 전후로, 즉 08줄과 11줄에서 세마포를 `acquire`하고 `release`해주면 됩니다.

세마포를 acquire하지 못한 스레드는 대기하고, release를 통해 깨어남을 통해 동기화가 이루어집니다.

```python
  1 from threading import Thread, Semaphore
  2
  3 num = 0
  4 sem = Semaphore(1)
  5
  6 def foo(sem):
  7     global num
  8     sem.acquire()
  9     for _ in range(100000):
 10         num += 1
 11     sem.release()
 12
 13 if __name__ == '__main__':
 14     t1 = Thread(target=foo, args=(sem,))
 15     t2 = Thread(target=foo, args=(sem,))
 16     t1.start()
 17     t2.start()
 18     t1.join()
 19     t2.join()
 20     print(num)
```



> 실행 결과

```
$ python3 sem.py
200000
$ python3 sem.py
200000
$ python3 sem.py
200000
$ python3 sem.py
200000
```





### 모니터

모니터를 지원하는 대표적인 프로그래밍 언어는 Java입니다. Java에서의 모니터 사용 방법은 간단합니다. 메서드에 `synchronized`키워드를 붙임으로써 모니터를 이용할 수 있지요. 그럼 직접 확인해보겠습니다.

물론 다른 프로그래밍 언어로도 얼마든지 모니터를 구현할 수 있습니다. 위에서 다룬 뮤텍스 락, 세마포를 이용해서요. 

아래 코드는 동기화되지 않은 코드의 예시입니다. 생산자와 소비자 스레드는 동시에 실행되면서, 생산자는 `insert`, 소비자는 `remove` 메서드를 호출합니다.

`insert`와 `remove` 메서드는 공통적으로 `buffer`를 조작하는 메서드이므로 동시에 실행되면 문제가 발생하겠죠?

```java
  1 public class BoundedBuffer<E>
  2 {
  3     private static final int BUFFER_SIZE = 5;
  4     private E[] buffer;
  5
  6     public BoundedBuffer() {
  7         count = 0;
  8         in = 0;
  9         out = 0;
 10         buffer = (E[]) new Object[BUFFER_SIZE];
 11     }
 12
 13     /* 생산자가 호출하는 코드 */
 14     public void insert(E item) {
 15         while (count == BUFFER_SIZE) {
 16             try {
 17                 wait();
 18             }
 19             catch (InterruptedException ie) {}
 20         }
 21
 22         buffer[in] = item;
 23         in = (in + 1) % BUFFER_SIZE;
 24         count++;
 25
 26         notify();
 27     }
 28
 29     /* 소비자가 호출하는 코드 */
 30     public E remove() {
 31         E item;
 32
 33         while (count == 0) {
 34             try {
 35                 wait();
 36             }
 37             catch (InterruptedException ie){}
 38         }
 39
 40         item = buffer[out];
 41         out = (out + 1) % BUFFER_SIZE;
 42         count--;
 43         notify();
 44
 45         return item;
 46     }
 47 }
```

> 예제 코드 출처: Operating System Concepts, Abraham Silberschatz, 외 2인 (10th edition)



위 코드를 동기화한 코드는 다음과 같습니다.

14줄과 30줄에 `synchronized`라는 키워드가 붙었습니다. `synchronized`로 선언된 메서드를 호출하기 위해서는 메서드를 실행하기 위한 락을 획득해야 합니다. 

만일 락을 획득할 수 없다면 (다른 스레드가 `synchronized` 메서드를 실행 중에 있다면)  락을 획득하지 못한 스레드는 그대로 대기 상태가 됩니다.

`synchronized` 메서드를 실행하는 스레드가 메서드 실행을 종료하면 락이 해제되고, 대기 상태에 있던 스레드가 깨어나 실행을 재개할 수 있게 됩니다.

책에서 설명한 바와 동일하지요?

```java
 1 public class BoundedBuffer<E>
  2 {
  3     private static final int BUFFER_SIZE = 5;
  4     private E[] buffer;
  5
  6     public BoundedBuffer() {
  7         count = 0;
  8         in = 0;
  9         out = 0;
 10         buffer = (E[]) new Object[BUFFER_SIZE];
 11     }
 12
 13     /* 생산자가 호출하는 코드 */
 14     public synchronized void insert(E item) {
 15         while (count == BUFFER_SIZE) {
 16             try {
 17                 wait();
 18             }
 19             catch (InterruptedException ie) {}
 20         }
 21
 22         buffer[in] = item;
 23         in = (in + 1) % BUFFER_SIZE;
 24         count++;
 25
 26         notify();
 27     }
 28
 29     /* 소비자가 호출하는 코드 */
 30     public synchronized E remove() {
 31         E item;
 32
 33         while (count == 0) {
 34             try {
 35                 wait();
 36             }
 37             catch (InterruptedException ie){}
 38         }
 39
 40         item = buffer[out];
 41         out = (out + 1) % BUFFER_SIZE;
 42         count--;
 43         notify();
 44
 45         return item;
 46     }
 47 }
```



추가 참고 자료

https://www.cs.cornell.edu/courses/JavaAndDS/files/boundedBuffer.pdf
