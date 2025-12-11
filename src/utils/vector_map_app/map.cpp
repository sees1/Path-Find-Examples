#include "utils/vector_map_app/map.hpp"

Map::Map(QWidget* parent)
: QWidget(parent)
{
  this->setStyleSheet("border:1px solid gray;");
  this->setMinimumSize(1000, 1000);
  this->setMouseTracking(true);

  scroll_area_ = qobject_cast<QScrollArea*>(parent);
}

// usual methods

void Map::pan(const QPoint& delta)
{
  viewport_offset_-= delta;

  auto* h = scroll_area_->horizontalScrollBar();
  auto* v = scroll_area_->verticalScrollBar();

  h->setValue(h->value() - delta.x());
  v->setValue(v->value() - delta.y());
}

QPoint Map::viewportToGlobalCoord(const QPoint& coord)
{
  QPoint res;
  res.setX(viewport_offset_.x() + coord.x());
  res.setY(viewport_offset_.y() + coord.y());

  return res;
}

QPoint Map::globalToViewportCood(const QPoint& coord)
{
  QPoint res;
  res.setX(viewport_offset_.x() - coord.x());
  res.setY(viewport_offset_.y() - coord.y());

  return res;
}

bool Map::saveData(const QString& filename)
{
  return true;
}

bool Map::loadData(const QString& filename)
{
  return true;
}

// overrided methods

void Map::paintEvent(QPaintEvent* ev)
{
  QPainter p(this);
  QPen pen(Qt::black);
  pen.setWidth(15);

  p.setPen(pen);
  p.fillRect(rect(), Qt::white);

  for (const auto& road : roads_) {
    if (road->isRoadBuilded())
      p.drawPath(road->getPath(viewport_offset_));
  }
}

void Map::mousePressEvent(QMouseEvent* ev)
{
  if (ev->button() == Qt::RightButton)
  {
    last_mouse_pos_ = ev->pos();
  }
  else if (ev->button() == Qt::LeftButton)
  {
    switch(mode_)
    {
      case Mode::MOVE_CAMERA:
      {
        last_mouse_pos_ = ev->pos();

        break;
      }
      case Mode::CREATE_ARC_ROAD:
      {
        if (roads_.empty())
        {
          roads_.push_back(std::make_shared<RoadArc>());
          roads_.back()->setNextPoint(viewportToGlobalCoord(ev->pos()));          
        }
        else
        {
          if (!roads_.back()->isRoadBuilded())
          {
            if (roads_.back()->getType() == Road::Type::Straight)
              roads_.back().reset(new RoadArc());
            roads_.back()->setNextPoint(viewportToGlobalCoord(ev->pos()));
          }
          else
          {
            roads_.push_back(std::make_shared<RoadArc>());
            roads_.back()->setNextPoint(viewportToGlobalCoord(ev->pos()));
          }
        }

        break;
      }
      case Mode::CREATE_STRAIGHT_ROAD:
      {
        if (roads_.empty())
        {
          roads_.push_back(std::make_shared<RoadStraight>());
          roads_.back()->setNextPoint(viewportToGlobalCoord(ev->pos()));
        }
        else
        {
          if (!roads_.back()->isRoadBuilded())
          {
            if (roads_.back()->getType() == Road::Type::Arc)
              roads_.back().reset(new RoadStraight());
            roads_.back()->setNextPoint(viewportToGlobalCoord(ev->pos()));
          }
          else
          {
            roads_.push_back(std::make_shared<RoadStraight>());
            roads_.back()->setNextPoint(viewportToGlobalCoord(ev->pos()));
          }
        }

        break;
      }
      case Mode::DELETE_ROAD:
      {
        roads_.erase(std::remove_if(roads_.begin(), roads_.end(),
          [this, &ev](const auto& road)
          {
            return road->underPoint(viewportToGlobalCoord(ev->pos())); 
          }),
          roads_.end());
        
        //  search for intersection betweed roads and mouse pose and delete from roads
        break;
      }
    }

    update();
  }
}

void Map::mouseMoveEvent(QMouseEvent* ev)
{
  if (ev->buttons() & Qt::RightButton)
  {
    QPoint delta = ev->pos() - last_mouse_pos_;
    last_mouse_pos_ = ev->pos();

    pan(delta);   // ← Вот здесь вызывается pan()
  }
  else if (ev->button() & Qt::LeftButton)
  {
    switch (mode_)
    {
      case Mode::MOVE_CAMERA:
      {
        QPoint delta = ev->pos() - last_mouse_pos_;
        last_mouse_pos_ = ev->pos();

        pan(delta);   // ← Вот здесь вызывается pan()
        break;
      }
    }
  }
}

void Map::mouseReleaseEvent(QMouseEvent* e)
{
  return;
}

// signals

void Map::setPolyCount(int poly_count)
{
  poly_count_ = poly_count;
}

void Map::setArcRoadCreateMode()
{
  mode_ = Mode::CREATE_ARC_ROAD;
}

void Map::setStraightRoadCreateMode()
{
  mode_ = Mode::CREATE_STRAIGHT_ROAD;
}

void Map::setDeleteRoadMode()
{
  mode_ = Mode::DELETE_ROAD;
}

void Map::setMoveCameraMode()
{
  mode_ = Mode::MOVE_CAMERA;
}