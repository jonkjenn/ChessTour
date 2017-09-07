#include "livematchsqlmodel.h"
#include "sqlhelper.h"
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

QString LiveMatchSqlModel::eventType() const
{
    return m_eventType;
}

int LiveMatchSqlModel::gamesPerMatch() const
{
    return m_gamesPerMatch;
}

void LiveMatchSqlModel::setCurrentGameNumber(int gameNumber)
{
    beginResetModel();
    m_currentGameNumber = gameNumber;
    updateData(m_currentPk,m_currentGameNumber);
    endResetModel();
}

int LiveMatchSqlModel::currentGameNumber()
{
    return m_currentGameNumber;
}

int LiveMatchSqlModel::rowCount(const QModelIndex &parent) const
{
    return m_rowCount;
}

QVariant LiveMatchSqlModel::data(const QModelIndex &index, int role) const
{
    int pk = rowToPk.value(index.row());

    return SqlHelper::selectMatchColumn(database,pk,roleIdToColumn.value(role));
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

void LiveMatchSqlModel::onCurrentRoundChanged(InternalMessages::RoundChangedData roundData,std::optional<InternalMessages::TournamentChangedData> tournamentData){
    beginResetModel();

    std::optional<int> gameNumber = std::nullopt;

    if(tournamentData){
        m_eventType = tournamentData.value().eventType;
        m_currentGameNumber = tournamentData.value().currentGame;
        if(m_currentGameNumber>0){
            gameNumber = std::optional<int>(m_currentGameNumber);
        }
    }
    else{
        m_currentGameNumber = 0;
    }

    m_gamesPerMatch = roundData.gamesPerMatch;
    m_currentPk = roundData.pk;

    updateData(m_currentPk,gameNumber);

    endResetModel();
    emit currentRoundLoaded();
}

void LiveMatchSqlModel::updateData(int roundPk, std::optional<int> gameNumber){
    QVector<int> pks = SqlHelper::selectMatchIds(database,roundPk,gameNumber);

    int i=0;
    rowToPk.clear();
    pkToRow.clear();
    for(auto p:pks){
        rowToPk.insert(i,p);
        pkToRow.insert(p,i);
        ++i;
    }
    m_rowCount = rowToPk.size();
}

int LiveMatchSqlModel::currentPk()
{
    return m_currentPk;
}
