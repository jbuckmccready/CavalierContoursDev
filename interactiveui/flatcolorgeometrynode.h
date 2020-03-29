#ifndef FLATCOLORGEOMETRYNODE_H
#define FLATCOLORGEOMETRYNODE_H

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

class FlatColorGeometryNode : public QSGGeometryNode {
public:
  FlatColorGeometryNode(bool useUInt32Index = false);
  QColor const &color() const;
  void setColor(QColor const &color);
  bool isVisible() const;
  void setIsVisible(bool isVisible);
  bool isSubtreeBlocked() const override;

protected:
  bool m_isVisible;
  QSGGeometry m_geometry;
  QSGFlatColorMaterial m_material;
};

#endif // FLATCOLORGEOMETRYNODE_H
