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

void LiveMatchSqlModel::possibleUpdates(const QVariantList &rounds){
    for(auto round:rounds){
        for(auto game:round.toList()){
        QVariantList gameData = game.toList();
        if(gameData.at(0).toInt() == currentRoundPk()){
            //int row = pkToRow.value(gameData.at(1).toInt());
            //int row = 0;//rowFromMatchAndGameNumber(gameData.value(1).toInt(),gameData.value(2).toInt())
            QVariant matchPk = SqlHelper::selectWhere(database,"Match","Id",{
                                                 {"RoundId",currentRoundPk()},
                                                 {"Number",gameData.at(1).toInt()},
                                                 {"GameNumber",gameData.at(2).toInt()}
                                             });
            if(!matchPk.isValid()){qDebug() << "Could not find pk"; continue;}
            int row = pkToRow.value(matchPk.toInt());
            QVector<int> roles;
            for(int i=1;i<gameData.size();++i){
                auto role = gameData.at(i).toString();
                if(columnToRoleId.contains(role)){
                    roles.append(columnToRoleId.value(role));
                    if(role == "EngineScore"){
                        //Since we have dont have access to the currently shown EngineScore in the view,
                        //a TempEngineScore is used to maintain the columns for the current EngineScore
                        // and PreviousEngineScore. Same for engine mate.

                        //PreviousScore << TempScore
                        SqlHelper::updateTable(database,"Match",
                        {{"PreviousEngineScore",data(index(row),columnToRoleId.value("TempEngineScore"))}}
                                               ,{{"Id",matchPk.toInt()}});

                        //TempScore << (new)EngineScore
                        SqlHelper::updateTable(database,"Match",
                        {{"TempEngineScore",data(index(row),columnToRoleId.value("EngineScore"))}}
                                               ,{{"Id",matchPk.toInt()}});

                        roles.append(columnToRoleId.value("PreviousEngineScore"));
                    }
                    if(role == columnToRoleId.value("EngineMate")){

                        //PreviousScore << TempScore
                        SqlHelper::updateTable(database,"Match",
                        {{"PreviousEngineMate",data(index(row),columnToRoleId.value("TempEngineMate"))}}
                                               ,{{"Id",matchPk.toInt()}});

                        //TempScore << (new)EngineScore
                        SqlHelper::updateTable(database,"Match",
                        {{"TempEngineMate",data(index(row),columnToRoleId.value("EngineMate"))}}
                                               ,{{"Id",matchPk.toInt()}});

                        roles.append(columnToRoleId.value("PreviousEngineMate"));
                    }
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
