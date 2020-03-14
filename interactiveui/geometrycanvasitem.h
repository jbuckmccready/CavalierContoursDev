#ifndef GEOMETRYCANVASITEM_H
#define GEOMETRYCANVASITEM_H

#include "cavc/polyline.h"
#include <QMatrix4x4>
#include <QQuickItem>

/// Base class for setting up interactive geometry views.
class GeometryCanvasItem : public QQuickItem {
  Q_OBJECT
public:
  explicit GeometryCanvasItem(QQuickItem *parent = nullptr);

signals:

protected:
  // members for mapping "real" coordinates to UI coordinates
  double m_uiScaleFactor;
  QMatrix4x4 m_realToUICoord;
  QMatrix4x4 m_uiToRealCoord;
  void updateCoordMatrices(qreal width, qreal height);
  void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
  QPointF convertToGlobalUICoord(const cavc::Vector2<double> &pt);
  std::size_t vertexUnderPosition(QPointF uiGlobalPos, const cavc::Polyline<double> &pline);
};

#endif // GEOMETRYCANVASITEM_H
