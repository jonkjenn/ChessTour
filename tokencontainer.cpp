#include "tokencontainer.h"

TokenContainer::TokenContainer(QObject *parent,QTimer &timer, int maxTokens, int initial):QObject(parent), maxTokens(maxTokens), tokens(initial)
{
    setTokenAvailable(tokens>0);
    connect(&timer,&QTimer::timeout,this,&TokenContainer::timePassed);
}

bool TokenContainer::tokenAvailable() const
{
    return m_tokenAvailable;
}

bool TokenContainer::getToken()
{
    if(tokens>0){
        tokens -= 1;
        setTokenAvailable(tokens>0);
        return true;
    }
    return false;
}

void TokenContainer::timePassed()
{
    if(tokens < maxTokens){
        tokens = tokens+1;
        setTokenAvailable(tokens>0);
    }
}

void TokenContainer::setTokenAvailable(bool tokenAvailable)
{
    if (m_tokenAvailable == tokenAvailable)
        return;

    m_tokenAvailable = tokenAvailable;
    emit notifyTokenAvailableChanged(m_tokenAvailable);
}
