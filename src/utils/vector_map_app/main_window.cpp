#include "utils/vector_map_app/main_window.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
{
  this->setMouseTracking(true);
  this->setMinimumSize(600, 400);
  this->setWindowTitle("RoadCreator");

  QWidget* central_widget = new QWidget(this);
  central_widget->setMouseTracking(true);
  this->setCentralWidget(central_widget);

  QVBoxLayout* main_layout = new QVBoxLayout(central_widget);
  map_ = new Map();
  QScrollArea* map_scrolled = new QScrollArea(map_);
  main_layout->addWidget(map_scrolled);

  QMenu* file_menu = menuBar()->addMenu("File");
  
  a_load_data_ = new QAction("Load map", this);
  a_save_data_ = new QAction("Save data", this);
  a_load_data_->setIcon(QIcon(":icons/load-data.png"));
  a_save_data_->setIcon(QIcon(":icons/save-as-data.png"));

  file_menu->addAction(a_load_data_);
  file_menu->addAction(a_save_data_);

  connect(a_load_data_, &QAction::triggered, this, &MainWindow::onLoadData);
  connect(a_save_data_, &QAction::triggered, this, &MainWindow::onSaveData);

  tools_dock_ = new ToolsDock(map_, this);
  settings_dock_ = new SettingsDock(map_, this);

  addDockWidget(Qt::TopDockWidgetArea, tools_dock_);
  addDockWidget(Qt::RightDockWidgetArea, settings_dock_);
}

void MainWindow::onLoadData()
{
  QString filename = QFileDialog::getOpenFileName(this, "Select file", ".", "data file(*.dat)");

  if (filename.isNull())
    return;
  
  if (!map_->loadData(filename))
    throw std::runtime_error("Error while load data from file!");
}

void MainWindow::onSaveData()
{
  QString filename = QFileDialog::getSaveFileName(this, "Select file", ".", "data file(*.dat)");

  if (filename.isNull())
    return;

  map_->saveData(filename);
}