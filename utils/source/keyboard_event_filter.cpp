#include "keyboard_event_filter.hpp"
#include <QEvent>
#include <QKeyEvent>

KeyboardEventFilter::KeyboardEventFilter(QObject* parent)
    : QObject(parent)
{

}

bool KeyboardEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_C) {
            emit capture();
            return true;
        }
        if (ke->key() == Qt::Key_Escape) {
            emit finish();
            return true;
        }
    }
    return false;
}
