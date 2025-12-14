#include "utils/vector_map_app/primitives.hpp"

RoadGeometry::RoadGeometry(std::shared_ptr<QPainterPath>& view_represent, 
                           std::vector<Vertice>& v,
                           int poly_count)
{
  int elem_count = view_represent->elementCount();

  if (elem_count == 2) // straight road (divide by ourself)
  {
    poly_count_ = poly_count;
    subdividePath(view_represent);
    ppoint_to_vertice_[0] = v[0];
    ppoint_to_vertice_[poly_count_] = v[1];
    vertice_to_ppoint[v[0]] = 0;
    vertice_to_ppoint[v[1]] = poly_count_;
  }
  else if (elem_count == 3)
  {
    path_polygon_ = std::make_shared<QVector<QPolygonF>>(std::move(view_represent->toSubpathPolygons()));
    poly_count_ = (path_polygon_->size() - 1);
    ppoint_to_vertice_[0] = v[0];
    ppoint_to_vertice_[poly_count_] = v[2];
    vertice_to_ppoint[v[0]] = 0;
    vertice_to_ppoint[v[2]] = poly_count_;
  }
}

Vertice RoadGeometry::addVertice(const QPointF& point, int id)
{
  int ppoint = findClosestPoint(point);

  ppoint_to_vertice_[ppoint] = Vertice{id, point};
  vertice_to_point_[ppoint_to_vertice_[ppoint]] = ppoint;

  return ppoint_to_vertice_[ppoint];
}

bool RoadGeometry::removeVerticeById(int id)
{
  auto needed_v_iter = std::find_if(vertice_to_ppoint_.begin(), vertice_to_ppoint_.end(),
    [this, &id](auto& vertice)
    {
      return id == vertice.id;
    }
  );

  if (needed_v_iter == vertice_to_ppoint_.end())
    return false;
  else
  {
    ppoint_to_vertice_->erase(needed_v_iter.second);
    vertice_to_ppoint_->erase(needed_v_iter);
    return true;
  }
}

std::tuple<Vertice, Vertice> RoadGeometry::betweenIds(int id)
{
  int ppoint = findClosestPoint(point);

  auto lower_v = ppoint_to_vertice_.lower_bound(ppoint);
  lower_v--; // get first less than ppoint vertice
  auto upper_v = ppoint_to_vertice_.upper_bound(ppoint);

  return {lower_v.second, upper_v.second};
}

std::vector<Vertice> RoadGeometry::getControlPoints()
{
  auto& start = vertice_to_ppoint.begin() + 1;
  auto& end = vertice_to_ppoint.end() - 1;

  std::vector<Vertice> res;
  for(start; start != end; ++start)
  {
    res.push_back(start.first);
  }

  return res;
}

std::map<Vertice, std::vector<Vertice>> getGraph()
{
  std::map<Vertice, std::vector<Vertice>> res;

  auto left = vertice_to_ppoint_.begin();
  auto right = vertice_to_ppoint_.begin() + 1;

  if (right == vertice_to_ppoint_.end())
  {
    res[left.first] = std::vector<Vertice>{right.first};
    res[right.first] = std::vector<Vertice>{left.first};

    return res;
  }

  while(right != vertice_to_ppoint_.end())
  {
    res[left.first].push_back(right.first);
    res[right.first].push_back(left.first);

    left = right;
    right++;
  }

  return res;
}

