#include "livematchsqlmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

LiveMatchSqlModel::LiveMatchSqlModel(QObject *parent, QSqlDatabase &database):QAbstractListModel(parent),database(database){
    QSqlQuery q(database);
    q.prepare("pragma table_info(Match)");
    if(!q.exec()){qDebug() << q.lastError();}

    // record() returns an empty QSqlRecord
    int i=0;
    while(q.next()){
        roles.insert(Qt::UserRole + i + 1, q.value(1).toString().toLower().toUtf8());
        roleIdToColumn.insert(Qt::UserRole + i + 1,q.value(1).toString());
        columnToRoleId.insert(q.value(1).toString(),Qt::UserRole + i + 1);
        ++i;
    }
}

void LiveMatchSqlModel::possibleUpdates(const QVariantList &lists){
    for(auto l:lists){
        for(auto ml:l.toList()){
        QVariantList li = ml.toList();
        if(li.at(0).toInt() == currentPk()){
            int row = pkToRow.value(li.at(1).toInt());
            QVector<int> roles;
            for(int i=1;i<li.size();++i){
                if(columnToRoleId.contains(li.at(i).toString())){
                    roles.append(columnToRoleId.value(li.at(i).toString()));
                }
            }
            emit dataChanged(index(row),index(row),roles);
        }else{
            break;
        }
        }
    }
}

int LiveMatchSqlModel::rowCount(const QModelIndex &parent) const
{
    return m_rowCount;
}

QVariant LiveMatchSqlModel::data(const QModelIndex &index, int role) const
{
    int pk = rowToPk.value(index.row());
    QSqlQuery q(database);
    if(roleIdToColumn.value(role) == "WhiteFide"){
        q.exec("select Players.Name from Match JOIN Players on Players.fideId = WhiteFide and Id = " + QString::number(pk));
    }else if(roleIdToColumn.value(role) == "BlackFide"){
        q.exec("select Players.Name from Match JOIN Players on Players.fideId = BlackFide and Id = " + QString::number(pk));
    }else{
        q.prepare("SELECT " + roleIdToColumn.value(role) + " FROM Match WHERE Id = ?");
        q.bindValue(0,pk);
        q.exec();
    }

    if(q.lastError().isValid()){qDebug() << q.lastError();}

    if(!q.next()){
        return "";
    }

    if(q.isNull(0)){
        return "";
    }
    return q.value(0);
}

bool LiveMatchSqlModel::setData(int row, QString column, const QVariant &value){
    return setData(index(row),value,columnToRoleId.value(column));
}

bool LiveMatchSqlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int pk = rowToPk.value(index.row());
    QSqlQuery q(database);
    qDebug() << "Update Match SET " + roleIdToColumn.value(role) + " = " + value.toString() + "WHERE Id = " + QString::number(pk);
    q.prepare("UPDATE Match SET " + roleIdToColumn.value(role) + " = :value WHERE Id = :id");
    //q.bindValue(":column",roleIdToColumn.value(role));
    q.bindValue(":value",value);
    q.bindValue(":id",pk);
    if(!q.exec()){qDebug() << q.lastError();}
    emit dataChanged(index,index,{role});
}

bool LiveMatchSqlModel::insertRows(int row, int count, const QModelIndex &parent)
{
}

QHash<int, QByteArray> LiveMatchSqlModel::roleNames() const
{
    return roles;
}

void LiveMatchSqlModel::setRound(int pk){
    beginResetModel();
    QSqlQuery q(database);
    q.prepare("SELECT Id FROM Match WHERE RoundId = :roundId");
    q.bindValue(":roundId",pk);
    q.exec();
    int i=0;
    rowToPk.clear();
    pkToRow.clear();
    while(q.next()){
        rowToPk.insert(i,q.value(0).toInt());
        pkToRow.insert(q.value(0).toInt(),i);
        ++i;
    }
    m_rowCount = rowToPk.size();

    m_currentPk = pk;
    endResetModel();
}

int LiveMatchSqlModel::currentPk()
{
    return m_currentPk;
}
