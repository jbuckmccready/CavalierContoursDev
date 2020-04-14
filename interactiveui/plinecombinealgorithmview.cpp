#include "plinecombinealgorithmview.h"
#include "cavc/polylinecombine.hpp"
#include "cavc/polylineintersects.hpp"
#include "graphicshelpers.h"
#include "polylinenode.h"
#include <QSGGeometryNode>
#include <QSGOpacityNode>
#include <QSGTransformNode>

using namespace cavc;

PlineCombineAlgorithmView::PlineCombineAlgorithmView(QQuickItem *parent)
    : GeometryCanvasItem(parent),
      m_polylineANode(nullptr),
      m_polylineBNode(nullptr),
      m_testPointNode(nullptr),
      m_intersectsNode(nullptr),
      m_dynamicPlinesParentNode(nullptr),
      m_windingNumber(0),
      m_vertexGrabbed(std::numeric_limits<std::size_t>::max()),
      m_polylineGrabbed(nullptr),
      m_interacting(false),
      m_showIntersects(true),
      m_plineCombineMode(NoCombine),
      m_showVertexes(true),
      m_flipArgOrder(false),
      m_showWindingNumberPoint(false) {

    m_plineA.addVertex(0, 1, 1);
    m_plineA.addVertex(10, 1, 1);
    m_plineA.isClosed() = true;

    m_plineB.addVertex(3, -10, 0);
    m_plineB.addVertex(6, -10, 0);
    m_plineB.addVertex(6, 10, 0);
    m_plineB.addVertex(3, 10, 0);
    m_plineB.isClosed() = true;

//  m_plineA.addVertex(0, 25, 1);
//  m_plineA.addVertex(-4, 0, 0);
////  m_plineA.addVertex(-8, 0, 0);
//  m_plineA.addVertex(2, 0, 1);
//  m_plineA.addVertex(10, 0, -0.5);
//  m_plineA.addVertex(8, 9, 0.374794619217547);
//  m_plineA.addVertex(21, 0, 0);
//  m_plineA.addVertex(23, 0, 1);
//  m_plineA.addVertex(32, 0, -0.5);
//  m_plineA.addVertex(28, 0, 0.5);
//  m_plineA.addVertex(39, 21, 0);
//  m_plineA.addVertex(28, 12, 0.5);
//  m_plineA.isClosed() = true;

//  m_plineB = m_plineA;
//  //  invertDirection(m_plineA);

//  //  m_plineB.addVertex(0, 25, 0);
//  //  m_plineB.addVertex(0, 0, 0);
//  //  m_plineB.addVertex(5, 15, 0);
//  //  m_plineB.isClosed() = true;
//  //  invertDirection(m_plineB);

//  m_plineB[0].y() = 30;
//  m_plineB[1].x() = -1;
//  m_plineB[2].x() = 3;
  //    invertDirection(m_plineB);
  //    m_plineB.addVertex(-15,20,0);
  //    m_plineB.addVertex(-15,10,0);
  //    m_plineB.addVertex(5,10,0);
  //    m_plineB.addVertex(5,20,0);
  //    m_plineB.isClosed() = true;

  //    auto radius = 20;
  //    auto centerX = 0;
  //    auto centerY = 0;
  //    std::size_t segmentCount = 10;
  //    for (std::size_t i = 0; i < segmentCount; ++i) {
  //      double angle = static_cast<double>(i) * utils::tau<double>() / segmentCount;
  //      m_plineB.addVertex(radius * std::cos(angle) + centerX,
  //                                radius * std::sin(angle) + centerY, i % 2 == 0 ? 1 : -1);
  //    }
  //    m_plineB.isClosed() = true;

  m_testPoint.addVertex(1, 1, 0);
}

int PlineCombineAlgorithmView::windingNumber() const { return m_windingNumber; }

bool PlineCombineAlgorithmView::interacting() const { return m_interacting; }

bool PlineCombineAlgorithmView::showIntersects() const { return m_showIntersects; }

