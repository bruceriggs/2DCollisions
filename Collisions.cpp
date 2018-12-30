#include "Collisions.h"

//------------------------------------------------------
// Returns a normal between two points.
// The first point is the FROM point
// The second point is the TO point (points from -> to)
//------------------------------------------------------
Point GetNormalBetweenPoints(Point& fromPoint, Point& toPoint) {
   Point normal = toPoint - fromPoint;
   normal.Normalize();
   return normal;
}

//------------------------------------------------------
// Given a line and a test point, returns the closest
// point on the line to the test point
//
// The bool is to indicate if you want the closest point
// ON the line segment (since lines are technically infinite)
// The bool defaults to true.
//------------------------------------------------------
Point ClosestPointOnLine(Line& theLine, Point& testPoint, bool pointOnSegment)
{
   return ClosestPointOnLine(theLine.Start(), theLine.End(), testPoint, pointOnSegment);
}

//------------------------------------------------------
// Given a line and a test point, returns the closest
// point on the line to the test point
//
// The bool is to indicate if you want the closest point
// ON the line segment (since lines are technically infinite)
// The bool defaults to true.
//------------------------------------------------------
Point ClosestPointOnLine(Point& startPoint, Point& endPoint, Point& testPoint, bool pointOnSegment) {
   // Get the line's normal
   Point lineNormal = GetNormalBetweenPoints(startPoint, endPoint);

   // Get the vector to the test point
   Point toTestPoint = testPoint - startPoint;

   // Dot the normal to the vector to get the distance in the normal direction
   float distance = toTestPoint.Dot(lineNormal);

   // If the distance is below 0
   Point answer;
   if (distance < 0.0f && pointOnSegment) {
      // Answer is the start point
      answer = startPoint;
   }
   // If the closest point is past the end point
   else if (distance * distance > Point(endPoint - startPoint).LengthSquared() && pointOnSegment) {
      // Answer is the end point
      answer = endPoint;
   }
   else {
      // Otherwise, it's somewhere in the middle of the line (or past start/end points, based on bool)
      answer = startPoint + (lineNormal * distance);
   }

   return answer;
}


//------------------------------------------------------
// Returns the absolute value of a number
//------------------------------------------------------
float absValue(float number)
{
   return (number >= 0.0f ? number : number * -1.0f);
}

bool sameSign(float a, float b) {
   return ((a * b) >= 0.0f);
}

float minDistance(float& distance, float& min, float& max)
{
   return (absValue(min - distance) < absValue(max - distance) ? min - distance : max - distance);
}

void MinMax(Point& normal, vector<Point>& points, float& min, float& max)
{
   // Assumed there is always at least 1 point
   min = max = normal.Dot(points[0]);
   float dot = 0.0f;
   for (unsigned int ii = 1; ii < points.size(); ++ii) {
      dot = points[ii].Dot(normal);
      if (dot < min) min = dot;
      if (dot > max) max = dot;
   }
}

bool MinMaxOverlap(Point minMaxA, Point minMaxB) {
   // Max is less than min of other (false)
   // Min is greater than max of other (false)
   return !(minMaxA.X() > minMaxB.Y() || minMaxA.Y() < minMaxB.X());
}

float OverlapDistance(Point minMaxA, Point minMaxB) {
   if (absValue(minMaxA.X() - minMaxB.Y()) < absValue(minMaxA.Y() - minMaxB.X())) {
      return (minMaxA.X() - minMaxB.Y());
   }
   else {
      return (minMaxA.Y() - minMaxB.X());
   }
}

