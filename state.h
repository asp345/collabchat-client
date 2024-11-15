#ifndef STATE_H
#define STATE_H

#include <QDate>
#include <QObject>

typedef struct _State {
  QString workspace, id, password, serverurl, token;
  QDate date;
} State;

#endif  // STATE_H
