#include "tokencontainer.h"

TokenContainer::TokenContainer(QObject *parent,QTimer &timer, int maxTokens, int initial):QObject(parent), maxTokens(maxTokens), tokens(initial)
{
    if(timer.parent() == nullptr){
        timer.setParent(this);
    }
    setTokenAvailable(tokens>0);
    connect(&timer,&QTimer::timeout,this,&TokenContainer::timePassed);
}

bool TokenContainer::tokenAvailable() const
{
    return m_tokenAvailable;
}

///
/// \brief TokenContainer::getToken
/// \return
///
/// If there are any available tokens returns true and decrease the number of tokens.
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