bool SatOverlap(vector<Point> normals, vector<Point> pointsA, vector<Point> pointsB, Point &overlapDir, float& overlap)
{
   // No normals or points? Bad function call.
   if (normals.size() <= 0
      || pointsA.size() <= 0
      || pointsB.size() <= 0) {
      return false;
   }
   
   // Declare variables
   vector<Point> aMinMaxes;
   vector<Point> bMinMaxes;
   float min, max, distance;
   overlap = 0.0f;
   bool firstOverlap = true;

   // For every normal
   for (unsigned int currentNormal = 0; currentNormal < normals.size(); ++currentNormal) {
      // Get the min/max for each shape
      MinMax(normals[currentNormal], pointsA, min, max);
      aMinMaxes.push_back(Point(min, max));
      MinMax(normals[currentNormal], pointsB, min, max);
      bMinMaxes.push_back(Point(min, max));
   }

   // For every saved min/max
   for (unsigned int ii = 0; ii < aMinMaxes.size(); ++ii) {
      // Save the overlap distance
      if (!MinMaxOverlap(aMinMaxes[ii], bMinMaxes[ii])) {
         // No overlap, thus no collision
         return false;
      }
      // Calculate overlap distance
      distance = OverlapDistance(aMinMaxes[ii], bMinMaxes[ii]);

      // Save the smallest distance & normal
      if (firstOverlap || absValue(distance) < absValue(overlap)) {
         overlapDir = normals[ii];
         overlap = distance;
         firstOverlap = false;
      }
   }

   // We have a collision
   return true;
}


//------------------------------------------------------
// Handles the collision between 2 shapes
//------------------------------------------------------
bool HandleCollision(Shape* objA, Shape* objB, float pushPercent) {
	// Check the validity of the objects
	if(objA != 0 && objB != 0)
	{
		if(objA->Type() == SHAPE_POINT) {
			if(objB->Type() == SHAPE_POINT) {
				return HandlePointvPoint(dynamic_cast<Point*>(objA), dynamic_cast<Point*>(objB), pushPercent);
			}
			else if(objB->Type() == LINE) {
				return HandlePointvLine(dynamic_cast<Point*>(objA), dynamic_cast<Line*>(objB), pushPercent);
			}
			else if(objB->Type() == CIRCLE) {
				return HandlePointvCircle(dynamic_cast<Point*>(objA), dynamic_cast<Circle*>(objB), pushPercent);
			}
			else if(objB->Type() == BOX) {
				return HandlePointvBox(dynamic_cast<Point*>(objA), dynamic_cast<Box*>(objB), pushPercent);
			}
		}
		else if(objA->Type() == LINE) {
			if(objB->Type() == SHAPE_POINT) {
				return HandleLinevPoint(dynamic_cast<Line*>(objA), dynamic_cast<Point*>(objB), pushPercent);
			}
			else if(objB->Type() == LINE) {
				return HandleLinevLine(dynamic_cast<Line*>(objA), dynamic_cast<Line*>(objB), pushPercent);
			}
			else if(objB->Type() == CIRCLE) {
				return HandleLinevCircle(dynamic_cast<Line*>(objA), dynamic_cast<Circle*>(objB), pushPercent);
			}
			else if(objB->Type() == BOX) {
				return HandleLinevBox(dynamic_cast<Line*>(objA), dynamic_cast<Box*>(objB), pushPercent);
			}
		}
		else if(objA->Type() == CIRCLE) {
			if(objB->Type() == SHAPE_POINT) {
            return HandleCirclevPoint(dynamic_cast<Circle*>(objA), dynamic_cast<Point*>(objB), pushPercent);
			}
			else if(objB->Type() == LINE) {
            return HandleCirclevLine(dynamic_cast<Circle*>(objA), dynamic_cast<Line*>(objB), pushPercent);
			}
			else if(objB->Type() == CIRCLE) {
            return HandleCirclevCircle(dynamic_cast<Circle*>(objA), dynamic_cast<Circle*>(objB), pushPercent);
			}
			else if(objB->Type() == BOX) {
            return HandleCirclevBox(dynamic_cast<Circle*>(objA), dynamic_cast<Box*>(objB), pushPercent);
			}
		}
		else if(objA->Type() == BOX) {
			if(objB->Type() == SHAPE_POINT) {
            return HandleBoxvPoint(dynamic_cast<Box*>(objA), dynamic_cast<Point*>(objB), pushPercent);
			}
			else if(objB->Type() == LINE) {
            return HandleBoxvLine(dynamic_cast<Box*>(objA), dynamic_cast<Line*>(objB), pushPercent);
			}
			else if(objB->Type() == CIRCLE) {
            return HandleBoxvCircle(dynamic_cast<Box*>(objA), dynamic_cast<Circle*>(objB), pushPercent);
			}
			else if(objB->Type() == BOX) {
            return HandleBoxvBox(dynamic_cast<Box*>(objA), dynamic_cast<Box*>(objB), pushPercent);
			}
		}
	}
  return false;
}

