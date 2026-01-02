#pragma once

#include "utils/vector_map_app/primitives/road_base.hpp"

class RoadArc : public Road
{
  Q_OBJECT
public:

  RoadArc()
  : Road(Type::Arc)
  { }

  RoadArc(std::shared_ptr<QList<QPolygonF>>& poly_set,
          const std::vector<Vertice>& all_v)
  : Road(poly_set, all_v, Type::Arc)
  { }

  RoadArc(const Vertice& s,
          const Vertice& m,
          const Vertice& e)
  : Road(s, m, e)
  { }

  void setNextPoint(const QPointF& s, int id) override;
  QPainterPath getPath(const QPoint& offset) override;
};