#ifndef GEOMETRYCANVASITEM_H
#define GEOMETRYCANVASITEM_H

#include "cavc/polyline.hpp"
#include <QMatrix4x4>
#include <QQuickItem>
#include <QtGlobal>

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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#else
  void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#endif
  QPointF convertToGlobalUICoord(const cavc::Vector2<double> &pt);
  std::size_t vertexUnderPosition(QPointF uiGlobalPos, const cavc::Polyline<double> &pline);
};

#endif // GEOMETRYCANVASITEM_H
