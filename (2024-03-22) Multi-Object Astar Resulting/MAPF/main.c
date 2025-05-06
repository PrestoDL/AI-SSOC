#pragma once
#include "list.h"
#include "queue.h"
#include <math.h>
#include <time.h>

#define INF 100000      // integer big enough
#define DISTANCE 10     // distance per pixel
#define X_RANGE 100     // x boundary of map
#define Y_RANGE 100     // y boundary of map
#define TRIAL 10000     // attempts
#define REDUCTION -0.5  // reduction value
#define AGENTS 50       // number of agents



typedef struct {
    int cost;
    float time;
} Result;

ppGrid** mapping() {
    ppGrid** grid = (ppGrid**)malloc(sizeof(ppGrid*) * Y_RANGE);
    if (grid == NULL) { return; }

    for (int i = 0; i < Y_RANGE; i++) {
        grid[i] = (ppGrid*)malloc(sizeof(ppGrid) * X_RANGE);
        if (grid[i] == NULL) { return; }
    }

    // setting components
    for (int j = 0; j < Y_RANGE; j++) {
        for (int i = 0; i < X_RANGE; i++) {
            grid[j][i].x = i;
            grid[j][i].y = j;
            grid[j][i].d = DISTANCE;
            grid[j][i].g = INF;
            grid[j][i].p = 4;
            grid[j][i].l = 0;
        }
    }



    // setting walls


    FILE* p = fopen("map20.txt", "r");
    if (p != NULL) {
        char line[X_RANGE + 2];
        for (int j = 0; j < Y_RANGE; j++) {
            fgets(line, sizeof(line), p);
            for (int i = 0; i < X_RANGE; i++) {
                if (line[i] == '1') { grid[j][i].d = INF; }
            }
        }
        fclose(p);
    }


    /*
    for (int j = 0; j < Y_RANGE; j++) {
       for (int i = 0; i < X_RANGE; i++) {
          if (rand() % 100 < density)   { (*grid)[j][i].d = INF; }
          else               { (*grid)[j][i].d = DISTANCE; }
       }
    }
    */

    /*
    for (int i = 33; i < 50; i++) { grid[0][i].d = INF; }
    for (int i = 0; i < 34; i++) { grid[16][i].d = INF; }
    for (int i = 16; i < 50; i++) { grid[33][i].d = INF; }
    for (int i = 0; i < 17; i++) { grid[49][i].d = INF; }
    for (int j = 16; j < 50; j++) { grid[j][0].d = INF; }
    for (int j = 33; j < 50; j++) { grid[j][16].d = INF; }
    for (int j = 0; j < 17; j++) { grid[j][33].d = INF; }
    for (int j = 0; j < 34; j++) { grid[j][49].d = INF; }
    */

    return grid;
}

void maprefresh(ppGrid** grid) {
    // setting components
    for (int j = 0; j < Y_RANGE; j++) {
        for (int i = 0; i < X_RANGE; i++) {
            grid[j][i].g = INF;
            grid[j][i].p = 4;
            grid[j][i].l = 0;
        }
    }
}

int checkShortest(ppGrid*** grid, Agent* agent, int x_goal, int y_goal) {
    // tracking path from goal to start
    ppGrid* check = &((*grid)[y_goal][x_goal]);
    int x_parent;
    int y_parent;
    int pathCost = 0;
    while ((*check).p != 4) { // while reach start point
        pathCost += DISTANCE;
        Coordinate* temppath = (Coordinate*)malloc(sizeof(Coordinate));
        if (temppath == NULL) { return; }
        (*temppath).x = (*check).x;
        (*temppath).y = (*check).y;
        Push((*agent).path, temppath);

        x_parent = ((*check).p % 3) - 1;
        y_parent = ((*check).p / 3) - 1;
        check = &(*grid)[(*check).y + y_parent][(*check).x + x_parent];
        (*check).l = 3;
    }

    return pathCost;
}

