#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

#include <raylib.h>

class Vector {
public:
    int x, y;

    Vector(int x_val, int y_val) : x(x_val), y(y_val) {}

    Vector operator+(const Vector& other) {
        return Vector(x + other.x, y + other.y);
    }
};

class Vec2 {
    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
        return os << "(" << vec.x << ", " << vec.y << ")";
    }

    public:
        float x;
        float y;

        Vec2(float x, float y) : x(x), y(y) {}

        //non-statics(nyon~)
        //Operators
        inline bool operator==(const Vec2& other) {
            if (x == other.x && y == other.y) return true;
            return false;
        }

        inline Vec2 operator+(const Vec2& other) { 
            return Vec2(x + other.x, y + other.y); }
        inline Vec2 operator+(float Scaler) { 
            return Vec2(x + Scaler, y + Scaler); }
        inline void operator+=(const Vec2& other) { 
            x += other.x;
            y += other.y; 
        }
        inline void operator+=(float Scaler) { 
            x += Scaler; 
            y += Scaler; 
        }
        
        inline Vec2 operator-(const Vec2& other) { 
            return Vec2(x - other.x, y - other.y); }
        inline Vec2 operator-(float Scaler) { 
            return Vec2(x - Scaler, y - Scaler); }
        inline void operator-=(const Vec2& other) { 
            x -= other.x;
            y -= other.y; 
        }
        inline void operator-=(float Scaler) { 
            x -= Scaler; 
            y -= Scaler; 
        }

        inline Vec2 operator*(const Vec2& other) { 
            return Vec2(x * other.x, y * other.y); }
        inline Vec2 operator*(float Scaler) { 
            return Vec2(x * Scaler, y * Scaler); }
        inline void operator*=(const Vec2& other) { 
            x *= other.x;
            y *= other.y; 
        }
        inline void operator*=(float Scaler) { 
            x *= Scaler; 
            y *= Scaler; 
        }

        inline Vec2 operator/(const Vec2& other) { 
            return Vec2(x / other.x, y / other.y); }
        inline Vec2 operator/(float Scaler) { 
            return Vec2(x / Scaler, y / Scaler); }
        inline void operator/=(const Vec2& other) { 
            x /= other.x;
            y /= other.y; 
        }
        inline void operator/=(float Scaler) { 
            x /= Scaler; 
            y /= Scaler; 
        }

        //Functions(meow)
        inline bool IsInvalid() {
            if (isnan(x) || isnan(y)) return true;
            return false;
        }

        inline float Magnitude() {
            return sqrt(x*x + y*y);
        }

        inline Vec2 Normalized() {
            Vec2 result = *this;
            return result / result.Magnitude();
        }

        //Staticssss
        //Consts(exlude random will you)
        static inline const struct VectorZero { operator Vec2() const { return Vec2(0.0f, 0.0f); } } ZERO;
        static inline const struct VectorUp { operator Vec2() const { return Vec2(0.0f, -1.0f); } } UP;
        static inline const struct VectorDown { operator Vec2() const { return Vec2(0.0f, 1.0f); } } DOWN;
        static inline const struct VectorLeft { operator Vec2() const { return Vec2(1.0f, 0.0f); } } LEFT;
        static inline const struct VectorRight { operator Vec2() const { return Vec2(-1.0f, 0.0f); } } RIGHT;
        static inline const struct VectorInvalid { operator Vec2() const { return Vec2(NAN, NAN); } } INVALID;
        static inline struct VectorRandom { operator Vec2() const { return Vec2(rand() % 11, rand() % 11); } } RANDOM;

        //Functions(nya~)
        static inline float DotProduct(Vec2 Vector1, Vec2 Vector2) {
            return Vector1.x * Vector2.x + Vector1.y * Vector2.y;
        }

        static inline Vec2 Reflect(Vec2 Incidence, Vec2 Normal) {
            return Incidence - Normal * 2 * Vec2::DotProduct(Incidence, Normal);
        }

        static inline Vec2 Normal(Vec2 Vector1, Vec2 Vector2, bool xNeg = true) {
            float dx = Vector1.x - Vector2.x;
            float dy = Vector1.y - Vector2.y;
            Vec2 Normal = (xNeg) ? Vec2(-dy, dx) : Vec2(dy, -dx);
            return Normal.Normalized();
        }
};

class Orb {
    friend std::ostream& operator<<(std::ostream& os, const Orb& vec) {
        return os << "(" << vec.Position << ", " << vec.Velocity << ", " << vec.Radius << ")";
    };

    public:
        Vec2 Position;
        Vec2 Velocity;
        float Radius;

        Orb(Vec2 Pos, Vec2 Vel, float Rad) : Position(Pos), Velocity(Vel), Radius(Rad) {}
};

class LineSegment {
    friend std::ostream& operator<<(std::ostream& os, const LineSegment& vec) {
        return os << "[" << vec.Point1 << ", " << vec.Point2 << "]";
    };

