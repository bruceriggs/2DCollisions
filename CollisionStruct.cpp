#include "CollisionStruct.h"

// For FLT_EPSILON
#include <cfloat>
// For sqrtf, and pi
#include <cmath>

const float pi = 3.14159265358f;

bool FloatEquals(const float a, const float b) {
   return (a >= b - FLT_EPSILON && a <= b + FLT_EPSILON);
}

//------------------------------------------------------
// Point Constructor - has defaults of 5.0f, 5.0f
//------------------------------------------------------
Point::Point(float x, float y) {
   this->shapeType = ShapeType::SHAPE_POINT;
   this->x = x;
   this->y = y;
   this->dirty = true;
   length = 0.0f;
}

//------------------------------------------------------
// Point constructor that takes int parameters.
// No defaults supplied.
//------------------------------------------------------
Point::Point(int x, int y) {
   this->shapeType = ShapeType::SHAPE_POINT;
   this->x = (float)x;
   this->y = (float)y;
   this->dirty = true;
   length = 0.0f;
}
//------------------------------------------------------
// Copy Constructor for a Point
//------------------------------------------------------
Point::Point(const Point& rhs) {
   this->shapeType = rhs.shapeType;
   this->x = rhs.x;
   this->y = rhs.y;
   this->dirty = rhs.dirty;
   this->length = rhs.length;
}

//------------------------------------------------------
// Assignment Operator for a Point
//------------------------------------------------------
Point& Point::operator=(const Point& rhs) {
   if (this != &rhs) {
      this->shapeType = rhs.shapeType;
      this->x = rhs.x;
      this->y = rhs.y;
      this->dirty = rhs.dirty;
      this->length = rhs.length;
   }
   return *this;
}

//------------------------------------------------------
// Determines if two points are equal
//------------------------------------------------------
bool Point::operator==(const Point& rhs) {
   return (FloatEquals(this->x, rhs.x) && FloatEquals(this->y, rhs.y));
}

//------------------------------------------------------
// Determines if two points are NOT equal
//------------------------------------------------------
bool Point::operator!=(const Point& rhs) {
   return !(this->x >= rhs.x - FLT_EPSILON && this->x <= rhs.x + FLT_EPSILON
      && this->y >= rhs.y - FLT_EPSILON && this->y <= rhs.y + FLT_EPSILON);
}

//------------------------------------------------------
// Subtracts rhs Point from this Point
//------------------------------------------------------
Point Point::operator-(const Point& rhs) const {
   return Point(this->x - rhs.x, this->y - rhs.y);
}

//------------------------------------------------------
// Adds rhs Point to this Point
//------------------------------------------------------
Point Point::operator+(const Point& rhs) const {
   return Point(this->x + rhs.x, this->y + rhs.y);
}

//------------------------------------------------------
// Subtracts rhs Point from this Point
//------------------------------------------------------
Point& Point::operator-=(const Point& rhs) {
   this->x -= rhs.x;
   this->y -= rhs.y;
   this->dirty = true;
   return *this;
}

//------------------------------------------------------
// Adds rhs Point to this Point
//------------------------------------------------------
Point& Point::operator+=(const Point& rhs) {
   this->x += rhs.x;
   this->y += rhs.y;
   this->dirty = true;
   return *this;
}

//------------------------------------------------------
// Multiplies this Point by a scalar
//------------------------------------------------------
Point Point::operator*(const float scalar) const {
   return Point(this->x * scalar, this->y * scalar);
}

//------------------------------------------------------
// Multiplies this Point by a scalar
//------------------------------------------------------
Point& Point::operator*=(const float scalar) {
   this->x *= scalar;
   this->y *= scalar;
   this->dirty = true;
   return *this;
}

//------------------------------------------------------
// Divides this Point by a scalar
//------------------------------------------------------
Point Point::operator/(const float scalar) const {
   return Point(this->x / scalar, this->y / scalar);
}

//------------------------------------------------------
// Divides this Point by a scalar
//------------------------------------------------------
Point& Point::operator/=(const float scalar) {
   this->x /= scalar;
   this->y /= scalar;
   this->dirty = true;
   return *this;
}

//------------------------------------------------------
// Calculates the Length of the Vector
//------------------------------------------------------
void Point::CalculateLength() {
   this->length = sqrtf(x * x + y * y);
   dirty = false;
}

//------------------------------------------------------
// Normalizes a vector
//------------------------------------------------------
void Point::Normalize() {
   // Ensure the length is correct
   if (dirty) {
      CalculateLength();
   }

   // Divide the vector by it's length
   if (length != 0.0f)
   {
      x /= length;
      y /= length;
      length = 1.0f;
   }
}

