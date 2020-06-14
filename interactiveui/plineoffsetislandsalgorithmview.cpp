#include "plineoffsetislandsalgorithmview.h"
#include "cavc/polylineoffsetislands.hpp"
#include "polylinenode.h"
#include <QSGTransformNode>

using namespace cavc;

PlineOffsetIslandsAlgorithmView::PlineOffsetIslandsAlgorithmView(QQuickItem *parent)
    : GeometryCanvasItem(parent),
      m_showVertexes(true),
      m_offsetDelta(1),
      m_offsetCount(20),
      m_vertexGrabbed(std::numeric_limits<std::size_t>::max()),
      m_polylineGrabbed(nullptr) {

  //  m_outerPline.addVertex(0, 1, 1);
  //  m_outerPline.addVertex(10, 1, 1);
  //  m_outerPline.isClosed() = true;

  auto radius = 40;
  auto centerX = 0;
  auto centerY = 0;
  std::size_t segmentCount = 16;
  cavc::Polyline<double> mainOuterPline;
  for (std::size_t i = 0; i < segmentCount; ++i) {
    double angle = static_cast<double>(i) * utils::tau<double>() / segmentCount;
    mainOuterPline.addVertex(radius * std::cos(angle) + centerX, radius * std::sin(angle) + centerY,
                             i % 2 == 0 ? 1 : -1);
  }
  mainOuterPline.isClosed() = true;
  m_ccwLoops.push_back(std::move(mainOuterPline));

  //  int currIslandCount = 0;
  //  auto addIslandCenteredAt = [&] (double x, double y) {
  //    cavc::Polyline<double> island;
  //    island.isClosed() = true;
  //        if (currIslandCount % 2 == 0) {
  //          double halfLength = 1.5;
  //          double halfWidth = 6;
  //          island.addVertex(x - halfWidth, y - halfLength, 0.0);
  //          island.addVertex(x - halfWidth, y + halfLength, 0.0);
  //          island.addVertex(x + halfWidth, y + halfLength, 0.0);
  //          island.addVertex(x + halfWidth, y - halfLength, 0.0);
  //        } else {
  //          double islandRadius = 1.5;
  //          island.addVertex(x - islandRadius, y, -1);
  //          island.addVertex(x + islandRadius, y, -1);
  //        }
  //    currIslandCount += 1;
  //    m_islands.push_back(std::move(island));
  //  };

  //  std::size_t totalIslandCount = 25;
  //  for (std::size_t i = 0; i < totalIslandCount; ++i) {
  //    double angle = 2 * static_cast<double>(i) * utils::tau<double>() / totalIslandCount;
  //    double spiralMag = 0.75 * static_cast<double>(i + 1) / totalIslandCount;
  //   addIslandCenteredAt(spiralMag * radius * std::cos(angle) + centerX, spiralMag * radius *
  //   std::sin(angle) + centerY);
  //  }

  //  for (std::size_t i = 0; i < totalIslandCount; ++i) {
  //    double angle = static_cast<double>(i) * utils::tau<double>() / totalIslandCount;
  //    double spiralMag = 0.65;
  //   addIslandCenteredAt(spiralMag * radius * std::cos(angle) + centerX, spiralMag * radius *
  //   std::sin(angle) + centerY);
  //  }

  //  for (std::size_t i = 0; i < totalIslandCount; ++i) {
  //    double angle = static_cast<double>(i) * utils::tau<double>() / totalIslandCount;
  //    double spiralMag = 0.35;
  //   addIslandCenteredAt(spiralMag * radius * std::cos(angle) + centerX, spiralMag * radius *
  //   std::sin(angle) + centerY);
  //  }

  //  addIslandCenteredAt(0, 0);

  cavc::Polyline<double> island1;
  island1.addVertex(-7, -25, 0);
  island1.addVertex(-4, -25, 0);
  island1.addVertex(-4, -15, 0);
  island1.addVertex(-7, -15, 0);
  island1.isClosed() = true;
  invertDirection(island1);
  m_cwLoops.push_back(std::move(island1));

  //  cavc::Polyline<double> island2;
  //  island2.addVertex(-3, -10, 0);
  //  island2.addVertex(0, -10, 0);
  //  island2.addVertex(0, 10, 0);
  //  island2.addVertex(-3, 10, 0);
  //  island2.isClosed() = true;
  //  invertDirection(island2);
  //  m_islands.push_back(std::move(island2));

  //  cavc::Polyline<double> island3;
  //  island3.addVertex(-9, -10, 0);
  //  island3.addVertex(-6, -10, 0);
  //  island3.addVertex(-6, 10, 0);
  //  island3.addVertex(-9, 10, 0);
  //  island3.isClosed() = true;
  //  invertDirection(island3);
  //  m_islands.push_back(std::move(island3));

  cavc::Polyline<double> island2;
  island2.addVertex(22, -20, 1);
  island2.addVertex(27, -20, 1);
  island2.isClosed() = true;
  invertDirection(island2);
  m_cwLoops.push_back(std::move(island2));

  cavc::Polyline<double> island3;
  island3.addVertex(0, 25, 1);
  island3.addVertex(-4, 0, 0);
  island3.addVertex(2, 0, 1);
  island3.addVertex(10, 0, -0.5);
  island3.addVertex(8, 9, 0.374794619217547);
  island3.addVertex(21, 0, 0);
  island3.addVertex(23, 0, 1);
  island3.addVertex(32, 0, -0.5);
  island3.addVertex(28, 0, 0.5);
  island3.addVertex(28, 12, 0.5);
  island3.isClosed() = true;
  invertDirection(island3);
  m_cwLoops.push_back(std::move(island3));
}

