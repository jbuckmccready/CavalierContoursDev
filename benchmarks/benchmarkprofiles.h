#include "cavc/polyline.h"
struct TestProfile {
  std::size_t offsetCount;
  double offsetDelta;
  cavc::Polyline<double> pline;
  TestProfile(std::size_t offsetCount, double offsetDelta, cavc::Polyline<double> pline)
      : offsetCount(offsetCount), offsetDelta(offsetDelta), pline(pline) {}
};

inline TestProfile profile1(std::size_t segmentCount) {
  auto radius = 40;
  auto centerX = 0;
  auto centerY = 0;
  cavc::Polyline<double> pline;
  pline.isClosed() = true;

  for (std::size_t i = 0; i < segmentCount; ++i) {
    double angle = static_cast<double>(i) * cavc::utils::tau<double> / segmentCount;
    pline.addVertex(radius * std::cos(angle) + centerX,
                              radius * std::sin(angle) + centerY, 0);
  }

  return TestProfile(300, 0.1, std::move(pline));
}

inline TestProfile profile2() {
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

inline TestProfile profile3() {
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
    double angle = static_cast<double>(i) * cavc::utils::tau<double> / segmentCount;
    pline.addVertex(radius * std::cos(angle) + centerX,
                              radius * std::sin(angle) + centerY, i % 2 == 0 ? 1 : -1);
  }

  return TestProfile(30, 1, std::move(pline));
}
