#include "utils/vector_map_app/map.hpp"
 
#include <algorithm>

Map::Map()
: vertices_idx_(0)
{ }

void Map::createArcRoad(const QPointF& first_point)
{
  if (roads_.empty())
  {
    roads_.push_back(std::make_shared<RoadArc>());
    setNextPoint(first_point);

    return;
  }

  if (!roads_.back()->isRoadBuilded() && roads_.back()->getType() == Road::Type::Straight)
  {
    roads_.back().reset(new RoadArc());
    setNextPoint(first_point);
  }
  else
  {
    roads_.push_back(std::make_shared<RoadArc>());
    setNextPoint(first_point);
  }
}

void Map::createStraightRoad(const QPointF& first_point)
{
  if (roads_.empty())
  {
    roads_.push_back(std::make_shared<RoadStraight>());
    setNextPoint(first_point);
    return;
  }

  if (!roads_.back()->isRoadBuilded() && roads_.back()->getType() == Road::Type::Arc)
  {
    roads_.back().reset(new RoadStraight());
    setNextPoint(first_point);
  }
  else
  {
    roads_.push_back(std::make_shared<RoadStraight>());
    setNextPoint(first_point);
  }
}

void Map::setNextPoint(const QPointF& point)
{
  roads_.back()->setNextPoint(point, ++vertices_idx_);
}

bool Map::onBuildStage()
{
  if (roads_.empty())
    return false;

  bool status = roads_.back()->isRoadBuilded();

  if (status)
    graph_.merge(roads_.back()->getGraph());
  
  return status;
}

void Map::deleteRoad(const QPointF& point)
{
  auto remove_iter = std::remove_if(roads_.begin(), roads_.end(),
    [this, &point](const auto& road)
    {
      return road->underPoint(point); 
    }
  );

  std::for_each(remove_iter, roads_.end(),
    [this](auto& remove_road)
    {
      auto control_vertice = remove_road->getControlPoints();
      Vertice base_vertice_l, base_vertice_r;
      std::tie(base_vertice_l, base_vertice_r) = remove_road->getBasePoints();
      remove_vertices_.insert(remove_vertices_.end(), control_vertice.begin(), control_vertice.end());

      // remove base vertice of deleted road
      for(auto& [u, neighbors] : graph_)
      {
        neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), base_vertice_l), neighbors.end());
        neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), base_vertice_r), neighbors.end());
      }

      graph_.erase(base_vertice_l);
      graph_.erase(base_vertice_r);

      // add roads that should be cleaned  
      roads_need_clean_.insert(roads_need_clean_.end(), intersection_[remove_road].begin(), intersection_[remove_road].end());

      // remove deleted road from dependency roads
      for(auto& [road, inter_road] : intersection_)
      {
        inter_road.erase(std::remove(inter_road.begin(), inter_road.end(), remove_road), inter_road.end());
      }

      intersection_.erase(remove_road);
    }
  );

  std::vector<std::shared_ptr<Road>> clean_roads;

  std::for_each(roads_need_clean_.begin(), roads_need_clean_.end(),
    [this, &clean_roads](auto& road)
    {
      if (intersection_.find(road) != intersection_.end())
        clean_roads.push_back(road);
    }
  );

  roads_need_clean_ = clean_roads;

  refreshGraph();
}

void Map::setPolyCount(int poly_count)
{
  poly_count_ = poly_count;
}

std::vector<std::shared_ptr<Road>>& Map::getRoads()
{
  return roads_;
}