//------------------------------------------------------
// Returns the length of the vector
//------------------------------------------------------
float Point::Length() {
   // Update length if needed
   if (dirty) {
      CalculateLength();
   }
   return length;
}

//------------------------------------------------------
// Returns the SQUARED length of the vector - Much faster than Length()
//------------------------------------------------------
float Point::LengthSquared() {
   return Dot(*this);
}

//------------------------------------------------------
// Does Dot Product against another vector
//------------------------------------------------------
float Point::Dot(const Point rhs) const {
   return this->x * rhs.x + this->y * rhs.y;
}

//------------------------------------------------------
// Line constructor that takes 4 floats to represent
// the 2 points on the line.
// Defaults to (-5, 0) through (5, 0)
//------------------------------------------------------
Line::Line(float startX, float startY, float endX, float endY) {
   this->shapeType = LINE;
   this->start.X(startX);
   this->start.Y(startY);
   this->end.X(endX);
   this->end.Y(endY);
}

//------------------------------------------------------
// Line constructor that takes integers which are
// then cast to floats
//------------------------------------------------------
Line::Line(int startX, int startY, int endX, int endY) {
   this->shapeType = LINE;
   this->start.XInt(startX);
   this->start.YInt(startY);
   this->end.XInt(endX);
   this->end.YInt(endY);
}

//------------------------------------------------------
// Line constructor that takes 2 points
//------------------------------------------------------
Line::Line(Point start, Point end) {
   this->shapeType = LINE;
   this->start = start;
   this->end = end;
}

//------------------------------------------------------
// Copy Constructor for a Line
//------------------------------------------------------
Line::Line(const Line& rhs) {
   this->shapeType = LINE;
   this->start.X(rhs.start.X());
   this->start.Y(rhs.start.Y());
   this->end.X(rhs.end.X());
   this->end.Y(rhs.end.Y());
}

//------------------------------------------------------
// Assignment Operator for a Line
//------------------------------------------------------
Line& Line::operator=(const Line& rhs) {
   if (this != &rhs) {
      this->shapeType = LINE;
      this->start.X(rhs.start.X());
      this->start.Y(rhs.start.Y());
      this->end.X(rhs.end.X());
      this->end.Y(rhs.end.Y());
   }
   return *this;
}

//------------------------------------------------------
// Moves a Line by the specified X and Y
//------------------------------------------------------
void Line::Move(float x, float y)
{
   this->start.X(this->start.X() + x);
   this->end.X(this->end.X() + x);
   this->start.Y(this->start.Y() + y);
   this->end.Y(this->end.Y() + y);
}

//------------------------------------------------------
// Gets the collision normal of a line (note this is NOT start to end normal)
//------------------------------------------------------
void Line::Normal(int normalIndex, float& x, float& y) const 
{
   if (normalIndex != 0) {
      x = y = 0.0f;
      return;
   }
   Point normal = this->end - this->start;
   normal.Normalize();
   x = normal.Y();
   y = -normal.X();
}

//------------------------------------------------------
// Get the line normal. (Note that this is start to end normal)
//------------------------------------------------------
Point Line::LineNormal() const
{
   Point normal = this->end - this->start;
   normal.Normalize();
   return normal;
}

//------------------------------------------------------
// Constructor for a Circle that takes an x, y,
// and radius
// Defaults to (0,0) with a radius of 5.0
//------------------------------------------------------
Circle::Circle(float x, float y, float radius) {
   this->shapeType = CIRCLE;
   this->center = Point(x, y);
   this->radius = radius;
}

//------------------------------------------------------
// Constructor for a Circle that takes a point
// and radius
//------------------------------------------------------
Circle::Circle(Point center, float radius) {
   this->shapeType = CIRCLE;
   this->center = center;
   this->radius = radius;
}

//------------------------------------------------------
// Copy Constructor for a Circle
//------------------------------------------------------
Circle::Circle(const Circle& rhs) {
   this->shapeType = CIRCLE;
   this->center = rhs.center;
   this->radius = rhs.radius;
}

//------------------------------------------------------
// Assignment Operator for a Circle
//------------------------------------------------------
Circle& Circle::operator=(const Circle& rhs) {
   if (this != &rhs) {
      this->shapeType = CIRCLE;
      this->center = rhs.center;
      this->radius = rhs.radius;
   }
   return *this;
}

//------------------------------------------------------
// Box Constructor. 0 rotation is assumed
//------------------------------------------------------
Box::Box(Point topLeft, Point bottomRight)
{
   // Set up the easy stuff
   this->shapeType = BOX;
   this->rotation = 0.0f;
   this->width = bottomRight.X() - topLeft.X();
   this->height = bottomRight.Y() - topLeft.Y();
   this->center = Point(topLeft.X() + (width * 0.5f), topLeft.Y() + (height * 0.5f));
   CalculateDiagonals();
}

