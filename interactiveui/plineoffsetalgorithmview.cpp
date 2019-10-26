#include "plineoffsetalgorithmview.h"
#include "graphicshelpers.h"
#include "plinesegmentnode.h"
#include "polylinenode.h"
#include "rawoffsetsegmentsnode.h"
#include "spatialindexboundingboxesnode.h"
#include <QSGNode>
#include <QSGTransformNode>
using namespace cavc;

namespace {
QSGGeometryNode *createSimpleGeomNode(int vertexCount, QColor const &color, float width,
                                      QSGGeometry::DrawingMode mode) {
  QSGGeometryNode *node = new QSGGeometryNode();
  node->setFlag(QSGNode::OwnedByParent);

  QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), vertexCount);
  geometry->setDrawingMode(mode);
  geometry->setLineWidth(width);
  node->setGeometry(geometry);
  node->setFlag(QSGNode::OwnsGeometry);

  QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
  material->setColor(color);
  node->setMaterial(material);
  node->setFlag(QSGNode::OwnsMaterial);

  return node;
}
}

PlineOffsetAlgorithmView::PlineOffsetAlgorithmView(QQuickItem *parent)
    : QQuickItem(parent), m_origPolylineNode(nullptr), m_rawOffsetPolylineNode(nullptr),
      m_untrimmedSegmentsParentNode(nullptr), m_selfIntersectsNode(nullptr),
      m_boundingBoxesNode(nullptr), m_slicesParentNode(nullptr), m_repeatOffsetsParentNode(nullptr),
      m_uiScaleFactor(20), m_globalMouseDownPoint(),
      m_vertexGrabbed(std::numeric_limits<std::size_t>::max()), m_interacting(false),
      m_showOrigPlineVertexes(true), m_showRawOffsetSegments(false), m_showRawOffsetPolyline(false),
      m_showRawOffsetPlineVertexes(false), m_plineOffset(0.5), m_repeatOffsetCount(0),
      m_spatialIndexTarget(None), m_selfIntersectsTarget(None),
      m_finishedPolyline(NoFinishedPline) {

  setFlag(ItemHasContents, true);
  setAcceptedMouseButtons(Qt::LeftButton);
  m_realToUICoord.translate(static_cast<float>(width() / 2), static_cast<float>(height() / 2));
  m_realToUICoord.scale(static_cast<float>(m_uiScaleFactor), static_cast<float>(-m_uiScaleFactor));
  //    m_inputPolyline.addVertex(0, 0, 0.5);
  //    m_inputPolyline.addVertex(1, 1, 0.5);
  //    m_inputPolyline.addVertex(2, 2, 0);
  //    m_inputPolyline.addVertex(2, 3, 1);
  //    m_inputPolyline.addVertex(2, 5, -0.1);
  //    m_inputPolyline.isClosed() = true;

//    m_inputPolyline.addVertex(0, 0, 0);
//    m_inputPolyline.addVertex(2, 1, 0.0);
//    m_inputPolyline.addVertex(2, 2, 0);
//    m_inputPolyline.addVertex(2, 3, 0.0);
//    m_inputPolyline.addVertex(2, 5, 0);
//    m_inputPolyline.isClosed() = true;

      m_inputPolyline.addVertex(0, 25, 1);
      m_inputPolyline.addVertex(0, 0, 0);
      m_inputPolyline.addVertex(2, 0, 1);
      m_inputPolyline.addVertex(10, 0, -0.5);
      m_inputPolyline.addVertex(8, 9, 0.374794619217547);
      m_inputPolyline.addVertex(21, 0, 0);
      m_inputPolyline.addVertex(23, 0, 1);
      m_inputPolyline.addVertex(32, 0, -0.5);
      m_inputPolyline.addVertex(28, 0, 0.5);
      m_inputPolyline.addVertex(39, 21, 0);
      m_inputPolyline.addVertex(28, 12, 0);
//      m_inputPolyline.isClosed() = true;

//  auto radius = 40;
//  auto centerX = 0;
//  auto centerY = 0;
//  std::size_t segmentCount = 10;
//  for (std::size_t i = 0; i < segmentCount; ++i) {
//    double angle = static_cast<double>(i) * utils::tau<double> / segmentCount;
//    m_inputPolyline.addVertex(radius * std::cos(angle) + centerX,
//                              radius * std::sin(angle) + centerY, i % 2 == 0 ? 1 : -1);
//  }
//  m_inputPolyline.isClosed() = true;
}