void Map::refreshGraph()
{
  if (!remove_vertices_.empty())
  {
    std::vector<std::pair<std::shared_ptr<Road>, Vertice>> remove_from_road;

    for (auto & road : roads_need_clean_)
    {
      auto control_points = road->getControlPoints();

      auto remove_iter = std::remove_if(control_points.begin(), control_points.end(),
                            [this](auto& point)
                            {
                              return std::find(remove_vertices_.begin(), remove_vertices_.end(), point) != remove_vertices_.end();
                            }
                         );
      
      std::for_each(remove_iter, control_points.end(),
        [this, &remove_from_road, &road](auto& point)
        {
          remove_from_road.push_back({road, point});
          road->removeVerticeById(point.id);
        }
      );
    }

    for(auto& [road, vertice] : remove_from_road)
    {
      for(auto& [u, neighbors] : graph_)
      {
        neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), vertice), neighbors.end());
      }

      graph_.erase(vertice);

      Vertice road_l, road_r;
      std::tie(road_l, road_r) = road->betweenIds(vertice.coord);

      graph_[road_l].push_back(road_r);
      graph_[road_r].push_back(road_l);
    }

    return;
  }

  const auto& new_road = roads_.back();
  const auto& new_road_path = roads_.back()->getPath(QPoint(0, 0));

  std::for_each(roads_.begin(), roads_.end() - 1,
    [this, &new_road, &new_road_path](const auto& road)
    {
      const auto& road_path = road->getPath(QPoint(0, 0));

      if (road_path.intersects(new_road_path))
      {
        auto road_poly = road->getPathPolygons();
        auto new_road_poly = new_road->getPathPolygons();

        QPointF inter_point = intersectPathPoint(road_poly, new_road_poly);
        vertices_idx_++;

        Vertice new_road_l, new_road_r;
        Vertice old_road_l, old_road_r;

        std::tie(new_road_l, new_road_r) = new_road->betweenIds(inter_point);
        std::tie(old_road_l, old_road_r) = road->betweenIds(inter_point);

        auto& vec_n_l = graph_[new_road_l];
        graph_[new_road_l].erase(std::remove(vec_n_l.begin(), vec_n_l.end(), new_road_r), vec_n_l.end());
        auto& vec_n_r = graph_[new_road_r];
        graph_[new_road_r].erase(std::remove(vec_n_r.begin(), vec_n_r.end(), new_road_l), vec_n_r.end());

        auto& vec_o_l = graph_[old_road_l];
        graph_[old_road_l].erase(std::remove(vec_o_l.begin(), vec_o_l.end(), old_road_r), vec_o_l.end());
        auto& vec_o_r = graph_[old_road_r];
        graph_[old_road_r].erase(std::remove(vec_o_r.begin(), vec_o_r.end(), old_road_l), vec_o_r.end());

        Vertice temp = new_road->addVertice(inter_point, vertices_idx_);
        road->addVertice(inter_point, vertices_idx_);

        graph_[new_road_l].push_back(temp);
        graph_[new_road_r].push_back(temp);
        graph_[old_road_l].push_back(temp);
        graph_[old_road_r].push_back(temp);
        graph_[temp].push_back(new_road_l);
        graph_[temp].push_back(new_road_r);
        graph_[temp].push_back(old_road_l);
        graph_[temp].push_back(old_road_r);

        intersection_[road].push_back(new_road);
        intersection_[new_road].push_back(road);
      }
    }
  );
}

QPointF Map::intersectPathPoint(const QList<QPolygonF>& lhs, const QList<QPolygonF>& rhs)
{
  // check if point lies within both segments
  auto between = [](double left_border, double right_border, double value)
  {
      return (value >= std::min(left_border, right_border) - 1e-6 &&
              value <= std::max(left_border, right_border) + 1e-6);
  };

  for (const QPolygonF& poly_lhs : lhs)
  {
    for(const QPolygonF& poly_rhs : rhs)
    {
      if (poly_rhs.intersects(poly_lhs))
      {
        for (int i = 0; i < poly_lhs.size() - 1; ++i)
        {
          QPointF p_l1 = poly_lhs[i];
          QPointF p_l2 = poly_lhs[i + 1];

          for (int j = 0; j < poly_rhs.size() - 1; ++j)
          {
            QPointF p_r1 = poly_rhs[j];
            QPointF p_r2 = poly_rhs[j + 1];

            double x1 = p_l1.x();
            double y1 = p_l1.y();
            double x2 = p_l2.x();
            double y2 = p_l2.y();
            double x3 = p_r1.x();
            double y3 = p_r1.y();
            double x4 = p_r2.x();
            double y4 = p_r2.y();

            double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4); // vector dot product
            if (qFuzzyIsNull(denom))
              continue;

            double px = ((x1*y2 - y1*x2)*(x3-x4) - (x1-x2)*(x3*y4 - y3*x4)) / denom;
            double py = ((x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4 - y3*x4)) / denom;

            if (between(x1, x2, px) && between(y1, y2, py) &&
                between(x3, x4, px) && between(y3, y4, py))
            {
              return QPointF(px, py);
            }
          }
        }
      }
    }
  }
}