void PlineCombineAlgorithmView::setShowIntersects(bool showIntersects) {
  if (m_showIntersects == showIntersects)
    return;

  m_showIntersects = showIntersects;
  update();
  emit showIntersectsChanged(m_showIntersects);
}

PlineCombineAlgorithmView::PlineCombineMode PlineCombineAlgorithmView::plineCombineMode() const {
  return m_plineCombineMode;
}

void PlineCombineAlgorithmView::setPlineCombineMode(
    PlineCombineAlgorithmView::PlineCombineMode plineCombineMode) {
  if (m_plineCombineMode == plineCombineMode)
    return;

  m_plineCombineMode = plineCombineMode;
  update();
  emit plineCombineModeChanged(m_plineCombineMode);
}

bool PlineCombineAlgorithmView::showVertexes() const { return m_showVertexes; }

void PlineCombineAlgorithmView::setShowVertexes(bool showVertexes) {
  if (m_showVertexes == showVertexes)
    return;

  m_showVertexes = showVertexes;
  update();
  emit showVertexesChanged(m_showVertexes);
}

bool PlineCombineAlgorithmView::flipArgOrder() const { return m_flipArgOrder; }

void PlineCombineAlgorithmView::setFlipArgOrder(bool flipArgOrder) {
  if (m_flipArgOrder == flipArgOrder)
    return;

  m_flipArgOrder = flipArgOrder;
  if (m_plineCombineMode != NoCombine) {
    update();
  }
  emit flipArgOrderChanged(m_flipArgOrder);
}

bool PlineCombineAlgorithmView::showWindingNumberPoint() const { return m_showWindingNumberPoint; }

void PlineCombineAlgorithmView::setShowWindingNumberPoint(bool showWindingNumberPoint) {
  if (m_showWindingNumberPoint == showWindingNumberPoint)
    return;

  m_showWindingNumberPoint = showWindingNumberPoint;
  update();
  emit showWindingNumberPointChanged(m_showWindingNumberPoint);
}

void PlineCombineAlgorithmView::setInteracting(bool interacting) {
  if (m_interacting == interacting)
    return;

  m_interacting = interacting;
  emit interactingChanged(m_interacting);
}

bool PlineCombineAlgorithmView::isVertexGrabbed() {
  return m_vertexGrabbed != std::numeric_limits<std::size_t>::max();
}

void PlineCombineAlgorithmView::setWindingNumber(int windingNumber) {
  if (m_windingNumber == windingNumber)
    return;

  m_windingNumber = windingNumber;
  emit windingNumberChanged(m_windingNumber);
}

