#ifndef POLYLINENODE_H
#define POLYLINENODE_H

#include "cavc/polyline.h"
#include "cavc/vector2.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>
#include <vector>

class PolylineNode : public QSGGeometryNode {
public:
  PolylineNode();
  void updateGeometry(cavc::Polyline<double> const &pline, double arcApproxError = 0.005);
  QColor const &color() const;
  void setColor(QColor const &color);

  QColor const &vertexesColor() const;
  void setVertexesColor(QColor const &vertexesColor);

  bool vertexesVisible() const;
  void setVertexesVisible(bool vertexesVisible);

  bool isVisible() const;
  void setIsVisible(bool isVisible);

  bool isSubtreeBlocked() const override;

private:
  bool m_vertexesVisible;
  QSGGeometryNode *m_vertexesPointNode;
  QColor m_vertexesColor;
  bool m_isVisible;
  std::vector<cavc::Vector2<float>> m_vertexesBuffer;
  QSGGeometry m_geometry;
  QSGFlatColorMaterial m_material;
};

#endif // POLYLINENODE_H
