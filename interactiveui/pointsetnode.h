#ifndef POINTSETNODE_H
#define POINTSETNODE_H
#include "cavc/polyline.hpp"
#include <QColor>
#include <QSGOpacityNode>

class PointSetNode : public QSGOpacityNode {
public:
  PointSetNode();
  QColor const &color() const;
  void setColor(QColor const &color);
  void addPoint(qreal x, qreal y);
  void addPolylineVertexes(cavc::Polyline<double> const &polyline);
  void clear();

private:
  QColor m_color;
};

#endif // POINTSETNODE_H
