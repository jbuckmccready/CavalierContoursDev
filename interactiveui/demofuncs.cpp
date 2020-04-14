#include "demofuncs.h"
#include "cavc/staticspatialindex.hpp"

DemoFuncs::DemoFuncs(QObject *parent) : QObject(parent) {}

unsigned int DemoFuncs::hilbertXYToIndex(unsigned x, unsigned int y) {
  return cavc::StaticSpatialIndex<double>::hilbertXYToIndex(x, y);
}
