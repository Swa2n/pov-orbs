#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <raylib.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

class Vec2 {
    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
        return os << "(" << vec.x << ", " << vec.y << ")"; }

    public:
        float x;
        float y;

        Vec2(float x, float y) : x(x), y(y) {}

        inline bool operator==(const Vec2& other) {
            if (x == other.x && y == other.y) return true;
            return false;
        }

        inline bool operator!=(const Vec2& other) {
            if (x != other.x && y != other.y) return true;
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

        static inline const struct VectorZero { operator Vec2() const { return Vec2(0.0f, 0.0f); } } ZERO;
        static inline const struct VectorUp { operator Vec2() const { return Vec2(0.0f, -1.0f); } } UP;
        static inline const struct VectorDown { operator Vec2() const { return Vec2(0.0f, 1.0f); } } DOWN;
        static inline const struct VectorLeft { operator Vec2() const { return Vec2(1.0f, 0.0f); } } LEFT;
        static inline const struct VectorRight { operator Vec2() const { return Vec2(-1.0f, 0.0f); } } RIGHT;
        static inline const struct VectorInvalid { operator Vec2() const { return Vec2(NAN, NAN); } } INVALID;
        static inline const struct VectorMousePos { operator Vec2() const { return Vec2(GetMouseX(), GetMouseY()); } } MOUSE;
        static inline const struct VectorRandom { operator Vec2() const { return Vec2(rand() % 3, rand() % 3); } } RANDOM;

        static inline float DotProduct(Vec2 Vector1, Vec2 Vector2) {
            return Vector1.x * Vector2.x + Vector1.y * Vector2.y; }

        static inline Vec2 Reflect(Vec2 Incidence, Vec2 Normal) {
            return Incidence - Normal * 2 * Vec2::DotProduct(Incidence, Normal); }

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

        inline bool operator==(const LineSegment& other) {
            if (Point1 == other.Point1 && Point2 == other.Point2) return true; 
            return false;
        }

        inline bool operator!=(const LineSegment& other) {
            if (Point1 != other.Point1 && Point2 != other.Point2) return true;
            return false;
        }

        Vec2 LineVector() { return this->Point2 - this->Point1; }
        Vec2 Normal() { return Vec2::Normal(this->Point1, this->Point2); }
        Vec2 OtherNormal() { return Vec2::Normal(this->Point1, this->Point2, false); }
        Vec2 MidPoint() { return this->Point1 + this->LineVector() / 2; }
        float Length() { return this->LineVector().Magnitude(); }

        inline bool IsInvalid() {
            if (Point1.IsInvalid() || Point2.IsInvalid()) return true;
            return false;
        }

        void DetectCollision(Orb &orb) {
            Vec2 &OrbPos = orb.Position;
            Vec2 &OrbVel = orb.Velocity;
            
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
                    if (abs(Vec2::DotProduct(P1ToBall, P1ToBall.Normalized())) <= orb.Radius)
                        OrbVel = Vec2::Reflect(OrbVel, P1ToBall.Normalized());
                    else if (abs(Vec2::DotProduct(P2ToBall, P2ToBall.Normalized())) <= orb.Radius) 
                        OrbVel = Vec2::Reflect(OrbVel, P2ToBall.Normalized());
                }
            }
        }

        static inline const struct LineInvalid { operator LineSegment() const { return LineSegment(Vec2::INVALID, Vec2::INVALID); } } INVALID;
};

const int ScreenWidth = 1920;
const int ScreenHeight = 1080;
std::vector<Orb> Orbs;
LineSegment TempLine = LineSegment::INVALID;
std::vector<LineSegment> Lines;
Vec2 LastClicked = Vec2::INVALID;

void DoFrame() {
    float delta = GetFrameTime();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    //Player Input
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //Checks for player Input
        if (LastClicked.IsInvalid()) { //Check if last clicked is invalid
            LastClicked = Vec2::MOUSE; //if so set it to mouse pos
            TempLine.Point1 = LastClicked;
        }
        else { //if not,
            Lines.push_back(LineSegment(LastClicked, Vec2::MOUSE)); //Add a new segment that has last clicked as p1 and new mouse pos as p2
            LastClicked = Vec2::INVALID; //and reset it back to invalid
            TempLine = LineSegment::INVALID;
        }  
    }

    if (!LastClicked.IsInvalid()) TempLine.Point2 = Vec2::MOUSE;    

    if (IsKeyPressed(KEY_SPACE)) 
        Orbs.push_back(Orb(Vec2::MOUSE, Vec2::RANDOM, 40));

    for (Orb& orb : Orbs) {
        orb.Position += orb.Velocity * delta * 150.0f;

        for (LineSegment& line : Lines) 
            line.DetectCollision(orb);

        Vec2 OrbPos = orb.Position;
        DrawCircle(OrbPos.x, OrbPos.y, orb.Radius, LIGHTGRAY);
        DrawCircleLines(OrbPos.x, OrbPos.y, orb.Radius + 0.5, BLACK);
    }

    if (!TempLine.IsInvalid())
        DrawLine(TempLine.Point1.x, TempLine.Point1.y, TempLine.Point2.x, TempLine.Point2.y, GRAY);;

    for (LineSegment& line : Lines) 
        DrawLine(line.Point1.x, line.Point1.y, line.Point2.x, line.Point2.y, BLACK);

    char fpsArray[20];
    sprintf(fpsArray, "%f", 1 / delta);
    string fpsString = "fps: ";
    fpsString.append(fpsArray);
    if (10 < fpsString.size()) fpsString.erase(10);

    DrawText(fpsString.data(), 2, 0, 20, BLACK);
        
    EndDrawing();
}


int main(void) {
    InitWindow(ScreenWidth, ScreenHeight, "Ball Game");

    Orbs.push_back(Orb(Vec2(500, 500), Vec2::RANDOM, 40));

    Vec2 Corners[4] = {
        Vec2(0,0),
        Vec2(0, ScreenHeight),
        Vec2(ScreenWidth, ScreenHeight),
        Vec2(ScreenWidth, 0)
    };

    Lines.push_back(LineSegment(Corners[0], Corners[1]));
    Lines.push_back(LineSegment(Corners[1], Corners[2]));
    Lines.push_back(LineSegment(Corners[2], Corners[3]));
    Lines.push_back(LineSegment(Corners[3], Corners[0]));

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(DoFrame, 0, 1);
    #else
        SetTargetFPS(100);
        while (!WindowShouldClose()) {
            DoFrame();
        }
    #endif
        CloseWindow();
        return 1;
}