//
// Created by Harshad on 7/17/2025.
//

#include "Item.h"

#include <numbers>

float Item::getWidth() const
{
    return m_width;
}

float& Item::getHeight()
{
    return m_height;
}

Vector2 Item::getPos()
{
    return m_pos;
}

inline float tau = 2 * (std::numbers::pi);

void Item::rotate(Ball& ball, Rectangle& itemRect, float& orbitSpeed, float& angle, float& drawAngle, float& width, float& height, float& radiusOffset, float& direction)
{
    float increment = (tau / 360.0f) * orbitSpeed * direction;
    Vector2 origin = Vector2(b2Body_GetPosition(ball.getBallId()).x * 30, b2Body_GetPosition(ball.getBallId()).y * 30);
    float radius = ball.getRadius() + radiusOffset;

    angle += increment;
    if (angle > tau)
        angle -= tau;

    itemRect = {origin.x + radius * cos(angle), origin.y + radius * sin(angle), width, height};
    drawAngle = RAD2DEG * (angle + std::numbers::pi / 2.0f);
}


void Item::render()
{
    DrawRectanglePro(m_itemRect, Vector2(m_width / 2, m_height / 2), m_drawAngle, BLUE);
    DrawRectanglePro(m_headRect, Vector2(m_width / 2, m_height / 2), m_drawAngle, RED);

}

Vector2 Item::Normalize(Vector2 v)
{
    float length = sqrt(v.x * v.x + v.y * v.y);
    return (length == 0) ? Vector2(0, 0) : Vector2(v.x / length, v.y / length);
}

float Item::Dot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2 Item::Perpendicular(Vector2 v)
{
    return Vector2(-v.y, v.x);
}

Vector2 Item::RotatePoint(Vector2 point, Vector2 origin, float angleRad)
{
    float s = sinf(angleRad);
    float c = cosf(angleRad);

    point = Vector2Subtract(point, origin);
    Vector2 rotated = {point.x * c - point.y * s, point.x * s + point.y * c};
    return Vector2Add(rotated, origin);
}

Item::Projection Item::projectPolygon(const std::vector<Vector2>& vertices, Vector2 axis)
{
    float min = Dot(vertices[0], axis);
    float max = min;

    for (int i{ 0 }; i < vertices.size(); ++i)
    {
        float proj = Dot(vertices[i], axis);
        if (proj < min) min = proj;
        if (proj > max) max = proj;
    }
    return {min, max};
}

bool Item::Overlaps(Projection a, Projection b)
{
    return !(a.max < b.min || b.max < a.min);
}

std::vector<Vector2> Item::getRotatedRect(Vector2 center, float width, float height, float angleRad)
{
    std::vector<Vector2> corners = {
        {-width / 2, -height / 2},
        {width / 2, -height / 2},
        {width / 2, height / 2},
        {-width / 2, height / 2}
    };

    for (Vector2& v : corners)
    {
        float s = sinf(angleRad);
        float c = cosf(angleRad);
        float x = v.x * c - v.y * s;
        float y = v.x * s + v.y * c;
        v = Vector2{center.x + x, center.y + y};
    }

    return corners;
}

bool Item::satCollisions(const std::vector<Vector2>& polyA, const std::vector<Vector2>& polyB)
{
    std::vector<Vector2> axes;

    auto getAxes = [&](const std::vector<Vector2>& poly)
    {
        for (int i{ 0 }; i < poly.size(); ++i)
        {
            Vector2 p1 = poly[i];
            Vector2 p2 = poly[(i + 1) % poly.size()];
            axes.push_back(Normalize(Perpendicular(Vector2Subtract(p2, p1))));
        }
    };

    getAxes(polyA);
    getAxes(polyB);

    for (const Vector2& axis : axes)
    {
        if (!Overlaps(projectPolygon(polyA, axis), projectPolygon(polyB, axis)))
        {
            return false;
        }
    }

    return true;
}

bool Item::satCircleVsPolygon(Vector2 circleCenter, float radius, const std::vector<Vector2>& poly)
{
    float minOverlap = INFINITY;
    Vector2 smallestAxis = {};

    for (int i{ 0 }; i < poly.size(); ++i)
    {
        Vector2 p1 = poly[i];
        Vector2 p2 = poly[(i + 1) % poly.size()];
        Vector2 edge = Vector2Subtract(p2, p1);
        Vector2 axis = Normalize(Perpendicular(edge));

        float centerProj = Dot(circleCenter, axis);
        Projection polyProj = projectPolygon(poly, axis);

        float minC = centerProj - radius;
        float maxC = centerProj + radius;

        if (!Overlaps({minC, maxC}, polyProj))
        {
            return false;
        }
    }

    float closestDist = INFINITY;
    Vector2 closestPoint = {};

    for (const Vector2& v : poly)
    {
        float dist = Vector2Distance(v, circleCenter);
        if (dist < closestDist)
        {
            closestDist = dist;
            closestPoint = v;
        }
    }

    Vector2 axis = Normalize(Vector2Subtract(closestPoint, circleCenter));
    float centerProj = Dot(circleCenter, axis);
    Projection polyProj = projectPolygon(poly, axis);

    float minC = centerProj - radius;
    float maxC = centerProj + radius;

    if (!Overlaps({minC, maxC}, polyProj))
    {
        return false;
    }

    return true;
}






