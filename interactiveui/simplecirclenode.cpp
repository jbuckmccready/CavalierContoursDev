#include "simplecirclenode.h"
#include "cavc/mathutils.hpp"

SimpleCircleNode::SimpleCircleNode() : FlatColorGeometryNode(true) {
  m_xPos = 0;
  m_yPos = 0;
  m_radius = 1;
  // Draw vertex markers as hollow squares for stable rendering across backends.
  m_geometry.setDrawingMode(QSGGeometry::DrawLineStrip);
  m_geometry.setLineWidth(1.5f);
}

void SimpleCircleNode::setGeometry(qreal x, qreal y, qreal radius) {
  m_xPos = x;
  m_yPos = y;
  m_radius = radius;
  updateGeometry();
}

void SimpleCircleNode::updateGeometry() {
  // 5 points (first repeated) to close the square with DrawLineStrip.
  int vertexCount = 5;

  m_geometry.allocate(vertexCount, vertexCount);
  std::uint32_t *segVertexIndices = m_geometry.indexDataAsUInt();
  for (int i = 0; i < vertexCount; ++i) {
    segVertexIndices[i] = static_cast<std::uint32_t>(i);
  }

  QSGGeometry::Point2D *vertexData = m_geometry.vertexDataAsPoint2D();
  vertexData[0].set(m_xPos - m_radius, m_yPos - m_radius);
  vertexData[1].set(m_xPos + m_radius, m_yPos - m_radius);
  vertexData[2].set(m_xPos + m_radius, m_yPos + m_radius);
  vertexData[3].set(m_xPos - m_radius, m_yPos + m_radius);
  vertexData[4].set(m_xPos - m_radius, m_yPos - m_radius);

  markDirty(QSGNode::DirtyGeometry);
}