int findClosestPoint(const QPointF& target)
{
  int best_ppoint = -1;
  float best_dist2 = std::numeric_limits<float>::max();

  for(int ppoint = 0; ppoint < poly_count_; ++ppoint)
  {
    QPolygonF& poly_copy = path_polygons_[ppoint];
    int poly_size = poly_copy.size();

    QPointF poly_corner_min = poly_copy[0];

    float dx = poly_corner_min.x() - target.x();
    float dy = poly_corner_min.y() - target.y();
    float dist2 = dx * dx + dy * dy;

    if (dist2 < best_dist2)
    {
      best_dist2 = dist2;
      best_ppoint = ppoint;
    }

    QPointF poly_corner_max = poly_copy[poly_size - 1];
    
    dx = poly_corner_min.x() - target.x();
    dy = poly_corner_min.y() - target.y();
    dist2 = dx * dx + dy * dy;

    if (dist2 < best_dist2)
    {
      best_dist2 = dist2;
      best_ppoint = ppoint + 1;
    }
  }

  return best_ppoint;
}

void RoadGeometry::subdividePath(std::shared_ptr<QPainterPath>& view_represent)
{
  if (path_polygons_ == nullptr)
  {
    path_polygons_->reserve(poly_count);

    qreal path_length = view_represent->length();
    qreal step = path_length / poly_count_;

    QPointF prev_point = view_represent->pointAtPercent(0.0);

    for(int i = 1; i <= poly_count; ++i)
    {
      qreal cur_len = i * step;
      qreal cur_point_per = view_represent->percentAtLenght(cur_len);
      QPointF cur_point = view_represent->pointAtPercent(cur_point_per);

      path_polygons_->emplace_back({prev_point, cur_point});
      prev_point = cur_point;
    }
  }
}

bool Road::underPoint(const QPoint& global_mouse_pose)
{
  QPainterPathStroker stroker;
  stroker.setWidth(15);

  QPainterPath stroke_path = stroker.createStroke(*path);
  bool fl = stroke_path.contains(global_mouse_pose);

  return fl;
}

bool Road::isRoadBuilded()
{
  size_t size_by_type = type == Type::Straight ? 2
                                               : 3;
  
  if (points.size() != size)
    return false;

  return true;
}

Vertice Road::addVertice(const QPoint& point, int id)
{
  return road_geometry_->addVertice(point, id);
}

bool Road::removeVerticeById(int id);
{
  return road_geometry_->removeVerticeById(id);
}

bool Road::betweenIds(const QPointF& point);
{
  return road_geometry_->betweenIds(point);
}

std::vector<Vertice> Road::getControlPoints()
{
  return road_geometry_->getControlPoints();
}

std::map<Vertices, std::vector<Vertices>> Road::getGraph()
{
  std::map<Vertices, std::vector<Vertices>> graph;

  if (type == Type::Arc)
  {
    graph[points[0]] = std::vector<Vertices>{points[2]};
    graph[points[2]] = std::vector<Vertices>{points[0]};
  }
  else if (type == Type::Straight)
  {
    graph[points[0]] = std::vector<Vertices>{points[1]};
    graph[points[1]] = std::vector<Vertices>{points[0]};
  }

  return graph;
}

std::shared_ptr<QVector<QPolygonF>> Road::getPathPolygons()
{
  return path->toSubpathPolygons();
}

size_t Road::pointsCount()
{
  return points.size();
}

void RoadArc::setNextPoint(const QPointF& s, int id)
{
  if (points.size() > 2)
    throw std::runtime_error("Can't add more points to arc road!");

  points.push_back({s, id});
}

QPainterPath RoadArc::getPath(const QPoint& offset)
{
  if (path == nullptr)
  {
    path = std::make_shared<QPainterPath>(points[0]);
    path->quadTo(points[1], points[2]);

    geometry = std::make_shared<RoadGeometry>(path);
  }
  
  return path->translated(-offset);
}

void RoadStraight::setNextPoint(const QPointF& s, int id)
{
  if (points.size() > 1)
    throw std::runtime_error("Can't add more points to straight road!");

  points.push_back({s, id});
}

QPainterPath RoadStraight::getPath(const QPoint& offset)
{
  if (path == nullptr)
  {
    path = std::make_shared<QPainterPath>(points[0]);
    path->lineTo(points[1]);

    geometry = std::make_shared<RoadGeometry>(path);
  }

  return path->translated(-offset);
}