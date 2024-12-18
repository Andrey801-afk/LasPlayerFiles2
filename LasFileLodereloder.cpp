#include "LasFileLoderder.h"

LasFileLoder::LasFileLoder(const char* fName): fileName(fName) {

}

std::vector<QVector3D> LasFileLoder::getLasPoints()
{
    std::string las_filepath = fileName;

    std::vector<QVector3D> points;

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
    return points;
}
