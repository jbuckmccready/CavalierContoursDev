#ifndef PLINEOFFSETALGORITHMVIEW_H
#define PLINEOFFSETALGORITHMVIEW_H

#include "polyline.h"
#include <QMatrix4x4>
#include <QQuickItem>
class PolylineNode;
class RawOffsetSegmentsNode;
class QSGGeometryNode;
class SpatialIndexBoundingBoxesNode;
class QSGOpacityNode;

class PlineOffsetAlgorithmView : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(bool interacting READ interacting NOTIFY interactingChanged)
  Q_PROPERTY(bool showOrigPlineVertexes READ showOrigPlineVertexes WRITE setShowOrigPlineVertexes
                 NOTIFY showOrigPlineVertexesChanged)
  Q_PROPERTY(bool showRawOffsetSegments READ showRawOffsetSegments WRITE setShowRawOffsetSegments
                 NOTIFY showRawOffsetSegmentsChanged)
  Q_PROPERTY(bool showRawOffsetPolyline READ showRawOffsetPolyline WRITE setShowRawOffsetPolyline
                 NOTIFY showRawOffsetPolylineChanged)
  Q_PROPERTY(bool showRawOffsetPlineVertexes READ showRawOffsetPlineVertexes WRITE
                 setShowRawOffsetPlineVertexes NOTIFY showRawOffsetPlineVertexesChanged)
  Q_PROPERTY(TargetPolyline spatialIndexTarget READ spatialIndexTarget WRITE setSpatialIndexTarget
                 NOTIFY spatialIndexTargetChanged)
  Q_PROPERTY(TargetPolyline selfIntersectsTarget READ selfIntersectsTarget WRITE
                 setSelfIntersectsTarget NOTIFY selfIntersectsTargetChanged)
  Q_PROPERTY(FinishedPolyline finishedPolyline READ finishedPolyline WRITE setFinishedPolyline
                 NOTIFY finishedPolylineChanged)
  Q_PROPERTY(double plineOffset READ plineOffset WRITE setPlineOffset NOTIFY plineOffsetChanged)
  Q_PROPERTY(int repeatOffsetCount READ offsetCount WRITE setOffsetCount NOTIFY repeatOffsetCountChanged)
public:
  enum TargetPolyline { None, OriginalPolyline, RawOffsetPolyline };
  Q_ENUM(TargetPolyline)

  enum FinishedPolyline { NoFinishedPline, Slices, Joined };
  Q_ENUM(FinishedPolyline)

  explicit PlineOffsetAlgorithmView(QQuickItem *parent = nullptr);
  bool interacting() const;

  bool showRawOffsetSegments() const;
  bool showRawOffsetPolyline() const;

  void setShowRawOffsetSegments(bool showRawOffsetSegments);
  void setShowRawOffsetPolyline(bool showRawOffsetPolyline);

  double plineOffset() const;
  void setPlineOffset(double plineOffset);

  bool showOrigPlineVertexes() const;
  void setShowOrigPlineVertexes(bool showOrigPlineVertexes);

  bool showRawOffsetPlineVertexes() const;

  void setShowRawOffsetPlineVertexes(bool showRawOffsetPlineVertexes);

  TargetPolyline spatialIndexTarget() const;
  void setSpatialIndexTarget(TargetPolyline spatialIndexTarget);

  TargetPolyline selfIntersectsTarget() const;
  void setSelfIntersectsTarget(TargetPolyline selfIntersectsTarget);

  FinishedPolyline finishedPolyline() const;
  void setFinishedPolyline(FinishedPolyline finishedPolyline);

  int offsetCount() const;
  void setOffsetCount(int offsetCount);

signals:
  void interactingChanged(bool interacting);
  void showRawOffsetSegmentsChanged(bool showRawOffsetSegments);
  void showRawOffsetPolylineChanged(bool showRawOffsetPolyline);
  void plineOffsetChanged(double plineOffset);
  void showOrigPlineVertexesChanged(bool showOrigPlineVertexes);
  void showRawOffsetPlineVertexesChanged(bool showRawOffsetPlineVertexes);
  void spatialIndexTargetChanged(TargetPolyline spatialIndexTarget);
  void selfIntersectsTargetChanged(TargetPolyline selfIntersectsTarget);
  void finishedPolylineChanged(FinishedPolyline finishedPolyline);
  void repeatOffsetCountChanged(int repeatOffsetCount);

protected:
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
  void setInteracting(bool interacting);
  cavc::Polyline<double> m_inputPolyline;
  PolylineNode *m_origPolylineNode;
  PolylineNode *m_rawOffsetPolylineNode;
  RawOffsetSegmentsNode *m_untrimmedSegmentsParentNode;
  QSGGeometryNode *m_selfIntersectsNode;
  SpatialIndexBoundingBoxesNode *m_boundingBoxesNode;
  QSGOpacityNode *m_slicesParentNode;
  QSGOpacityNode *m_repeatOffsetsParentNode;

  // members for mapping "real" coordinates to UI coordinates
  double m_uiScaleFactor;
  QMatrix4x4 m_realToUICoord;
  QMatrix4x4 m_uiToRealCoord;

  // members for interaction
  bool isVertexGrabbed();
  QPointF m_globalMouseDownPoint;
  std::size_t m_vertexGrabbed;
  QPointF m_origVertexGlobalPos;
  bool m_interacting;

  // members for controlling generation
  bool m_showOrigPlineVertexes;
  bool m_showRawOffsetSegments;
  bool m_showRawOffsetPolyline;
  bool m_showRawOffsetPlineVertexes;
  double m_plineOffset;
  int m_repeatOffsetCount;
  TargetPolyline m_spatialIndexTarget;
  TargetPolyline m_selfIntersectsTarget;
  FinishedPolyline m_finishedPolyline;
};

#endif // PLINEOFFSETALGORITHMVIEW_H
