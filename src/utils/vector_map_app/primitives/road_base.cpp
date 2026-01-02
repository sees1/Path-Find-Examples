#include "utils/vector_map_app/primitives/road_base.hpp"

Road::Road(std::shared_ptr<QList<QPolygonF>>& poly_set,
           const std::vector<Vertice>& all_v,
           const Type& t)
: Road(t)
{
  all_v_s = all_v.size();

  base_points_.push_back(all_v[0]);
  base_points_.push_back(all_v[all_v_s - 1]);

  path = std::make_shared<QPainterPath>();

  for (const auto& poly : *poly_set)
    path->addPolygon(poly);

  road_geometry_ = std::make_shared<RoadGeometry>(path, all_v);
}

bool Road::underPoint(const QPointF& global_mouse_pose)
{
  if (path_ == nullptr)
    return false;

  QPainterPathStroker stroker;
  stroker.setWidth(15);

  QPainterPath stroke_path = stroker.createStroke(*path_);
  bool fl = stroke_path.contains(global_mouse_pose);

  return fl;
}

bool Road::isRoadBuilded()
{
  size_t size_by_type = (type_ == Type::Straight) ? 2 : 3;
  
  if (base_points_.size() != size_by_type)
    return false;

  return true;
}

Vertice Road::addVertice(const QPointF& point, int id)
{
  return road_geometry_->addVertice(point, id);
}

bool Road::removeVerticeById(int id)
{
  return road_geometry_->removeVerticeById(id);
}

std::tuple<Vertice, Vertice> Road::betweenIds(const QPointF& point)
{
  return road_geometry_->betweenIds(point);
}

QPolygonF Road::polygonBetweenIds(const Vertice& first, const Vertice& second)
{
  return road_geometry_->polygonBetweenIds(first, second);
}

std::vector<Vertice> Road::getControlPoints()
{
  return road_geometry_->getControlPoints();
}

std::tuple<Vertice, Vertice> Road::getBasePoints()
{
  return road_geometry_->getBasePoints();
}

// TODO: ban this method for road that contains control points
std::map<Vertice, std::vector<Vertice>> Road::getGraph()
{
  std::map<Vertice, std::vector<Vertice>> graph;

  if (type_ == Type::Arc)
  {
    graph[base_points_[0]] = std::vector<Vertice>{base_points_[2]};
    graph[base_points_[2]] = std::vector<Vertice>{base_points_[0]};
  }
  else if (type_ == Type::Straight)
  {
    graph[base_points_[0]] = std::vector<Vertice>{base_points_[1]};
    graph[base_points_[1]] = std::vector<Vertice>{base_points_[0]};
  }

  return graph;
}

QList<QPolygonF> Road::getPathPolygons()
{
  return path_->toSubpathPolygons();
}

int Road::getPolyCount()
{
  return 1;
}

void Road::setPolyCount(int poly_count)
{
  return;
}