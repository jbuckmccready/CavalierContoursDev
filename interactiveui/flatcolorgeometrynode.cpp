#include "flatcolorgeometrynode.h"

FlatColorGeometryNode::FlatColorGeometryNode(bool useUInt32Index)
    : m_isVisible(true),
      m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0,
                 useUInt32Index ? QSGGeometry::UnsignedIntType : QSGGeometry::UnsignedShortType) {
  m_geometry.setLineWidth(1);
  m_geometry.setDrawingMode(QSGGeometry::DrawLineStrip);
  setGeometry(&m_geometry);
  m_material.setColor(Qt::darkGreen);
  setMaterial(&m_material);
}

const QColor &FlatColorGeometryNode::color() const { return m_material.color(); }

void FlatColorGeometryNode::setColor(const QColor &color) {
  m_material.setColor(color);
  markDirty(QSGNode::DirtyMaterial);
}

bool FlatColorGeometryNode::isVisible() const { return m_isVisible; }

void FlatColorGeometryNode::setIsVisible(bool isVisible) {
  if (m_isVisible != isVisible) {
    m_isVisible = isVisible;
    markDirty(QSGNode::DirtySubtreeBlocked);
  }
}

bool FlatColorGeometryNode::isSubtreeBlocked() const { return !m_isVisible; }
