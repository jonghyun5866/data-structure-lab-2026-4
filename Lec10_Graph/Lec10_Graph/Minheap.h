#pragma once
#include <iostream>

// 힙에 저장될 노드 구조체
struct HeapNode {
    int key;
    int v1;
    int v2;
};

class MinHeap {
private:
    HeapNode node[1001]; // 힙을 저장할 배열 (최대 크기 예시)
    int size;            // 현재 힙에 저장된 원소 개수
    int capacity;        // 힙의 최대 용량

public:
    MinHeap() : size(0), capacity(1000) { }

    bool isFull() const {
        return size >= capacity;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void insert(int key, int u, int v) {
        if (isFull()) return;

        int i = ++size; // 새로운 원소가 들어갈 임시 위치 (가장 끝)

        // 부모 노드(i / 2)와 비교하며 최소 힙 속성 유지 (Up-Heap)
        while (i != 1 && key < node[i / 2].key) {
            node[i] = node[i / 2]; // 부모 노드를 아래로 끌어내림
            i /= 2;                // 인덱스를 부모 위치로 이동
        }

        // 최종 결정된 위치에 데이터 삽입
        node[i].key = key;
        node[i].v1 = u;
        node[i].v2 = v;
    }
};