QSGNode *PlineCombineAlgorithmView::updatePaintNode(QSGNode *oldNode,
                                                    QQuickItem::UpdatePaintNodeData *) {
  QSGTransformNode *rootNode = nullptr;
  if (!oldNode) {
    rootNode = new QSGTransformNode();
    m_polylineANode = new PolylineNode();
    m_polylineANode->setFlag(QSGNode::OwnedByParent);
    m_polylineANode->setColor(Qt::blue);
    m_polylineANode->setVertexesColor(Qt::blue);
    rootNode->appendChildNode(m_polylineANode);

    m_polylineBNode = new PolylineNode();
    m_polylineBNode->setFlag(QSGNode::OwnedByParent);
    m_polylineBNode->setColor(Qt::red);
    m_polylineBNode->setVertexesColor(Qt::red);
    rootNode->appendChildNode(m_polylineBNode);

    m_testPointNode = new PolylineNode();
    m_testPointNode->setFlag(QSGNode::OwnedByParent);
    m_testPointNode->setVertexesColor(Qt::darkMagenta);
    rootNode->appendChildNode(m_testPointNode);
  } else {
    rootNode = static_cast<QSGTransformNode *>(oldNode);
  }
  rootNode->setMatrix(m_realToUICoord);

  setWindingNumber(cavc::getWindingNumber(m_plineA, m_testPoint[0].pos()));

  m_polylineANode->setVertexesVisible(m_showVertexes);
  m_polylineBNode->setVertexesVisible(m_showVertexes);

  m_polylineANode->updateGeometry(m_plineA);
  m_polylineBNode->updateGeometry(m_plineB);

  m_testPointNode->setVertexesVisible(m_showWindingNumberPoint);
  if (m_showWindingNumberPoint) {
    m_testPointNode->updateGeometry(m_testPoint);
  }

  auto plineASpatialIndex = createApproxSpatialIndex(m_plineA);
  PlineIntersectsResult<double> inters;
  findIntersects(m_plineA, m_plineB, plineASpatialIndex, inters);
  std::vector<PlineIntersect<double>> intersectsAB;
  for (auto const &intr : inters.intersects) {
    intersectsAB.push_back(intr);
  }

  for (auto const &intr : inters.coincidentIntersects) {
    intersectsAB.push_back(PlineIntersect<double>(intr.sIndex1, intr.sIndex2, intr.point1));
    intersectsAB.push_back(PlineIntersect<double>(intr.sIndex1, intr.sIndex2, intr.point1));
  }

  if (m_showIntersects) {
    if (!m_intersectsNode) {
      m_intersectsNode = gh::createSimpleGeomNode(static_cast<int>(intersectsAB.size()),
                                                  Qt::darkCyan, 8, QSGGeometry::DrawPoints);
      rootNode->appendChildNode(m_intersectsNode);
    } else {
      m_intersectsNode->geometry()->allocate(static_cast<int>(intersectsAB.size()));
    }
    QSGGeometry::Point2D *intrsVertexes = m_intersectsNode->geometry()->vertexDataAsPoint2D();
    for (std::size_t i = 0; i < intersectsAB.size(); ++i) {
      intrsVertexes[i].set(static_cast<float>(intersectsAB[i].pos.x()),
                           static_cast<float>(intersectsAB[i].pos.y()));
    }
    m_intersectsNode->markDirty(QSGNode::DirtyGeometry);
  } else if (m_intersectsNode) {
    rootNode->removeChildNode(m_intersectsNode);
    delete m_intersectsNode;
    m_intersectsNode = nullptr;
  }

  if (m_plineCombineMode != NoCombine) {
    m_polylineANode->setPathVisible(false);
    m_polylineBNode->setPathVisible(false);
    if (!m_dynamicPlinesParentNode) {
      m_dynamicPlinesParentNode = new QSGOpacityNode();
      rootNode->appendChildNode(m_dynamicPlinesParentNode);
    }
    m_dynamicPlinesParentNode->setOpacity(1);

    std::size_t sliceIndex = 0;
    PolylineNode *plineNode = static_cast<PolylineNode *>(m_dynamicPlinesParentNode->firstChild());
    auto addPline = [&](cavc::Polyline<double> const &pline,
                        PolylineNode::PathDrawMode drawMode = PolylineNode::NormalPath) {
      if (!plineNode) {
        plineNode = new PolylineNode();
        m_dynamicPlinesParentNode->appendChildNode(plineNode);
      }
      plineNode->setColor(gh::indexToColor(sliceIndex));
      plineNode->setIsVisible(true);
      plineNode->setVertexesVisible(false);
      plineNode->updateGeometry(pline, drawMode);
      plineNode = static_cast<PolylineNode *>(plineNode->nextSibling());
      sliceIndex++;
    };

    if (m_plineCombineMode == CoincidentSlices) {
      auto result = internal::processForCombine(m_plineA, m_plineB, plineASpatialIndex);
      for (const auto &slice : result.coincidentSlices) {
        addPline(slice);
      }
    } else {
      cavc::PlineCombineMode mode;
      switch (m_plineCombineMode) {
      case PlineCombineAlgorithmView::Union:
        mode = cavc::PlineCombineMode::Union;
        break;
      case PlineCombineAlgorithmView::Exclude:
        mode = cavc::PlineCombineMode::Exclude;
        break;
      case PlineCombineAlgorithmView::Intersect:
        mode = cavc::PlineCombineMode::Intersect;
        break;
      case PlineCombineAlgorithmView::XOR:
        mode = cavc::PlineCombineMode::XOR;
        break;
      default:
        mode = cavc::PlineCombineMode::Union;
        break;
      }

      auto combineResult = m_flipArgOrder ? combinePolylines(m_plineB, m_plineA, mode)
                                          : combinePolylines(m_plineA, m_plineB, mode);

      for (const auto &slice : combineResult.remaining) {
        addPline(slice);
      }

      for (const auto &slice : combineResult.subtracted) {
        addPline(slice, PolylineNode::DashedPath);
      }
    }

    while (plineNode) {
      plineNode->setIsVisible(false);
      plineNode = static_cast<PolylineNode *>(plineNode->nextSibling());
    }
  } else {
    m_polylineANode->setPathVisible(true);
    m_polylineBNode->setPathVisible(true);
    if (m_dynamicPlinesParentNode) {
      m_dynamicPlinesParentNode->setOpacity(0);
    }
  }

  return rootNode;
}

