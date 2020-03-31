#include "benchmarkprofiles.h"
#include "cavc/staticspatialindex.h"
#include <benchmark/benchmark.h>

const double arcError = 0.00001;

static void BM_createSpatialIndex(benchmark::State &state) {
  auto profile = circle();
  profile.pline = cavc::convertArcsToLines(profile.pline, arcError);
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    cavc::createApproxSpatialIndex(profile.pline);
  }

}
BENCHMARK(BM_createSpatialIndex)->Unit(benchmark::kMicrosecond);

static void BM_querySpatialIndex(benchmark::State &state) {
  auto profile = circle();
  profile.pline = cavc::convertArcsToLines(profile.pline, arcError);
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  auto spatialIndex = cavc::createApproxSpatialIndex(profile.pline);
  std::vector<std::size_t> queryResults;
  for (auto _ : state) {
    (void)_;
    for (std::size_t i = 0; i < profile.pline.size(); ++i) {
      cavc::AABB<double> bb;
      bb.xMin = bb.xMax = profile.pline[i].x();
      bb.yMin = bb.yMax = profile.pline[i].y();
      queryResults.clear();
      const double delta = 5.0;
      bb.expand(delta);
      spatialIndex.query(bb.xMin, bb.yMin, bb.xMax, bb.yMax, queryResults);
    }
  }
}
BENCHMARK(BM_querySpatialIndex)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
