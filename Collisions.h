#include "CollisionStruct.h"
#include <vector>
using std::vector;

Point ClosestPointOnLine(Line& theLine, Point& testPoint, bool pointOnSegment = true);

Point ClosestPointOnLine(Point& startPoint, Point& endPoint, Point& testPoint, bool pointOnSegment = true);

Point GetNormalBetweenPoints(Point& fromPoint, Point& toPoint);

float absValue(float value);

bool sameSign(float a, float b);

void MinMax(Point& normal, vector<Point>& points, float& min, float& max);

bool MinMaxOverlap(Point minMaxA, Point minMaxB);

float OverlapDistance(Point minMaxA, Point minMaxB);

float minDistance(float& distance, float& min, float& max);

bool SatOverlap(vector<Point> normals, vector<Point> pointsA, vector<Point> pointsB, Point &overlapDir, float& overlap);

/*
  Handles collisions between any two shapes.
  For the push percent, 0.0f means nothing can stop A
  1.0f means nothing can push B
  -1.0f means NO PUSHING FOR EITHER SIDE (collisions off, basically)
*/
bool HandleCollision(Shape* objA, Shape* objB, float pushPercent = -1.0f);

// Point collisions
bool HandlePointvPoint(Point* pointA, Point* pointB, float pushPercent);

bool HandlePointvLine(Point* point, Line* line, float pushPercent);

bool HandlePointvCircle(Point* point, Circle* circle, float pushPercent);

bool HandlePointvBox(Point* point, Box* rectangle, float pushPercent);

// Line collisions
bool HandleLinevPoint(Line* line, Point* point, float pushPercent);

bool HandleLinevLine(Line* lineA, Line* lineB, float pushPercent);

bool HandleLinevCircle(Line* line, Circle* circle, float pushPercent);

bool HandleLinevBox(Line* line, Box* box, float pushPercent);

// Circle collisions
bool HandleCirclevPoint(Circle* circle, Point* point, float pushPercent);

bool HandleCirclevLine(Circle* circle, Line* line, float pushPercent);

bool HandleCirclevCircle(Circle* circleA, Circle* circleB, float pushPercent);

bool HandleCirclevBox(Circle* circle, Box* box, float pushPercent);

// Rectangle collisions
bool HandleBoxvPoint(Box* box, Point* point, float pushPercent);

bool HandleBoxvLine(Box* box, Line* line, float pushPercent);

bool HandleBoxvCircle(Box* box, Circle* circle, float pushPercent);

bool HandleBoxvBox(Box* boxA, Box* boxB, float pushPercent);