    public:
        Vec2 Point1;
        Vec2 Point2;

        LineSegment(Vec2 P1, Vec2 P2) : Point1(P1), Point2(P2) {}

        //Non-statics
        Vec2 LineVector() { return this->Point2 - this->Point1; }
        Vec2 Normal() { return Vec2::Normal(this->Point1, this->Point2); }
        Vec2 OtherNormal() { return Vec2::Normal(this->Point1, this->Point2, false); }
        Vec2 MidPoint() { return this->Point1 + this->LineVector() / 2; }
        float Length() { return this->LineVector().Magnitude(); }

        Vec2 DetectCollision(Orb orb) {
            Vec2 OrbPos = orb.Position;
            Vec2 OrbVel = orb.Velocity;
            
            Vec2 MidToBall = OrbPos - this->MidPoint(); // Find the dir vector of Midpoint to the ball
            const float DotBallToLine = Vec2::DotProduct(MidToBall, this->Normal()); // Get the dot of dir vec and the normal
            if (abs(DotBallToLine) <= orb.Radius) { // Compare the abs of the dot to the radius to see of it is apart of the line
                const float DotP1ToBall = Vec2::DotProduct(this->LineVector(), OrbPos - this->Point1); // Gets dot of orbpos and p1
                const float DotP2ToBall = Vec2::DotProduct(this->LineVector() * -1, OrbPos - this->Point2); // Gets dot of orbpos and p2(reverse the line so it points the other way)
                if (DotP1ToBall > 0 && DotP2ToBall > 0) { // Compare these values to see if the ball is within the lines sections(SEGMENTS)
                    if (Vec2::DotProduct(OrbVel, this->Normal()) < 0)
                        OrbVel = Vec2::Reflect(OrbVel, this->Normal());
                    else if (Vec2::DotProduct(OrbVel, this->OtherNormal()) < 0)
                        OrbVel = Vec2::Reflect(OrbVel, this->OtherNormal());
                } else {
                    Vec2 P1ToBall = OrbPos - Point1;
                    Vec2 P2ToBall = OrbPos - Point2;
                    if (abs(Vec2::DotProduct(OrbPos, Point1)) <= orb.Radius){
                        cout << "Point1";
                        OrbVel = Vec2::Reflect(OrbVel, P1ToBall.Normalized());
                    }
                    else if (abs(Vec2::DotProduct(OrbPos, Point2)) <= orb.Radius) {
                        cout << "Point2";
                        OrbVel = Vec2::Reflect(OrbVel, P2ToBall.Normalized());
                    }
                }
            }
            return OrbVel;
        }
};

int main(void) {
    const int ScreenWidth = 1920;
    const int ScreenHeight = 1080;

    InitWindow(ScreenWidth, ScreenHeight, "Ball Game");

    SetTargetFPS(300);

    std::vector<Orb> Orbs;
    Orbs.push_back(Orb(Vec2(500, 500), Vec2(1,1), 40));

    Vec2 Corners[4] = {
        Vec2(0,0),
        Vec2(0, ScreenHeight),
        Vec2(ScreenWidth, ScreenHeight),
        Vec2(ScreenWidth, 0)
    };

    std::vector<LineSegment> Lines;
    Lines.push_back(LineSegment(Corners[0], Corners[1]));
    Lines.push_back(LineSegment(Corners[1], Corners[2]));
    Lines.push_back(LineSegment(Corners[2], Corners[3]));
    Lines.push_back(LineSegment(Corners[3], Corners[0]));

    Vec2 LastClicked = Vec2::INVALID;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        //Player Input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //Checks for player Input
            if (LastClicked.IsInvalid()) //Check if last clicked is invalid
                LastClicked = Vec2(GetMouseX(), GetMouseY()); //if so set it to mouse pos
            else { //if not,
                Lines.push_back(LineSegment(LastClicked, Vec2(GetMouseX(), GetMouseY()))); //Add a new segment that has last clicked as p1 and new mouse pos as p2
                LastClicked = Vec2::INVALID; //and set last click back to invalid
            }  
        }

        //Simulation
        for (Orb& orb : Orbs) {
            orb.Position += orb.Velocity * delta * 300.0f;

            for (LineSegment& line : Lines) 
                orb.Velocity = line.DetectCollision(orb);

            Vec2 OrbPos = orb.Position;
            DrawCircle(OrbPos.x, OrbPos.y, orb.Radius, LIGHTGRAY);
            DrawCircleLines(OrbPos.x, OrbPos.y, orb.Radius + 0.5, BLACK);
        }

        for (LineSegment& line : Lines) 
            DrawLine(line.Point1.x, line.Point1.y, line.Point2.x, line.Point2.y, BLACK);

        EndDrawing();
    }
    CloseWindow();
    return 1;
}