#include "clipper/clipper.hpp"
#include "benchmarkprofiles.h"
#include "cavc/polyline.h"
#include <benchmark/benchmark.h>

const double clipperScaleFactor = 1e8;
const double unscaledArcError = 0.01;

static ClipperLib::Path polylineToClipperPath(const cavc::Polyline<double> &pline,
                                              double unscaledArcError) {
  auto noArcsPline = cavc::convertArcsToLines(pline, unscaledArcError);

  ClipperLib::Path clipperPath;
  clipperPath.reserve(noArcsPline.size());
  for (const auto &v : noArcsPline.vertexes()) {
    ClipperLib::cInt xInt = static_cast<ClipperLib::cInt>(v.x() * clipperScaleFactor);
    ClipperLib::cInt yInt = static_cast<ClipperLib::cInt>(v.y() * clipperScaleFactor);
    clipperPath.push_back(ClipperLib::IntPoint(xInt, yInt));
  }

  return clipperPath;
}

static void clipperOffset(const ClipperLib::Path &path, double unscaledDelta,
                          double unscaledArcError, ClipperLib::Paths &results) {
  ClipperLib::ClipperOffset clipperOffset(2, unscaledArcError * clipperScaleFactor);
  clipperOffset.AddPath(path, ClipperLib::JoinType::jtRound, ClipperLib::EndType::etClosedPolygon);
  // negate delta to match orientation that cavc uses
  clipperOffset.Execute(results, -unscaledDelta * clipperScaleFactor);
}

static void runProfile(TestProfile const &profile, ClipperLib::Path const &path,
                       ClipperLib::Paths &results, bool negateDelta) {
  double offsetDelta = negateDelta ? -profile.offsetDelta : profile.offsetDelta;

  for (std::size_t i = 1; i <= profile.offsetCount; ++i) {
    double offset = i * offsetDelta;
    clipperOffset(path, offset, unscaledArcError, results);
  }
}

static void BM_Profile1(benchmark::State &state) {
  auto profile = profile1(static_cast<std::size_t>(state.range(0)));
  auto path = polylineToClipperPath(profile.pline, unscaledArcError);
  ClipperLib::Paths results;
  for (auto _ : state) {
    (void)_;
    runProfile(profile, path, results, false);
    runProfile(profile, path, results, true);
  }
}
BENCHMARK(BM_Profile1)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(8, 8 << 7);

static void BM_Profile2(benchmark::State &state) {
  auto profile = profile2();
  auto path = polylineToClipperPath(profile.pline, unscaledArcError);
  ClipperLib::Paths results;
  for (auto _ : state) {
    (void)_;
    runProfile(profile, path, results, false);
    runProfile(profile, path, results, true);
  }
}
BENCHMARK(BM_Profile2)->Unit(benchmark::kMillisecond);

static void BM_Profile3(benchmark::State &state) {
  auto profile = profile3();
  auto path = polylineToClipperPath(profile.pline, unscaledArcError);
  ClipperLib::Paths results;
  for (auto _ : state) {
    (void)_;
    runProfile(profile, path, results, false);
    runProfile(profile, path, results, true);
  }
}
BENCHMARK(BM_Profile3)->Unit(benchmark::kMillisecond);

static void BM_PathologicalProfile1(benchmark::State &state) {
  auto profile = pathologicalProfile1(static_cast<std::size_t>(state.range(0)));
  auto path = polylineToClipperPath(profile.pline, unscaledArcError);
  ClipperLib::Paths results;
  for (auto _ : state) {
    (void)_;
    runProfile(profile, path, results, false);
    runProfile(profile, path, results, true);
  }
}
BENCHMARK(BM_PathologicalProfile1)
    ->Unit(benchmark::kMillisecond)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100);

BENCHMARK_MAIN();