bool PlineOffsetIslandsAlgorithmView::showVertexes() const { return m_showVertexes; }

void PlineOffsetIslandsAlgorithmView::setShowVertexes(bool showVertexes) {
  if (m_showVertexes == showVertexes)
    return;

  m_showVertexes = showVertexes;
  update();
  emit showVertexesChanged(m_showVertexes);
}

double PlineOffsetIslandsAlgorithmView::offsetDelta() const { return m_offsetDelta; }

void PlineOffsetIslandsAlgorithmView::setOffsetDelta(double offsetDelta) {
  if (qFuzzyCompare(m_offsetDelta, offsetDelta))
    return;

  m_offsetDelta = offsetDelta;
  update();
  emit offsetDeltaChanged(m_offsetDelta);
}

int PlineOffsetIslandsAlgorithmView::offsetCount() const { return m_offsetCount; }

void PlineOffsetIslandsAlgorithmView::setOffsetCount(int offsetCount) {
  if (m_offsetCount == offsetCount)
    return;

  m_offsetCount = offsetCount;
  update();
  emit offsetCountChanged(m_offsetCount);
}

QSGNode *PlineOffsetIslandsAlgorithmView::updatePaintNode(QSGNode *oldNode,
                                                          QQuickItem::UpdatePaintNodeData *) {
  QSGTransformNode *rootNode = nullptr;
  if (!oldNode) {
    rootNode = new QSGTransformNode();
    m_dynamicPlinesParentNode = new QSGOpacityNode();
    rootNode->appendChildNode(m_dynamicPlinesParentNode);
    m_dynamicPlinesParentNode->setOpacity(1);
  } else {
    rootNode = static_cast<QSGTransformNode *>(oldNode);
  }
  rootNode->setMatrix(m_realToUICoord);

  PolylineNode *plineNode = static_cast<PolylineNode *>(m_dynamicPlinesParentNode->firstChild());
  auto addPline = [&](cavc::Polyline<double> const &pline,
                      PolylineNode::PathDrawMode drawMode = PolylineNode::NormalPath,
                      QColor color = QColor("blue"), bool vertexesVisible = false,
                      QColor vertexesColor = QColor("red")) {
    if (!plineNode) {
      plineNode = new PolylineNode();
      m_dynamicPlinesParentNode->appendChildNode(plineNode);
    }
    plineNode->setColor(color);
    plineNode->setIsVisible(true);
    plineNode->setVertexesVisible(vertexesVisible);
    plineNode->setVertexesColor(vertexesColor);
    plineNode->updateGeometry(pline, drawMode);
    plineNode = static_cast<PolylineNode *>(plineNode->nextSibling());
  };

  // outer loops
  for (auto const &loop : m_ccwLoops) {
    addPline(loop, PolylineNode::PathDrawMode::NormalPath, QColor("blue"), m_showVertexes,
             QColor("blue"));
  }

  // islands
  for (auto const &island : m_cwLoops) {
    addPline(island, PolylineNode::PathDrawMode::DashedPath, QColor("red"), m_showVertexes);
  }

  if (!utils::fuzzyEqual(m_offsetDelta, 0.0) && m_offsetCount > 0) {
    ParallelOffsetIslands<double> alg;
    OffsetLoopSet<double> loopSet;
    for (auto const &loop : m_ccwLoops) {
      loopSet.ccwLoops.push_back({0, loop, createApproxSpatialIndex(loop)});
    }
    for (auto const &loop : m_cwLoops) {
      loopSet.cwLoops.push_back({0, loop, createApproxSpatialIndex(loop)});
    }

    int i = 0;
    while (i < m_offsetCount) {
      loopSet = alg.compute(loopSet, m_offsetDelta);
      if (loopSet.cwLoops.size() == 0 && loopSet.ccwLoops.size() == 0) {
        break;
      }
      for (auto const &loop : loopSet.cwLoops) {
        addPline(loop.polyline);
      }
      for (auto const &loop : loopSet.ccwLoops) {
        addPline(loop.polyline);
      }
      i += 1;
    }
  }

  while (plineNode) {
    plineNode->setIsVisible(false);
    plineNode = static_cast<PolylineNode *>(plineNode->nextSibling());
  }

  return rootNode;
}

