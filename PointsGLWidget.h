#ifndef POINTSGLWIDGET_H
#define POINTSGLWIDGET_H

#include <QObject>
#include <QtOpenGLWidgets/QtOpenGLWidgets>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QVector3D>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QRandomGenerator>
#include <QMouseEvent>

#include <pdal/Reader.hpp>
#include <pdal/StageFactory.hpp>
#include <pdal/PointView.hpp>


class PointsGlWidget: public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    PointsGlWidget(QWidget *parent = nullptr);
    ~PointsGlWidget();
    void loadLasFile(const char* fileName);
    void drawLasPoints(std::vector<QVector3D> arr_points);
    void switchViewMode(int mode);
    std::vector<QVector3D> points;
    void changeCameraPos(QVector3D pos);
    void stopSignal(bool run);
    bool getStopSignal();


protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    float z;
    float xRot, yRot, zRot;
    QPoint mousePos;
    QTimer timer;
    QVector2D m_mousePosition;
    QQuaternion m_rotation;
    float angle;
    QVector3D m_globalCameraPos;
    int currentViewMode;
    bool canRun;

    QVector3D m_center;
    QVector3D m_radius;
    QVector3D m_rotationAxis;
    float m_rotationAngle;
    QVector3D m_lastMousePosition;
    QVector3D m_mouseDelta;


public slots:
    void updateFrame();
};

#endif // POINTSGLWIDGET_H
