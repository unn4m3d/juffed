/*
JuffEd - A simple text editor
Copyright 2007-2008 Mikhail Murzin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License 
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _JUFFED_PLUGIN_INTERFACE_H_
#define _JUFFED_PLUGIN_INTERFACE_H_

class QAction;
class QObject;
class QMenu;
class QToolBar;
class QWidget;

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <Qt>

typedef QList<QAction*> ActionList;
typedef QMap<QString, QVariant> SettingsMap;

class JuffPlugin {
public:
	virtual ~JuffPlugin() { qDebug("Plugin deleted"); }
	//	initialization
	virtual void setHandler(QObject* handler) = 0;

	//	deinitialization
	virtual void deinit() = 0;

	//	info
	virtual QString name() const = 0;
	virtual QString description() const = 0;

	//	controls
	virtual ActionList getMenuActions(const QString& menuName) const = 0;
	virtual QToolBar* toolBar() const = 0;
	virtual QMenu* menu() const = 0;
	virtual QWidget* settingsPage() const = 0;
	virtual void applySettings() = 0;
	virtual QWidget* dockWidget(Qt::DockWidgetArea& area) const = 0;


	SettingsMap settings() const {
		return settings_;
	}
	void setSettings(const SettingsMap& settingsMap) {
		settings_ = settingsMap;
	}

protected:
	bool getBoolValue(const QString& name, bool defValue = false) {
		return settings_.value(name, QVariant(defValue)).toBool();
	}
	int getIntValue(const QString& name, int defValue = 0) {
		return settings_.value(name, QVariant(defValue)).toInt();
	}
	double getDoubleValue(const QString& name, double defValue) {
		return settings_.value(name, QVariant(defValue)).toDouble();
	}
	QString getStringValue(const QString& name, const QString& defValue = "") {
		return settings_.value(name, QVariant(defValue)).toString();
	}

	void setBoolValue(const QString& name, bool value) {
		settings_[name] = QVariant(value);
	}
	void setIntValue(const QString& name, int value) {
		settings_[name] = QVariant(value);
	}
	void setDoubleValue(const QString& name, double value) {
		settings_[name] = QVariant(value);
	}
	void setStringValue(const QString& name, const QString& value) {
		settings_[name] = QVariant(value);
	}

private:
	SettingsMap settings_;
};

Q_DECLARE_INTERFACE(JuffPlugin, "JuffEd.JuffPlugin/1.0")

#endif