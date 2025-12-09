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

  QHBoxLayout* dock_layout = new QHBoxLayout(this);
  dock_layout->addWidget(e_road_poly_count_);
}

void SettingsDock::savePolyCount()
{
  poly_count_ = e_road_poly_count_->text().toInt();

  emit sigPassPolyCount(poly_count_);
}