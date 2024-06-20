#include "canvas.h"
#include <qalgorithms.h>

Canvas::Canvas() {}

QVector<QPointF> Canvas::points()
{
    return m_points;
}

void Canvas::setPoints(const QVector<QPointF>& p)
{
    if (p != m_points)
    {
        m_points = p;
        emit pointsChanged();
    }
}

int Canvas::sizePoints()
{
    return m_points.size();
}

QVector<QPointF> Canvas::hull()
{
    return m_hull;
}

void Canvas::setHull(const QVector<QPointF>& h)
{
    if (h != m_hull)
    {
        m_hull = h;
        emit hullChanged();
    }
}

int Canvas::sizeHull()
{
    return m_hull.size();
}

qreal Canvas::area()
{
    return m_area;
}

void Canvas::calculateArea()
{
    calculateHull();
    m_area = 0.0;
    for (int i = 0; i < m_hull.size(); i++)
    {
        QPointF curr = m_hull[i];
        QPointF next, prev;
        if (i == 0)
        {
            prev = m_hull.back();
        } else
        {
            prev = m_hull[i - 1];
        }

        if (i == m_hull.size() - 1)
        {
            next = m_hull.front();
        } else
        {
            next = m_hull[i + 1];
        }
        m_area += curr.x() * (next.y() - prev.y());
    }
    m_area *= 0.5;
    emit areaChanged();
}

void Canvas::addPoint(double x, double y)
{
    m_points.push_back(QPointF(x, y));
    emit sizePointsChanged();
}

void Canvas::clear()
{
    m_points.clear();
    m_hull.clear();
    m_area = 0.0;
    emit sizeHullChanged();
    emit sizePointsChanged();
    emit pointsChanged();
    emit hullChanged();
    emit areaChanged();
}

void Canvas::calculateHull()
{
    m_hull = {};
    int n = m_points.size();
    int first, q, next, i;
    double sign;

    first = 0;
    for (i = 1; i < n; ++i)
    {
        if (m_points[i].x() < m_points[first].x()
            || (m_points[i].x() == m_points[first].x()
            && m_points[i].y() < m_points[first].y()))
        {
            first = i;
        }
    }

    q = first;
    do {
        m_hull.push_back(m_points[q]);
        next = q;
        for (i = n - 1; i >= 0; --i) {
            if (m_points[i].x() != m_points[q].x()
                || m_points[i].y() != m_points[q].y())
            {
                sign = areaTriangle(m_points[q], m_points[i], m_points[next]);

                if (next == q
                    || sign > 0
                    || (sign == 0 && pointInBox(m_points[next], m_points[q], m_points[i])))
                {
                    next = i;
                }
            }
        }
        q = next;
    } while (q != first);
    emit hullChanged();
}

int Canvas::orientation(QPointF p, QPointF q, QPointF r)
{
    double val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

double Canvas::areaTriangle(QPointF p, QPointF q, QPointF r)
{
    return 0.5 * ((q.x() - p.x()) * (r.y() - p.y()) - (r.x() - p.x()) * (q.y() - p.y()));
}

bool Canvas::pointInBox(QPointF p, QPointF q, QPointF r)
{
    return (p.x() >= std::min(q.x(), r.x()) && p.x() <= std::max(q.x(), r.x()) &&
            p.y() >= std::min(q.y(), r.y()) && p.y() <= std::max(q.y(), r.y()));
}
