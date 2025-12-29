#include "utils/vector_map_app/map_data.hpp"

#include "utils/vector_map_app/map.hpp"

#include <fstream>

std::ostream& operator<<(std::ostream& out, QPolygonF& obj)
{
  for (const QPointF& p : obj)
    out << "(" << std::to_string(p.x()) << "," << std::to_string(p.y()) << ")" << " ";

  return out;
}

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

  return true;
}

void MapData::saveTo(const QString& filename, Map* map, int poly_count)
{
  std::ofstream file(filename.toStdString());

  if (!file.is_open())
    throw std::runtime_error("Can't open file to save roads!");

  std::map<Vertice, std::vector<Vertice>> graph = map->getGraph();

  file << "Graph:" << std::endl;
  for(const auto& [vertice, adj_vertices] : graph)
  {
    file << std::to_string(vertice.id) << "(" << std::to_string(vertice.coord.x()) << "," << std::to_string(vertice.coord.y()) << ")" << "-";
    
    for(const auto& v : adj_vertices)
      file << std::to_string(v.id) << "(" << std::to_string(vertice.coord.x()) << "," << std::to_string(vertice.coord.y()) << ")" << " ";
  }

  file << std::endl;

  std::map<std::pair<Vertice, Vertice>, QPolygonF> polygons = map->getGraphPolygons();

  size_t polygons_count = polygons.size() / 2;

  size_t poly_idx = 0;
  for(size_t i = 0; i < polygons_count; ++i)
  {
    auto iter = polygons.begin();
    auto rev_elem_iter = polygons.find(std::make_pair(iter->first.second, iter->first.first));

    file << "Polygon-" << std::to_string(poly_idx) << ":" << " " << iter->second << std::endl;
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
  // std::vector<QLineF> res;
  // res.reserve(poly_count);

  // QPainterPath path = road->getPath(QPoint(0, 0));
  // qreal path_length = path.length();
  // qreal step = path_length / poly_count;

  // QPointF prev_point = path.pointAtPercent(0.0);

  // for(int i = 1; i <= poly_count; ++i)
  // {
  //   qreal cur_len = i * step;
  //   qreal cur_point_per = path.percentAtLength(cur_len);
  //   QPointF cur_point = path.pointAtPercent(cur_point_per);

  //   res.emplace_back(prev_point, cur_point);
  //   prev_point = cur_point;
  // }

  // return res;
}