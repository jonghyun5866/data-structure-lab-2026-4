#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "VertexSets.h" 

#define MAX_VTXS 20
#define INF 9999

// Kruskal 알고리즘용 간선 구조체
struct Edge {
	int u, v;    // 연결된 두 정점의 인덱스
	int weight;  // 간선의 가중치
};

class AdjMatGraph {
protected:
	int size;
	char vertices[MAX_VTXS];
	int adj[MAX_VTXS][MAX_VTXS];
public:
	AdjMatGraph() { reset(); }
	char getVertex(int i) { return vertices[i]; }
	int getEdge(int i, int j) { return adj[i][j]; }
	void setEdge(int i, int j, int val) { adj[i][j] = val; }
	bool isEmpty() { return size == 0; }
	bool isFull() { return size >= MAX_VTXS; }

	void reset() {
		size = 0;
		for (int i = 0; i < MAX_VTXS; i++)
			for (int j = 0; j < MAX_VTXS; j++)
				setEdge(i, j, 0);
	}

	void insertVertex(char name) {
		if (!isFull()) vertices[size++] = name;
		else printf("Error: 그래프 정점 개수 초과\n");
	}

	void insertEdge(int u, int v) {
		setEdge(u, v, 1);
		setEdge(v, u, 1);
	}

	void display(FILE* fp = stdout) {
		fprintf(fp, "%d\n", size);
		for (int i = 0; i < size; i++) {
			fprintf(fp, "%c", getVertex(i));
			for (int j = 0; j < size; j++)
				fprintf(fp, "%3d", getEdge(i, j));
			fprintf(fp, "\n");
		}
	}
};

class WGraph : public AdjMatGraph {
private:
	bool visited[MAX_VTXS];

	int getMinVertex(bool selected[], int dist[]) {
		int min_v = -1;
		int min_dist = INF;
		for (int i = 0; i < size; i++) {
			if (!selected[i] && dist[i] < min_dist) {
				min_dist = dist[i];
				min_v = i;
			}
		}
		return min_v;
	}

public:
	void insertEdge(int u, int v, int weight) {
		if (weight > INF) weight = INF;
		setEdge(u, v, weight);
	}

	bool hasEdge(int i, int j) {
		int w = getEdge(i, j);
		return (w > 0 && w < INF);
	}

	void resetVisited() {
		for (int i = 0; i < MAX_VTXS; i++) visited[i] = false;
	}

	void DFS(int v) {
		if (v < 0 || v >= size) return;
		visited[v] = true;
		printf("%c ", getVertex(v));

		for (int w = 0; w < size; w++) {
			if (getEdge(v, w) != 0 && getEdge(v, w) < INF && !visited[w]) {
				DFS(w);
			}
		}
	}

	void load(const char* filename) {
		FILE* fp;
		fopen_s(&fp, filename, "r");
		if (fp == NULL) {
			printf("\n[오류] '%s' 파일을 읽지 못했습니다. 경로를 다시 확인해주세요.\n\n", filename);
			return;
		}

		int n;
		if (fscanf_s(fp, "%d", &n) == 1) {
			reset();
			for (int i = 0; i < n; i++) {
				char str[80];
				if (fscanf_s(fp, "%s", str, (unsigned int)sizeof(str)) == 1) {
					insertVertex(str[0]);
				}
				for (int j = 0; j < n; j++) {
					int val;
					fscanf_s(fp, "%d", &val);
					insertEdge(i, j, val);
				}
			}
		}
		fclose(fp);
	}

	void Kruskal() {
		if (isEmpty()) {
			printf("Kruskal MST 오류: 불러온 그래프 데이터가 존재하지 않습니다.\n");
			return;
		}

		Edge edges[MAX_VTXS * MAX_VTXS];
		int edgeCount = 0;

		for (int i = 0; i < size; i++) {
			for (int j = i + 1; j < size; j++) {
				int w = getEdge(i, j);
				if (w > 0 && w < INF) {
					edges[edgeCount++] = { i, j, w };
				}
			}
		}

		for (int i = 0; i < edgeCount - 1; i++) {
			for (int j = 0; j < edgeCount - i - 1; j++) {
				if (edges[j].weight > edges[j + 1].weight) {
					Edge temp = edges[j];
					edges[j] = edges[j + 1];
					edges[j + 1] = temp;
				}
			}
		}

		VertexSets sets(size);
		int mstEdges = 0;
		int totalWeight = 0;

		printf("Kruskal MST 알고리즘 실행 결과:\n");
		printf("=================================\n");

		for (int i = 0; i < edgeCount; i++) {
			int uRoot = sets.findSet(edges[i].u);
			int vRoot = sets.findSet(edges[i].v);

			if (uRoot != vRoot) {
				sets.unionSets(uRoot, vRoot);
				mstEdges++;
				totalWeight += edges[i].weight;

				printf("간선 선택 %d: (%c, %c) 가중치: %d\n",
					mstEdges, getVertex(edges[i].u), getVertex(edges[i].v), edges[i].weight);

				if (mstEdges == size - 1) break;
			}
		}
		printf("=================================\n");
		printf("MST 총 가중치 합: %d\n\n", totalWeight);
	}

