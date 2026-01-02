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

  RoadStraight(std::shared_ptr<QList<QPolygonF>>& poly_set,
               const std::vector<Vertice>& all_v)
  : Road(poly_set, all_v, Type::Straight)
  { }

  void setNextPoint(const QPointF& s, int id) override;
  QPainterPath getPath(const QPoint& offset) override;
};