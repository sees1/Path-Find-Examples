#include "utils/vector_map_app/tools_dock.hpp"

#include <ament_index_cpp/get_package_share_directory.hpp>

ToolsDock::ToolsDock(Map* copy, QWidget* parent)
: QDockWidget(parent),
  map_(copy)
{
  std::string package_path = ament_index_cpp::get_package_share_directory("path_find_examples");

  b_create_road_ = new QToolButton(this);
  b_create_road_->setIcon(QIcon(QString::fromStdString(package_path) + "/resources/icons/road.png"));

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
  b_delete_road_->setIcon(QIcon(QString::fromStdString(package_path) + "/resources/icons/eraser.png"));
  b_move_camera_ = new QPushButton(this);
  b_move_camera_->setIcon(QIcon(QString::fromStdString(package_path) + "/resources/icons/mouse.png"));

  connect(b_delete_road_, &QPushButton::clicked, map_, &Map::setDeleteRoadMode);
  connect(b_move_camera_, &QPushButton::clicked, map_, &Map::setMoveCameraMode);

  QWidget* container = new QWidget(this);
  QHBoxLayout* dock_layout = new QHBoxLayout(container);

  dock_layout->addWidget(b_move_camera_);
  dock_layout->addWidget(b_create_road_);
  dock_layout->addWidget(b_delete_road_);

  container->setLayout(dock_layout);
  setWidget(container);
}