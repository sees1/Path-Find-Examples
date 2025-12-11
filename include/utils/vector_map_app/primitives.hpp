#pragma once

#include <optional>
#include <memory>

#include <QtWidgets>

class Road : public QObject
{
  Q_OBJECT
public:
  enum Type {Arc = 0, Straight};

  Road(const Type& t)
  : type(t)
  { }
  Road(const QPointF& s,
       const QPointF& e)
  : points({s, e}),
    type(Type::Straight)
  { }
  Road(const QPointF& s,
       const QPointF& m,
       const QPointF& e)
  : points({s, e, m}),
    type(Type::Arc)
  { }

  virtual void setNextPoint(const QPointF& s) = 0;
  virtual QPainterPath getPath(const QPoint& offset) = 0;
  
  Type getType() { return type; };
  bool underPoint(const QPoint& offset);
  bool isRoadBuilded();

protected:
  std::vector<std::optional<QPointF>> points;
  std::shared_ptr<QPainterPath> path;
  Type type;
};

class RoadArc : public Road
{
  Q_OBJECT
public:

  RoadArc()
  : Road(Type::Arc)
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
  : Road(Type::Straight)
  { }

  RoadStraight(const QPointF& s,
               const QPointF& e)
  : Road(s, e)
  { }

  void setNextPoint(const QPointF& s) override;
  QPainterPath getPath(const QPoint& offset) override;
};