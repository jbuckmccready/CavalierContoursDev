#ifndef PLINEOFFSETISLANDSALGORITHMVIEW_H
#define PLINEOFFSETISLANDSALGORITHMVIEW_H

#include "cavc/polyline.hpp"
#include "geometrycanvasitem.h"
#include <vector>

class PolylineNode;
class QSGOpacityNode;

class PlineOffsetIslandsAlgorithmView : public GeometryCanvasItem {
  Q_OBJECT
  Q_PROPERTY(bool showVertexes READ showVertexes WRITE setShowVertexes NOTIFY showVertexesChanged)
  Q_PROPERTY(double offsetDelta READ offsetDelta WRITE setOffsetDelta NOTIFY offsetDeltaChanged)
  Q_PROPERTY(int offsetCount READ offsetCount WRITE setOffsetCount NOTIFY offsetCountChanged)
public:
  explicit PlineOffsetIslandsAlgorithmView(QQuickItem *parent = nullptr);

  bool showVertexes() const;
  void setShowVertexes(bool showVertexes);

  double offsetDelta() const;
  void setOffsetDelta(double offsetDelta);

  int offsetCount() const;
  void setOffsetCount(int offsetCount);

signals:
  void showVertexesChanged(bool showVertexes);
  void offsetDeltaChanged(double offsetDelta);
  void offsetCountChanged(int offsetCount);

protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  std::vector<cavc::Polyline<double>> m_ccwLoops;
  std::vector<cavc::Polyline<double>> m_cwLoops;
  QSGOpacityNode *m_dynamicPlinesParentNode;
  bool m_showVertexes;
  double m_offsetDelta;
  int m_offsetCount;

  bool isVertexGrabbed();
  QPointF m_globalMouseDownPoint;
  std::size_t m_vertexGrabbed;
  cavc::Polyline<double> *m_polylineGrabbed;
  QPointF m_origVertexGlobalPos;
};

#endif // PLINEOFFSETISLANDSALGORITHMVIEW_H