//------------------------------------------------------
// Handles collision between 2 points
// This would be a very rare occasion!
// If push percent < 0.0f, nobody would get pushed
// If push percent <= 0.5f, pointB would get pushed
// If push percent > 0.5f, pointA would get pushed
//------------------------------------------------------
bool HandlePointvPoint(Point* pointA, Point* pointB, float pushPercent) {
	if(pointA == pointB) {
		// A  gets pushed
		if(pushPercent > 0.5f) {
			pointA->X(pointA->X() - 1);
		}
		// B gets pushed
		else if(pushPercent >= 0.0f) {
			pointB->X(pointB->X() - 1);
		}
		return true;
	}
	return false;
}

//------------------------------------------------------
// Handles collision between a point and a line
// This would be a very rare occasions.
// If push percent < 0.0f, nobody will get pushed
// If push percent <= 0.5f, line will get pushed
// If push percent > 0.5f, point will get pushed
//------------------------------------------------------
bool HandlePointvLine(Point* point, Line* line, float pushPercent) {
   // Get the closest point on the line to our point
   Point closestPoint = ClosestPointOnLine(line->Start(), line->End(), *point);

   // If the closest point matches the point
   if(closestPoint == *point) {
      // They collided. Push someone.
      Point lineNormal = line->LineNormal();

      if(pushPercent > 0.5f) {
         // Point gets pushed
         if(Point(-1.0f, 0.0f) != lineNormal && Point(1.0f, 0.0f) != lineNormal) {
            point->X(point->X() - 1.0f);
         }
         else {
            point->Y(point->Y() - 1.0f);
         }
      }
      else if(pushPercent >= 0.0f) {
         // Line gets pushed
         if(Point(-1.0f, 0.0f) != lineNormal && Point(1.0f, 0.0f) != lineNormal) {
            line->Start(line->StartX() + 1, line->StartY());
            line->End(line->EndX() + 1, line->EndY());
         }
         else {
            line->Start(line->StartX(), line->StartY() + 1);
            line->End(line->EndX(), line->EndY() + 1);
         }
      }
      return true;
   }
   return false;
}

//------------------------------------------------------
// Handles the collision between a point and a circle.
// If the push percentage is -1.0f, nothing is pushed
//
// However, any number from 0.0f to 1.0f will be applied
// as a push percentage on the point, with the remainder
// being applied to the circle
//
// Example: 0.25f will push the point 25% of the distance
// and the circle will be pushed the remainder 75%
//------------------------------------------------------
bool HandlePointvCircle(Point* point, Circle* circle, float pushPercent)
{
	// Is the distance from Point to Center less than Radius?
	Point pointToCircle = circle->Center() - *point;
	Point toCircleNormal = GetNormalBetweenPoints(*point, circle->Center());
	float distanceSquared = pointToCircle.Dot(pointToCircle);

	// Is the point within the circle?
	if(distanceSquared <= circle->RadiusSquared()) {
		// Collision!
		// Pushing?
		if(pushPercent >= 0.0f) {
			// Determine the push percentages
			float pointPush = pushPercent;
			float circlePush = 1.0f - pushPercent;
			
			// Determine the actual pushing distance
			float distance = pointToCircle.Length();
			distance = circle->Radius() - distance;

			// Push the circle
			// circle Center + (away Vector * distance * percent)

         Point temp = (toCircleNormal * (distance * circlePush));
         circle->Move(temp.X(), temp.Y());

			// Push the point
			// point + (away vector * distance * percent)
			*point = Point(*point + ((toCircleNormal * -1.0f) * (distance * pointPush)));
		}
		return true;
	}
	return false;
}

