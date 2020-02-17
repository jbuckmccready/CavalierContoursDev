#include "benchmarkprofiles.h"
#include "cavc/polyline.h"
#include <benchmark/benchmark.h>

const double arcError = 0.01;

static void runProfile(TestProfile const &profile, bool negateDelta) {
  double offsetDelta = negateDelta ? -profile.offsetDelta : profile.offsetDelta;

  for (std::size_t i = 1; i <= profile.offsetCount; ++i) {
    double offset = i * offsetDelta;
    auto result = cavc::parallelOffset(profile.pline, offset);
  }
}

static void BM_square(benchmark::State &state) {
  auto profile = square();
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_square)->Unit(benchmark::kMillisecond);

static void BM_diamond(benchmark::State &state) {
  auto profile = diamond();
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_diamond)->Unit(benchmark::kMillisecond);

static void BM_circle(benchmark::State &state) {
  auto profile = circle();
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_circle)->Unit(benchmark::kMillisecond);

static void BM_roundedRectangle(benchmark::State &state) {
  auto profile = roundedRectangle();
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_roundedRectangle)->Unit(benchmark::kMillisecond);

static void BM_Profile1(benchmark::State &state) {
  auto profile = profile1();
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_Profile1)->Unit(benchmark::kMillisecond);

static void BM_Profile2(benchmark::State &state) {
  auto profile = profile2();
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_Profile2)->Unit(benchmark::kMillisecond);

static void BM_PathologicalProfile1(benchmark::State &state) {
  auto profile = pathologicalProfile1(static_cast<std::size_t>(state.range(0)));
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_PathologicalProfile1)
    ->Unit(benchmark::kMillisecond)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100);

static void BM_circleNoArcs(benchmark::State &state) {
  auto profile = circle();
  profile.pline = cavc::convertArcsToLines(profile.pline, arcError);
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_circleNoArcs)->Unit(benchmark::kMillisecond);

static void BM_roundedRectangleNoArcs(benchmark::State &state) {
  auto profile = roundedRectangle();
  profile.pline = cavc::convertArcsToLines(profile.pline, arcError);
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_roundedRectangleNoArcs)->Unit(benchmark::kMillisecond);

static void BM_Profile1NoArcs(benchmark::State &state) {
  auto profile = profile1();
  profile.pline = cavc::convertArcsToLines(profile.pline, arcError);
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_Profile1NoArcs)->Unit(benchmark::kMillisecond);

static void BM_Profile2NoArcs(benchmark::State &state) {
  auto profile = profile2();
  profile.pline = cavc::convertArcsToLines(profile.pline, arcError);
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_Profile2NoArcs)->Unit(benchmark::kMillisecond);

static void BM_PathologicalProfile1NoArcs(benchmark::State &state) {
  auto profile = pathologicalProfile1(static_cast<std::size_t>(state.range(0)));
  profile.pline = cavc::convertArcsToLines(profile.pline, arcError);
  state.counters["vertexCount"] = static_cast<double>(profile.pline.size());
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_PathologicalProfile1NoArcs)
    ->Unit(benchmark::kMillisecond)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100);

BENCHMARK_MAIN();
