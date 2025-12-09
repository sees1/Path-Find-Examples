#pragma once

#include <QMainWindow>

#include "utils/vector_map_app/settings_dock.hpp"
#include "utils/vector_map_app/tools_dock.hpp"
#include "utils/vector_map_app/map.hpp"

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);

public:
  void onLoadData();
  void onSaveData();

private:
  Map* map_;

  QAction* a_load_data_;
  QAction* a_save_data_;

  ToolsDock* tools_dock_;
  SettingsDock* settings_dock_;
};