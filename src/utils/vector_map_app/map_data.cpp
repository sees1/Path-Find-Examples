#include "utils/vector_map_app/map_data.hpp"

#include <fstream>

MapData::MapData(const QPoint& offset, float resolution)
: map_offset_(offset),
  map_resolution_(resolution)
{ }

bool MapData::hasCostmapInfo()
{
  return (map_offset_.has_value() && map_resolution_.has_value());
}

bool MapData::setCostmapInfo(float resolution, const QPoint& offset)
{
  map_offset_ = offset;
  map_resolution_ = resolution;
}

void MapData::saveTo(const QString& filename, const std::vector<std::shared_ptr<Road>>& roads, int poly_count)
{
  std::ofstream file(filename.toStdString());

  if (!file.is_open())
    throw std::runtime_error("Can't open file to save roads!");

  for(const auto& road : roads)
  {
    
  }
}

void MapData::load(const QString& filename)
{
  std::ifstream file(filename.toStdString());

  if (!file.is_open())
    throw std::runtime_error("Can't open file to load roads!");
}

std::vector<QLineF> MapData::subdivideRoad(std::shared_ptr<Road> road, int poly_count)
{
  std::vector<QLineF> res;
  res.reserve(poly_count);

  QPointPath path = road->getPath(QPoint(0, 0));
  qreal path_length = path.length();
  qreal step = path_length / poly_count;

  QPointF prev_point = path.pointAtPercent(0.0);

  for(int i = 1; i <= poly_count; ++i)
  {
    qreal cur_len = i * step;
    qreal cur_point_per = path.percentAtLenght(cur_len);
    QPointF cur_point = path.pointAtPercent(cur_point_per);

    res.emplace_back(prev_point, cur_point);
    prev_point = cur_point;
  }

  return res;
}