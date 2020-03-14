#include "plineboolopsalgorithmview.h"
#include "polylinenode.h"
#include <QSGTransformNode>

using namespace cavc;

PlineBoolOpsAlgorithmView::PlineBoolOpsAlgorithmView(QQuickItem *parent)
    : GeometryCanvasItem(parent),
      m_polylineANode(nullptr),
      m_polylineBNode(nullptr),
      m_windingNumber(0),
      m_vertexGrabbed(std::numeric_limits<std::size_t>::max()),
      m_polylineGrabbed(nullptr),
      m_interacting(false) {

  m_plineA.addVertex(0, 25, 1);
  m_plineA.addVertex(0, 0, 0);
  m_plineA.addVertex(2, 0, 1);
  m_plineA.addVertex(10, 0, -0.5);
  m_plineA.addVertex(8, 9, 0.374794619217547);
  m_plineA.addVertex(21, 0, 0);
  m_plineA.addVertex(23, 0, 1);
  m_plineA.addVertex(32, 0, -0.5);
  m_plineA.addVertex(28, 0, 0.5);
  m_plineA.addVertex(39, 21, 0);
  m_plineA.addVertex(28, 12, 0.5);
  m_plineA.isClosed() = true;

  m_plineB.addVertex(1, 1, 0);
}

int PlineBoolOpsAlgorithmView::windingNumber() const { return m_windingNumber; }

bool PlineBoolOpsAlgorithmView::interacting() const { return m_interacting; }

void PlineBoolOpsAlgorithmView::setInteracting(bool interacting) {
  if (m_interacting == interacting)
    return;

  m_interacting = interacting;
  emit interactingChanged(m_interacting);
}

bool PlineBoolOpsAlgorithmView::isVertexGrabbed() {
  return m_vertexGrabbed != std::numeric_limits<std::size_t>::max();
}

void PlineBoolOpsAlgorithmView::setWindingNumber(int windingNumber) {
  if (m_windingNumber == windingNumber)
    return;

  m_windingNumber = windingNumber;
  emit windingNumberChanged(m_windingNumber);
}

QSGNode *PlineBoolOpsAlgorithmView::updatePaintNode(QSGNode *oldNode,
                                                    QQuickItem::UpdatePaintNodeData *) {
  QSGTransformNode *rootNode = nullptr;
  if (!oldNode) {
    rootNode = new QSGTransformNode();
    m_polylineANode = new PolylineNode();
    m_polylineANode->setVertexesVisible(true);
    m_polylineANode->setFlag(QSGNode::OwnedByParent);
    rootNode->appendChildNode(m_polylineANode);

    m_polylineBNode = new PolylineNode();
    m_polylineBNode->setVertexesVisible(true);
    m_polylineBNode->setFlag(QSGNode::OwnedByParent);
    m_polylineBNode->setColor(Qt::darkGreen);
    m_polylineBNode->setVertexesColor(Qt::darkCyan);
    rootNode->appendChildNode(m_polylineBNode);
  } else {
    rootNode = static_cast<QSGTransformNode *>(oldNode);
  }
  rootNode->setMatrix(m_realToUICoord);

  setWindingNumber(cavc::windingNumber(m_plineA, m_plineB[0].pos()));
  m_polylineANode->updateGeometry(m_plineA);
  m_polylineBNode->updateGeometry(m_plineB);

  return rootNode;
}

void PlineBoolOpsAlgorithmView::mousePressEvent(QMouseEvent *event) {
  m_globalMouseDownPoint = QPointF(event->globalX(), event->globalY());
  m_vertexGrabbed = vertexUnderPosition(m_globalMouseDownPoint, m_plineA);

  if (isVertexGrabbed()) {
    m_polylineGrabbed = &m_plineA;
    m_origVertexGlobalPos = convertToGlobalUICoord(m_plineA[m_vertexGrabbed].pos());
  } else {
    m_vertexGrabbed = vertexUnderPosition(m_globalMouseDownPoint, m_plineB);
    if (!isVertexGrabbed()) {
      event->ignore();
      return;
    }

    m_polylineGrabbed = &m_plineB;
    m_origVertexGlobalPos = convertToGlobalUICoord(m_plineB[m_vertexGrabbed].pos());
  }

  setInteracting(true);
  event->accept();
}

void PlineBoolOpsAlgorithmView::mouseMoveEvent(QMouseEvent *event) {
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
  }

  update();
}

void PlineBoolOpsAlgorithmView::mouseReleaseEvent(QMouseEvent *event) {
  if (!isVertexGrabbed()) {
    return;
  }
  setInteracting(false);
  m_vertexGrabbed = std::numeric_limits<std::size_t>::max();
  event->accept();
}
