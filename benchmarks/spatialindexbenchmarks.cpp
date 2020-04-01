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
    profile.pline.visitSegIndices([&](std::size_t i, std::size_t j) {
      cavc::AABB<double> bb = cavc::createFastApproxBoundingBox(profile.pline[i], profile.pline[j]);
      queryResults.clear();
      bb.expand(5);
      spatialIndex.query(bb.xMin, bb.yMin, bb.xMax, bb.yMax, queryResults);
      return true;
    });
  }
}
BENCHMARK(BM_querySpatialIndex)->Unit(benchmark::kMicrosecond);

static void BM_querySpatialIndexReuseStack(benchmark::State &state) {
  auto profile = circle();
  profile.pline = cavc::convertArcsToLines(profile.pline, arcError);
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  auto spatialIndex = cavc::createApproxSpatialIndex(profile.pline);
  std::vector<std::size_t> queryResults;
  std::vector<std::size_t> queryStack;
  for (auto _ : state) {
    (void)_;
    profile.pline.visitSegIndices([&](std::size_t i, std::size_t j) {
      cavc::AABB<double> bb = cavc::createFastApproxBoundingBox(profile.pline[i], profile.pline[j]);
      queryResults.clear();
      bb.expand(5);
      spatialIndex.query(bb.xMin, bb.yMin, bb.xMax, bb.yMax, queryResults, queryStack);
      return true;
    });
  }
}
BENCHMARK(BM_querySpatialIndexReuseStack)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
