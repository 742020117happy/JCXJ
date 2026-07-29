#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem> // ✅ 新增：用于绘制独立线段
#include <QWheelEvent>
#include <QResizeEvent>
#include <QMutex>
#include <QPainter>
#include <QList>         // ✅ 新增

class Map_Widget : public QGraphicsView {
    Q_OBJECT
public:
    explicit Map_Widget(QWidget *parent = nullptr);
    ~Map_Widget() override;

public slots:
    void updatePosition(double x, double y, double yaw);
    void clearMap();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    QGraphicsScene *m_scene;
    QGraphicsPolygonItem *m_robotItem;
    
    // ✅ 核心修改：废弃 QPainterPath，改用 QGraphicsLineItem 列表
    QList<QGraphicsLineItem*> m_trajectoryLines;
    QPointF m_lastScenePoint;
    bool m_hasLastPoint = false;
    QPen m_trajectoryPen;

    QMutex m_mutex;
    double m_currentX = 0.0;
    double m_currentY = 0.0;
    double m_currentYaw = 0.0;

    void updateRobotIcon();
};