bool PlineOffsetAlgorithmView::interacting() const { return m_interacting; }

bool PlineOffsetAlgorithmView::showRawOffsetSegments() const { return m_showRawOffsetSegments; }

bool PlineOffsetAlgorithmView::showRawOffsetPolyline() const { return m_showRawOffsetPolyline; }

void PlineOffsetAlgorithmView::setShowRawOffsetSegments(bool showRawOffsetSegments) {
  if (m_showRawOffsetSegments == showRawOffsetSegments)
    return;

  m_showRawOffsetSegments = showRawOffsetSegments;
  update();
  emit showRawOffsetSegmentsChanged(m_showRawOffsetSegments);
}

void PlineOffsetAlgorithmView::setShowRawOffsetPolyline(bool showRawOffsetPolyline) {
  if (m_showRawOffsetPolyline == showRawOffsetPolyline)
    return;

  m_showRawOffsetPolyline = showRawOffsetPolyline;
  update();
  emit showRawOffsetPolylineChanged(m_showRawOffsetPolyline);
}

double PlineOffsetAlgorithmView::plineOffset() const { return m_plineOffset; }

void PlineOffsetAlgorithmView::setPlineOffset(double plineOffset) {
  if (qFuzzyCompare(m_plineOffset, plineOffset))
    return;

  m_plineOffset = plineOffset;
  update();
  emit plineOffsetChanged(m_plineOffset);
}

bool PlineOffsetAlgorithmView::showOrigPlineVertexes() const { return m_showOrigPlineVertexes; }

void PlineOffsetAlgorithmView::setShowOrigPlineVertexes(bool showOrigPlineVertexes) {
  if (m_showOrigPlineVertexes == showOrigPlineVertexes)
    return;

  m_showOrigPlineVertexes = showOrigPlineVertexes;
  update();
  emit showOrigPlineVertexesChanged(m_showOrigPlineVertexes);
}

bool PlineOffsetAlgorithmView::showRawOffsetPlineVertexes() const {
  return m_showRawOffsetPlineVertexes;
}

void PlineOffsetAlgorithmView::setShowRawOffsetPlineVertexes(bool showRawOffsetPlineVertexes) {
  if (m_showRawOffsetPlineVertexes == showRawOffsetPlineVertexes)
    return;

  m_showRawOffsetPlineVertexes = showRawOffsetPlineVertexes;
  update();
  emit showRawOffsetPlineVertexesChanged(m_showRawOffsetPlineVertexes);
}

PlineOffsetAlgorithmView::TargetPolyline PlineOffsetAlgorithmView::spatialIndexTarget() const {
  return m_spatialIndexTarget;
}

void PlineOffsetAlgorithmView::setSpatialIndexTarget(
    PlineOffsetAlgorithmView::TargetPolyline spatialIndexTarget) {
  if (m_spatialIndexTarget == spatialIndexTarget)
    return;

  m_spatialIndexTarget = spatialIndexTarget;
  update();
  emit spatialIndexTargetChanged(m_spatialIndexTarget);
}

PlineOffsetAlgorithmView::TargetPolyline PlineOffsetAlgorithmView::selfIntersectsTarget() const {
  return m_selfIntersectsTarget;
}

void PlineOffsetAlgorithmView::setSelfIntersectsTarget(
    PlineOffsetAlgorithmView::TargetPolyline selfIntersectsTarget) {
  if (m_selfIntersectsTarget == selfIntersectsTarget)
    return;

  m_selfIntersectsTarget = selfIntersectsTarget;
  update();
  emit selfIntersectsTargetChanged(m_selfIntersectsTarget);
}

PlineOffsetAlgorithmView::FinishedPolyline PlineOffsetAlgorithmView::finishedPolyline() const {
  return m_finishedPolyline;
}

