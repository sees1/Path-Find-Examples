#include "utils/vector_map_app/primitives.hpp"

bool Road::isRoadBuilded()
{
  size_t size = type == Type::Straight ? 2
                                       : 3;
  
  if (points.size() != size)
    return false;

  for(const auto& point : points)
  {
    // qDebug() << point.has_value();
    if (!point.has_value())
      return false;
  }

  return true;
}

bool Road::underPoint(const QPoint& global_mouse_pose)
{
  QPainterPathStroker stroker;
  stroker.setWidth(15);

  QPainterPath stroke_path = stroker.createStroke(*path);
  bool fl = stroke_path.contains(global_mouse_pose);

  return fl;
}

void RoadArc::setNextPoint(const QPointF& s)
{
  if (points.size() > 2)
    throw std::runtime_error("Can't add more points to arc road!");

  points.push_back(s);
}

QPainterPath RoadArc::getPath(const QPoint& offset)
{
  if (path == nullptr)
  {
    path = std::make_shared<QPainterPath>(*points[0]);
    path->quadTo(*points[1], *points[2]);
  }
  
  return path->translated(-offset);
}

void RoadStraight::setNextPoint(const QPointF& s)
{
  if (points.size() > 1)
    throw std::runtime_error("Can't add more points to straight road!");

  qDebug() << "Set " << points.size() << " point!";
  points.push_back(s);
}

QPainterPath RoadStraight::getPath(const QPoint& offset)
{
  if (path == nullptr)
  {
    path = std::make_shared<QPainterPath>(*points[0]);
    path->lineTo(*points[1]);

    // qDebug() << "Start" << *points[0] << " End: " << *points[1];
  }

  return path->translated(-offset);
}