#include "utils/vector_map_app/settings_dock.hpp"

SettingsDock::SettingsDock(Map* copy, QWidget* parent)
: QDockWidget(parent),
  map_(copy)
{
  e_road_poly_count_ = new QLineEdit(this);
  e_road_poly_count_->setFixedWidth(50);
  e_road_poly_count_->setText(QString::number(10));

  connect(e_road_poly_count_, &QLineEdit::editingFinished, this, &SettingsDock::savePolyCount);
  connect(this, &SettingsDock::sigPassPolyCount, map_, &Map::setPolyCount);

  QWidget* container = new QWidget(this);
  QHBoxLayout* dock_layout = new QHBoxLayout(container);
  dock_layout->addWidget(e_road_poly_count_);

  container->setLayout(dock_layout);
  setWidget(container);
}

void SettingsDock::savePolyCount()
{
  poly_count_ = e_road_poly_count_->text().toInt();

  emit sigPassPolyCount(poly_count_);
}