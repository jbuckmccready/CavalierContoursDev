#ifndef PLINECOMBINEALGORITHMVIEW_H
#define PLINECOMBINEALGORITHMVIEW_H

#include "cavc/polyline.hpp"
#include "geometrycanvasitem.h"

class PolylineNode;
class QSGGeometryNode;
class QSGOpacityNode;

class PlineCombineAlgorithmView : public GeometryCanvasItem {
  Q_OBJECT
  Q_PROPERTY(bool interacting READ interacting NOTIFY interactingChanged)
  Q_PROPERTY(int windingNumber READ windingNumber NOTIFY windingNumberChanged)
  Q_PROPERTY(
      bool showIntersects READ showIntersects WRITE setShowIntersects NOTIFY showIntersectsChanged)
  Q_PROPERTY(PlineCombineMode plineCombineMode READ plineCombineMode WRITE setPlineCombineMode
                 NOTIFY plineCombineModeChanged)
  Q_PROPERTY(bool showVertexes READ showVertexes WRITE setShowVertexes NOTIFY showVertexesChanged)
  Q_PROPERTY(bool flipArgOrder READ flipArgOrder WRITE setFlipArgOrder NOTIFY flipArgOrderChanged)
  Q_PROPERTY(bool showWindingNumberPoint READ showWindingNumberPoint WRITE setShowWindingNumberPoint
                 NOTIFY showWindingNumberPointChanged)
public:
  enum PlineCombineMode { NoCombine, Union, Exclude, Intersect, XOR, CoincidentSlices };
  Q_ENUM(PlineCombineMode)
  PlineCombineAlgorithmView(QQuickItem *parent = nullptr);

  int windingNumber() const;
  bool interacting() const;

  bool showIntersects() const;
  void setShowIntersects(bool showIntersects);

  PlineCombineMode plineCombineMode() const;
  void setPlineCombineMode(PlineCombineMode plineCombineMode);

  bool showVertexes() const;
  void setShowVertexes(bool showVertexes);

  bool flipArgOrder() const;
  void setFlipArgOrder(bool flipArgOrder);

  bool showWindingNumberPoint() const;
  void setShowWindingNumberPoint(bool showWindingNumberPoint);

signals:
  void windingNumberChanged(int windingNumber);
  void interactingChanged(bool interacting);
  void showIntersectsChanged(bool showIntersects);
  void plineCombineModeChanged(PlineCombineMode plineCombineMode);
  void showVertexesChanged(bool showVertexes);
  void flipArgOrderChanged(bool flipArgOrder);

  void showWindingNumberPointChanged(bool showWindingNumberPoint);

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
  cavc::Polyline<double> m_testPoint;
  PolylineNode *m_testPointNode;
  QSGGeometryNode *m_intersectsNode;
  QSGOpacityNode *m_dynamicPlinesParentNode;
  int m_windingNumber;

  bool isVertexGrabbed();
  QPointF m_globalMouseDownPoint;
  std::size_t m_vertexGrabbed;
  cavc::Polyline<double> *m_polylineGrabbed;
  QPointF m_origVertexGlobalPos;
  bool m_interacting;
  bool m_showIntersects;
  PlineCombineMode m_plineCombineMode;
  bool m_showVertexes;
  bool m_flipArgOrder;
  bool m_showWindingNumberPoint;
};

#endif // PLINECOMBINEALGORITHMVIEW_H
