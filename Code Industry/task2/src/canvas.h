#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include <QVector>
#include <QPointF>

class Canvas : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<QPointF> points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(QVector<QPointF> hull READ hull WRITE setHull NOTIFY hullChanged FINAL)
    Q_PROPERTY(qreal area READ area NOTIFY areaChanged)
    Q_PROPERTY(int sizePoints READ sizePoints NOTIFY sizePointsChanged)
    Q_PROPERTY(int sizeHull READ sizeHull NOTIFY sizeHullChanged)

public:
    Canvas();

    QVector<QPointF> points();
    void setPoints(const QVector<QPointF>& p);
    int sizePoints();

    QVector<QPointF> hull();
    void setHull(const QVector<QPointF>& h);
    int sizeHull();

    qreal area();

public slots:
    void calculateArea();
    void addPoint(double x, double y);
    void clear();

signals:
    void pointsChanged();
    void hullChanged();
    void areaChanged();
    void sizePointsChanged();
    void sizeHullChanged();

private:
    QVector<QPointF> m_points = {};
    QVector<QPointF> m_hull;
    qreal m_area = 0.0;

    void calculateHull();
    int orientation(QPointF p, QPointF q, QPointF r);
    double areaTriangle(QPointF p, QPointF q, QPointF r);
    bool pointInBox(QPointF p, QPointF q, QPointF r);
};

#endif // CANVAS_H
