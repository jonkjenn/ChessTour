#include "chesshelper.h"

ChessHelper::ChessHelper(QObject *parent):QObject(parent)
{

}

QVariantMap ChessHelper::parseFEN(QString fen)
{
    QStringList l = fen.split(" ");

    QVariantList position;
    int rank = 8;
    int x = 0;
    for(auto s:l.at(0)){
        if(s.isDigit()){x+=s.digitValue();continue;}
        if(s == "/"){rank-=1;x=0;continue;}
        position.append(s);
        position.append(x);
        position.append(rank-1);
        x+=1;
    }

    QVariantMap result;
    result.insert("position",position);

    result.insert("tomove",l.at(1));
    result.insert("castle",l.at(2));
    result.insert("enpassant",l.at(3));
    result.insert("halfmove",l.at(4));
    result.insert("fullmove",l.at(5));

    return result;
}
