#pragma once
#include "list.h"
#include <math.h>
#include <time.h>

#define INF 100000      // integer big enough
#define DISTANCE 10      // distance per pixel
#define X_RANGE 100
#define Y_RANGE 100

// knu.txt            X      Y
// 경북대학교북문      61      48
// 경북대학교정문      118      144
// IT1호관            94      108
// 백호관            6      100
// 사회과학대학         126      97



void mapping(ppGrid*** grid, int x_start, int y_start, int x_goal, int y_goal, int density) {
   // setting components
   for (int j = 0; j < Y_RANGE; j++) {
      for (int i = 0; i < X_RANGE; i++) {
         (*grid)[j][i].x = i;
         (*grid)[j][i].y = j;
         (*grid)[j][i].g = INF;
         (*grid)[j][i].h = DISTANCE * sqrt(pow(fabs(i - x_goal), 2) + pow(fabs(j - y_goal), 2));
         (*grid)[j][i].p = 4;
         (*grid)[j][i].l = 0;
      }
   }

	// wall placement
   for (int j = 0; j < Y_RANGE; j++) {
      for (int i = 0; i < X_RANGE; i++) {
         if (rand() % 100 < density) { (*grid)[j][i].d = INF; }
         else { (*grid)[j][i].d = DISTANCE; }
      }
   }

   // setting startpoint
   (*grid)[y_start][x_start].g = 0;
   (*grid)[y_start][x_start].d = DISTANCE;
   (*grid)[y_goal][x_goal].d = DISTANCE;
}

void mapRefresh(ppGrid*** grid, int x_start, int y_start) {
	for (int j = 0; j < Y_RANGE; j++) {
      for (int i = 0; i < X_RANGE; i++) {
         (*grid)[j][i].g = INF;
         (*grid)[j][i].p = 4;
         (*grid)[j][i].l = 0;
      }
   }
   (*grid)[y_start][x_start].g = 0;
}

int checkShortest(ppGrid*** grid, int x_goal, int y_goal) {
   // tracking path from goal to start
   ppGrid* check = &((*grid)[y_goal][x_goal]);
   int x_parent;
   int y_parent;
   int pathCost = 0;

   while ((*check).p != 4) { // while reach start point
      if (((*check).p % 2) == 1) { pathCost += DISTANCE * 1; } // vertical movement 1.0
      else { pathCost += DISTANCE * 1.4; } // diagonal movement 1.4

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
         if (grid[j][i].d == INF) { printf("■"); } // print wall

         else if ((i == x_start) && (j == y_start)) { printf("◇"); } // print start point

         else if ((i == x_goal) && (j == y_goal)) { printf("★"); } // print goal point

         else if (grid[j][i].l == 3) { // print path
            switch (grid[j][i].p) {
            case 0: { printf("↘"); break; }
            case 1: { printf("↓"); break; }
            case 2: { printf("↙"); break; }
            case 3: { printf("→"); break; }
            case 5: { printf("←"); break; }
            case 6: { printf("↗"); break; }
            case 7: { printf("↑"); break; }
            case 8: { printf("↖"); break; }
            default: { printf("er"); break; }
            }
         }

         else if (grid[j][i].l == 2) { printf("ㆍ"); } // print checked point

         else { printf("  "); } // empty space
      }
      printf("\n");
   }
}

void fprintStatus(ppGrid** grid, FILE* file, int x_start, int y_start, int x_goal, int y_goal) {
	// export result as file
	for (int j = 0; j < X_RANGE; j++) {
		for (int i = 0; i < Y_RANGE; i++) {
			if (grid[j][i].d == INF) { fprintf(file, "%d ", 8); } // wall
			else if (grid[j][i].l == 3) { fprintf(file, "%d ", 6); } // shortestPath
			else if (grid[j][i].l == 2) { fprintf(file, "%d ", 4); } // closePath
			else if (grid[j][i].l == 1) { fprintf(file, "%d ", 3); } // openPath
			else if (((i == x_start) && (j == y_start)) || ((i == x_goal) && (j == y_goal))) { fprintf(file, "%d ", 1); } // start/goal
			else { fprintf(file, "%d ", 0); } // blank
		}
		fprintf(file, "\n");
	}
}

