#include "ccompleter.h"
#include <QAbstractItemView>

CCompleter::CCompleter()
{
}

CCompleter::CCompleter(const QStringList& words, QObject * parent) :
QCompleter(parent), m_list(words), m_model()
{
    setModel(&m_model);
    popup()->setFont(QFont("Segoe UI", 13));
}

void CCompleter::update(QString word)
{
    QStringList filtered = m_list.filter(word, caseSensitivity());
    m_model.setStringList(m_list);
    m_word = word;
    complete();
}

QString CCompleter::word()
{
    return m_word;
}
