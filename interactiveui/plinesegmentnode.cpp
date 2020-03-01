#include "plinesegmentnode.h"
#include "graphicshelpers.h"
using namespace cavc;
PlineSegmentNode::PlineSegmentNode() {}

void PlineSegmentNode::updateGeometry(const cavc::PlineVertex<double> &v1,
                                      const cavc::PlineVertex<double> &v2, double arcApproxError) {

  if (v1.bulgeIsZero() || fuzzyEqual(v1.pos(), v2.pos())) {
    m_geometry.allocate(2);
    m_geometry.vertexDataAsPoint2D()[0].set(static_cast<float>(v1.x()), static_cast<float>(v1.y()));
    m_geometry.vertexDataAsPoint2D()[1].set(static_cast<float>(v2.x()), static_cast<float>(v2.y()));
  } else {
    auto arc = arcRadiusAndCenter(v1, v2);
    auto startAngle = angle(arc.center, v1.pos());
    auto endAngle = angle(arc.center, v2.pos());
    double deltaAngle = std::abs(utils::deltaAngle(startAngle, endAngle));

    double segmentSubAngle = std::abs(2.0 * std::acos(1.0 - arcApproxError / arc.radius));
    std::size_t segmentCount = static_cast<std::size_t>(std::ceil(deltaAngle / segmentSubAngle));
    // update segment subangle for equal length segments
    segmentSubAngle = deltaAngle / segmentCount;

    if (v1.bulge() < 0.0) {
      segmentSubAngle = -segmentSubAngle;
    }

    m_geometry.allocate(static_cast<int>(segmentCount + 1));

    for (std::size_t i = 0; i <= segmentCount; ++i) {
      double angle = i * segmentSubAngle + startAngle;
      m_geometry.vertexDataAsPoint2D()[i].set(
          static_cast<float>(arc.radius * std::cos(angle) + arc.center.x()),
          static_cast<float>(arc.radius * std::sin(angle) + arc.center.y()));
    }
  }

  markDirty(QSGNode::DirtyGeometry);
}
