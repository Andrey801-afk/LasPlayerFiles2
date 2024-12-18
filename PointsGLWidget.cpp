#include "PointsGLWidget.h"
#include "QMatrix4x4"

PointsGlWidget::PointsGlWidget(QWidget *parent): QOpenGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    timer.start(17);
    m_globalCameraPos = {20,-30,-80};
    currentViewMode = 1;
}

PointsGlWidget::~PointsGlWidget()
{

}

void PointsGlWidget::loadLasFile(const char* fileName)
{

    if (!canRun) return;
    std::string las_filepath = fileName;

    pdal::StageFactory factory;
    pdal::Stage* reader = factory.createStage("readers.las");

    pdal::Options options;
    options.add("filename", las_filepath);
    reader->setOptions(options);

    pdal::PointTable table;
    reader->prepare(table);
    points.clear();

    pdal::PointViewSet viewSet = reader->execute(table);

    for (pdal::PointViewPtr view : viewSet) {
        for (pdal::PointId idx = 0; idx < view->size(); ++idx) {
            float x = view->getFieldAs<float>(pdal::Dimension::Id::X, idx);
            float y = view->getFieldAs<float>(pdal::Dimension::Id::Y, idx);
            float z = view->getFieldAs<float>(pdal::Dimension::Id::Z, idx);
            points.push_back(QVector3D{x, y, z});
        }
    }
    repaint();
}

void PointsGlWidget::drawLasPoints(std::vector<QVector3D> arr_points)
{
    points = arr_points;
    repaint();
}

void PointsGlWidget::switchViewMode(int mode)
{
    currentViewMode = mode;
    std::cout << "new: " << currentViewMode << std::endl;
    // resize(1280, 800);
    // updateProjectionMatrix();
    switch(currentViewMode)
    {
    case 1:
        m_globalCameraPos = {30, -20, -80};
        break;
    case 2:
        m_globalCameraPos = {10, -5, -8};
        break;
    case 3:
        m_globalCameraPos = {10, -5, -8};
        break;
    case 4:
        m_globalCameraPos = {30, -20, -80};
        break;

    }
    glTranslatef((GLfloat) m_globalCameraPos.x(), (GLfloat) m_globalCameraPos.y(), (GLfloat) m_globalCameraPos.z());
}

void PointsGlWidget::changeCameraPos(QVector3D pos)
{
    m_globalCameraPos = pos;

    // Устанавливаем новую позицию и направление взгляда
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadIdentity();


    gluLookAt(
        pos.x(), pos.y(), pos.z(),
        // Точка, на которую смотрит камера
        pos.x() + 0, pos.y() + 0, pos.z() + 100,
        // Направление вектора, вдоль которого идет взгляда
        0, 0, -1
        );

    std::cout << "gluLookAt new pos" << std::endl;
}

void PointsGlWidget::stopSignal(bool run)
{
    canRun = run;
}

bool PointsGlWidget::getStopSignal()
{
    return canRun;
}


void PointsGlWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST | GL_CULL_FACE);
    glClearColor(0, 0, 0, 1);
}

float arr_line_Ox[] = {-1000,0,0, 1000,0,0, 0,0,0};
float arr_line_Oy[] = {0,-1000,0, 0,1000,0, 0,0,0};
float arr_line_Oz[] = {0,0,-1000, 0,0,1000, 0,0,0};

void PointsGlWidget::paintGL()
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glMatrixMode(GL_PROJECTION);
    glRotatef(angle,(GLfloat) m_rotation.x(), (GLfloat) m_rotation.y(), (GLfloat)m_rotation.z());

    glColor3f(0,1,0);
    glVertexPointer(3, GL_FLOAT, 0, &arr_line_Ox);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINES,0,3);
    glDisableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &arr_line_Oy);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINES,0,3);
    glDisableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &arr_line_Oz);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINES,0,3);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBegin(GL_POINTS);
    glColor3f(1,0,0);
    for(const QVector3D &point: points) {
        glVertex3f(point.x(), point.y(), point.z());
    }
    glEnd();
}

void PointsGlWidget::resizeGL(int w, int h)
{
    float aspect = 5 * w / (float)h;

    glViewport(0, 0, w ,h);

    switch(currentViewMode)
    {
    case 1:
        m_globalCameraPos = {5, -20, -100};
        break;
    case 2:
        m_globalCameraPos = {10, -5, -8};
        break;
    case 3:
        m_globalCameraPos = {10, -5, -8};
        break;
    case 4:
        m_globalCameraPos = {30, -20, -80};
        break;

    }
    std::cout << currentViewMode << std::endl;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(aspect, -aspect, aspect, -aspect, 10, 1400.0f);
    glTranslatef(m_globalCameraPos.x(), m_globalCameraPos.y(), m_globalCameraPos.z());

}

void PointsGlWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        m_mousePosition = QVector2D(event->position());
    }
    event->accept();
}

void PointsGlWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton){
        angle = 0;
        std::cout << "ret\n";
        return;
    }
    QVector2D diff = QVector2D(event->position()) - m_mousePosition;
    m_mousePosition = QVector2D(event->position());

    angle = diff.length();
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);
    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;
}

void PointsGlWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        angle = 0.0f;
    }
}

void PointsGlWidget::updateFrame()
{
    repaint();
}
