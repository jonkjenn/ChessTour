#include "tournamentssqlmodel.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlIndex>

TournamentsSqlModel::TournamentsSqlModel(QObject *parent,QSqlDatabase database):
    QSqlTableModel(parent,database),database(database)
{
    /*//Could use if not exist create instead of checking
    QSqlQuery query("SELECT name from sqlite_master where type = \"table\" and name = \"Tournament\"",database);
    if(!query.next()){
        database.transaction();
        QSqlQuery fk_off("PRAGMA foreign_keys = off;",database);
        QSqlQuery match("CREATE TABLE Match (ResultWhite STRING, ResultBlack STRING, White STRING NOT NULL, BLACK STRING NOT NULL, WhiteFide INTEGER, BlackFide INTEGER, Priority INTEGER, GamePosition STRING, FullGameRKey STRING, CurrentFEN STRING, EarlierFEN STRING, LastMoves STRING, Engine STRING, DeepPrevious DOUBLE, DeepCurrent DOUBLE, DeepDepth INTEGER, DeepChange DOUBLE, DeepMovePrevious STRING, DeepMoveCurrent STRING, RoundId INTEGER REFERENCES Round (Id) ON DELETE CASCADE);",database);
        QSqlQuery round("CREATE TABLE Round (Number INT NOT NULL, Date STRING, Description STRING, TournamentId INTEGER REFERENCES Tournament (Id) ON DELETE CASCADE, Id INTEGER PRIMARY KEY AUTOINCREMENT);",database);
        QSqlQuery tournamnet("CREATE TABLE Tournament (Name STRING NOT NULL UNIQUE, OriginalOrder INT UNIQUE, Status STRING, EventType STRING, CurrentShownRound INT, Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, Subscribed BOOLEAN);",database);
        QSqlQuery fk_on("PRAGMA foreign_keys = on;",database);
        database.commit();
    }*/
}

QVariant TournamentsSqlModel::data(int row, QString columnName) const{
    return data(index(row,fieldIndex(columnName)),Qt::DisplayRole);
}

QVariant TournamentsSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (index.isValid()) {
       if (role < Qt::UserRole) {
           value = QSqlQueryModel::data(index, role);
       } else {
           int columnIdx = role - Qt::UserRole - 1;
           QModelIndex modelIndex = this->index(index.row(), columnIdx);
           value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
       }
    }
    return value;
}

bool TournamentsSqlModel::setData(int row,QString column, const QVariant &value){
    QModelIndex idx = index(row,fieldIndex(column));
    return setData(idx,value,Qt::EditRole);
}

bool TournamentsSqlModel::setData(const QModelIndex &item, const QVariant &value, int role)
{
    if (item.isValid() && role == Qt::EditRole) {
        QSqlTableModel::setData(item, value,role);
        emit dataChanged(item, item);
        return true;
    }
    return false;
}

QHash<int, QByteArray> TournamentsSqlModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    // record() returns an empty QSqlRecord
    for (int i = 0; i < this->record().count(); i ++) {
        roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8().toLower());
    }
    return roles;
}

void TournamentsSqlModel::forceRefresh()
{
    select();
}

int TournamentsSqlModel::getRow(int primaryKey)
{
    return rowFromPrimary.value(primaryKey);
}

int TournamentsSqlModel::getPk(int row)
{
    return primaryValues(row).value(0).toInt();
}

InternalMessages::TournamentChangedData TournamentsSqlModel::getTournamentData(int row){
    InternalMessages::TournamentChangedData d(
                getPk(row),
                data(row,"CurrentRound").toInt(),
                data(row,"Name").toString(),
                data(row,"EventType").toString(),
                false
                );

    QVariant currentGame = data(row,"CurrentGame");
    if(currentGame.isValid()){
        d.currentGame = currentGame.toInt();
    }

    return d;
}

void TournamentsSqlModel::setCurrentIndex(int row)
{
    emit currentTournamentChanged(getTournamentData(row));
}

bool TournamentsSqlModel::select()
{
    setSort(fieldIndex("OriginalOrder"),Qt::SortOrder::AscendingOrder);
    QSqlTableModel::select();

    //Create index PrimaryKey -> row (the row number used internally in model not in database)
    rowFromPrimary.clear();
    for(int i=0;i<rowCount();++i){
        rowFromPrimary.insert(primaryValues(i).value(0).toInt(0),i);
    }
}
