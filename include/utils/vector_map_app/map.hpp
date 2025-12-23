#pragma once

#include <QtWidgets>
#include <vector>
#include <memory>
#include <map>

#include "utils/vector_map_app/primitives.hpp"

class Map : public QObject
{
  Q_OBJECT
public:
  using RoadIter = std::vector<std::shared_ptr<Road>>;
  using ConstRoadIter = std::vector<std::shared_ptr<Road>>; 
public:
  Map();

  void createArcRoad(const QPointF& first_point);
  void createStraightRoad(const QPointF& first_point);
  void setNextPoint(const QPointF& point);
  void deleteRoad(const QPointF& point);

  bool onBuildStage();
  
  void setPolyCount(int poly_count);

  std::vector<std::shared_ptr<Road>>& getRoads();

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