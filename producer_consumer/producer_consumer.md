## producer consumer 문제

이 곳에 첨부된 producer_consumer.cpp는 Chapter 12에서 언급한 대표적인 동기화 문제를 간단히 코드로 구현한 것입니다.

코드는 크게 `produce` 라는 함수를 10만번 실행하는 `producer` 스레드와

`consume` 이라는 함수를 10만번 실행하는 `consumer` 스레드로 이루어져 있습니다.

```c++
    std::thread producer(produce);
    std::thread consumer(consume);
```

그리고 `producer`와 `consumer`가 실행하기 전 총 합계에 해당하는 `sum`을 출력하고,

실행한 이후 총 합계를 출력합니다.

```c++
    std::cout << "초기 합계: " <<  sum << std::endl;
    std::thread producer(produce);
    std::thread consumer(consume);

    producer.join();
    consumer.join();

    std::cout << "producer, consumer 스레드 실행 이후 합계: " <<  sum << std::endl;
```

`produce` 는 큐에 임의의 값(1)을 총 합계를 1 증가시키는 것을 10만번 반복하는 코드이고,

```c++
void produce() {
    for(int i = 0; i < 100000; i++) {
        //q.push(1);
        sum++;
    }
}
```

`consume` 은 큐의 값을 하나 뺴낸 뒤 총 합계를 1 감소시키는 것을 10만번 반복하는 코드입니다.

```c++
void consume() {
    for(int i = 0; i < 100000; i++) {
        //q.pop();
        sum--;
    }
}
```

위 코드는 얼핏 보면 아무런 문제가 없어 보일지 모르나 실행하면 예상치 못한 결과를 초래합니다.

큐에 값을 넣고 빼는 코드 (`q.push(1)`, `q.pop()`)을 포함하여 실행하면 에러가 발생하고,

이를 주석 처리하고 실행한다고 해도 실행 후 합계는 0이 아닌 전혀 다른 값이 됩니다.

>큐에 값을 넣고 빼는 코드를 포함하여 실행하면 에러가 발생하는 이유는,
>
>producer와 consumer 사이에 레이스 컨디션이 발생하여 
>
>큐에 아무것도 값이 남아있지 않았음에도 불구하고 값을 빼내려 했기 때문입니다.

## 코드 실행 영상

이 코드를 실행한 영상은 다음과 같습니다.

[![asciicast](https://asciinema.org/a/pubZoqu6UlaT1FBF7a87xfszw.svg)](https://asciinema.org/a/pubZoqu6UlaT1FBF7a87xfszw)

