#pragma once

#include <memory>
#include <tuple>
#include <map>
#include <numeric_limits>
#include <algorithm>

#include <QtWidgets>

struct Vertice
{
  int id;
  QPointF coord;
};

class RoadGeometry
{
public:
  RoadGeometry() = delete;
  RoadGeometry(std::shared_ptr<QPainterPath>& view_represent,
               std::vector<Vertice>& v,
               int poly_count);

  Vertice addVertice(const QPointF& point, int id);
  bool removeVerticeById(int id);
  std::tuple<Vertice, Vertice> betweenIds(const QPointF& point);
  std::vector<Vertice> getControlPoints();
  std::map<Vertice, std::vector<Vertice>> getGraph();

private:
  int findClosestPoint(const QPointF& target);
  void subdividePath(std::shared_ptr<QPainterPath>& view_represent);

private:
  std::map<int, Vertice> ppoint_to_vertice_;
  std::map<Vertice, int> vertice_to_ppoint_;
  std::shared_ptr<QVector<QPolygonF>> path_polygons_;

  int poly_count_;
};

class Road : public QObject
{
  Q_OBJECT
public:
  enum Type {Arc = 0, Straight};

  Road(const Type& t)
  : type_(t)
  { }
  Road(const QPointF& s,
       const QPointF& e)
  : points_({s, e}),
    type_(Type::Straight)
  { }
  Road(const QPointF& s,
       const QPointF& m,
       const QPointF& e)
  : control_points_({s, e, m}),
    type_(Type::Arc)
  { }

  // render path invoke
  virtual void setNextPoint(const QPointF& s, int id) = 0;
  virtual QPainterPath getPath(const QPoint& offset) = 0;
  bool underPoint(const QPoint& offset);
  bool isRoadBuilded();
  
  Type getType() { return type_; };

  // geometry invoke
  Vertice addVertice(const QPointF& point, int id);
  bool removeVerticeById(int id);
  std::tuple<Vertice, Vertice> betweenIds(const QPointF& point);
  std::vector<Vertice> getControlPoints();
  std::map<Vertice, std::vector<Vertice>> getGraph();
  std::shared_ptr<QVector<QPolygonF>> getPathPolygons();

protected:
  std::vector<QPointF> control_points_;
  std::shared_ptr<QPainterPath> path_;
  std::shared_ptr<RoadGeometry> road_geometry_;
  Type type_;
};

class RoadArc : public Road
{
  Q_OBJECT
public:

  RoadArc()
  : Road(Type::Arc)
  { }

  RoadArc(const QPointF& s,
          const QPointF& m,
          const QPointF& e)
  : Road(s, m, e)
  { }

  void setNextPoint(const QPointF& s, int id) override;
  QPainterPath getPath(const QPoint& offset) override;
};

class RoadStraight : public Road
{
  Q_OBJECT
public:

  RoadStraight()
  : Road(Type::Straight)
  { }

  RoadStraight(const QPointF& s,
               const QPointF& e)
  : Road(s, e)
  { }

  void setNextPoint(const QPointF& s, int id) override;
  QPainterPath getPath(const QPoint& offset) override;
};