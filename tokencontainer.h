#ifndef TOKENCONTAINER_H
#define TOKENCONTAINER_H
#include <QObject>
#include <QTimer>

class TokenContainer:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool tokenAvailable READ tokenAvailable NOTIFY notifyTokenAvailableChanged)
public:
    TokenContainer(QObject *parent, QTimer &timer, int maxTokens, int initial=0);
    bool tokenAvailable() const;
    bool getToken();

signals:
    void notifyTokenAvailableChanged(bool tokenAvailable);

private:
    void setTokenAvailable(bool tokenAvailable);
    int maxTokens = 0;
    int tokens = 0;
    bool m_tokenAvailable = false;
    void timePassed();
};

#endif // TOKENCONTAINER_H
