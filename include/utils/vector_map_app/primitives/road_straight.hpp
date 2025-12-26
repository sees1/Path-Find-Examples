#pragma once

#include "utils/vector_map_app/primitives/road_base.hpp"

class RoadStraight : public Road
{
  Q_OBJECT
public:

  RoadStraight()
  : Road(Type::Straight)
  { }

  RoadStraight(const Vertice& s,
               const Vertice& e)
  : Road(s, e)
  { }

  void setNextPoint(const QPointF& s, int id) override;
  QPainterPath getPath(const QPoint& offset) override;
};