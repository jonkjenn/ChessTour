#ifndef ROUNDSSQLMODEL_H
#define ROUNDSSQLMODEL_H
#include <QtSql/QSqlTableModel>
#include <QHash>
#include <QSqlDatabase>
#include "internalmessages.h"
#include <optional>

class RoundsSqlModel:public QSqlTableModel
{
    Q_OBJECT
    Q_SIGNAL void roundDataChanged();
    //Q_PROPERTY(int currentTournamentPk READ currentTournamentPk NOTIFY currentTournamentPkChanged)
public:
    RoundsSqlModel(QObject *parent, QSqlDatabase database);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool select() override;

    int getRow(int primaryKey);
    int getPk(int row);
    //Q_INVOKABLE int currentShownRound();*/

    Q_INVOKABLE void setCurrentIndex(int index);
    Q_INVOKABLE int currentRoundIndex();

    void saveCurrentShownRound(int currentShownRound);
    //void forceRefresh();

    int currentTournamentPk() const;

    void onTournamentLoaded(InternalMessages::TournamentChangedData data);
    QVariant data(int row, QString columnName) const;
signals:
    //void currentTournamentPkChanged(int currentTournamentPk);
    void roundChanged(InternalMessages::RoundChangedData,std::optional<InternalMessages::TournamentChangedData> = std::nullopt);

private:
    QHash<int,int>rowFromPrimary;
    QSqlDatabase database;
    int m_currentTournamentPk = -1;
    int m_currentRoundIndex=0;
    int m_currentPk=-1;
};

#endif // ROUNDSSQLMODEL_H