void PlineOffsetIslandsAlgorithmView::mousePressEvent(QMouseEvent *event) {
  m_globalMouseDownPoint = QPointF(event->globalX(), event->globalY());

  for (auto &loop : m_ccwLoops) {
    m_vertexGrabbed = vertexUnderPosition(m_globalMouseDownPoint, loop);
    if (isVertexGrabbed()) {
      m_polylineGrabbed = &loop;
      m_origVertexGlobalPos = convertToGlobalUICoord(loop[m_vertexGrabbed].pos());
      break;
    }
  }

  if (!isVertexGrabbed()) {
    for (auto &island : m_cwLoops) {
      m_vertexGrabbed = vertexUnderPosition(m_globalMouseDownPoint, island);
      if (isVertexGrabbed()) {
        m_polylineGrabbed = &island;
        m_origVertexGlobalPos = convertToGlobalUICoord(island[m_vertexGrabbed].pos());
        break;
      }
    }
  }

  if (!isVertexGrabbed()) {
    event->ignore();
    m_polylineGrabbed = nullptr;
    return;
  }

  event->accept();
}

void PlineOffsetIslandsAlgorithmView::mouseMoveEvent(QMouseEvent *event) {
  if (!isVertexGrabbed()) {
    return;
  }

  // convert back from global coordinates to get real delta
  QPointF mouseDelta = QPointF(event->globalX(), event->globalY()) - m_globalMouseDownPoint;
  QPointF newGlobalVertexPos = mouseDelta + m_origVertexGlobalPos;
  QPointF newLocalVertexPos = mapFromGlobal(newGlobalVertexPos);
  QPointF newRealVertexPos = m_uiToRealCoord * newLocalVertexPos;

  (*m_polylineGrabbed)[m_vertexGrabbed].x() = newRealVertexPos.x();
  (*m_polylineGrabbed)[m_vertexGrabbed].y() = newRealVertexPos.y();

  update();
}

void PlineOffsetIslandsAlgorithmView::mouseReleaseEvent(QMouseEvent *event) {
  if (!isVertexGrabbed()) {
    return;
  }
  m_vertexGrabbed = std::numeric_limits<std::size_t>::max();
  event->accept();
}

bool PlineOffsetIslandsAlgorithmView::isVertexGrabbed() {
  return m_vertexGrabbed != std::numeric_limits<std::size_t>::max();
}