void printResult(ppGrid** grid, int x_start, int y_start, int x_goal, int y_goal) {
    for (int j = 0; j < Y_RANGE; j++) {
        for (int i = 0; i < X_RANGE; i++) {
            if (grid[j][i].d == INF) { printf("  "); } // print wall

            else if ((i == x_start) && (j == y_start)) { printf("  "); } // print start point

            else if ((i == x_goal) && (j == y_goal)) { printf("  "); } // print goal point

            else if (grid[j][i].l == 3) { // print path
                switch (grid[j][i].p) {
                case 0: { printf("  "); break; }
                case 1: { printf("  "); break; }
                case 2: { printf("  "); break; }
                case 3: { printf("  "); break; }
                case 5: { printf("  "); break; }
                case 6: { printf("  "); break; }
                case 7: { printf("  "); break; }
                case 8: { printf("  "); break; }
                default: { printf("er"); break; }
                }
            }

            else if (grid[j][i].l == 1) { printf("  "); } // print checked point

            else { printf("  "); } // empty space
        }
        printf("\n");
    }
    printf("\n\n\n\n");
}

void fprintStatus(ppGrid** grid, Queue* agentQueue, FILE* file) {
    // export result as file
    Agent* tempAgent;
    int agentCount = CountQueue(agentQueue);

    /*
    for (int j = 0; j < X_RANGE; j++) {
        for (int i = 0; i < Y_RANGE; i++) {
            if (grid[j][i].l == 4) { grid[j][i].l = 3; }
        }
    }*/

    maprefresh(grid);

    for (int i = 0; i < agentCount; i++) {
        tempAgent = Dequeue(agentQueue);
        grid[tempAgent->y][tempAgent->x].l = 4;
        Enqueue(agentQueue, tempAgent);
    }

    for (int j = 0; j < X_RANGE; j++) {
        for (int i = 0; i < Y_RANGE; i++) {
            if (grid[j][i].d == INF) { fprintf(file, "%d ", 5); }
            else if (grid[j][i].l == 4) { fprintf(file, "%d ", 3); }
            else { fprintf(file, "%d ", 0); }
        }
        fprintf(file, "\n");
    }

    maprefresh(grid);
}

void printStatus(ppGrid** grid, Queue* agentQueue) {
    Agent* tempAgent;
    int agentCount = CountQueue(agentQueue);

    for (int j = 0; j < X_RANGE; j++) {
        for (int i = 0; i < Y_RANGE; i++) {
            if (grid[j][i].l == 4) { grid[j][i].l = 3; }
        }
    }

    for (int i = 0; i < agentCount; i++) {
        tempAgent = Dequeue(agentQueue);
        grid[tempAgent->y][tempAgent->x].l = 4;
        Enqueue(agentQueue, tempAgent);
    }

    for (int j = 0; j < X_RANGE; j++) {
        for (int i = 0; i < Y_RANGE; i++) {
            if (grid[j][i].d == INF) { printf("  "); }
            else if (grid[j][i].l == 4) { printf("  "); }
            else { printf("  "); }
        }
        printf("\n");
    }

    printf("\n\n\n\n");
}

Boolean aStar(ppGrid** grid, Agent* agent, int x_goal, int y_goal, float reductionvalue) {
    for (int j = 0; j < Y_RANGE; j++) {
        for (int i = 0; i < X_RANGE; i++) {
            grid[j][i].h = DISTANCE * (abs(i - x_goal) + abs(j - y_goal));
        }
    }

    List* OpenList = CreateList();
    List* CloseList = CreateList();

    // newly closed node
    int x_temp = (*agent).x;
    int y_temp = (*agent).y;
    AddNode(CloseList, &grid[y_temp][x_temp]);
    grid[y_temp][x_temp].g = 0;
    grid[y_temp][x_temp].l = 2;

    int found = 1;

    while (grid[y_goal][x_goal].l != 2) {
        // searching adjacent grids
        for (int j = -1; j < 2; j++) {
            for (int i = -1; i < 2; i++) {
                if ((0 <= x_temp + i) && (x_temp + i < X_RANGE) &&
                    (0 <= y_temp + j) && (y_temp + j < Y_RANGE) &&
                    ((i + j) % 2 != 0) && (SearchNode(CloseList, &grid[y_temp + j][x_temp + i]) == 0)) {
                    AddNode(OpenList, &grid[y_temp + j][x_temp + i]);
                    grid[y_temp + j][x_temp + i].l = 1;

                    if (grid[y_temp + j][x_temp + i].g > (grid[y_temp][x_temp].g + 1 * grid[y_temp + j][x_temp + i].d)) {
                        grid[y_temp + j][x_temp + i].g = (grid[y_temp][x_temp].g + 1 * grid[y_temp + j][x_temp + i].d);
                        grid[y_temp + j][x_temp + i].p = (-j) * 3 - i + 4;
                    }

                    if (grid[y_temp + j][x_temp + i].d == INF) { // tracing walls
                        grid[y_temp][x_temp].g += (DISTANCE * reductionvalue);
                    }

                }
            }
        }


        // update lowest valued node
        ppGrid* min = NULL;
        int mindist = INF;
        ListNode* pPre = NULL, * pPos = NULL;
        for (pPre = OpenList->head; pPre != NULL; pPre = pPos) {
            pPos = pPre->next;
            if (mindist > ((*pPre->data).g + (*pPre->data).h)) {
                mindist = ((*pPre->data).g + (*pPre->data).h);
                min = pPre->data;
            }
        }

        if (min == NULL) { found = 0; break; }
        // update closed node
        RemoveNode(OpenList, min);
        AddNode(CloseList, min);
        (*min).l = 2;
        x_temp = (*min).x;
        y_temp = (*min).y;
    }

    if (found == 1) {
        checkShortest(&grid, agent, x_goal, y_goal);
    }

    else {
        printf("\n\n################################\n\nerror detected : a* cannot find the way.\n\n################################\n\n");
        /*
        Coordinate* stay = (Coordinate*)malloc(sizeof(Coordinate));
        stay->x = agent->x;
        stay->y = agent->y;
        Push(agent->path, stay);
        Enqueue(holdQueue, agent);
        */
    }

    DestroyList(CloseList);
    DestroyList(OpenList);
    maprefresh(grid);

    return found;
}

