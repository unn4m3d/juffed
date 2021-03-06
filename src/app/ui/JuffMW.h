/*
JuffEd - An advanced text editor
Copyright 2007-2010 Mikhail Murzin

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License 
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef __JUFFED_MAIN_WINDOW_H__
#define __JUFFED_MAIN_WINDOW_H__

class AboutDlg;
//class FindDlg;
class QHBoxLayout;
class QVBoxLayout;
class Popup;
class SearchPopup;

#include "Types.h"

#include <QMainWindow>

namespace Juff {
	class Document;
}

class JuffMW : public QMainWindow {
Q_OBJECT
public:
	JuffMW();

	void setMainWidget(QWidget*);

	QString getOpenFileName(const QString& dir, const QString& filters);
	QStringList getOpenFileNames(const QString& dir, const QString& filters);
	QString getSaveFileName(const QString& fileName, const QString& fileTitle, const QString& filters);
//	QString getSavePrjName(const QString& title);
	QString getRenameFileName(const QString& curFileName);
	int getGotoLineNumber(int);
	QString getJumpToFileName(const QStringList&);
	int askForSave(const QString& fileName);
	bool askForSave(const QStringList&, QStringList&);

	QString getOpenSessionName( bool& );

	// information display
	void addToolBar(QToolBar*);
	void addMenu(QMenu*);
	void insertMenu(QMenu* before, QMenu* menu);
	void message(const QIcon& icon, const QString& title, const QString& message, Qt::Alignment align = Qt::AlignTop | Qt::AlignRight, int timeout = 10);

	SearchPopup* searchPopup() const;
//	void showFindDialog(const QString&, bool replace = false);
//	void hideFindDialog();
//	void getSearchParams(Juff::SearchParams&);
//	bool searchPopupVisible() const;
//	void closeSearchPopup();

	bool isFullScreen() const;
	void toggleFullscreen();

	void applySettings();
	void saveState();
	void restoreState();

signals:
	void closeRequested(bool&);
//	void searchRequested(const Juff::SearchParams&);

public slots:
	void slotAbout();
	void slotAboutQt();
	void hideSearchPopup();

protected slots:
	void onPopupDismissed();
	void onSearchPopupOpened();
	void onSearchPopupClosed();

protected:
	virtual void closeEvent(QCloseEvent*);
    //virtual void resizeEvent(QResizeEvent*);
    //virtual void moveEvent(QMoveEvent*);
	virtual void keyPressEvent(QKeyEvent*);
	virtual bool eventFilter(QObject*, QEvent*);
    //virtual void changeEvent(QEvent*);

private:
	void updatePopupsPositions();

	QWidget* viewer_;
	QWidget* mainWidget_;
	AboutDlg* aboutDlg_;
//	FindDlg* findDlg_;
	QList<QToolBar*> allToolBars_;
	QList<QToolBar*> hiddenToolBars_;
	SearchPopup* searchPopup_;

	QList<Popup*> popups_;
};

#endif // __JUFFED_MAIN_WINDOW_H__
