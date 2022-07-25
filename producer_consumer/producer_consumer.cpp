#include <iostream>
#include <queue>
#include <thread>

void produce();
void consume();

//std::queue<int> q;
int sum = 0;

int main() {

    std::cout << "초기 합계: " <<  sum << std::endl;
    std::thread producer(produce);
    std::thread consumer(consume);

    producer.join();
    consumer.join();
    
    std::cout << "producer, consumer 스레드 실행 이후 합계: " <<  sum << std::endl;
    
    return 0;
}

void produce() {
    for(int i = 0; i < 100000; i++) {
        // q.push(1);
        sum++;
    }
}

void consume() {
    for(int i = 0; i < 100000; i++) {
        // q.pop();
        sum--;
    }
}