Boolean avoidCollision(ppGrid** tempGrid, Queue* agentQueue, Agent* agent, int x_goal, int y_goal) {
    int agentCount = CountQueue(agentQueue);
    int pathCount = CountStackItem(agent->path);
    Agent* tempAgent;
    Stack* nowStack = CreateStack();
    Stack** ptempStack = (Stack**)malloc(sizeof(Stack*) * (agentCount - 1));
    Coordinate* temppath;
    Coordinate* agentpath;
    if (ptempStack == NULL) { return; }
    for (int _ = 0; _ < (agentCount - 1); _++) {
        ptempStack[_] = CreateStack();
    }

    for (int i = 0; i < pathCount; i++) {
        for (int j = 0; j < (agentCount - 1); j++) {
            tempAgent = Dequeue(agentQueue);

            if (IsEmptyStack(tempAgent->path) != 1) {
                temppath = Pop(tempAgent->path);
                tempGrid[temppath->y][temppath->x].l = 1;
                Push(ptempStack[j], temppath);
            }

            Enqueue(agentQueue, tempAgent);
        }
        Enqueue(agentQueue, Dequeue(agentQueue));

        temppath = Pop(agent->path);

        if (IsEmptyStack(agent->path) != 1) {
            if ((tempGrid[temppath->y][temppath->x].l == 1) || (tempGrid[Top(agent->path)->y][Top(agent->path)->x].l) == 1) {
                tempGrid[temppath->y][temppath->x].d = INF;

                while (nowStack->count > 0) {
                    Pop(nowStack);
                }

                while (agent->path->count > 0) {
                    Pop(agent->path);
                }

                for (int k = 0; k < (agentCount - 1); k++) {

                    tempAgent = Dequeue(agentQueue);

                    while (ptempStack[k]->count > 0) {
                        Push(tempAgent->path, Pop(ptempStack[k]));
                    }

                    Enqueue(agentQueue, tempAgent);
                }
                Enqueue(agentQueue, Dequeue(agentQueue));

                maprefresh(tempGrid);
                //printStatus(tempGrid, agentQueue);

                return 1;
            }
        }

        maprefresh(tempGrid);
        Push(nowStack, temppath);
    }

    while (nowStack->count > 0) {
        Push(agent->path, Pop(nowStack));
    }

    for (int k = 0; k < (agentCount - 1); k++) {
        tempAgent = Dequeue(agentQueue);

        while (ptempStack[k]->count > 0) {
            Push(tempAgent->path, Pop(ptempStack[k]));
        }

        Enqueue(agentQueue, tempAgent);
    }
    Enqueue(agentQueue, Dequeue(agentQueue));



    for (int _ = 0; _ < (agentCount - 1); _++) {
        free(ptempStack[_]);
    }
    free(ptempStack);
    DestroyStack(nowStack);


    return 0;
}