	void Prim(int s) {
		if (isEmpty()) {
			printf("Prim MST 오류: 그래프 데이터가 없습니다.\n");
			return;
		}
		if (s < 0 || s >= size) {
			printf("Prim MST 오류: 시작 정점 인덱스가 잘못되었습니다.\n");
			return;
		}

		bool selected[MAX_VTXS];
		int dist[MAX_VTXS];
		int parent[MAX_VTXS];

		for (int i = 0; i < size; i++) {
			dist[i] = INF;
			selected[i] = false;
			parent[i] = -1;
		}

		dist[s] = 0;
		int totalWeight = 0;

		printf("Prim MST 알고리즘 실행 결과 (시작 정점: %c):\n", getVertex(s));
		printf("=================================\n");

		for (int i = 0; i < size; i++) {
			int u = getMinVertex(selected, dist);
			if (u == -1 || dist[u] == INF) {
				printf("경고: 그래프가 연결되어 있지 않습니다.\n");
				return;
			}

			selected[u] = true;
			totalWeight += dist[u];

			if (parent[u] != -1) {
				printf("간선 선택 %d: (%c, %c) 가중치: %d\n",
					i, getVertex(parent[u]), getVertex(u), dist[u]);
			}

			for (int v = 0; v < size; v++) {
				int weight = getEdge(u, v);
				if (weight > 0 && weight < INF) {
					if (!selected[v] && weight < dist[v]) {
						dist[v] = weight;
						parent[v] = u;
					}
				}
			}
		}
		printf("=================================\n");
		printf("MST 총 가중치 합: %d\n\n", totalWeight);
	}
};

// ==========================================================
// [수업자료 추가 반영] Dijkstra 최단 경로 알고리즘 클래스
// ==========================================================
class WGraphDijkstra : public WGraph {
private:
	int dist[MAX_VTXS];     // 시작노드로부터의 최단경로 거리
	bool found[MAX_VTXS];   // 방문한 정점 표시

	// 수업 자료의 chooseVertex() 구현
	int chooseVertex() {
		int min = INF;
		int minpos = -1;
		for (int i = 0; i < size; i++) {
			if (dist[i] < min && !found[i]) {
				min = dist[i];
				minpos = i;
			}
		}
		return minpos;
	}

public:
	// 수업 자료의 printDistance() 구현
	void printDistance() {
		for (int i = 0; i < size; i++) {
			if (dist[i] == INF) printf("%5d", INF); // 또는 수업자료 매칭용 9999 표기 원할시 변경 가능
			else printf("%5d", dist[i]);
		}
		printf("\n");
	}

	// 수업 자료의 ShortestPath 테스트 함수 연동
	void ShortestPath(int s) {
		if (isEmpty()) return;

		// 초기화 
		for (int i = 0; i < size; i++) {
			dist[i] = getEdge(s, i);
			found[i] = false;
		}

		found[s] = true;
		dist[s] = 0;

		// 1단계 출력 부가 구현 (교재 터미널 화면 대응)
		printf("Step 1:");
		printDistance();

		// 정점 확정 루프 진행 (Step 2부터 시작)
		for (int step = 2; step <= size; step++) {
			int u = chooseVertex();
			if (u == -1 || dist[u] == INF) break;

			found[u] = true;

			for (int v = 0; v < size; v++) {
				if (!found[v]) {
					int weight = getEdge(u, v);
					if (weight > 0 && weight < INF) {
						if (dist[u] + weight < dist[v]) {
							dist[v] = dist[u] + weight;
						}
					}
				}
			}
			printf("Step %d:", step);
			printDistance();
		}
	}
};