void PlineOffsetAlgorithmView::setFinishedPolyline(
    PlineOffsetAlgorithmView::FinishedPolyline finishedPolyline) {
  if (m_finishedPolyline == finishedPolyline)
    return;

  m_finishedPolyline = finishedPolyline;
  update();
  emit finishedPolylineChanged(m_finishedPolyline);
}

int PlineOffsetAlgorithmView::offsetCount() const { return m_repeatOffsetCount; }

void PlineOffsetAlgorithmView::setOffsetCount(int offsetCount) {
  if (m_repeatOffsetCount == offsetCount)
    return;

  m_repeatOffsetCount = offsetCount;
  update();
  emit repeatOffsetCountChanged(m_repeatOffsetCount);
}

void PlineOffsetAlgorithmView::setInteracting(bool interacting) {
  if (m_interacting == interacting)
    return;

  m_interacting = interacting;
  emit interactingChanged(m_interacting);
}

QSGNode *PlineOffsetAlgorithmView::updatePaintNode(QSGNode *oldNode,
                                                   QQuickItem::UpdatePaintNodeData *) {
  QSGTransformNode *rootNode = nullptr;
  if (!oldNode) {
    rootNode = new QSGTransformNode();
    m_origPolylineNode = new PolylineNode();
    m_origPolylineNode->setVertexesVisible(true);
    m_origPolylineNode->setFlag(QSGNode::OwnedByParent);
    rootNode->appendChildNode(m_origPolylineNode);

    m_rawOffsetPolylineNode = new PolylineNode();
    m_rawOffsetPolylineNode->setFlag(QSGNode::OwnedByParent);
    m_rawOffsetPolylineNode->setColor(Qt::darkGreen);
    rootNode->appendChildNode(m_rawOffsetPolylineNode);

    m_untrimmedSegmentsParentNode = new RawOffsetSegmentsNode();
    m_untrimmedSegmentsParentNode->setOpacity(0);
    m_untrimmedSegmentsParentNode->setFlag(QSGNode::OwnedByParent);
    rootNode->appendChildNode(m_untrimmedSegmentsParentNode);

    m_boundingBoxesNode = new SpatialIndexBoundingBoxesNode();
    m_boundingBoxesNode->setOpacity(0);
    m_boundingBoxesNode->setFlag(QSGNode::OwnedByParent);
    rootNode->appendChildNode(m_boundingBoxesNode);
  } else {
    rootNode = static_cast<QSGTransformNode *>(oldNode);
  }

  rootNode->setMatrix(m_realToUICoord);
  auto prunedPline = pruneSingularities(m_inputPolyline);
  m_origPolylineNode->setVertexesVisible(m_showOrigPlineVertexes);
  m_origPolylineNode->updateGeometry(prunedPline, m_uiScaleFactor);

  cavc::Polyline<double> rawOffsetPline;
  // raw offset polyline
  if (m_showRawOffsetPolyline) {
    rawOffsetPline = createRawOffsetPline(prunedPline, m_plineOffset, 1);
    m_rawOffsetPolylineNode->setIsVisible(true);
    m_rawOffsetPolylineNode->setVertexesVisible(m_showRawOffsetPlineVertexes);
    m_rawOffsetPolylineNode->updateGeometry(rawOffsetPline, m_uiScaleFactor);
  } else {
    m_rawOffsetPolylineNode->setIsVisible(false);
  }

  // raw offset segments
  if (m_showRawOffsetSegments) {
    m_untrimmedSegmentsParentNode->setOpacity(1);
    auto rawOffsetSegments = createUntrimmedOffsetSegments(prunedPline, m_plineOffset);
    m_untrimmedSegmentsParentNode->updateGeometry(rawOffsetSegments, m_uiScaleFactor);
  } else {
    m_untrimmedSegmentsParentNode->setOpacity(0);
  }

  // self intersects
  std::vector<cavc::PlineSelfIntersect<double>> selfIntersects;
  switch (m_selfIntersectsTarget) {
  case PlineOffsetAlgorithmView::None:
    break;
  case PlineOffsetAlgorithmView::OriginalPolyline: {
    auto spatialindex = createApproxSpatialIndex(prunedPline);
    allSelfIntersects(prunedPline, selfIntersects, spatialindex);
  } break;
  case PlineOffsetAlgorithmView::RawOffsetPolyline: {
    if (rawOffsetPline.size() == 0) {
      rawOffsetPline = createRawOffsetPline(prunedPline, m_plineOffset, 1);
    }
    auto spatialIndex = createApproxSpatialIndex(rawOffsetPline);
    allSelfIntersects(rawOffsetPline, selfIntersects, spatialIndex);
  } break;
  }

  if (!prunedPline.isClosed()) {
    static QSGGeometryNode *startNode = nullptr;
    static QSGGeometryNode *endNode = createSimpleGeomNode(50, QColor("orange"), 1, QSGGeometry::DrawLineLoop);
    if (!startNode) {
      startNode = createSimpleGeomNode(50, QColor("orange"), 1, QSGGeometry::DrawLineLoop);
      rootNode->appendChildNode(startNode);
      rootNode->appendChildNode(endNode);
    }

    auto *vData = startNode->geometry()->vertexDataAsPoint2D();
    auto rad = std::abs(m_plineOffset);
    for (std::size_t i = 0; i < 50; ++i) {
      double angle = static_cast<double>(i) / 50 * cavc::utils::tau<double>;
      auto pt = cavc::pointOnCircle(rad, prunedPline[0].pos(), angle);
      vData[i].set(pt.x(), pt.y());
    }

    vData = endNode->geometry()->vertexDataAsPoint2D();
    for (std::size_t i = 0; i < 50; ++i) {
      double angle = static_cast<double>(i) / 50 * cavc::utils::tau<double>;
      auto pt = cavc::pointOnCircle(rad, prunedPline.lastVertex().pos(), angle);
      vData[i].set(pt.x(), pt.y());
    }

    startNode->markDirty(QSGNode::DirtyGeometry);
    endNode->markDirty(QSGNode::DirtyGeometry);

  }


  if (selfIntersects.size() != 0) {
    if (!m_selfIntersectsNode) {
      m_selfIntersectsNode = createSimpleGeomNode(static_cast<int>(selfIntersects.size()),
                                                  Qt::darkCyan, 8, QSGGeometry::DrawPoints);
      rootNode->appendChildNode(m_selfIntersectsNode);
    } else {
      m_selfIntersectsNode->geometry()->allocate(static_cast<int>(selfIntersects.size()));
    }

    QSGGeometry::Point2D *siVertexes = m_selfIntersectsNode->geometry()->vertexDataAsPoint2D();
    for (std::size_t i = 0; i < selfIntersects.size(); ++i) {
      siVertexes[i].set(static_cast<float>(selfIntersects[i].pos.x()),
                        static_cast<float>(selfIntersects[i].pos.y()));
    }
    m_selfIntersectsNode->markDirty(QSGNode::DirtyGeometry);
  } else if (m_selfIntersectsNode) {
    rootNode->removeChildNode(m_selfIntersectsNode);
    delete m_selfIntersectsNode;
    m_selfIntersectsNode = nullptr;
  }

  // spatial index bounding box
  switch (m_spatialIndexTarget) {
  case PlineOffsetAlgorithmView::None:
    m_boundingBoxesNode->setOpacity(0);
    break;
  case PlineOffsetAlgorithmView::OriginalPolyline: {
    m_boundingBoxesNode->setOpacity(1);
    auto spatialIndex = createApproxSpatialIndex(prunedPline);
    m_boundingBoxesNode->updateGeometry(spatialIndex, m_uiScaleFactor);
    break;
  }
  case PlineOffsetAlgorithmView::RawOffsetPolyline: {
    if (rawOffsetPline.size() > 1) {
      m_boundingBoxesNode->setOpacity(1);
      auto spatialIndex = createApproxSpatialIndex(rawOffsetPline);
      m_boundingBoxesNode->updateGeometry(spatialIndex, m_uiScaleFactor);
    } else {
      m_boundingBoxesNode->setOpacity(0);
    }
    break;
  }
  }

  // slices
  if (m_finishedPolyline != PlineOffsetAlgorithmView::NoFinishedPline) {
    if (rawOffsetPline.size() == 0) {
      rawOffsetPline = createRawOffsetPline(prunedPline, m_plineOffset, 1);
    }
    if (!m_slicesParentNode) {
      m_slicesParentNode = new QSGOpacityNode();
      rootNode->appendChildNode(m_slicesParentNode);
    }

    m_slicesParentNode->setOpacity(1);
    PolylineNode *sliceNode = static_cast<PolylineNode *>(m_slicesParentNode->firstChild());
    std::size_t sliceIndex = 0;
    auto slices = sliceAtIntersects(prunedPline, rawOffsetPline, m_plineOffset);

    auto addPline = [&](cavc::Polyline<double> const &pline) {
      if (!sliceNode) {
        sliceNode = new PolylineNode();
        m_slicesParentNode->appendChildNode(sliceNode);
      }
      sliceNode->setColor(gh::indexToColor(sliceIndex));
      sliceNode->setIsVisible(true);
      sliceNode->setVertexesVisible(false);
      sliceNode->updateGeometry(pline, m_uiScaleFactor);
      sliceNode = static_cast<PolylineNode *>(sliceNode->nextSibling());
      sliceIndex++;
    };

    if (m_finishedPolyline == PlineOffsetAlgorithmView::Slices) {
      for (const auto &slice : slices) {
        addPline(slice.pline);
      }

    } else {
      auto stichedPlines = stitchSlicesTogether(slices, rawOffsetPline.isClosed(), rawOffsetPline.size() - 1);
      for (const auto &pline : stichedPlines) {
        addPline(pline);
      }
    }

    while (sliceNode) {
      sliceNode->setIsVisible(false);
      sliceNode = static_cast<PolylineNode *>(sliceNode->nextSibling());
    }

  } else if (m_slicesParentNode) {
    m_slicesParentNode->setOpacity(0);
  }

  // repeat offsets
  if (m_repeatOffsetCount > 0 && !qFuzzyCompare(m_plineOffset, 0.0)) {
    if (!m_repeatOffsetsParentNode) {
      m_repeatOffsetsParentNode = new QSGOpacityNode();
      rootNode->appendChildNode(m_repeatOffsetsParentNode);
    }

    m_repeatOffsetsParentNode->setOpacity(1);
    PolylineNode *offsetNode = static_cast<PolylineNode *>(m_repeatOffsetsParentNode->firstChild());

    auto addPline = [&](cavc::Polyline<double> const &pline, QColor color) {
      if (!offsetNode) {
        offsetNode = new PolylineNode();
        m_repeatOffsetsParentNode->appendChildNode(offsetNode);
      }
      offsetNode->setColor(color);
      offsetNode->setIsVisible(true);
      offsetNode->setVertexesVisible(false);
      offsetNode->updateGeometry(pline, m_uiScaleFactor);
      offsetNode = static_cast<PolylineNode *>(offsetNode->nextSibling());
    };

    std::vector<cavc::Polyline<double>> prevOffsets;
    double origPlineA = area(prunedPline);
    if (std::abs(origPlineA) > 1e-4) {
      prevOffsets.push_back(prunedPline);
    }

    for (int i = 0; i < m_repeatOffsetCount; ++i) {
      if (prevOffsets.size() == 0) {
        break;
      }
      std::vector<cavc::Polyline<double>> newOffsets;
      for (const auto &pline : prevOffsets) {
        auto offs = paralleOffset(pline, m_plineOffset);
        newOffsets.insert(newOffsets.end(), std::make_move_iterator(offs.begin()), std::make_move_iterator(offs.end()));
      }
      auto copy = prevOffsets;
      prevOffsets = std::move(newOffsets);
      prevOffsets.erase(std::remove_if(prevOffsets.begin(), prevOffsets.end(),
                                       [&](const auto &pline) {
                                         double a = area(pline);
                                         return (a > 0 != origPlineA > 0) ||
                                                std::abs(a) < 1e-4;
                                       }),
                        prevOffsets.end());
      if (prevOffsets.size() == 0) {
      for (const auto &pline : copy) {
        auto rOffset = createRawOffsetPline(pline, m_plineOffset, origPlineA > 0);
        addPline(rOffset, QColor("red"));
        auto retry = paralleOffset(pline, m_plineOffset);
      }
      }
      for (const auto &pline : prevOffsets) {
        addPline(pline, QColor("blue"));
      }
    }

    while (offsetNode) {
      offsetNode->setIsVisible(false);
      offsetNode = static_cast<PolylineNode *>(offsetNode->nextSibling());
    }

  } else if (m_repeatOffsetsParentNode) {
    m_repeatOffsetsParentNode->setOpacity(0);
  }

  return rootNode;
}

