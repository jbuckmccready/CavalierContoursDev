#include "graphicshelpers.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

QSGGeometryNode *gh::createSimpleGeomNode(int vertexCount, const QColor &color, float width,
                                          QSGGeometry::DrawingMode mode) {
  QSGGeometryNode *node = new QSGGeometryNode();
  node->setFlag(QSGNode::OwnedByParent);

  QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), vertexCount);
  geometry->setDrawingMode(mode);
  geometry->setLineWidth(width);
  node->setGeometry(geometry);
  node->setFlag(QSGNode::OwnsGeometry);

  QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
  material->setColor(color);
  node->setMaterial(material);
  node->setFlag(QSGNode::OwnsMaterial);

  return node;
}

QColor gh::indexToColor(std::size_t index) {
  switch (index % 11) {
  case 0:
    return QColor("red");
  case 1:
    return QColor("blue");
  case 2:
    return QColor("green");
  case 3:
    return QColor("magenta");
  case 4:
    return QColor("orange");
  case 5:
    return QColor("greenyellow");
  case 6:
    return QColor("olive");
  case 7:
    return QColor("turquoise");
  case 8:
    return QColor("purple");
  case 9:
    return QColor("crimson");
  case 10:
    return QColor("black");
  }

  return QColor("black");
}
