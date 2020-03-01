#include "polylinenode.h"
#include "graphicshelpers.h"
#include <QDebug>
using namespace cavc;

PolylineNode::PolylineNode()
    : m_vertexesVisible(false),
      m_vertexesPointNode(nullptr),
      m_vertexesColor(Qt::red),
      m_isVisible(true),
      m_geometry(QSGGeometry::defaultAttributes_Point2D(), 0, 0, GL_UNSIGNED_INT) {
  m_geometry.setLineWidth(1);
  m_geometry.setDrawingMode(QSGGeometry::DrawLineStrip);
  setGeometry(&m_geometry);
  m_material.setColor(Qt::blue);
  setMaterial(&m_material);
}

void PolylineNode::updateGeometry(const cavc::Polyline<double> &pline, double arcApproxError) {
  // update vertexes buffer
  m_vertexesBuffer.clear();

  auto visitor = [&](std::size_t i, std::size_t j) {
    const auto &v1 = pline[i];
    const auto &v2 = pline[j];
    if (v1.bulgeIsZero() || fuzzyEqual(v1.pos(), v2.pos())) {
      m_vertexesBuffer.emplace_back(v1.x(), v1.y());
    } else {

      auto arc = arcRadiusAndCenter(v1, v2);
      auto startAngle = angle(arc.center, v1.pos());
      auto endAngle = angle(arc.center, v2.pos());
      double deltaAngle = std::abs(cavc::utils::deltaAngle(startAngle, endAngle));

      double segmentSubAngle = std::abs(2.0 * std::acos(1.0 - arcApproxError / arc.radius));
      std::size_t segmentCount = static_cast<std::size_t>(std::ceil(deltaAngle / segmentSubAngle));
      // update segment subangle for equal length segments
      segmentSubAngle = deltaAngle / segmentCount;

      if (v1.bulge() < 0.0) {
        segmentSubAngle = -segmentSubAngle;
      }
      // add the start point
      m_vertexesBuffer.emplace_back(v1.x(), v1.y());

      // add remaining points
      for (std::size_t i = 1; i < segmentCount; ++i) {
        double angle = i * segmentSubAngle + startAngle;
        m_vertexesBuffer.emplace_back(arc.radius * std::cos(angle) + arc.center.x(),
                                      arc.radius * std::sin(angle) + arc.center.y());
      }
    }

    return true;
  };

  iterateSegIndices(pline, visitor);

  if (pline.isClosed()) {
    m_vertexesBuffer.push_back(m_vertexesBuffer[0]);
  } else {
    m_vertexesBuffer.emplace_back(pline.lastVertex().x(), pline.lastVertex().y());
  }

  // update node geometry from vertexes buffer
  const int plineSegVertexCount = static_cast<int>(m_vertexesBuffer.size());
  m_geometry.allocate(plineSegVertexCount, plineSegVertexCount);
  std::uint32_t *segVertexIndices = m_geometry.indexDataAsUInt();
  for (int i = 0; i < plineSegVertexCount; ++i) {
    segVertexIndices[i] = static_cast<std::uint32_t>(i);
  }

  QSGGeometry::Point2D *vertexData = m_geometry.vertexDataAsPoint2D();
  for (std::size_t i = 0; i < m_vertexesBuffer.size(); ++i) {
    vertexData[i].set(m_vertexesBuffer[i].x(), m_vertexesBuffer[i].y());
  }

  const int plinePointVertexCount = static_cast<int>(pline.size());
  if (m_vertexesVisible) {
    if (!m_vertexesPointNode) {
      m_vertexesPointNode = new QSGGeometryNode();
      QSGGeometry *pointGeometry =
          new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), plinePointVertexCount,
                          plinePointVertexCount, GL_UNSIGNED_INT);
      pointGeometry->setDrawingMode(QSGGeometry::DrawPoints);
      pointGeometry->setLineWidth(8);
      m_vertexesPointNode->setGeometry(pointGeometry);
      m_vertexesPointNode->setFlag(QSGNode::OwnsGeometry);

      QSGFlatColorMaterial *pointMaterial = new QSGFlatColorMaterial();
      pointMaterial->setColor(m_vertexesColor);
      m_vertexesPointNode->setMaterial(pointMaterial);
      m_vertexesPointNode->setFlag(QSGNode::OwnsMaterial);
      appendChildNode(m_vertexesPointNode);
      m_vertexesPointNode->setFlag(QSGNode::OwnedByParent);
    } else {
      m_vertexesPointNode->geometry()->allocate(plinePointVertexCount, plinePointVertexCount);
    }

    std::uint32_t *pointVertexIndices = m_vertexesPointNode->geometry()->indexDataAsUInt();
    for (int i = 0; i < plinePointVertexCount; ++i) {
      pointVertexIndices[i] = static_cast<std::uint32_t>(i);
    }
    QSGGeometry::Point2D *pointVertexData = m_vertexesPointNode->geometry()->vertexDataAsPoint2D();
    for (std::size_t i = 0; i < pline.size(); ++i) {
      pointVertexData[i].set(static_cast<float>(pline[i].x()), static_cast<float>(pline[i].y()));
    }
    m_vertexesPointNode->markDirty(QSGNode::DirtyGeometry);
  } else {
    if (m_vertexesPointNode) {
      removeChildNode(m_vertexesPointNode);
      delete m_vertexesPointNode;
      m_vertexesPointNode = nullptr;
    }
  }

  markDirty(QSGNode::DirtyGeometry);
}

const QColor &PolylineNode::color() const { return m_material.color(); }

void PolylineNode::setColor(const QColor &color) {
  if (m_material.color() != color) {
    m_material.setColor(color);
    markDirty(QSGNode::DirtyMaterial);
  }
}

const QColor &PolylineNode::vertexesColor() const { return m_vertexesColor; }

void PolylineNode::setVertexesColor(const QColor &vertexesColor) {
  if (vertexesColor != m_vertexesColor) {
    m_vertexesColor = vertexesColor;
    if (m_vertexesPointNode) {
      auto *material = static_cast<QSGFlatColorMaterial *>(m_vertexesPointNode->material());
      material->setColor(m_vertexesColor);
      m_vertexesPointNode->markDirty(QSGNode::DirtyMaterial);
    }
  }
}

bool PolylineNode::vertexesVisible() const { return m_vertexesVisible; }

void PolylineNode::setVertexesVisible(bool vertexesVisible) {
  if (vertexesVisible != m_vertexesVisible) {
    m_vertexesVisible = vertexesVisible;
    markDirty(QSGNode::DirtyGeometry);
  }
}

bool PolylineNode::isVisible() const { return m_isVisible; }

void PolylineNode::setIsVisible(bool isVisible) {
  if (m_isVisible != isVisible) {
    m_isVisible = isVisible;
    markDirty(QSGNode::DirtySubtreeBlocked);
  }
}

bool PolylineNode::isSubtreeBlocked() const { return !m_isVisible; }
