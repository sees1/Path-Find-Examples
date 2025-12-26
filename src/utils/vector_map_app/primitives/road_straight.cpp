#include "utils/vector_map_app/primitives/road_straight.hpp"

void RoadStraight::setNextPoint(const QPointF& s, int id)
{
  if (control_points_.size() > 1)
    throw std::runtime_error("Can't add more points to straight road!");

  control_points_.push_back(Vertice{id, s});
}

QPainterPath RoadStraight::getPath(const QPoint& offset)
{
  if (path_ == nullptr)
  {
    path_ = std::make_shared<QPainterPath>(control_points_[0].coord);
    path_->lineTo(control_points_[1].coord);

    road_geometry_ = std::make_shared<RoadGeometry>(path_, control_points_, 100);
  }

  return path_->translated(offset);
}