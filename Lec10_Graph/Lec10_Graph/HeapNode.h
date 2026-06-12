#pragma once

class HeapNode {
private: // 가독성을 위해 명시적 private 지정
	int key;
	int v1;
	int v2;

public:
	// 1. 기본 생성자 추가 (배열 선언을 위해 필수)
	HeapNode() : key(0), v1(0), v2(0) { }

	// 2. 기존 생성자
	HeapNode(int k, int u, int v) : key(k), v1(u), v2(v) { }

	void setKey(int k) { key = k; }

	// 3. 안전성을 위해 const 추가 (조회용 함수)
	int getKey() const { return key; }
	int getV1()  const { return v1; }
	int getV2()  const { return v2; }
};
