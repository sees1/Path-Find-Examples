#include "utils/vector_map_app/map_manager.hpp"

MapManager::MapManager(QWidget* parent)
: QWidget(parent),
  mode_(Mode::MOVE_CAMERA),
  costmap_img_(nullptr)
{
  this->setStyleSheet("border:1px solid gray;");
  this->setMinimumSize(1000, 1000);
  this->setMouseTracking(true);
}

MapManager::~MapManager()
{
  delete costmap_img_;
  delete data_;
}

// usual methods

void MapManager::pan(const QPoint& delta)
{
  viewport_offset_-= delta;

  update();
}

QPoint MapManager::viewportToGlobalCoord(const QPoint& coord)
{
  QPoint res;
  res.setX(viewport_offset_.x() + coord.x());
  res.setY(viewport_offset_.y() + coord.y());

  return res;
}

QPoint MapManager::globalToViewportCoord(const QPoint& coord)
{
  QPoint res;
  res.setX(coord.x() - viewport_offset_.x());
  res.setY(coord.y() - viewport_offset_.y());

  return res;
}

bool MapManager::saveData(const QString& filename)
{
  if (data_ == nullptr)
    return false;

  data_->saveTo(filename, roads_, poly_count_);

  return true;
}

bool MapManager::loadData(const QString& filename)
{
  if (data_ == nullptr)
  {
    if (costmap_img_ == nullptr)
      data_ = new MapData();
    else
      data_ = new MapData(costmap_resolution_, map_zero_offset_);
  }

  data_->load(filename);

  update();

  return true;
}

bool MapManager::loadCostmap(const QString& filename)
{
  if (costmap_img_ == nullptr)
    costmap_img_ = new QPixmap();
  
  costmap_img_->load(filename);

  if (data_ != nullptr)
  {
    if (!data_->hasCostmapInfo())
      data_->setCostmapInfo(costmap_resolution_, map_zero_offset_);
  }

  map_zero_offset_ = QPoint(0, -costmap_img_->height());

  update();

  return true;
}

// overrided methods

void MapManager::paintEvent(QPaintEvent* ev)
{
  QPainter p(this);
  QPen pen(Qt::black);
  pen.setWidth(15);

  p.setPen(pen);
  p.fillRect(rect(), Qt::white);

  if (costmap_img_ != nullptr)
    p.drawPixmap(globalToViewportCoord({0, 0}), *costmap_img_);

  for (const auto& road : map_->getRoads()) 
      p.drawPath(road->getPath(viewport_offset_));
}

void MapManager::mousePressEvent(QMouseEvent* ev)
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
        if (map_->onBuildStage())
          map_->createArcRoad(viewportToGlobalCoord(ev->pos()));
        else
          map_->setNextPoint(viewportToGlobalCoord(ev->pos()));

        update();

        break;
      }
      case Mode::CREATE_STRAIGHT_ROAD:
      {
        if (map_->onBuildStage())
          map_->createStraightRoad(viewportToGlobalCoord(ev->pos()));
        else
          map_->setNextPoint(viewportToGlobalCoord(ev->pos()));

        update();

        break;
      }
      case Mode::DELETE_ROAD:
      {
        map_->deleteRoad(viewportToGlobalCoord(ev->pos()));
        
        update();

        //  search for intersection betweed roads and mouse pose and delete from roads
        break;
      }
    }
  }
}

void MapManager::mouseMoveEvent(QMouseEvent* ev)
{
  if (ev->buttons() & Qt::RightButton)
  {
    QPoint delta = ev->pos() - last_mouse_pos_;
    last_mouse_pos_ = ev->pos();

    pan(delta);   // ← Вот здесь вызывается pan()
  }
  else if (ev->buttons() & Qt::LeftButton)
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

void MapManager::mouseReleaseEvent(QMouseEvent* e)
{
  return;
}

// slots

void MapManager::setPolyCount(int poly_count)
{
  map_->setPolyCount(poly_count);
}

void MapManager::setArcRoadCreateMode()
{
  mode_ = Mode::CREATE_ARC_ROAD;
}

void MapManager::setStraightRoadCreateMode()
{
  mode_ = Mode::CREATE_STRAIGHT_ROAD;
}

void MapManager::setDeleteRoadMode()
{
  mode_ = Mode::DELETE_ROAD;
}

void MapManager::setMoveCameraMode()
{
  mode_ = Mode::MOVE_CAMERA;
}

void MapManager::goToZeroViewport()
{
  viewport_offset_ = QPoint(0, 0);
  update();
}