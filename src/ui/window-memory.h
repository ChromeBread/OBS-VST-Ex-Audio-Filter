#pragma once

#include "plugin-meta.h"

#include <QByteArray>
#include <QEvent>
#include <QObject>
#include <QString>
#include <QWidget>

// Restores a window's size and position, and saves them when it hides. Qt moves a window back on screen if the
// monitor it was on is gone.
class WindowMemory : public QObject {
public:
	WindowMemory(QWidget *window, const QString &key) : QObject(window), key(key)
	{
		QString saved = PluginMeta::instance().uiValue(key).toString();
		if (!saved.isEmpty())
			window->restoreGeometry(QByteArray::fromBase64(saved.toLatin1()));
		window->installEventFilter(this);
	}

protected:
	bool eventFilter(QObject *obj, QEvent *event) override
	{
		if (event->type() == QEvent::Hide) {
			QByteArray geometry = static_cast<QWidget *>(obj)->saveGeometry().toBase64();
			PluginMeta::instance().setUiValue(key, QString::fromLatin1(geometry));
		}
		return false;
	}

private:
	QString key;
};
