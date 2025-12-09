#include "utils/vector_map_app/tools_dock.hpp"

ToolsDock::ToolsDock(Map* copy, QWidget* parent)
: QDockWidget(parent),
  map_(copy)
{

  b_create_road_ = new QToolButton(this);
  b_create_road_->setIcon(QIcon(":icons/road.png"));

  QMenu* menu = new QMenu(this);
  a_create_arc_road_      = new QAction("Arc road", this);
  a_create_straight_road_ = new QAction("Straight road", this);
  menu->addAction(a_create_arc_road_);
  menu->addAction(a_create_straight_road_);

  b_create_road_->setPopupMode(QToolButton::MenuButtonPopup);
  b_create_road_->setMenu(menu);

  connect(a_create_arc_road_, &QAction::triggered, map_, &Map::setArcRoadCreateMode);
  connect(a_create_straight_road_, &QAction::triggered, map_, &Map::setStraightRoadCreateMode);

  b_delete_road_ = new QPushButton(this);
  b_delete_road_->setIcon(QIcon(":icons/eraser.png"));
  b_move_camera_ = new QPushButton(this);
  b_move_camera_->setIcon(QIcon(":icons/mouse.png"));

  connect(b_delete_road_, &QPushButton::clicked, map_, &Map::setDeleteRoadMode);
  connect(b_move_camera_, &QPushButton::clicked, map_, &Map::setMoveCameraMode);

  QHBoxLayout* dock_layout = new QHBoxLayout(this);
  dock_layout->addWidget(b_move_camera_);
  dock_layout->addWidget(b_create_road_);
  dock_layout->addWidget(b_delete_road_);
}