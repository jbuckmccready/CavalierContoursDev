#include "rawoffsetsegmentsnode.h"

RawOffsetSegmentsNode::RawOffsetSegmentsNode() {}

void RawOffsetSegmentsNode::updateGeometry(
    const std::vector<cavc::internal::PlineOffsetSegment<double>> &rawOffsetSegments,
    double arcApproxError) {
  PlineSegmentNode *segNode = static_cast<PlineSegmentNode *>(this->firstChild());
  for (std::size_t i = 0; i < rawOffsetSegments.size(); ++i) {
    auto const &seg = rawOffsetSegments[i];

    if (segNode == nullptr) {
      segNode = new PlineSegmentNode();
      segNode->setFlag(QSGNode::OwnedByParent);
      this->appendChildNode(segNode);
    }

    segNode->setIsVisible(true);

    segNode->updateGeometry(seg.v1, seg.v2, arcApproxError);
    if (seg.collapsedArc) {
      // collapsed arc
      segNode->setColor(Qt::red);
    } else {
      segNode->setColor(Qt::darkMagenta);
    }
    segNode = static_cast<PlineSegmentNode *>(segNode->nextSibling());
  }

  // hide remaining nodes (keep around as cache to avoid repeated alloc/dealloc)
  while (segNode) {
    segNode->setIsVisible(false);
    segNode = static_cast<PlineSegmentNode *>(segNode->nextSibling());
  }
}
