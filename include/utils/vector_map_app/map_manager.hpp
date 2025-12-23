#pragma once

#include <vector>
#include <stack>

#include <QtWidgets>

#include "utils/vector_map_app/map.hpp"
#include "utils/vector_map_app/map_data.hpp"

enum class Mode {
  IDLE = 0,
  CREATE_ARC_ROAD,
  CREATE_STRAIGHT_ROAD,
  DELETE_ROAD,
  MOVE_CAMERA
};

class MapManager : public QWidget
{
  Q_OBJECT
public:
  MapManager(QWidget* parent = nullptr);
  ~MapManager();

  // viewport modifiers
  void pan(const QPoint& delta);
  // void scale(double factor, QPointF center);
  // void resizeCanvas(int new_w, int new_h);

  // IO methods
  bool saveData(const QString& filename);
  bool loadData(const QString& filename); 
  bool loadCostmap(const QString& filename);

private:
  QPoint viewportToGlobalCoord(const QPoint& coord);
  QPoint globalToViewportCoord(const QPoint& coord);

protected:
  void paintEvent(QPaintEvent* ev) override;
  void mousePressEvent(QMouseEvent* ev) override;
  void mouseMoveEvent(QMouseEvent* ev) override;
  void mouseReleaseEvent(QMouseEvent* ev) override;
  // void wheelEvent(QWheelEvent* ev) override;

public slots:
  void setPolyCount(int poly_count);
  void setArcRoadCreateMode();
  void setStraightRoadCreateMode();
  void setDeleteRoadMode();
  void setMoveCameraMode();
  void goToZeroViewport();

private:
  Mode mode_;

  bool panning_;

  QPoint last_mouse_pos_;

  QPoint viewport_offset_;
  QPoint map_zero_offset_;

  QPixmap* costmap_img_;

  Map* map_;
  MapData* data_;
};