//------------------------------------------------------
// Handles the collision between a point and a box.
// If the push percentage is -1.0f, nothing is pushed
//
// However, any number from 0.0f to 1.0f will be applied
// as a push percentage on the point, with the remainder
// being applied to the box
//
// Example: 0.25f will push the point 25% of the distance
// and the box will be pushed the remainder 75%
//------------------------------------------------------
bool HandlePointvBox(Point* point, Box* box, float pushPercent) {
   // Useful variables
   vector<Point> normals;
   vector<Point> shapeA;
   vector<Point> shapeB;
   Point finalNormal;
   float finalMin;
   float tempX, tempY;

   // Create Normal array
   box->Normal(0, tempX, tempY);
   normals.push_back(Point(tempX, tempY));
   box->Normal(1, tempX, tempY);
   normals.push_back(Point(tempX, tempY));

   // Create PointsA Array
   shapeA.push_back(box->TL());
   shapeA.push_back(box->TR());
   shapeA.push_back(box->BL());
   shapeA.push_back(box->BR());

   // Create PointsB Array
   shapeB.push_back(*point);

   // If there is a collision
   if (SatOverlap(normals, shapeA, shapeB, finalNormal, finalMin)) {
      // Push the shapes?
      if (pushPercent != -1.0f) {
         // Use the final Min & final Normal to push by percentage
         Point moveDistance = finalNormal * finalMin;
         point->Move(moveDistance.X() * pushPercent, moveDistance.Y() * pushPercent);
         moveDistance *= -1.0f;
         box->Move(moveDistance.X() * (1.0f - pushPercent), moveDistance.Y() * (1.0f - pushPercent));
      }
      // We have collided
      return true;
   }

   // If we made it this far, there's no collision
   return false;
}

bool HandleLinevPoint(Line* line, Point* point, float pushPercent) {
	return HandlePointvLine(point, line, (pushPercent < 0.0f ? pushPercent : 1.0f - pushPercent));
}

bool HandleLinevLine(Line* lineA, Line* lineB, float pushPercent) {

   // Declare variables
   Point collideSpot;
   Point pushDir;
   float pushDist, temp;
   float a1, a2, b1, b2, c1, c2;
   float r1, r2, r3, r4;
   float denom, offset, num;

   a1 = lineA->End().Y() - lineA->Start().Y();
   b1 = lineA->Start().X() - lineA->End().X();
   c1 = (lineA->End().X() * lineA->Start().Y()) - (lineA->Start().X() * lineA->End().Y());

   r3 = ((a1 * lineB->Start().X()) + (b1 * lineB->Start().Y()) + c1);
   r4 = ((a1 * lineB->End().X()) + (b1 * lineB->End().Y()) + c1);

   if ((r3 != 0) && (r4 != 0) && sameSign(r3, r4)) {
      return false;
   }

   // Compute a2, b2, c2
   a2 = lineB->End().Y() - lineB->Start().Y();
   b2 = lineB->Start().X() - lineB->End().X();
   c2 = (lineB->End().X() * lineB->Start().Y()) - (lineB->Start().X() * lineB->End().Y());
   // Compute r1 and r2
   r1 = (a2 * lineA->Start().X()) + (b2 * lineA->Start().Y()) + c2;
   r2 = (a2 * lineA->End().X()) + (b2 * lineA->End().Y()) + c2;

   if ((r1 != 0) && (r2 != 0) && (sameSign(r1, r2))) {
      return false;
   }

   // Line segments intersect: compute intersection point.
   denom = (a1 * b2) - (a2 * b1); 

   // Same line? (collinear)
   if (denom == 0) {
      return false;
   }

   if (denom < 0) {
      offset = -denom / 2;
   }
   else {
      offset = denom / 2;
   }

   num = (b1 * c2) - (b2 * c1);
   if (num < 0) {
      collideSpot.X((num - offset) / denom);
   }
   else {
      collideSpot.X((num + offset) / denom);
   }

   num = (a2 * c1) - (a1 * c2);

   if (num < 0) {
      
      collideSpot.Y((num - offset) / denom);
   }
   else {
      collideSpot.Y((num + offset) / denom);
   }

   // Determine smallest collide distance
   // A start
   pushDist = (collideSpot - lineA->Start()).Dot(collideSpot - lineA->Start());
   pushDir = lineA->Start();

   // A end
   temp = (collideSpot - lineA->End()).Dot(collideSpot - lineA->End());
   if (temp < pushDist) {
      pushDir = lineA->End();
      pushDist = temp;
   }

   // B Start
   temp = (collideSpot - lineB->Start()).Dot(collideSpot - lineB->Start());
   if (temp < pushDist) {
      pushDir = lineB->Start();
      pushDist = temp;
   }

   // B end
   temp = (collideSpot - lineB->End()).Dot(collideSpot - lineB->End());
   if (temp < pushDist) {
      pushDir = lineB->End();
      pushDist = temp;
   }

   // Now we have the smallest distance point, actually calculate the distance
   pushDir = collideSpot - pushDir;
   pushDist = pushDir.Length();
   pushDist += 0.1f;
   pushDir.Normalize();
   pushDir *= pushDist;

   // Pushing?
   if (pushPercent >= 0.0f) {
      // Push the 2 lines
      Point tempPush = pushDir * pushPercent;
      lineA->Move(tempPush.X(), tempPush.Y());
      tempPush = (pushDir * -1.0f) * (1.0f - pushPercent);
      lineB->Move(tempPush.X(), tempPush.Y());
   }

   return true;
}


