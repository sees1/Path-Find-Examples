#pragma once

#include "utils/vector_map_app/primitives.hpp"

#include <QString>

#include <optional>

class MapData {
public:
  MapData() { }
  MapData(const QPoint& offset, float resolution);

  bool hasCostmapInfo();
  bool setCostmapInfo(float resolution, const QPoint& offset);

  void saveTo(const QString& filename, const std::vector<std::shared_ptr<Road>>& roads, int poly_count);
  void load(const QString& filename);

private:
  std::vector<QLineF> subdivideRoad(std::shared_ptr<Road> road, int poly_count);

private:
  std::optional<QPoint> map_offset_;
  std::optional<float>  map_resolution_;
};