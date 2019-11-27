#include <QtTest>
#include "cavc/polyline.h"

using namespace cavc;

class Benchmarks : public QObject
{
    Q_OBJECT

public:
    Benchmarks();
    ~Benchmarks();

private slots:
    void simpleRepeatedOffsets();
    void foldedRepeatedOffsets();

private:
  Polyline<double> pline1;
};

Benchmarks::Benchmarks()
{
//  pline1.isClosed() = true;
//  pline1.addVertex(0, 0, 0.0);
//  pline1.addVertex(2, 0, 1.0);
//  pline1.addVertex(10, 0, -0.5);
//  pline1.addVertex(10, 10, 0.5);
//  pline1.addVertex(14, 20, -0.5);
//  pline1.addVertex(0, 20, 0);
  auto radius = 40;
  auto centerX = 0;
  auto centerY = 0;
  std::size_t segmentCount = 1000;
  for (std::size_t i = 0; i < segmentCount; ++i) {
    double angle = static_cast<double>(i) * utils::tau<double> / segmentCount;
    pline1.addVertex(radius * std::cos(angle) + centerX,
                              radius * std::sin(angle) + centerY, 0);
  }
  pline1.isClosed() = true;

}

Benchmarks::~Benchmarks()
{

}

void Benchmarks::simpleRepeatedOffsets()
{
  const double offsetIncr = 0.1;

  QBENCHMARK {
    for (std::size_t i = 1; i < 201; ++i) {
      double offset = i * offsetIncr;
      auto results = parallelOffset(pline1, offset);
    }
  }
}

void Benchmarks::foldedRepeatedOffsets()
{
  const double offsetIncr = 0.1;
  double origPlineA = area(pline1);

  QBENCHMARK {
    std::vector<Polyline<double>> prevOffsets = { pline1 };
    std::vector<Polyline<double>> newOffsets;
    for (std::size_t i = 0; i < 200; ++i) {
      newOffsets = std::vector<Polyline<double>>();
      for (auto const &prevOffs : prevOffsets) {
          auto results = parallelOffset(prevOffs, offsetIncr);
          newOffsets.insert(newOffsets.end(), std::make_move_iterator(results.begin()), std::make_move_iterator(results.end()));
      }

      prevOffsets = std::move(newOffsets);
      prevOffsets.erase(std::remove_if(prevOffsets.begin(), prevOffsets.end(),
                                       [&](const auto &pline) {
                                         double a = area(pline);
                                         return (a > 0 != origPlineA > 0) || std::abs(a) < 1e-4;
                                       }),
                        prevOffsets.end());
      if (prevOffsets.size() == 0) {
        break;
      }
    }
  }

}

QTEST_APPLESS_MAIN(Benchmarks)

#include "benchmarks.moc"
