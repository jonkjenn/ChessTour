#include "roundssqlmodel.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

RoundsSqlModel::RoundsSqlModel(QObject *parent, QSqlDatabase database):
    QSqlTableModel(parent), database(database)
{
}


QVariant RoundsSqlModel::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> RoundsSqlModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    // record() returns an empty QSqlRecord
    for (int i = 0; i < this->record().count(); i ++) {
        roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8().toLower());
    }
    return roles;
}



bool RoundsSqlModel::select()
{
    if(!QSqlTableModel::select()){return false;}

    //Create index PrimaryKey -> row (the row number used internally in model not in database)
    rowFromPrimary.clear();
    for(int i=0;i<rowCount();++i){
        rowFromPrimary.insert(primaryValues(i).value(0).toInt(),i);
    }

    emit roundDataChanged();
    return true;
}

int RoundsSqlModel::getRow(int primaryKey)
{
    if(primaryKey<0){return -1;}
    return rowFromPrimary.value(primaryKey);
}

int RoundsSqlModel::getPk(int row)
{
    return primaryValues(row).value(0).toInt();
}

void RoundsSqlModel::setCurrentIndex(int index)
{
    setCurrentShownRound(index);
}

void RoundsSqlModel::tournamentChanged(int pk)
{
    setFilter("TournamentId = " + QString::number(pk));
    setSort(0,Qt::SortOrder::AscendingOrder);
    select();
    m_currentPK = pk;
}

void RoundsSqlModel::forceRefresh(){
    select();
}

int RoundsSqlModel::currentPK()
{
    return m_currentPK;
}

int RoundsSqlModel::currentShownRound()
{
    QModelIndex i = index(0,fieldIndex("TournamentId"));
    int tournamentId = data(i,Qt::DisplayRole).toInt();

    QSqlQuery sql(database);
    sql.prepare("SELECT CurrentShownRound FROM Tournament WHERE Id = :tournamentId");
    sql.bindValue(":tournamentId",tournamentId);
    sql.exec();
    if(!sql.next()){qDebug() << "Could nto retrieve current round";}

    int currRound = getRow(sql.value(0).toInt());

    if(currRound<0){
        currRound = 0;
    }

    return currRound;
}

void RoundsSqlModel::setCurrentShownRound(int currentShownRound)
{
    QModelIndex i = index(currentShownRound,fieldIndex("TournamentId"));
    int tournamentId = data(i,Qt::DisplayRole).toInt();

    QSqlQuery sql(database);
    sql.prepare("UPDATE Tournament SET CurrentShownRound = :currentShownRound WHERE Id = :tournamentId");
    sql.bindValue(":currentShownRound", getPk(currentShownRound));
    sql.bindValue(":tournamentId",tournamentId);
    sql.exec();
    if(sql.lastError().isValid()){ qDebug() << sql.lastError();}
    //qDebug() << sql.lastQuery();
}
