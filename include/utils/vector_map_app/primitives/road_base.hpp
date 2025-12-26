#pragma once

#include "utils/vector_map_app/primitives/road_geometry.hpp"

class Road : public QObject
{
  Q_OBJECT
public:
  enum Type {Arc = 0, Straight};

  Road(const Type& t)
  : type_(t)
  { }
  Road(const Vertice& s,
       const Vertice& e)
  : control_points_({s, e}),
    type_(Type::Straight)
  { }
  Road(const Vertice& s,
       const Vertice& m,
       const Vertice& e)
  : control_points_({s, e, m}),
    type_(Type::Arc)
  { }

  // render path invoke
  virtual void setNextPoint(const QPointF& s, int id) = 0;
  virtual QPainterPath getPath(const QPoint& offset) = 0;
  bool underPoint(const QPointF& offset);
  bool isRoadBuilded();
  
  Type getType() { return type_; };

  // geometry invoke
  Vertice addVertice(const QPointF& point, int id);
  bool removeVerticeById(int id);
  std::tuple<Vertice, Vertice> betweenIds(const QPointF& point);
  std::vector<Vertice> getControlPoints();
  std::tuple<Vertice, Vertice> getBasePoints();
  std::map<Vertice, std::vector<Vertice>> getGraph();
  QList<QPolygonF> getPathPolygons();
  int getPolyCount();
  void setPolyCount(int poly_count);

protected:
  std::vector<Vertice> control_points_;
  std::shared_ptr<QPainterPath> path_;
  std::shared_ptr<RoadGeometry> road_geometry_;
  Type type_;
};