void addAgent(Queue* agentQueue, int x_start, int y_start, int x_goal, int y_goal, float reductionvalue) {
    Agent* agent = (Agent*)malloc(sizeof(Agent));
    ppGrid** tempGrid = mapping();
    if (agent == NULL) { return; }
    (*agent).x = x_start;
    (*agent).y = y_start;
    (*agent).path = CreateStack();
    (*agent).x_goal = x_goal;
    (*agent).y_goal = y_goal;
    Enqueue(agentQueue, agent);

    Boolean collision;
    do {
        if (aStar(tempGrid, agent, x_goal, y_goal, reductionvalue) == 0) { break; }
        collision = avoidCollision(tempGrid, agentQueue, agent, x_goal, y_goal);
    } while (collision == 1);

    for (int _ = 0; _ < Y_RANGE; _++) { free(tempGrid[_]); }
    free(tempGrid);
}

void holdAgent(Agent* agent, Queue* agentQueue, Queue* holdQueue, float reductionvalue) {
    ppGrid** tempGrid = mapping();

    Boolean collision;
    do {
        if (aStar(tempGrid, agent, holdQueue, agent->x_goal, agent->y_goal, reductionvalue) == 0) { break; }
        collision = avoidCollision(tempGrid, agentQueue, agent, agent->x_goal, agent->y_goal);
    } while (collision == 1);

    for (int _ = 0; _ < Y_RANGE; _++) { free(tempGrid[_]); }
    free(tempGrid);
}



void main() {
    srand(time(NULL));
    //FILE* p = fopen("resultMap.txt", "a");
    Result result;
    result.cost = 0;
    result.time = 0;

    // assign grid[y][x] = (x, y)
    ppGrid** grid = mapping();
    Queue* agentQueue = CreateQueue();
    //Queue* holdQueue = CreateQueue();
    Agent* tempAgent;
    Coordinate temppath;

    clock_t start;
    clock_t end;




    start = clock(); // initiate time recording


    for (int agents = 50; agents <= 50; agents += 5) {

        FILE* r = fopen("result.txt", "a");

        start = clock(); // initiate time recording

        for (int tri = 0; tri < TRIAL; tri++) {

            // add object, initiate MAPF aStar
            int sx; int sy;
            int gx; int gy;
            for (int j = 0; j < agents; j++) {
                do {
                    sx = rand() % 100; sy = rand() % 100;
                } while (grid[sy][sx].d == INF || grid[sy][sx].l == 4);

                do {
                    gx = rand() % 100; gy = rand() % 100;
                } while (grid[gy][gx].d == INF || grid[gy][gx].l == 4 || ((sx == gx) && (sy == gy)));

                addAgent(agentQueue, sx, sy, gx, gy, REDUCTION);
            }



            do {
                int agentCount = CountQueue(agentQueue);

                for (int i = 0; i < agentCount; i++) { // step movement
                    tempAgent = Dequeue(agentQueue);

                    if (IsEmptyStack(tempAgent->path) != 1) { // agent not reached goal
                        temppath = *Pop(tempAgent->path);

                        tempAgent->x = temppath.x;
                        tempAgent->y = temppath.y;

                        Enqueue(agentQueue, tempAgent);
                    }
                }

                /*
                int holdCount = holdQueue->count;
                for (int h = 0; h < holdCount; h++) {
                    tempAgent = Dequeue(holdQueue);
                    holdAgent(tempAgent, agentQueue, holdQueue, -red);
                }
                */
                //printf("\n<%d agents left>\n", holdCount);

                //fprintStatus(grid, agentQueue, p);

                if (agentCount == 0) { break; };
                result.cost++;
            } while (1);

            printf("%d\n", tri);

        }

        end = clock(); // finish time recording

        result.time += (float)(end - start) / CLOCKS_PER_SEC;

        printf("\nAgents : %d\nTotal Costs : %d\nTotal Running Time : %.3f s\n\n", agents, result.cost, result.time);
        fprintf(r, "\nAgents : %d\nTotal Costs : %d\nTotal Running Time : %.3f s\n\n", agents, result.cost, result.time);
        result.cost = 0;
        result.time = 0;

        fclose(r);
    }


    // free heap
    for (int _ = 0; _ < Y_RANGE; _++) { free(grid[_]); }
    free(grid);
    DestroyQueue(agentQueue);
    //DestroyQueue(holdQueue);

    //fclose(p);
}