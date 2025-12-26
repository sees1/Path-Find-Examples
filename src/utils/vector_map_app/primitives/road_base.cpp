#include "utils/vector_map_app/primitives/road_base.hpp"

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
  
  if (control_points_.size() != size_by_type)
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

std::vector<Vertice> Road::getControlPoints()
{
  return road_geometry_->getControlPoints();
}

std::tuple<Vertice, Vertice> Road::getBasePoints()
{
  return road_geometry_->getBasePoints();
}

std::map<Vertice, std::vector<Vertice>> Road::getGraph()
{
  std::map<Vertice, std::vector<Vertice>> graph;

  if (type_ == Type::Arc)
  {
    graph[control_points_[0]] = std::vector<Vertice>{control_points_[2]};
    graph[control_points_[2]] = std::vector<Vertice>{control_points_[0]};
  }
  else if (type_ == Type::Straight)
  {
    graph[control_points_[0]] = std::vector<Vertice>{control_points_[1]};
    graph[control_points_[1]] = std::vector<Vertice>{control_points_[0]};
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