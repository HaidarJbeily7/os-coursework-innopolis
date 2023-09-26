#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Point{
    float x, y;
};

struct Point* createPointStructFromXnY(float x, float y) {
  struct Point* p = malloc(sizeof(struct Point));
  p->x = x;
  p->y = y;
  return p;
}

float distance(struct Point* a, struct Point* b){
    float x_2 = (a->x - b->x) * (a->x - b->x);
    float y_2 = (a->y - b->y) * (a->y - b->y);
    return sqrt(x_2 + y_2);
}

float area(struct Point* a, struct Point* b, struct Point* c){
    float x1 = a->x, y1 = a->y;

    float x2 = b->x, y2 = b->y;

    float x3 = c->x, y3 = c->y;

    return fabs(x1*y2 - x2*y1 + x2*y3 - x3*y2 + x3*y1 - x1*y3) / 2;
}

int main()
{
    struct Point* a = createPointStructFromXnY(2.5, 6);
    struct Point* b = createPointStructFromXnY(1, 2.2);
    struct Point* c = createPointStructFromXnY(10, 6);
    printf("%f\n", distance(a,b));
    printf("%f\n", area(a,b,c));

    return 0;
}