void PlineCombineAlgorithmView::mousePressEvent(QMouseEvent *event) {
  m_globalMouseDownPoint = QPointF(event->globalX(), event->globalY());
  m_vertexGrabbed = vertexUnderPosition(m_globalMouseDownPoint, m_plineA);

  if (isVertexGrabbed()) {
    m_polylineGrabbed = &m_plineA;
    m_origVertexGlobalPos = convertToGlobalUICoord(m_plineA[m_vertexGrabbed].pos());
  } else {
    m_vertexGrabbed = vertexUnderPosition(m_globalMouseDownPoint, m_plineB);
    if (isVertexGrabbed()) {
      m_polylineGrabbed = &m_plineB;
      m_origVertexGlobalPos = convertToGlobalUICoord(m_plineB[m_vertexGrabbed].pos());
    } else {
      m_vertexGrabbed = vertexUnderPosition(m_globalMouseDownPoint, m_testPoint);
      if (!isVertexGrabbed()) {
        event->ignore();
        m_polylineGrabbed = nullptr;
        return;
      }

      m_polylineGrabbed = &m_testPoint;
      m_origVertexGlobalPos = convertToGlobalUICoord(m_testPoint[m_vertexGrabbed].pos());
    }
  }

  setInteracting(true);
  event->accept();
}

void PlineCombineAlgorithmView::mouseMoveEvent(QMouseEvent *event) {
  if (!isVertexGrabbed()) {
    return;
  }

  // convert back from global coordinates to get real delta
  QPointF mouseDelta = QPointF(event->globalX(), event->globalY()) - m_globalMouseDownPoint;
  QPointF newGlobalVertexPos = mouseDelta + m_origVertexGlobalPos;
  QPointF newLocalVertexPos = mapFromGlobal(newGlobalVertexPos);
  QPointF newRealVertexPos = m_uiToRealCoord * newLocalVertexPos;

  if (m_polylineGrabbed == &m_plineA) {
    m_plineA[m_vertexGrabbed].x() = newRealVertexPos.x();
    m_plineA[m_vertexGrabbed].y() = newRealVertexPos.y();
  } else if (m_polylineGrabbed == &m_plineB) {
    m_plineB[m_vertexGrabbed].x() = newRealVertexPos.x();
    m_plineB[m_vertexGrabbed].y() = newRealVertexPos.y();
  } else if (m_polylineGrabbed == &m_testPoint) {
    m_testPoint[m_vertexGrabbed].x() = newRealVertexPos.x();
    m_testPoint[m_vertexGrabbed].y() = newRealVertexPos.y();
  }

  update();
}

void PlineCombineAlgorithmView::mouseReleaseEvent(QMouseEvent *event) {
  if (!isVertexGrabbed()) {
    return;
  }
  setInteracting(false);
  m_vertexGrabbed = std::numeric_limits<std::size_t>::max();
  event->accept();
}
