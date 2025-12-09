#pragma once

#include <vector>
#include <stack>

#include <QtWidgets>

#include "utils/vector_map_app/primitives.hpp"

enum class Mode {
  IDLE = 0,
  CREATE_ARC_ROAD,
  CREATE_STRAIGHT_ROAD,
  DELETE_ROAD,
  MOVE_CAMERA
};

class Map : public QWidget
{
  Q_OBJECT
public:
  Map(QWidget* parent = nullptr);

  // viewport modifiers
  void pan(const QPoint& delta);
  // void scale(double factor, QPointF center);
  // void resizeCanvas(int new_w, int new_h);

  // IO methods
  bool saveData(const QString& filename);
  bool loadData(const QString& filename); 

private:
  QPoint viewportToGlobalCoord(const QPoint& coord);
  QPoint globalToViewportCood(const QPoint& coord);

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

private:
  Mode mode_;

  int poly_count_;

  std::vector<std::shared_ptr<Road>> roads_;

  bool panning_;
  QPoint last_mouse_pos_;
  QPoint viewport_offset_;

  QScrollArea* scroll_area_;

  QImage costmap_img_;
};