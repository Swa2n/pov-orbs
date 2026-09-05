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

        inline float Magnitude() {
            return sqrt(this->x*this->x + this->y*this->y);
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

int main(void) {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Ball Game");

    SetTargetFPS(60);

    std::vector<Orb> Orbs;
    Orbs.push_back(Orb(Vec2(500, 500), Vec2(1,1), 40));

    std::vector<Vec2> Corners;
    Corners.push_back(Vec2(0,0));
    Corners.push_back(Vec2(0, screenHeight));
    Corners.push_back(Vec2(screenWidth, screenHeight));
    Corners.push_back(Vec2(screenWidth, 0));

    std::vector<LineSegment> Lines;
    Lines.push_back(LineSegment(Corners[0], Corners[1]));
    Lines.push_back(LineSegment(Corners[1], Corners[2]));
    Lines.push_back(LineSegment(Corners[2], Corners[3]));
    Lines.push_back(LineSegment(Corners[3], Corners[0]));

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);      

        for (Orb& orb : Orbs) {
            // cout << orb << "\n";
            orb.Position += orb.Velocity * delta * 300.0f;
            Vec2 OrbPos = orb.Position;
            Vec2 OrbVel = orb.Velocity;

            for (LineSegment& line : Lines) {
                const Vec2 MidToBall = OrbPos - line.MidPoint();
                const float DotBallToLine = Vec2::DotProduct(MidToBall, line.Normal());

                if (abs(DotBallToLine) <= orb.Radius) {
                    const float DotP1ToBall = Vec2::DotProduct(line.LineVector(), OrbPos - line.Point1);
                    const float DotP2ToBall = Vec2::DotProduct(line.LineVector() * -1, OrbPos - line.Point2);
                    if (DotP1ToBall > 0 && DotP2ToBall > 0) {
                        if (Vec2::DotProduct(OrbVel, line.Normal()) < 0)
                            OrbVel = Vec2::Reflect(OrbVel, line.Normal());
                        else if (Vec2::DotProduct(OrbVel, line.OtherNormal()) < 0)
                            OrbVel = Vec2::Reflect(OrbVel, line.OtherNormal());
                    }
                }
            }

            orb.Velocity = OrbVel;

            DrawCircle(OrbPos.x, OrbPos.y, orb.Radius, LIGHTGRAY);
            DrawCircleLines(OrbPos.x, OrbPos.y, orb.Radius + 0.5, BLACK);
        }

        for (LineSegment& line : Lines) {
            DrawLine(line.Point1.x, line.Point1.y, line.Point2.x, line.Point2.y, BLACK);
        }

        EndDrawing();
    }
    CloseWindow();
    return 1;
}