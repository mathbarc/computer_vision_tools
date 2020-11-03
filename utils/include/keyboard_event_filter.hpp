#ifndef KEYBOARDEVENTFILTER_HPP
#define KEYBOARDEVENTFILTER_HPP

#include <QObject>

class KeyboardEventFilter : public QObject
{
    Q_OBJECT
    public:
        KeyboardEventFilter(QObject* parent = nullptr);

        bool eventFilter(QObject *watched, QEvent *event);


    signals:
        void capture();
        void finish();

};

#endif // KEYBOARDEVENTFILTER_HPP