void PlineOffsetAlgorithmView::mousePressEvent(QMouseEvent *event) {
  if (!m_showOrigPlineVertexes) {
    event->ignore();
    return;
  }
  // converting to global coordinates to get screen resolution delta even if current scale != 1
  m_globalMouseDownPoint = QPointF(event->globalX(), event->globalY());
  auto containsVertex = [&](const QPointF &vPosInGlobal) {
    return utils::fuzzyEqual(vPosInGlobal.x(), m_globalMouseDownPoint.x(), 5.0) &&
           utils::fuzzyEqual(vPosInGlobal.y(), m_globalMouseDownPoint.y(), 5.0);
  };

  m_vertexGrabbed = std::numeric_limits<std::size_t>::max();
  double minDist = std::numeric_limits<double>::infinity();
  for (std::size_t i = 0; i < m_inputPolyline.size(); ++i) {
    QPointF vPosInGlobal =
        mapToGlobal(m_realToUICoord * QPointF(m_inputPolyline[i].x(), m_inputPolyline[i].y()));
    if (containsVertex(vPosInGlobal)) {
      // grab the vertex closest to the mouse in the case of vertexes being very close to each other
      QPointF vDiff = vPosInGlobal - m_globalMouseDownPoint;
      double dist = QPointF::dotProduct(vDiff, vDiff);
      if (dist < minDist) {
        m_origVertexGlobalPos = vPosInGlobal;
        m_vertexGrabbed = i;
        minDist = dist;
      }
    }
  }

  if (m_vertexGrabbed == std::numeric_limits<std::size_t>::max()) {
    event->ignore();
    return;
  }

  setInteracting(true);
  event->accept();
}

