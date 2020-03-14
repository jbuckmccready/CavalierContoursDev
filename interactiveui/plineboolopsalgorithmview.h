#ifndef PLINEBOOLOPSALGORITHMVIEW_H
#define PLINEBOOLOPSALGORITHMVIEW_H

#include "geometrycanvasitem.h"

class PolylineNode;

class PlineBoolOpsAlgorithmView : public GeometryCanvasItem {
  Q_OBJECT
  Q_PROPERTY(bool interacting READ interacting NOTIFY interactingChanged)
  Q_PROPERTY(int windingNumber READ windingNumber NOTIFY windingNumberChanged)
public:
  PlineBoolOpsAlgorithmView(QQuickItem *parent = nullptr);

  int windingNumber() const;
  bool interacting() const;

signals:
  void windingNumberChanged(int windingNumber);

  void interactingChanged(bool interacting);

protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  void setWindingNumber(int windingNumber);
  void setInteracting(bool interacting);
  cavc::Polyline<double> m_plineA;
  PolylineNode *m_polylineANode;
  cavc::Polyline<double> m_plineB;
  PolylineNode *m_polylineBNode;
  int m_windingNumber;

  // members for interaction
  bool isVertexGrabbed();
  QPointF m_globalMouseDownPoint;
  std::size_t m_vertexGrabbed;
  cavc::Polyline<double> *m_polylineGrabbed;
  QPointF m_origVertexGlobalPos;
  bool m_interacting;
};

#endif // PLINEBOOLOPSALGORITHMVIEW_H
