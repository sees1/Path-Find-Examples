#pragma once

#include "utils/vector_map_app/primitives/road_base.hpp"

class RoadArc : public Road
{
  Q_OBJECT
public:

  RoadArc()
  : Road(Type::Arc)
  { }

  RoadArc(const Vertice& s,
          const Vertice& m,
          const Vertice& e)
  : Road(s, m, e)
  { }

  void setNextPoint(const QPointF& s, int id) override;
  QPainterPath getPath(const QPoint& offset) override;
};