bool HandleLinevCircle(Line* line, Circle* circle, float pushPercent) {
	// Find the closest point on the line to the circle center
	Point closestPoint = ClosestPointOnLine(line->Start(), line->End(), circle->Center());

	// Determine if they collided
	bool collided = HandlePointvCircle(&closestPoint, circle, -1.0f);

	// If they did collide
	if(collided && pushPercent >= 0.0f) {
		// Get the distance we need to push
		// And a vector in the right direction, of that distance
		Point vectorToPoint = closestPoint - circle->Center();
		float distance = vectorToPoint.Length();
		vectorToPoint.Normalize();
		distance = circle->Radius() - distance;

		// Calculate the push for each object
		Point linePush = vectorToPoint * (distance * pushPercent);
		Point circlePush = vectorToPoint * -1.0f * (distance * (1.0f - pushPercent));

		// Actually push the objects
		line->Start(line->Start() + linePush);
		line->End(line->End() + linePush);
		circle->Center(circle->Center() + circlePush);
	}

	return collided;
}

bool HandleLinevBox(Line* line, Box* box, float pushPercent) {
   vector<Point> normals;
   vector<Point> shapeA;
   vector<Point> shapeB;
   Point overlapDir;
   float x, y, overlap;

   // Add box normals
   box->Normal(0, x, y);
   normals.push_back(Point(x, y));
   box->Normal(1, x, y);
   normals.push_back(Point(x, y));
   
   // Add line normals
   line->Normal(0, x, y);
   normals.push_back(Point(x, y));

   // Add line points
   shapeA.push_back(line->Start());
   shapeA.push_back(line->End());

   // Add box points
   shapeB.push_back(box->TL());
   shapeB.push_back(box->TR());
   shapeB.push_back(box->BL());
   shapeB.push_back(box->BR());

   // If they collide
   if (SatOverlap(normals, shapeA, shapeB, overlapDir, overlap)) {
      // Pushing?
      if (pushPercent != -1.0f) {
         // Push line
         Point push = (overlapDir * -1.0f) * (overlap * pushPercent);
         line->Move(push.X(), push.Y());
         // Push the box
         push = (overlapDir) * (overlap * (1.0f - pushPercent));
         box->Move(push.X(), push.Y());
      }

      return true;
   }
   return false;
}

bool HandleCirclevPoint(Circle* circle, Point* point, float pushPercent) {
	return HandlePointvCircle(point, circle, (pushPercent < 0.0f ? pushPercent : 1.0f - pushPercent));
}

bool HandleCirclevLine(Circle* circle, Line* line, float pushPercent) {
	return HandleLinevCircle(line, circle, (pushPercent < 0.0f ? pushPercent : 1.0f - pushPercent));
}

