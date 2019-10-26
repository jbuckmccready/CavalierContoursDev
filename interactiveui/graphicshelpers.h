#ifndef GRAPHICSHELPERS_H
#define GRAPHICSHELPERS_H
#include <QColor>

namespace gh {
inline QColor indexToColor(std::size_t index) {
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
}

#endif // GRAPHICSHELPERS_H
