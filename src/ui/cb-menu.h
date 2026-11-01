#pragma once

#include <obs-frontend-api.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QString>

// Tools > #cbOBScripts. VST Ex and VST Enigmax Loader are separate modules that load in either order, so the menu is
// looked up by object name and items are kept sorted by their order number.
inline QMenu *cb_menu()
{
	auto *main = static_cast<QMainWindow *>(obs_frontend_get_main_window());
	auto *menu = main->findChild<QMenu *>("cbOBScriptsMenu");
	if (menu)
		return menu;
	menu = new QMenu(main);
	menu->setObjectName("cbOBScriptsMenu");
	static_cast<QAction *>(obs_frontend_add_tools_menu_qaction("#cbOBScripts"))->setMenu(menu);
	return menu;
}

inline void cb_menu_insert(QAction *action, int order)
{
	QMenu *menu = cb_menu();
	action->setProperty("cbOrder", order);
	QAction *before = nullptr;
	for (QAction *a : menu->actions()) {
		if (a->property("cbOrder").toInt() > order) {
			before = a;
			break;
		}
	}
	menu->insertAction(before, action);
}

template<class Fn> inline void cb_menu_add(int order, const QString &text, Fn fn)
{
	auto *action = new QAction(text, cb_menu());
	QObject::connect(action, &QAction::triggered, action, fn);
	cb_menu_insert(action, order);
}
