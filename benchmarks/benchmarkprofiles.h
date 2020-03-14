#include "cavc/polyline.h"
struct TestProfile {
  std::size_t offsetCount;
  double offsetDelta;
  cavc::Polyline<double> pline;
  TestProfile(std::size_t offsetCount, double offsetDelta, cavc::Polyline<double> pline)
      : offsetCount(offsetCount), offsetDelta(offsetDelta), pline(pline) {}
};

inline TestProfile square() {
  auto centerX = 5.0;
  auto centerY = 5.0;
  auto width = 40.0;
  auto height = 40.0;
  cavc::Polyline<double> pline;
  pline.isClosed() = true;
  pline.addVertex(centerX - width / 2, centerY - height / 2, 0);
  pline.addVertex(centerX + width / 2, centerY - height / 2, 0);
  pline.addVertex(centerX + width / 2, centerY + height / 2, 0);
  pline.addVertex(centerX - width / 2, centerY + height / 2, 0);

  return TestProfile(30, 1, std::move(pline));
}

inline TestProfile diamond() {
  auto centerX = 5.0;
  auto centerY = 5.0;
  auto width = 40.0;
  auto height = 40.0;
  cavc::Polyline<double> pline;
  pline.isClosed() = true;
  pline.addVertex(centerX - width / 2, centerY, 0);
  pline.addVertex(centerX, centerY - height / 2, 0);
  pline.addVertex(centerX + width / 2, centerY, 0);
  pline.addVertex(centerX, centerY + height / 2, 0);

  return TestProfile(30, 1, std::move(pline));
}

inline TestProfile circle() {
  auto centerX = 5.0;
  auto centerY = 5.0;
  auto radius = 40.0;
  cavc::Polyline<double> pline;
  pline.isClosed() = true;
  pline.addVertex(centerX - radius, centerY, 1.0);
  pline.addVertex(centerX + radius, centerY, 1.0);

  return TestProfile(30, 1, std::move(pline));
}

inline TestProfile roundedRectangle() {
  auto centerX = 5.0;
  auto centerY = 5.0;
  auto totalWidth = 40.0;
  auto totalHeight = 20.0;
  auto cornerRadius = 5.0;
  auto width = totalWidth - 2 * cornerRadius;
  auto height = totalHeight - 2 * cornerRadius;
  // 90 deg corner radii
  auto bulge = std::tan(cavc::utils::pi<double>() / 8.0);

  cavc::Polyline<double> pline;
  pline.isClosed() = true;
  // start YMin edge
  pline.addVertex(centerX - width / 2, centerY - totalHeight / 2, 0);
  // start XMaxYMin radius
  pline.addVertex(centerX + width / 2, centerY - totalHeight / 2, bulge);
  // start XMax edge
  pline.addVertex(centerX + totalWidth / 2, centerY - height / 2, 0);
  // start XMaxYMax radius
  pline.addVertex(centerX + totalWidth / 2, centerY + height / 2, bulge);
  // start YMax edge
  pline.addVertex(centerX + width / 2, centerY + totalHeight / 2, 0);
  // start XMinYMax radius
  pline.addVertex(centerX - width / 2, centerY + totalHeight / 2, bulge);
  // start XMin edge
  pline.addVertex(centerX - totalWidth / 2, centerY + height / 2, 0);
  // start XMinYMin radius
  pline.addVertex(centerX - totalWidth / 2, centerY - height / 2, bulge);

  return TestProfile(30, 0.5, std::move(pline));
}

inline TestProfile profile1() {
  cavc::Polyline<double> pline;
  pline.isClosed() = true;

  pline.addVertex(0, 0, 0.0);
  pline.addVertex(2, 0, 1.0);
  pline.addVertex(10, 0, -0.5);
  pline.addVertex(10, 10, 0.5);
  pline.addVertex(14, 20, -0.5);
  pline.addVertex(0, 20, 0);

  return TestProfile(40, 0.1, std::move(pline));
}

inline TestProfile profile2() {
  cavc::Polyline<double> pline;
  pline.addVertex(0, 25, 1);
  pline.addVertex(0, 0, 0);
  pline.addVertex(2, 0, 1);
  pline.addVertex(10, 0, -0.5);
  pline.addVertex(8, 9, 0.374794619217547);
  pline.addVertex(21, 0, 0);
  pline.addVertex(23, 0, 1);
  pline.addVertex(32, 0, -0.5);
  pline.addVertex(28, 0, 0.5);
  pline.addVertex(39, 21, 0);
  pline.addVertex(28, 12, 0);
  pline.isClosed() = true;

  return TestProfile(40, 0.1, std::move(pline));
}

inline TestProfile pathologicalProfile1(std::size_t segmentCount) {
  auto radius = 40;
  auto centerX = 0;
  auto centerY = 0;
  cavc::Polyline<double> pline;
  pline.isClosed() = true;

  for (std::size_t i = 0; i < segmentCount; ++i) {
    double angle = static_cast<double>(i) * cavc::utils::tau<double>() / segmentCount;
    pline.addVertex(radius * std::cos(angle) + centerX, radius * std::sin(angle) + centerY,
                    i % 2 == 0 ? 1 : -1);
  }

  return TestProfile(30, 1, std::move(pline));
}
