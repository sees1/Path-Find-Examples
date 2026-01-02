#pragma once

#include <memory>
#include <tuple>
#include <map>
#include <limits>
#include <algorithm>

#include <QtWidgets>

struct Vertice
{
  int id;
  QPointF coord;

  bool operator<(const Vertice& other) const { return id < other.id; }
  bool operator==(const Vertice& other) const { return id == other.id; }
};

class RoadGeometry
{
public:
  RoadGeometry() = delete;
  RoadGeometry(std::shared_ptr<QList<QPolygonF>>& poly_represent,
               std::vector<Vertice>& all_v);
  RoadGeometry(std::shared_ptr<QPainterPath>& view_represent,
               std::vector<Vertice>& v,
               int poly_count);

  Vertice addVertice(const QPointF& point, int id);
  bool removeVerticeById(int id);
  std::tuple<Vertice, Vertice> betweenIds(const QPointF& point);
  QPolygonF polygonBetweenIds(const Vertice& first, const Vertice& second);
  std::vector<Vertice> getControlPoints();
  std::tuple<Vertice, Vertice> getBasePoints();
  std::map<Vertice, std::vector<Vertice>> getGraph();

private:
  int findClosestPoint(const QPointF& target);
  void subdividePath(std::shared_ptr<QPainterPath>& view_represent);

private:
  std::map<int, Vertice> ppoint_to_vertice_;
  std::map<Vertice, int> vertice_to_ppoint_;
  // TODO: add here invariant that use control points as pivot for main QPolygonF.
  std::shared_ptr<QList<QPolygonF>> path_polygons_;

  int poly_count_;
};