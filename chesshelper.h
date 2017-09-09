#ifndef CHESSHELPER_H
#define CHESSHELPER_H
#include <QObject>
#include <QVariant>
#include <QVariantMap>


class ChessHelper:public QObject
{
    Q_OBJECT
public:
    ChessHelper(QObject *);
    static QVariantMap parseFEN(QString fen);
};

#endif // CHESSHELPER_H
