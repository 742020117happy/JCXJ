#include "Map_Widget.h"
#include <cmath>
#include <QtMath>
#include <algorithm> // for std::min, std::max

Map_Widget::Map_Widget(QWidget *parent) : QGraphicsView(parent) {
    // 1. 初始化场景：初始范围 50x50 米
    m_scene = new QGraphicsScene(-25.0, -25.0, 50.0, 50.0, this);
    m_scene->setBackgroundBrush(QColor(245, 245, 245));
    setScene(m_scene);

    // 2. 配置视图属性
    setRenderHint(QPainter::Antialiasing, true);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // ✅ 3. 配置轨迹线画笔 (纯黑，2像素，不随缩放变粗)
    m_trajectoryPen = QPen(Qt::black, 2, Qt::SolidLine);
    m_trajectoryPen.setCosmetic(true);
    m_trajectoryPen.setCapStyle(Qt::RoundCap); // 圆角端点，连接更平滑

    // 4. 初始化机器人图标
    QPolygonF robotPolygon;
    robotPolygon << QPointF(0.0, 0.15) << QPointF(0.1, 0.05) << QPointF(0.1, -0.15)
                 << QPointF(-0.1, -0.15) << QPointF(-0.1, 0.05);
    
    QPen robotPen(Qt::white, 1);
    robotPen.setCosmetic(true);
    QBrush robotBrush(QColor(255, 60, 60));
    
    m_robotItem = m_scene->addPolygon(robotPolygon, robotPen, robotBrush);
    m_robotItem->setZValue(20); // 确保机器人在轨迹线上方
    
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

Map_Widget::~Map_Widget() {}

// 核心重写：高性能绘制背景网格
void Map_Widget::drawBackground(QPainter *painter, const QRectF &rect) {
    QGraphicsView::drawBackground(painter, rect);
    double scale = transform().m11(); 
    QVector<double> gridSteps = {5.0, 0.5};
    QVector<QColor> gridColors = { QColor(140, 140, 140), QColor(210, 210, 210) };

    for (int i = 0; i < gridSteps.size(); ++i) {
        double step = gridSteps[i];
        if (step * scale < 15.0) continue; 

        QPen pen(gridColors[i]);
        pen.setCosmetic(true);
        pen.setWidth(step >= 5.0 ? 2 : 1);
        painter->setPen(pen);

        double left = std::floor(rect.left() / step) * step;
        double right = std::ceil(rect.right() / step) * step;
        double top = std::floor(rect.top() / step) * step;
        double bottom = std::ceil(rect.bottom() / step) * step;

        for (double x = left; x <= right; x += step) {
            if (qFuzzyIsNull(x)) x = 0.0; 
            painter->drawLine(QLineF(x, rect.top(), x, rect.bottom()));
        }
        for (double y = top; y <= bottom; y += step) {
            if (qFuzzyIsNull(y)) y = 0.0;
            painter->drawLine(QLineF(rect.left(), y, rect.right(), y));
        }
    }

    QPen axisPen(QColor(80, 80, 80), 2);
    axisPen.setCosmetic(true);
    painter->setPen(axisPen);
    painter->drawLine(QLineF(0, rect.top(), 0, rect.bottom()));
    painter->drawLine(QLineF(rect.left(), 0, rect.right(), 0));
}

// ✅ 核心修复：更新位置和绘制轨迹
void Map_Widget::updatePosition(double x, double y, double yaw) {
    QMutexLocker locker(&m_mutex);
    m_currentX = x;
    m_currentY = y;
    m_currentYaw = yaw;

    // 坐标系转换：惯导 (X东, Y北) -> Scene (X右, Y下)
    double sceneX = m_currentX;
    double sceneY = -m_currentY;
    QPointF currentPoint(sceneX, sceneY);

    // ✅ 核心修复 1：动态扩展 sceneRect，防止轨迹超出边界被裁剪
    QRectF currentSceneRect = m_scene->sceneRect();
    if (!currentSceneRect.contains(currentPoint)) {
        double margin = 10.0; // 扩展 10 米余量
        double newLeft = std::min(currentSceneRect.left(), sceneX - margin);
        double newTop = std::min(currentSceneRect.top(), sceneY - margin);
        double newRight = std::max(currentSceneRect.right(), sceneX + margin);
        double newBottom = std::max(currentSceneRect.bottom(), sceneY + margin);
        m_scene->setSceneRect(newLeft, newTop, newRight - newLeft, newBottom - newTop);
    }

    // ✅ 核心修复 2：使用 QGraphicsLineItem 绘制轨迹，彻底解决隐形 Bug
    if (m_hasLastPoint) {
        // 添加一条从上一个点到当前点的独立线段
        QGraphicsLineItem* line = m_scene->addLine(QLineF(m_lastScenePoint, currentPoint), m_trajectoryPen);
        line->setZValue(10); // 确保在网格之上，机器人之下
        m_trajectoryLines.append(line);

        // 限制最大线段数量 (保留最近 10000 条)，防止内存泄漏
        if (m_trajectoryLines.size() > 10000) {
            m_scene->removeItem(m_trajectoryLines.first());
            delete m_trajectoryLines.first();
            m_trajectoryLines.removeFirst();
        }
    }
    
    m_lastScenePoint = currentPoint;
    m_hasLastPoint = true;

    updateRobotIcon();
}

void Map_Widget::updateRobotIcon() {
    double sceneX = m_currentX;
    double sceneY = -m_currentY;
    m_robotItem->setPos(sceneX, sceneY);
    m_robotItem->setRotation(-m_currentYaw * 180.0 / M_PI);
}

void Map_Widget::clearMap() {
    QMutexLocker locker(&m_mutex);
    for (QGraphicsLineItem* line : m_trajectoryLines) {
        m_scene->removeItem(line);
        delete line;
    }
    m_trajectoryLines.clear();
    m_hasLastPoint = false;
    
    m_currentX = 0.0;
    m_currentY = 0.0;
    m_currentYaw = 0.0;
    
    updateRobotIcon();
    
    m_scene->setSceneRect(-25.0, -25.0, 50.0, 50.0);
    resetTransform();
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void Map_Widget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void Map_Widget::wheelEvent(QWheelEvent *event) {
    double scaleFactor = 1.15;
    if (event->angleDelta().y() < 0) {
        scaleFactor = 1.0 / scaleFactor;
    }
    scale(scaleFactor, scaleFactor);
    event->accept();
}