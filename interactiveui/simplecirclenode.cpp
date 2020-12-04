#include "simplecirclenode.h"
#include "cavc/mathutils.hpp"

SimpleCircleNode::SimpleCircleNode() : FlatColorGeometryNode(true) {
  m_xPos = 0;
  m_yPos = 0;
  m_radius = 1;
  m_geometry.setDrawingMode(QSGGeometry::DrawTriangleFan);
}

void SimpleCircleNode::setGeometry(qreal x, qreal y, qreal radius) {
  m_xPos = x;
  m_yPos = y;
  m_radius = radius;
  updateGeometry();
}

void SimpleCircleNode::updateGeometry() {
  int surroundingVertexes = 12;
  int vertexCount = surroundingVertexes + 2;

  m_geometry.allocate(vertexCount, vertexCount);
  std::uint32_t *segVertexIndices = m_geometry.indexDataAsUInt();
  for (int i = 0; i < vertexCount; ++i) {
    segVertexIndices[i] = static_cast<std::uint32_t>(i);
  }

  QSGGeometry::Point2D *vertexData = m_geometry.vertexDataAsPoint2D();
  vertexData[0].set(m_xPos, m_yPos);
  for (int i = 1; i < vertexCount; ++i) {
    double angle = cavc::utils::tau<double>() * static_cast<double>(i - 1) /
                   static_cast<double>(surroundingVertexes);
    vertexData[i].set(m_xPos + m_radius * std::cos(angle), m_yPos + m_radius * std::sin(angle));
  }

  markDirty(QSGNode::DirtyGeometry);
}
