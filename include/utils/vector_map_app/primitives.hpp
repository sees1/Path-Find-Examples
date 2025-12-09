#pragma once

#include <optional>
#include <memory>

#include <QtWidgets>

class Road : public QObject
{
  Q_OBJECT
public:
  Road() { }
  Road(const QPointF& s,
       const QPointF& e)
  : points({s, e})
  { }
  Road(const QPointF& s,
       const QPointF& m,
       const QPointF& e)
  : points({s, e, m})
  { }

  virtual void setNextPoint(const QPointF& s) = 0;
  virtual QPainterPath getPath(const QPoint& offset) = 0;
  
  bool underPoint(const QPoint& offset);
  bool isRoadBuilded();

protected:
  std::vector<std::optional<QPointF>> points;
  std::shared_ptr<QPainterPath> path;
};

class RoadArc : public Road
{
  Q_OBJECT
public:

  RoadArc()
  : Road()
  { }

  RoadArc(const QPointF& s,
          const QPointF& m,
          const QPointF& e)
  : Road(s, m, e)
  { }

  void setNextPoint(const QPointF& s) override;
  QPainterPath getPath(const QPoint& offset) override;
};

class RoadStraight : public Road
{
  Q_OBJECT
public:

  RoadStraight()
  : Road()
  { }

  RoadStraight(const QPointF& s,
               const QPointF& e)
  : Road(s, e)
  { }

  void setNextPoint(const QPointF& s) override;
  QPainterPath getPath(const QPoint& offset) override;
};