#ifndef POLYLINENODE_H
#define POLYLINENODE_H

#include "cavc/polyline.hpp"
#include "cavc/vector2.hpp"
#include <QSGFlatColorMaterial>
#include <QSGOpacityNode>
#include <vector>

class FlatColorGeometryNode;
class PointSetNode;

class PolylineNode : public QSGOpacityNode {
public:
  enum PathDrawMode { NormalPath, DashedPath };
  PolylineNode();
  void updateGeometry(cavc::Polyline<double> const &pline, PathDrawMode pathDrawMode = NormalPath,
                      double arcApproxError = 0.005);
  QColor const &color() const;
  void setColor(QColor const &color);

  bool pathVisible() const;
  void setPathVisible(bool pathVisible);

  QColor const &vertexesColor() const;
  void setVertexesColor(QColor const &vertexesColor);

  bool vertexesVisible() const;
  void setVertexesVisible(bool vertexesVisible);

  void setIsVisible(bool isVisible);

private:
  bool m_pathVisible;
  bool m_vertexesVisible;
  FlatColorGeometryNode *m_pathNode;
  PointSetNode *m_vertexesNode;
  QColor m_pathColor;
  QColor m_vertexesColor;
  bool m_isVisible;
  std::vector<cavc::Vector2<float>> m_vertexesBuffer;

  void updatePathNode(cavc::Polyline<double> const &pline, double arcApproxError,
                      PathDrawMode drawMode);
  void updateVertexesNode(cavc::Polyline<double> const &pline);
};

#endif // POLYLINENODE_H
