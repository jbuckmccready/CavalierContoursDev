#include <QtTest>
#include "polyline.h"

using namespace cavc;

class Benchmarks : public QObject
{
    Q_OBJECT

public:
    Benchmarks();
    ~Benchmarks();

private slots:
    void simpleRepeatedOffsets();

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
  const double offsetIncr = 0.5;

  QBENCHMARK {
    for (std::size_t i = 1; i < 41; ++i) {
      double offset = i * offsetIncr;
      auto results = paralleOffset(pline1, offset, 1);
    }
  }
}

QTEST_APPLESS_MAIN(Benchmarks)

#include "benchmarks.moc"
