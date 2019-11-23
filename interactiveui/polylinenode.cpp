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

void PolylineNode::updateGeometry(const cavc::Polyline<double> &pline, double uiScale) {
  // update vertexes buffer
  m_vertexesBuffer.clear();

  auto lineVisitor = [&](PlineVertex<double> const &v1, PlineVertex<double> const &v2) {
    Q_UNUSED(v1)
    m_vertexesBuffer.emplace_back(static_cast<float>(v2.x()), static_cast<float>(v2.y()));
  };

  auto arcVisitor = [&](PlineVertex<double> const &v1, PlineVertex<double> const &v2) {
    auto arc = arcRadiusAndCenter(v1, v2);
    auto startAngle = angle(arc.center, v1.pos());
    auto endAngle = angle(arc.center, v2.pos());
    double deltaAngle = std::abs(utils::deltaAngle(startAngle, endAngle));
    std::size_t segmentCount =
        static_cast<std::size_t>(std::ceil(arc.radius * deltaAngle * uiScale));
    const double sweepAngle = v1.bulge() > 0 ? deltaAngle : -deltaAngle;

    for (std::size_t i = 0; i <= segmentCount; ++i) {
      double angle = static_cast<double>(i) / segmentCount * sweepAngle + startAngle;
      m_vertexesBuffer.emplace_back(
          static_cast<float>(arc.radius * std::cos(angle) + arc.center.x()),
          static_cast<float>(arc.radius * std::sin(angle) + arc.center.y()));
    }
  };

  auto segVisitor = [&](PlineVertex<double> const &v1, PlineVertex<double> const &v2) {
    if (v1.bulgeIsZero() || fuzzyEqual(v1.pos(), v2.pos())) {
      lineVisitor(v1, v2);
    } else {
      arcVisitor(v1, v2);
    }
  };

  m_vertexesBuffer.emplace_back(static_cast<float>(pline[0].x()), static_cast<float>(pline[0].y()));

  for (std::size_t i = 1; i < pline.size(); ++i) {
    segVisitor(pline[i - 1], pline[i]);
  }

  if (pline.isClosed()) {
    segVisitor(pline.lastVertex(), pline[0]);
    m_vertexesBuffer.push_back(m_vertexesBuffer[0]);
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
