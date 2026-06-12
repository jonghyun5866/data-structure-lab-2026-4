#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "AdjMatGraph.h" // WGraph 및 WGraphDijkstra 클래스가 포함된 헤더
#include "SrchAMGraph.h" // 기본 그래프의 DFS 탐색용 헤더

int main()
{
    // C++ 표준 스트림과 C printf/scanf 간의 버퍼 동기화 해제 (출력 순서 꼬임 방지)
    // ※ 터미널 창의 결과물 순서가 깨져 나오면 아래 두 줄을 주석(//) 처리해 주세요.
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // ==========================================================
    // 1. 기본 인접 행렬 그래프 및 DFS 탐색 (SrchAMGraph 사용)
    // ==========================================================
    SrchAMGraph g;

    for (int i = 0; i < 4; i++)
        g.insertVertex('A' + i);
    g.insertEdge(0, 1);
    g.insertEdge(0, 3);
    g.insertEdge(1, 2);
    g.insertEdge(1, 3);
    g.insertEdge(2, 3);

    printf("인접 행렬로 표현한 그래프\n");
    g.display();

    printf("그래프(graph.txt)\n");
    g.display();

    // text_out.txt 파일로 그래프 출력 저장
    FILE* fp_out = NULL;
    fopen_s(&fp_out, "../text_out.txt", "w");
    if (fp_out != NULL) {
        g.display(fp_out);
        fclose(fp_out);
    }

    // SrchAMGraph의 DFS 기능 호출
    printf("DFS==>");
    g.resetVisited();
    g.DFS(0);
    printf("\n\n");


    // ==========================================================
    // 2. 가중치 그래프 파일 로드 및 요구사항 터미널 출력 (WGraphDijkstra 사용)
    // ==========================================================
    // [최종 수정] Kruskal, Prim, ShortestPath를 하나의 객체로 제어하기 위해 클래스 다형성 지정
    WGraphDijkstra wg;

    // 소스 코드와 같은 폴더에 둔 graph.txt를 로드합니다.
    wg.load("graph.txt");

    // 실행 환경(Visual Studio 작업 디렉터리 세팅 등)에 따른 예외 경로 처리
    if (wg.isEmpty()) {
        wg.load("../graph.txt");
    }

    // 파일 로드 실패 시 안전하게 프로그램을 종료시키는 방어 코드
    if (wg.isEmpty()) {
        printf("[오류] graph.txt 파일을 찾을 수 없습니다. 경로를 확인해 주세요.\n");
        return -1;
    }

    // [요구사항 1] F행 가중치 출력 (F 인덱스: 5)
    printf("F ");
    for (int j = 0; j < 7; j++) {
        printf("%d ", wg.getEdge(5, j));
    }
    printf("\n");

    // [요구사항 2] G행 가중치 출력 (G 인덱스: 6)
    printf("G ");
    for (int j = 0; j < 7; j++) {
        printf("%d ", wg.getEdge(6, j));
    }
    printf("\n\n");

    // [요구사항 3] 가중치 그래프 출력 양식 맞춤을 위한 DFS
    printf("DFS ==> ");
    wg.resetVisited();
    wg.DFS(0);
    printf("\n\n");

    // [요구사항 4] 3x3 크기의 A, B, C 정점 가중치 부분 행렬 출력
    printf(" 3 \n");
    for (int i = 0; i < 3; i++) {
        printf(" %c ", wg.getVertex(i));
        for (int j = 0; j < 3; j++) {
            int val = wg.getEdge(i, j);
            if (i == j && val == 0) val = 9999;
            printf("%d ", val);
        }
        printf("\n");
    }
    printf("\n");


    // ==========================================================
    // 3. Kruskal MST 알고리즘 호출
    // ==========================================================
    wg.Kruskal();


    // ==========================================================
    // 4. Prim MST 알고리즘 호출
    // ==========================================================
    wg.Prim(0);


    // ==========================================================
    // 5. Dijkstra 최단 경로 테스트 함수 호출
    // ==========================================================
    printf("Shortest Path By Dijkstra Algorithm\n");
    wg.ShortestPath(0); // 0번 노드를 시작으로 Step별 최단 거리 배열 상태 출력


    return 0;
}
