#include <QDockWidget>

#include "utils/vector_map_app/map.hpp"

class ToolsDock : public QDockWidget
{
  Q_OBJECT
public:
  ToolsDock(Map* copy, QWidget* parent = nullptr);

private:
  Map* map_;

  QPushButton* b_move_camera_;
  QToolButton* b_create_road_;
  QAction* a_create_arc_road_;
  QAction* a_create_straight_road_;
  QPushButton* b_delete_road_;
};