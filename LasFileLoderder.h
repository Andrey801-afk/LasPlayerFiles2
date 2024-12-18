#ifndef LASFILELODERDER_H
#define LASFILELODERDER_H

#include <QVector3D>

#include <pdal/Reader.hpp>
#include <pdal/StageFactory.hpp>
#include <pdal/PointView.hpp>

class LasFileLoder
{
public:
    LasFileLoder(const char* fileName);
    std::vector<QVector3D> getLasPoints();

private:
    std::string fileName;
};

#endif // LASFILELODERDER_H
