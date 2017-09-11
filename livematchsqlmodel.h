#ifndef LIVEMATCHSQLMODEL_H
#define LIVEMATCHSQLMODEL_H
#include <QAbstractListModel>
#include <QtSql/QSqlDatabase>
#include <optional>
#include "internalmessages.h"

class LiveMatchSqlModel:public QAbstractListModel
{
    Q_OBJECT
public:
    LiveMatchSqlModel(QObject *parent, QSqlDatabase &database);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(int row, QString column, const QVariant &value);
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    QHash<int, QByteArray> roleNames() const override;

    int currentRoundPk();
    Q_INVOKABLE QString eventType() const;
    Q_INVOKABLE int gamesPerMatch() const;
    Q_INVOKABLE void setCurrentGameNumber(int gameNumber);
    Q_INVOKABLE int currentGameNumber();

    void updateData(int roundPk, std::optional<int> gameNumber=std::nullopt);
public slots:
    void onCurrentRoundChanged(InternalMessages::RoundChangedData roundData, std::optional<InternalMessages::TournamentChangedData> tournamentData);
    void possibleUpdates(int tournamentPk, const QVariantMap &map, const QVariantMap &matchPks);

private:
    int rowFromMatchAndGameNumber(int matchNumber, int gameNumber);
    QSqlDatabase &database;
    int m_rowCount = 0;
    int m_currentRoundPk = -1;
    QString m_eventType;
    QHash<int, QByteArray> roles;
    QHash<int, QString> roleIdToColumn;
    QHash<QString, int> columnToRoleId;
    QHash<int,int> rowToPk;
    QHash<int,int> pkToRow;
    int m_currentGameNumber = 0;
    int m_gamesPerMatch = 1;
    int m_currentTournamentPk = -1;

signals:
    void currentRoundLoaded();
};

#endif // LIVEMATCHSQLMODEL_H