bool HandleCirclevCircle(Circle* circleA, Circle* circleB, float pushPercent) {
	// Get the squared distance between the 2 centers
	Point toA = circleA->Center() - circleB->Center();
	float squaredDistance = toA.LengthSquared();
	float radiusSumSquared = (circleA->Radius() + circleB->Radius()) * (circleA->Radius() + circleB->Radius());

	// Did they collide?
	if(squaredDistance <= radiusSumSquared) {
		
		// Are we pushing?
		if(pushPercent >= 0.0f) {
			// Calculate the distance
			float distance = (circleA->Radius() + circleB->Radius()) - toA.Length();
			toA.Normalize();

			// Push them
			circleA->Center(circleA->Center() + (toA * (distance * pushPercent)));
			circleB->Center(circleB->Center() + (toA * -1.0f * (distance * (1.0f - pushPercent))));
		}

		return true;
	}

	// No collision
	return false;
}

bool HandleCirclevBox(Circle* circle, Box* box, float pushPercent) {
   Point v;
   Point currentBoxCorner;
   Point centerBox;
   bool first = true;
   float max = 0.0f;

   Point boxToCircle = circle->Center() - box->Center();
   Point boxToCircleNormal = boxToCircle;
   boxToCircleNormal.Normalize();

   // Get the maximum
   // For every box corner
   for (int ii = 0; ii < Box::MAX_DIAGONALS; ++ii) {
      currentBoxCorner = box->Corner((Box::DIAGONAL)ii);
      v = currentBoxCorner - box->Center();
      float currentProj = v.Dot(boxToCircleNormal);

      if (first || max < currentProj) {
         max = currentProj;
         first = false;
      }
   }

   float pushAmount = boxToCircle.Length() - max - circle->Radius();
   if (pushAmount > 0.0f
      && boxToCircle.Length() > 0) {
      return false;
   }
   else {
      // Pushing?
      if (pushPercent != -1.0f) {
         Point movement = boxToCircleNormal * (pushAmount * pushPercent);
         box->Move(movement.X(), movement.Y());
         movement = (boxToCircleNormal * -1.0f) * (pushAmount * (1.0f - pushPercent));
         circle->Move(movement.X(), movement.Y());
      }
      return true;
   }
}

bool HandleBoxvPoint(Box* box, Point* point, float pushPercent) {
	return HandlePointvBox(point, box, (pushPercent < 0.0f ? pushPercent : 1.0f - pushPercent));
}

bool HandleBoxvLine(Box* box, Line* line, float pushPercent) {
	return HandleLinevBox(line, box, (pushPercent < 0.0f ? pushPercent : 1.0f - pushPercent));
}

bool HandleBoxvCircle(Box* box, Circle* circle, float pushPercent) {
	return HandleCirclevBox(circle, box, (pushPercent < 0.0f ? pushPercent : 1.0f - pushPercent));
}

bool HandleBoxvBox(Box* boxA, Box* boxB, float pushPercent) {
   // Declare useful variables
   vector<Point> normals;
   vector<Point> shapeA;
   vector<Point> shapeB;
   Point overlapDir;
   float overlap, x, y;

   // Add box A normals
   boxA->Normal(0, x, y);
   normals.push_back(Point(x, y));
   boxA->Normal(1, x, y);
   normals.push_back(Point(x, y));

   // Add box b normals
   boxB->Normal(0, x, y);
   normals.push_back(Point(x, y));
   boxB->Normal(1, x, y);
   normals.push_back(Point(x, y));

   // Add box A points
   shapeA.push_back(boxA->TL());
   shapeA.push_back(boxA->TR());
   shapeA.push_back(boxA->BL());
   shapeA.push_back(boxA->BR());
   
   // Add box B points
   shapeB.push_back(boxB->TL());
   shapeB.push_back(boxB->TR());
   shapeB.push_back(boxB->BL());
   shapeB.push_back(boxB->BR());

   if (SatOverlap(normals, shapeA, shapeB, overlapDir, overlap))
   {
      // Push the other first
      Point movement = overlapDir * (overlap * pushPercent);
      boxB->Move(movement.X(), movement.Y());
      movement = (overlapDir * -1.0f) * (overlap * (1.0f - pushPercent));
      boxA->Move(movement.X(), movement.Y());
      return true;
   }
   return false;
}