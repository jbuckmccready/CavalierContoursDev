#include "plinesegmentnode.h"
#include "graphicshelpers.h"
using namespace cavc;
PlineSegmentNode::PlineSegmentNode() {}

void PlineSegmentNode::updateGeometry(const cavc::PlineVertex<double> &v1,
                                      const cavc::PlineVertex<double> &v2, double uiScale) {

  if (v1.bulgeIsZero() || fuzzyEqual(v1.pos(), v2.pos())) {
    m_geometry.allocate(2);
    m_geometry.vertexDataAsPoint2D()[0].set(static_cast<float>(v1.x()), static_cast<float>(v1.y()));
    m_geometry.vertexDataAsPoint2D()[1].set(static_cast<float>(v2.x()), static_cast<float>(v2.y()));
  } else {
    auto arc = arcRadiusAndCenter(v1, v2);
    auto startAngle = angle(arc.center, v1.pos());
    auto endAngle = angle(arc.center, v2.pos());
    double deltaAngle = std::abs(utils::deltaAngle(startAngle, endAngle));

    std::size_t segmentCount =
        static_cast<std::size_t>(std::ceil(arc.radius * deltaAngle * uiScale));
    const double sweepAngle = v1.bulge() > 0 ? deltaAngle : -deltaAngle;
    m_geometry.allocate(static_cast<int>(segmentCount + 1));

    for (std::size_t i = 0; i <= segmentCount; ++i) {
      double angle = static_cast<double>(i) / segmentCount * sweepAngle + startAngle;
      m_geometry.vertexDataAsPoint2D()[i].set(
          static_cast<float>(arc.radius * std::cos(angle) + arc.center.x()),
          static_cast<float>(arc.radius * std::sin(angle) + arc.center.y()));
    }
  }

  markDirty(QSGNode::DirtyGeometry);
}
