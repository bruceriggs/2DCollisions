#ifndef COLLISIONSTRUCT_H_
#define COLLISIONSTRUCT_H_

   //------------------------------------------------------
   // Helper Function
   //------------------------------------------------------
   bool FloatEquals(const float a, const float b);

   //------------------------------------------------------
   // Types of shapes
   //------------------------------------------------------
   enum ShapeType
   {
      SHAPE_POINT = 0,
      LINE,
      CIRCLE,
      BOX,
      NUM_SHAPES
   };

   //------------------------------------------------------
   // The base class - a shape, which has a ShapeType
   //------------------------------------------------------
   class Shape
   {
   protected:
      // All shapes have a type of shape
      ShapeType shapeType;

   public:
      Shape() {}
      virtual ~Shape() {}
      // Accessor
      ShapeType Type() const { return this->shapeType; }
      inline int NormalCount() const { return 0; }
      inline void Normal(int normalIndex, float& x, float& y) const { x = y = 0.0f; }
      inline void Move(float x, float y) {}
   };

   //------------------------------------------------------
   // A point in 2D space. Internal numbers are floats,
   // but ints can be fetched using the int functions.
   //
   // This is also used as a math VECTOR/NORMAL.
   // Which might bite me in the ass later.
   //------------------------------------------------------
   class Point : public Shape {
   private:
      float x;
      float y;
      bool dirty;
      float length;

      void CalculateLength();
   public:
      // Constructors
      Point(float x = 5.0f, float y = 5.0f);
      Point(int x, int y);

      // Copy Constructor
      Point(const Point& rhs);

      // Assignment Operator
      Point& operator=(const Point& rhs);

      // Equals Check
      bool operator==(const Point& rhs);
      bool operator!=(const Point& rhs);

      // Math Operators
      Point operator-(const Point& rhs) const;
      Point operator+(const Point& rhs) const;
      Point& operator-=(const Point& rhs);
      Point& operator+=(const Point& rhs);
      Point operator*(const float scalar) const;
      Point& operator*=(const float scalar);
      Point operator/(const float scalar) const;
      Point& operator/=(const float scalar);

      // Accessors
      float X() const { return x; }
      float Y() const { return y; }
      int XInt() const { return (int)x; }
      int YInt() const { return (int)y; }
      inline int NormalCount() const { return 0; }
      inline void Normal(int normalIndex, float& x, float& y) const { x = y = 0.0f; }

      // Mutators
      void X(float x) { this->x = x; dirty = true; }
      void Y(float y) { this->y = y; dirty = true; }
      void XInt(int x) { this->x = (float)x; dirty = true; }
      void YInt(int y) { this->y = (float)y; dirty = true; }
      void Move(float x, float y) { this->x += x; this->y += y; dirty = true; }

      // Normalizes a vector
      void Normalize();

      // Returns the length of the vector
      float Length();

      // Returns the SQUARED length of the vector - Much faster than Length()
      float LengthSquared();

      // Does Dot Product against another vector
      float Dot(const Point rhs) const;
   };

   //------------------------------------------------------
   // A line in 2D space. Made up of 2 points.
   //------------------------------------------------------
   class Line : public Shape {
   private:
      Point start;
      Point end;

   public:
      // Constructors
      Line(float startX = -5.0f, float startY = 0.0f, float endX = 5.0f, float endY = 0.0f);
      Line(int startX, int startY, int endX, int endY);
      Line(Point start, Point end);

      // Copy Constructor
      Line(const Line& rhs);

      // Assignment Operator
      Line& operator=(const Line& rhs);

      // Accessors
      Point Start() const { return start; }
      Point End() const { return end; }
      float StartX() const { return start.X(); }
      float StartY() const { return start.Y(); }
      float EndX() const { return end.X(); }
      float EndY() const { return end.Y(); }
      Point LineNormal() const;
      void Normal(int normalIndex, float& x, float& y) const;
      inline int NormalCount() const { return 1; }

      // Mutators
      void Start(Point start) { this->start = start; }
      void End(Point end) { this->end = end; }
      void Start(float x, float y) { this->start.X(x); this->start.Y(y); }
      void End(float x, float y) { this->end.X(x); this->end.Y(y); }
      void StartInt(int x, int y) { this->start.XInt(x); this->start.YInt(y); }
      void EndInt(int x, int y) { this->end.XInt(x); this->end.YInt(y); }
      void Move(float x, float y);
      
   };

   //------------------------------------------------------
   // A circle
   //------------------------------------------------------
   class Circle : public Shape
   {
   private:
      Point center;
      float radius;
   public:
      // Constructor
      Circle(float x = 0.0f, float y = 0.0f, float radius = 5.0f);
      Circle(Point center, float radius);

      // Copy Constructor
      Circle(const Circle& rhs);

      // Assignment Operator
      Circle& operator=(const Circle& rhs);

      // Accessors
      Point Center() const { return center; }
      float CenterX() const { return center.X(); }
      float CenterY() const { return center.Y(); }
      float Radius() const { return radius; }
      float RadiusSquared() const { return radius * radius; }
      inline int NormalCount() const { return 0; }
      inline void Normal(int normalIndex, float& x, float& y) const { x = y = 0.0f; }

      // Mutators
      void Center(Point center) { this->center = center; }
      void Center(float x, float y) { this->center = Point(x, y); }
      void CenterX(float x) { this->center.X(x); }
      void CenterY(float y) { this->center.Y(y); }
      void Radius(float radius) { this->radius = radius; }
      void Move(float xDistance, float yDistance) { this->Center(this->CenterX() + xDistance, this->CenterY() + yDistance); }
   };

   //------------------------------------------------------
   // A box, which could be rotated
   //------------------------------------------------------
   class Box : public Shape {
   public:
      enum DIAGONAL {
         TOPLEFT = 0,
         TOPRIGHT,
         BOTTOMLEFT,
         BOTTOMRIGHT,
         MAX_DIAGONALS
      };

      enum SIDE {
         LEFT = 0,
         TOP,
         RIGHT,
         BOTTOM,
         MAX_SIDES
      };

   private:

      // Members
      Point center;
      Point diagonalVectors[4];
      Point faceNormals[2];
      float width;
      float height;
      float diagonalLength;
      float rotation;

      // Calculates the diagonals of the box
      void CalculateDiagonals();
      float CalculateRadians(float degrees);


   public:
      Box(Point topLeft, Point bottomRight);
      Box(Point center, float width, float height, float rotation);
      Box(float topLeftX = 0.0f, float topLeftY = 0.0f, float width = 128.0f, float height = 128.0f, float rotation = 0.0f);

      // Get all 4 corners

      // Get all 4 lines

      // Get any x or y (top, left, right, bottom)

      // Get any width/height (or half width height)

      // rotation
      Point TopLeft() const;
      Point TopRight() const;
      Point BottomLeft() const;
      Point BottomRight() const;
      Point TL() const;
      Point TR() const;
      Point BL() const;
      Point BR() const;
      Point Corner(DIAGONAL corner) const;
      Line Line(SIDE side) const;
      float Left() const;
      float Right() const;
      float Top() const;
      float Bottom() const;
      inline Point Center() const { return center; }
      inline float Width() const { return width; }
      inline float Height() const { return height; }
      inline float HalfWidth() const { return width * 0.5f; }
      inline float HalfHeight() const { return height * 0.5f; }
      inline float Rotation() const { return rotation; }
      inline int NormalCount() const { return 2; }
      void Normal(int normalIndex, float& x, float& y) const;

      // Mutators
      inline void Center(Point newCenter) { this->center = newCenter; }
      void Width(float newWidth);
      void Height(float newHeight);
      void Rotation(float newRotation);
      void Move(float x, float y) { this->center += Point(x, y); }
   };

#endif // COLLISIONSTRUCT_H_