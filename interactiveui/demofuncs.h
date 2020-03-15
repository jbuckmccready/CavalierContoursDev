#ifndef DEMOFUNCS_H
#define DEMOFUNCS_H

#include <QObject>

class DemoFuncs : public QObject
{
  Q_OBJECT
public:
  explicit DemoFuncs(QObject *parent = nullptr);
  Q_INVOKABLE static unsigned int hilbertXYToIndex(unsigned int x, unsigned int y);
};

#endif // DEMOFUNCS_H