void dijkstra(ppGrid** grid, int x_start, int y_start, int x_goal, int y_goal) {
   clock_t start;
   clock_t end;
   start = clock(); // initiate time recording

   List* OpenList = CreateList();
   List* CloseList = CreateList();

   // newly closed node
   AddNode(CloseList, &grid[y_start][x_start]);
   grid[y_start][x_start].l = 2;
   int x_temp = x_start;
   int y_temp = y_start;

   int found = 1;
   int trial = 0;
   int cost;
   float time;

   do {

      // searching adjacent grids
      for (int j = -1; j < 2; j++) {
         for (int i = -1; i < 2; i++) {
            if ((0 <= x_temp + i) && (x_temp + i < X_RANGE) &&
               (0 <= y_temp + j) && (y_temp + j < Y_RANGE) &&
               (SearchNode(CloseList, &grid[y_temp + j][x_temp + i]) == 0)) {
               AddNode(OpenList, &grid[y_temp + j][x_temp + i]);
               grid[y_temp + j][x_temp + i].l = 1;

               if (i * j == 0) { // vertical, calculate distance 1.0
                  if (grid[y_temp + j][x_temp + i].g > (grid[y_temp][x_temp].g + 1 * grid[y_temp + j][x_temp + i].d)) {
                     grid[y_temp + j][x_temp + i].g = (grid[y_temp][x_temp].g + 1 * grid[y_temp + j][x_temp + i].d);
                     grid[y_temp + j][x_temp + i].p = (-j) * 3 - i + 4;
                  }
               }

               else { // diagonal, calculate distance 1.4
                  if (grid[y_temp + j][x_temp + i].g > (grid[y_temp][x_temp].g + 1.4 * grid[y_temp + j][x_temp + i].d)) {
                     grid[y_temp + j][x_temp + i].g = (grid[y_temp][x_temp].g + 1.4 * grid[y_temp + j][x_temp + i].d);
                     grid[y_temp + j][x_temp + i].p = (-j) * 3 - i + 4;
                  }
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
         if (mindist > (*pPre->data).g) {
            mindist = (*pPre->data).g;
            min = pPre->data;
         }
      }

      if (min == NULL) { break; }

      // update closed node
      RemoveNode(OpenList, min);
      AddNode(CloseList, min);
      (*min).l = 2;
      x_temp = (*min).x;
      y_temp = (*min).y;
      trial++;

   } while (IsEmptyList(OpenList) == 0);

   end = clock(); // finish time recording
   time = (float)(end - start) / CLOCKS_PER_SEC;
   // (*result).time += time;

   if (found == 0) { printf("failed\n"); }

   else {
      cost = checkShortest(&grid, x_goal, y_goal);
      // (*result).cost += cost;
      // (*result).trial += trial;
      printResult(grid, x_start, y_start, x_goal, y_goal);
      printf("%4d trials excuted.    Path Cost : %4d m.    Running Time : %.2f\n", trial, cost, time);
   }

   DestroyList(CloseList);
   DestroyList(OpenList);
}

void aStar(ppGrid** grid, FILE* file, int x_start, int y_start, int x_goal, int y_goal, float distvalue) {
   clock_t start;
   clock_t end;
   start = clock(); // initiate time recording

   List* OpenList = CreateList();
   List* CloseList = CreateList();

   // newly closed node
   AddNode(CloseList, &grid[y_start][x_start]);
   grid[y_start][x_start].l = 2;
   int x_temp = x_start;
   int y_temp = y_start;

   int found = 1;
   int trial = 0;
   int cost;
   float time;

   while (grid[y_goal][x_goal].l != 2) {

      // searching adjacent grids
      for (int j = -1; j < 2; j++) {
         for (int i = -1; i < 2; i++) {
            if ((0 <= x_temp + i) && (x_temp + i < X_RANGE) &&
               (0 <= y_temp + j) && (y_temp + j < Y_RANGE) &&
               (SearchNode(CloseList, &grid[y_temp + j][x_temp + i]) == 0)) {
               AddNode(OpenList, &grid[y_temp + j][x_temp + i]);
               grid[y_temp + j][x_temp + i].l = 1;

               if (i * j == 0) { // vertical, calculate distance 1.0
                  if (grid[y_temp + j][x_temp + i].g > (grid[y_temp][x_temp].g + 1 * grid[y_temp + j][x_temp + i].d)) {
                     grid[y_temp + j][x_temp + i].g = (grid[y_temp][x_temp].g + 1 * grid[y_temp + j][x_temp + i].d);
                     grid[y_temp + j][x_temp + i].p = (-j) * 3 - i + 4;
                  }
               }

               else { // diagonal, calculate distance 1.4
                  if (grid[y_temp + j][x_temp + i].g > (grid[y_temp][x_temp].g + 1.4 * grid[y_temp + j][x_temp + i].d)) {
                     grid[y_temp + j][x_temp + i].g = (grid[y_temp][x_temp].g + 1.4 * grid[y_temp + j][x_temp + i].d);
                     grid[y_temp + j][x_temp + i].p = (-j) * 3 - i + 4;
                  }
               }

               if (grid[y_temp + j][x_temp + i].d == INF) { // tracing walls
                  grid[y_temp][x_temp].g += (DISTANCE * distvalue);
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
      trial++;

      fprintStatus(grid, file, x_start, y_start, x_goal, y_goal);
   }

   end = clock(); // finish time recording
   time = (float)(end - start) / CLOCKS_PER_SEC;
   

   if (found == 0) { /*(*result).failed++;*/ }

   else {
      cost = checkShortest(&grid, x_goal, y_goal);
      printf("## Reduction Value : %.1f\n", distvalue);
      printf("  Calculating Time : %.0f ms\n", time * 1000);
      printf("     Path Distance : %d\n\n\n", cost);
      // (*result).cost += cost;
      // (*result).trial += trial;
      fprintStatus(grid, file, x_start, y_start, x_goal, y_goal);
      //printf("%4d trials excuted.    Path Cost : %4d m.    Running Time : %.2f\n", trial, cost, time);
   }

   DestroyList(CloseList);
   DestroyList(OpenList);
}

int main() {
   srand(time(NULL));

	int x_start = (int)(rand() % X_RANGE);
	int y_start = (int)(rand() % Y_RANGE);
	int x_goal = (int)(rand() % X_RANGE);
	int y_goal = (int)(rand() % Y_RANGE);

   int density;
   printf("input wall density(0 ~ 100) : ");
   scanf("%d", &density);

	printf("\n\n\n");

	ppGrid** grid;

   // assign grid[y][x] = (x, y)
   grid = (ppGrid**)malloc(sizeof(ppGrid*) * Y_RANGE);
   if (grid == NULL) { return; }

   for (int i = 0; i < Y_RANGE; i++) {
      grid[i] = (ppGrid*)malloc(sizeof(ppGrid) * X_RANGE);
      if (grid[i] == NULL) { return; }
   }

   mapping(&grid, x_start, y_start, x_goal, y_goal, density);





   FILE *file00 = fopen("output00", "w");
   aStar(grid, file00, x_start, y_start, x_goal, y_goal, 0);
	fclose(file00);
	mapRefresh(&grid, x_start, y_start);
	
   FILE *file04 = fopen("output04", "w");
   aStar(grid, file04, x_start, y_start, x_goal, y_goal, -0.4);
	fclose(file04);
	mapRefresh(&grid, x_start, y_start);
	
   FILE *file08 = fopen("output08", "w");
   aStar(grid, file08, x_start, y_start, x_goal, y_goal, -0.8);
	fclose(file08);
	mapRefresh(&grid, x_start, y_start);
	
   FILE *file12 = fopen("output12", "w");
   aStar(grid, file12, x_start, y_start, x_goal, y_goal, -1.2);
	fclose(file12);
	mapRefresh(&grid, x_start, y_start);



   // free heap
   for (int _ = 0; _ < Y_RANGE; _++) { free(grid[_]); }
   free(grid);



   return 0;
}