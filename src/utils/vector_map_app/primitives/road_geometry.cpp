#include "utils/vector_map_app/primitives/road_geometry.hpp"

RoadGeometry::RoadGeometry(std::shared_ptr<QPainterPath>& view_represent, 
                           std::vector<Vertice>& v,
                           int poly_count)
{
  int elem_count = view_represent->elementCount();

  poly_count_ = poly_count;
  subdividePath(view_represent);
  ppoint_to_vertice_[0] = v[0];
  ppoint_to_vertice_[poly_count_] = v[1];
  vertice_to_ppoint_[v[0]] = 0;
  vertice_to_ppoint_[v[1]] = poly_count_;
}

Vertice RoadGeometry::addVertice(const QPointF& point, int id)
{
  int ppoint = findClosestPoint(point);

  ppoint_to_vertice_[ppoint] = Vertice{id, point};
  vertice_to_ppoint_[ppoint_to_vertice_[ppoint]] = ppoint;

  return ppoint_to_vertice_[ppoint];
}

bool RoadGeometry::removeVerticeById(int id)
{
  auto needed_v_iter = std::find_if(vertice_to_ppoint_.begin(), vertice_to_ppoint_.end(),
    [this, &id](auto& elem)
    {
      return id == elem.first.id;
    }
  );

  if (needed_v_iter == vertice_to_ppoint_.end())
    return false;
  else
  {
    ppoint_to_vertice_.erase(needed_v_iter->second);
    vertice_to_ppoint_.erase(needed_v_iter);
    return true;
  }
}

std::tuple<Vertice, Vertice> RoadGeometry::betweenIds(const QPointF& point)
{
  int ppoint = findClosestPoint(point);

  auto lower_v = ppoint_to_vertice_.lower_bound(ppoint);
  lower_v--; // get first less than ppoint vertice
  auto upper_v = ppoint_to_vertice_.upper_bound(ppoint);

  return std::make_tuple((*lower_v).second, (*upper_v).second);
}

std::vector<Vertice> RoadGeometry::getControlPoints()
{
  // veritce is sorted by id!!!
  auto start = vertice_to_ppoint_.begin();
  start++;
  start++;

  std::vector<Vertice> res;
  for(start; start != vertice_to_ppoint_.end(); ++start)
  {
    res.push_back((*start).first);
  }

  return res;
}

std::tuple<Vertice, Vertice> RoadGeometry::getBasePoints()
{
  // veritce is sorted by id!!!
  auto left = vertice_to_ppoint_.begin();
  auto right = left;
  right++;
  return std::make_tuple((*left).first, (*(right)).first);
}

std::map<Vertice, std::vector<Vertice>> RoadGeometry::getGraph()
{
  std::map<Vertice, std::vector<Vertice>> res;

  auto left = vertice_to_ppoint_.begin();
  auto right = vertice_to_ppoint_.begin();
  right++;

  if (right == vertice_to_ppoint_.end())
  {
    res[left->first] = std::vector<Vertice>{right->first};
    res[right->first] = std::vector<Vertice>{left->first};

    return res;
  }

  while(right != vertice_to_ppoint_.end())
  {
    res[left->first].push_back(right->first);
    res[right->first].push_back(left->first);

    left = right;
    right++;
  }

  return res;
}

int RoadGeometry::findClosestPoint(const QPointF& target)
{
  int best_ppoint = -1;
  float best_dist2 = std::numeric_limits<float>::max();

  for(int ppoint = 0; ppoint < poly_count_; ++ppoint)
  {
    QPolygonF& poly_copy = (*path_polygons_)[ppoint];
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
    path_polygons_ = std::make_shared<QList<QPolygonF>>();
    path_polygons_->reserve(poly_count_);

    qreal path_length = view_represent->length();
    qreal step = path_length / poly_count_;

    QPointF prev_point = view_represent->pointAtPercent(0.0);

    for(int i = 1; i <= poly_count_; ++i)
    {
      qreal cur_len = i * step;
      qreal cur_point_per = view_represent->percentAtLength(cur_len);
      QPointF cur_point = view_represent->pointAtPercent(cur_point_per);

      QPolygonF temp (QVector<QPointF>({prev_point, cur_point}));
      (*path_polygons_).append(temp);
      prev_point = cur_point;
    }
  }
}