#include "chesshelper.h"
#include "livematchsqlmodel.h"
#include "sqlhelper.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

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

//TODO: Improve this mess
void LiveMatchSqlModel::possibleUpdates(int tournamentPk, const QVariantMap &rounds, const QVariantMap &matchPks){
    if(m_currentTournamentPk != tournamentPk){return;}

    //If the round we're showing gets updates
    if(!rounds.keys().contains(QString::number(currentRoundPk()))){return;}
    QVariantList round = rounds.value(QString::number(currentRoundPk())).toList();
    QVariantList pks = matchPks.value(QString::number(currentRoundPk())).toList();
    Q_ASSERT(round.size() == pks.size());
    for(int i=0;i<round.size();++i){
        QVariantList gameData = round.at(i).toList();
            /*QVariant matchPk = SqlHelper::selectWhere(database,"Match","Id",{
                                                          {"RoundId",currentRoundPk()},
                                                          {"Number",gameData.at(1).toInt()},
                                                          {"GameNumber",gameData.at(2).toInt()}
                                                      });
            if(!matchPk.isValid()){qDebug() << "Could not find pk"; continue;}*/
            int row = pkToRow.value(pks.at(i).toInt());
            QVector<int> roles;
            for(int i=1;i<gameData.size();++i){
                auto role = gameData.at(i).toString();
                if(columnToRoleId.contains(role)){
                    roles.append(columnToRoleId.value(role));
                }
            }
            emit dataChanged(index(row),index(row),roles);
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
    updateData(m_currentRoundPk,m_currentGameNumber);
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

    if(role == columnToRoleId.value("GamePosition")
            || role == columnToRoleId.value("LastMoves")){
        return QJsonDocument::fromJson(SqlHelper::selectMatchColumn(database,pk,roleIdToColumn.value(role)).toString().toUtf8()).array();
    }
    return SqlHelper::selectMatchColumn(database,pk,roleIdToColumn.value(role));
}

bool LiveMatchSqlModel::setData(int row, QString column, const QVariant &value){
    return setData(index(row),value,columnToRoleId.value(column));
}

bool LiveMatchSqlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int pk = rowToPk.value(index.row());
    //qDebug() << "Update Match SET " + roleIdToColumn.value(role) + " = " + value.toString() + "WHERE Id = " + QString::number(pk);
    SqlHelper::updateTable(database,"Match",{{roleIdToColumn.value(role),value}},{{"Id",pk}});
    /*q.prepare("UPDATE Match SET " + roleIdToColumn.value(role) + " = :value WHERE Id = :id");
    //q.bindValue(":column",roleIdToColumn.value(role));
    q.bindValue(":value",value);
    q.bindValue(":id",pk);
    if(!q.exec()){qDebug() << q.lastError();}*/
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
        m_currentTournamentPk = tournamentData.value().pk;
    }
    else{
        m_currentGameNumber = 0;
    }

    m_gamesPerMatch = roundData.gamesPerMatch;
    m_currentRoundPk = roundData.pk;

    updateData(m_currentRoundPk,gameNumber);

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

int LiveMatchSqlModel::currentRoundPk()
{
    return m_currentRoundPk;
}
