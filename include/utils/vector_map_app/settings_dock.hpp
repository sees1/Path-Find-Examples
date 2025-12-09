#include <QDockWidget>

#include "utils/vector_map_app/map.hpp"

class SettingsDock : public QDockWidget
{
  Q_OBJECT
public:
  SettingsDock(Map* copy, QWidget* parent = nullptr);

signals:
  void sigPassPolyCount(int poly_count);

public slots:
  void savePolyCount();

private:
  Map* map_;

  QLineEdit* e_road_poly_count_;
  int poly_count_;
};