//------------------------------------------------------
// Assignment Operator for a Circle
//------------------------------------------------------
Box::Box(Point center, float width, float height, float rotation)
{
   // Set up the easy stuff
   this->shapeType = BOX;
   this->center = center;
   this->width = width;
   this->height = height;
   this->rotation = rotation;
   CalculateDiagonals();
}

Box::Box(float topLeftX, float topLeftY, float width, float height, float rotation) {
   // Calculate center
   this->shapeType = BOX;
   this->center = Point(topLeftX + (width * 0.5f), topLeftY + (height * 0.5f));
   this->width = width;
   this->height = height;
   this->rotation = rotation;
   CalculateDiagonals();
}

//------------------------------------------------------
// Gets the specified corner of the box
//------------------------------------------------------
Point Box::Corner(DIAGONAL corner) const
{
   switch (corner) {
      case TOPLEFT:
      {
         return TL();
         break;
      }
      case TOPRIGHT:
      {
         return TR();
         break;
      }
      case BOTTOMLEFT:
      {
         return BL();
         break;
      }
      case BOTTOMRIGHT:
      default:
      {
         return BR();
         break;
      }
   };
}

//------------------------------------------------------
// Calculates the Diagonal Vectors of a box
//------------------------------------------------------
void Box::CalculateDiagonals()
{
   // Make some useful temporary variables
   Point topLeft(center.X() - (width * 0.5f), center.Y() - (height * 0.5f));
   Point bottomRight(center.X() + (width * 0.5f), center.Y() + (height * 0.5f));

   // Get topleft diagonal vector
   diagonalVectors[TOPLEFT] = topLeft - center;
   diagonalLength = diagonalVectors[TOPLEFT].Length();
   diagonalVectors[TOPLEFT].Normalize();
   // Get the topright diagonal vector
   diagonalVectors[TOPRIGHT] = Point(bottomRight.X(), topLeft.Y()) - center;
   diagonalVectors[TOPRIGHT].Normalize();

   // Face normals (assuming no rotation)
   faceNormals[0] = Point(1.0f, 0.0f);
   faceNormals[1] = Point(0.0f, 1.0f);

   if(this->rotation != 0.0f) {
      // Rotation must be applied
      float radians = CalculateRadians(rotation);
      float newX = diagonalVectors[TOPLEFT].X() * cos(radians) - diagonalVectors[TOPLEFT].Y() * sin(radians);
      float newY = diagonalVectors[TOPLEFT].X() * sin(radians) + diagonalVectors[TOPLEFT].Y() * cos(radians);
      diagonalVectors[TOPLEFT] = Point(newX, newY);
      diagonalVectors[TOPLEFT].Normalize();

      newX = diagonalVectors[TOPRIGHT].X() * cos(radians) - diagonalVectors[TOPRIGHT].Y() * sin(radians);
      newY = diagonalVectors[TOPRIGHT].X() * sin(radians) + diagonalVectors[TOPRIGHT].Y() * cos(radians);
      diagonalVectors[TOPRIGHT] = Point(newX, newY);
      diagonalVectors[TOPRIGHT].Normalize();

      // Rotate the face normals too
      newX = faceNormals[0].X() * cos(radians) - faceNormals[0].Y() * sin(radians);
      newY = faceNormals[0].X() * sin(radians) + faceNormals[0].Y() * cos(radians);
      faceNormals[0] = Point(newX, newY);
      faceNormals[0].Normalize();

      newX = faceNormals[1].X() * cos(radians) - faceNormals[1].Y() * sin(radians);
      newY = faceNormals[1].X() * sin(radians) + faceNormals[1].Y() * cos(radians);
      faceNormals[1] = Point(newX, newY);
      faceNormals[1].Normalize();
   }

   // Get the bottomleft diagonal vector
   diagonalVectors[BOTTOMLEFT] = diagonalVectors[TOPRIGHT] * -1.0f;
   // Get the bottomright diagonal vector
   diagonalVectors[BOTTOMRIGHT] = diagonalVectors[TOPLEFT] * -1.0f;
}

//------------------------------------------------------
// Change the width of a box
//------------------------------------------------------
void Box::Width(float newWidth)
{
   this->width = newWidth;
   CalculateDiagonals();
}

//------------------------------------------------------
// Change the height of a box
//------------------------------------------------------
void Box::Height(float newHeight)
{
   this->height = newHeight;
   CalculateDiagonals();
}

//------------------------------------------------------
// Change the rotation of a box
//------------------------------------------------------
void Box::Rotation(float newRotation)
{
   this->rotation = newRotation;
   CalculateDiagonals();
}

