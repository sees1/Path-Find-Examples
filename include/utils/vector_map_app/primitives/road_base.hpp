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

  Road(std::shared_ptr<QList<QPolygonF>>& poly_set,
       const std::vector<Vertice>& all_v,
       const Type& t);

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
  QPolygonF polygonBetweenIds(const Vertice& first, const Vertice& second);
  std::vector<Vertice> getControlPoints();
  std::tuple<Vertice, Vertice> getBasePoints();
  std::map<Vertice, std::vector<Vertice>> getGraph();
  QList<QPolygonF> getPathPolygons();
  int getPolyCount();
  void setPolyCount(int poly_count);

protected:
  std::vector<Vertice> base_points_;
  std::shared_ptr<QPainterPath> path_;
  std::shared_ptr<RoadGeometry> road_geometry_;
  Type type_;
};
