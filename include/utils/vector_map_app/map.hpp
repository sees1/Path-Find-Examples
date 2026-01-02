#pragma once

#include <QtWidgets>
#include <vector>
#include <memory>
#include <map>
#include <tuple>

#include "utils/vector_map_app/primitives/road_base.hpp"

class Map : public QObject
{
  Q_OBJECT
public:
  Map();

  void createArcRoad(const QPointF& first_point);
  void createStraightRoad(const QPointF& first_point);
  void setNextPoint(const QPointF& point);
  void deleteRoad(const QPointF& point);

  bool onBuildStage();
  
  void setPolyCount(int poly_count);

  std::vector<std::shared_ptr<Road>>& getRoads();

  // TODO: think about move this code to MapData class, bcse it have access to Map and road data...
  // method's used by MapData object
  std::vector<std::vector<std::tuple<Vertice, Vertice, QPolygonF, int>>> getGraphPolygons() const
  std::map<Vertice, std::vector<Vertice>> getGraph() const;

  void loadRoads(std::vector<std::vector<std::tuple<Vertice, Vertice, QPolygonF, int>>>& polygons);
  void loadGraph(std::map<Vertice, std::vector<Vertice>>& graph);

private:
  void refreshGraph();
  QPointF intersectPathPoint(const QList<QPolygonF>& lhs, const QList<QPolygonF>& rhs);

private:
  std::vector<std::shared_ptr<Road>> roads_;
  std::map<std::shared_ptr<Road>, std::vector<std::shared_ptr<Road>>> intersection_;
  std::map<Vertice, std::vector<Vertice>> graph_;

  std::vector<Vertice> remove_vertices_;
  std::vector<std::shared_ptr<Road>> roads_need_clean_;

  int poly_count_;

  int vertices_idx_;
  int road_idx_;
};