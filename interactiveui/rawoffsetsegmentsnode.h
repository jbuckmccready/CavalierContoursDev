#ifndef RAWOFFSETSEGMENTSNODE_H
#define RAWOFFSETSEGMENTSNODE_H

#include "cavc/polyline.hpp"
#include "cavc/polylineoffset.hpp"
#include "plinesegmentnode.h"
#include <QSGOpacityNode>
#include <vector>

class RawOffsetSegmentsNode : public QSGOpacityNode {
public:
  RawOffsetSegmentsNode();
  void
  updateGeometry(std::vector<cavc::internal::PlineOffsetSegment<double>> const &rawOffsetSegments,
                 double arcApproxError);
};

#endif // RAWOFFSETSEGMENTSNODE_H
