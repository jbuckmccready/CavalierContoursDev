#include "benchmarkprofiles.h"
#include "cavc/polyline.h"
#include <benchmark/benchmark.h>

static void runProfile(TestProfile const &profile, bool negateDelta) {
  double offsetDelta = negateDelta ? -profile.offsetDelta : profile.offsetDelta;

  for (std::size_t i = 1; i <= profile.offsetCount; ++i) {
    double offset = i * offsetDelta;
    auto result = cavc::parallelOffset(profile.pline, offset);
  }
}

static void runProfileFolded(TestProfile const &profile, bool negateDelta) {
  // folded repeat offsets
  double offsetDelta = negateDelta ? -profile.offsetDelta : profile.offsetDelta;
  std::vector<cavc::Polyline<double>> prevOffsets;
  prevOffsets.push_back(profile.pline);

  double origPlineA = area(profile.pline);
  std::vector<cavc::Polyline<double>> newOffsets;
  for (std::size_t i = 0; i < profile.offsetCount; ++i) {
    if (prevOffsets.size() == 0) {
      break;
    }

    newOffsets = std::vector<cavc::Polyline<double>>();
    for (const auto &pline : prevOffsets) {
      auto offsetPlines = parallelOffset(pline, offsetDelta);
      newOffsets.insert(newOffsets.end(), std::make_move_iterator(offsetPlines.begin()),
                        std::make_move_iterator(offsetPlines.end()));
    }
    auto copy = prevOffsets;
    prevOffsets = std::move(newOffsets);
    prevOffsets.erase(std::remove_if(prevOffsets.begin(), prevOffsets.end(),
                                     [&](const auto &pline) {
                                       if (!profile.pline.isClosed()) {
                                         return false;
                                       }
                                       double a = area(pline);
                                       return (a > 0 != origPlineA > 0) || std::abs(a) < 1e-4;
                                     }),
                      prevOffsets.end());
  }
}

static void BM_Profile1(benchmark::State &state) {
  auto profile = profile1(static_cast<std::size_t>(state.range(0)));
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_Profile1)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(8, 8 << 7);

static void BM_FoldedProfile1(benchmark::State &state) {
  auto profile = profile1(static_cast<std::size_t>(state.range(0)));
  for (auto _ : state) {
    (void)_;
    runProfileFolded(profile, false);
    runProfileFolded(profile, true);
  }
}
BENCHMARK(BM_FoldedProfile1)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(8, 8 << 7);

static void BM_Profile2(benchmark::State &state) {
  auto profile = profile2();
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_Profile2)->Unit(benchmark::kMillisecond);

static void BM_FoldedProfile2(benchmark::State &state) {
  auto profile = profile2();
  for (auto _ : state) {
    (void)_;
    runProfileFolded(profile, false);
    runProfileFolded(profile, true);
  }
}
BENCHMARK(BM_FoldedProfile2)->Unit(benchmark::kMillisecond);

static void BM_Profile3(benchmark::State &state) {
  auto profile = profile3();
  for (auto _ : state) {
    (void)_;
    runProfile(profile, false);
    runProfile(profile, true);
  }
}
BENCHMARK(BM_Profile3)->Unit(benchmark::kMillisecond);

static void BM_FoldedProfile3(benchmark::State &state) {
  auto profile = profile3();
  for (auto _ : state) {
    (void)_;
    runProfileFolded(profile, false);
    runProfileFolded(profile, true);
  }
}
BENCHMARK(BM_FoldedProfile3)->Unit(benchmark::kMillisecond);

static void BM_PathologicalProfile1(benchmark::State &state) {
  auto profile = pathologicalProfile1(static_cast<std::size_t>(state.range(0)));
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

static void BM_FoldedPathologicalProfile1(benchmark::State &state) {
  auto profile = pathologicalProfile1(static_cast<std::size_t>(state.range(0)));
  for (auto _ : state) {
    (void)_;
    runProfileFolded(profile, false);
    runProfileFolded(profile, true);
  }
}
BENCHMARK(BM_FoldedPathologicalProfile1)
    ->Unit(benchmark::kMillisecond)
    ->Arg(10)
    ->Arg(25)
    ->Arg(50)
    ->Arg(100);

BENCHMARK_MAIN();