//------------------------------------------------------
// Turns degrees into radians
//------------------------------------------------------
float Box::CalculateRadians(float degrees)
{
   return rotation *  pi / 180.0f;
}

//------------------------------------------------------
// Gets the Topleft point of the box
//------------------------------------------------------
Point Box::TopLeft() const
{
   return center + (diagonalVectors[TOPLEFT] * diagonalLength);
}

//------------------------------------------------------
// Gets the TopRight point of the box
//------------------------------------------------------
Point Box::TopRight() const
{
   return center + (diagonalVectors[TOPRIGHT] * diagonalLength);
}

//------------------------------------------------------
// Gets the bottomleft point of the box
//------------------------------------------------------
Point Box::BottomLeft() const
{
   return center + (diagonalVectors[BOTTOMLEFT] * diagonalLength);
}

//------------------------------------------------------
// Gets the bottomright point of the box
//------------------------------------------------------
Point Box::BottomRight() const
{
   return center + (diagonalVectors[BOTTOMRIGHT] * diagonalLength);
}

//------------------------------------------------------
// Gets the topleft point of the box
//------------------------------------------------------
Point Box::TL() const
{
   return center + (diagonalVectors[TOPLEFT] * diagonalLength);
}

//------------------------------------------------------
// Gets the topright point of the box
//------------------------------------------------------
Point Box::TR() const
{
   return center + (diagonalVectors[TOPRIGHT] * diagonalLength);
}

//------------------------------------------------------
// Gets the bottomleft point of the box
//------------------------------------------------------
Point Box::BL() const
{
   return center + (diagonalVectors[BOTTOMLEFT] * diagonalLength);
}

//------------------------------------------------------
// Gets the bottomright point of the box
//------------------------------------------------------
Point Box::BR() const
{
   return center + (diagonalVectors[BOTTOMRIGHT] * diagonalLength);
}

//------------------------------------------------------
// Gets an outer line for a box (regardless of rotation)
//------------------------------------------------------
Line Box::Line(Box::SIDE side) const
{
   auto chosenOne = Line::Line();
   switch (side)
   {
      case TOP:
      {
         chosenOne.Start(center + (diagonalVectors[TOPLEFT] * diagonalLength));
         chosenOne.End(center + (diagonalVectors[TOPRIGHT] * diagonalLength));
         break;
      }
      case BOTTOM:
      {
         chosenOne.Start(center + (diagonalVectors[BOTTOMRIGHT] * diagonalLength));
         chosenOne.End(center + (diagonalVectors[BOTTOMLEFT] * diagonalLength));
         break;
      }
      case LEFT:
      {
         chosenOne.Start(center + (diagonalVectors[BOTTOMLEFT] * diagonalLength));
         chosenOne.End(center + (diagonalVectors[TOPLEFT] * diagonalLength));
         break;
      }
      case RIGHT:
      default:
      {
         chosenOne.Start(center + (diagonalVectors[TOPRIGHT] * diagonalLength));
         chosenOne.End(center + (diagonalVectors[BOTTOMRIGHT] * diagonalLength));
         break;
      }
   };
   return chosenOne;
}

//------------------------------------------------------
// Gets the left X value of the box (IGNORES ROTATION)
//------------------------------------------------------
float Box::Left() const
{
   // Get left line
   auto chosenOne = this->Line(Box::LEFT);
   
   // Get middle point
   return ((chosenOne.End() + chosenOne.Start()) * 0.5f).X();
}

//------------------------------------------------------
// Gets the right x value of the box (IGNORES ROTATION)
//------------------------------------------------------
float Box::Right() const
{
   // Get left line
   auto chosenOne = this->Line(Box::RIGHT);

   // Get the middle point
   return ((chosenOne.End() + chosenOne.Start()) * 0.5f).X();
}

//------------------------------------------------------
// Gets the top y value of the box (IGNORES ROTATION)
//------------------------------------------------------
float Box::Top() const
{
   // Get left line
   auto chosenOne = this->Line(Box::TOP);

   // Get the middle point
   return ((chosenOne.End() + chosenOne.Start()) * 0.5f).Y();
}

//------------------------------------------------------
// Gets the bottom y value of a box (IGNORES ROTATION)
//------------------------------------------------------
float Box::Bottom() const
{
   // Get left line
   auto chosenOne = this->Line(Box::BOTTOM);

   // Get the middle point
   return ((chosenOne.End() + chosenOne.Start()) * 0.5f).Y();
}

void Box::Normal(int normalIndex, float& x, float& y) const
{
   // Quit if it's a bad index
   if (normalIndex > 1 || normalIndex < 0) {
      x = y = 0.0f;
   }
   else {
      x = faceNormals[normalIndex].X();
      y = faceNormals[normalIndex].Y();
   }
}