void PlineOffsetAlgorithmView::mouseMoveEvent(QMouseEvent *event) {
  if (!isVertexGrabbed()) {
    return;
  }

  // convert back from global coordinates to get real delta
  QPointF mouseDelta = QPointF(event->globalX(), event->globalY()) - m_globalMouseDownPoint;
  QPointF newGlobalVertexPos = mouseDelta + m_origVertexGlobalPos;
  QPointF newLocalVertexPos = mapFromGlobal(newGlobalVertexPos);
  QPointF newRealVertexPos = m_uiToRealCoord * newLocalVertexPos;

  m_inputPolyline[m_vertexGrabbed].x() = newRealVertexPos.x();
  m_inputPolyline[m_vertexGrabbed].y() = newRealVertexPos.y();
  update();
}

void PlineOffsetAlgorithmView::mouseReleaseEvent(QMouseEvent *event) {
  if (!isVertexGrabbed()) {
    return;
  }
  setInteracting(false);
  m_vertexGrabbed = std::numeric_limits<std::size_t>::max();
  event->accept();
}

bool PlineOffsetAlgorithmView::isVertexGrabbed() {
  return m_vertexGrabbed != std::numeric_limits<std::size_t>::max();
}

void PlineOffsetAlgorithmView::geometryChanged(const QRectF &newGeometry,
                                               const QRectF &oldGeometry) {
  Q_UNUSED(oldGeometry)
  QQuickItem::geometryChanged(newGeometry, oldGeometry);
  m_realToUICoord.setToIdentity();
  m_realToUICoord.translate(static_cast<float>(newGeometry.width() / 2),
                            static_cast<float>(newGeometry.height() / 2));
  m_realToUICoord.scale(static_cast<float>(m_uiScaleFactor), static_cast<float>(-m_uiScaleFactor));
  m_uiToRealCoord = m_realToUICoord.inverted();
  update();
}
