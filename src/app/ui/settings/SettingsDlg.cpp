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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "SettingsDlg.h"

//	Qt headers
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

//	Local headers
#ifdef Q_OS_WIN
#include "AppInfo.win.h"
#else
#include "AppInfo.h"
#endif

#include "ColorButton.h"
#include "IconManager.h"
#include "Log.h"
#include "MultiPage.h"
#include "MainSettings.h"
#include "../../PluginManager.h"
#include "PluginSettings.h"
#include "EditorSettings.h"
//#include "AutocompleteSettings.h"
//#include "PrintSettings.h"
#include "SettingsItem.h"

#include "ui_MainSettingsPage.h"

#include "SettingsCheckItem.h"
#include "SettingsSelectItem.h"

class MainSettingsPage : public QWidget {
public:
	MainSettingsPage() : QWidget() {
		ui.setupUi(this);
	}

	void init(QList<SettingsItem*>& items) {
		JUFFDEBUG("Initialization: main page");
//		int startupVariant = MainSettings::startupVariant();
//		switch (startupVariant) {
//			case 1:
//				ui.openLastSessionBtn->setChecked(true);
//				break;
//
//			case 2:
//				ui.openEmptySessionBtn->setChecked(true);
//				break;
//
//			case 0:
//			default:
//				ui.showSessionDlgBtn->setChecked(true);
//		}

		items
			  << new SettingsCheckItem("main", "exitOnLastDocClosed", ui.exitOnLastDocClosedChk)
			  << new SettingsCheckItem("main", "syncOpenDialogToCurDoc", ui.syncOpenDlgChk)
			  << new SettingsCheckItem("main", "makeBackupOnSave", ui.makeBackupChk)
			  << new SettingsCheckItem("main", "stripTrailingSpaces", ui.stripSpacesChk)
			  << new SettingsCheckItem("main", "singleInstance", ui.singleInstanceChk)
		;
#ifndef Q_OS_UNIX
//		ui.singleInstanceChk->hide();
#endif
	}

	Ui::MainSettingsPage ui;
};

#include "ui_ViewSettingsPage.h"

class ViewSettingsPage : public QWidget {
public:
	ViewSettingsPage() : QWidget() {
		ui.setupUi(this);
	}

	void init(QList<SettingsItem*>& items) {
		//	icon themes
		JUFFDEBUG("Initialization: icon theme");
//		QStringList themes = IconManager::instance()->themeList();
//		ui.iconThemeCmb->clear();
//		ui.iconThemeCmb->addItem("<default>");
//		ui.iconThemeCmb->addItems(themes);

		items << new SettingsSelectItem("main", "iconTheme", ui.iconThemeCmb, SettingsSelectItem::StringMode)
		      << new SettingsSelectItem("main", "toolButtonStyle", ui.toolButtonStyleCmb, SettingsSelectItem::IndexMode)
			  << new SettingsSelectItem("main", "iconSize", ui.iconSizeCmb, SettingsSelectItem::IndexMode)
			  << new SettingsSelectItem("main", "tabPosition", ui.tabPositionCmb, SettingsSelectItem::IndexMode)
		;
#if QT_VERSION >= 0x040500
		items << new SettingsCheckItem("main", "closeButtonsOnTabs", ui.closeBtnsChk);
#else
		ui.closeBtnsChk->hide();
#endif
	}
	
	Ui::ViewSettingsPage ui;
};

#include "ui_EditorSettingsPage.h"

class EditorSettingsPage : public QWidget {
public:
	EditorSettingsPage() : QWidget () {
		LOGGER;
	
		ui.setupUi(this);

		//	Creating ColorButton extensions. We shouldn't delete them
		//	manually 'cause they will be deleted automatically when their 
		//	parent buttons are deleted
		curLineColorBtn_ = new ColorButton(ui.curLineColorBtn, EditorSettings::get(EditorSettings::CurLineColor));
//		markersColorBtn_ = new ColorButton(ui.markerColorBtn, TextDocSettings::markersColor());
		fontColorBtn_ = new ColorButton(ui.fontColorBtn, EditorSettings::get(EditorSettings::DefaultFontColor));
		bgColorBtn_ = new ColorButton(ui.bgColorBtn, EditorSettings::get(EditorSettings::DefaultBgColor));
//		braceColorBtn_ = new ColorButton(ui.braceColorBtn, TextDocSettings::matchedBraceBgColor());
//		indentsColorBtn_ = new ColorButton(ui.indentColorBtn, TextDocSettings::indentsColor());
		selectionBgColorBtn_ = new ColorButton(ui.selectionBgColorBtn, EditorSettings::get(EditorSettings::SelectionBgColor));
	}
	
	void init(QList<SettingsItem*>& items) {
		Log::debug("Initialization: editor page");
		ui.fontCmb->setCurrentFont(EditorSettings::font());
		ui.fontSizeSpin->setValue(EditorSettings::font().pointSize());
		int chars = EditorSettings::get(EditorSettings::LineLengthIndicator);
		if (chars > 0) {
			ui.showLineLengthChk->setChecked(true);
			ui.lineLengthSpin->setValue(chars);
		}
		else {
			ui.showLineLengthChk->setChecked(false);
			ui.lineLengthSpin->setValue(-chars);
		}
		ui.tabStopWidthSpin->setValue(EditorSettings::get(EditorSettings::TabWidth));

		items << new SettingsCheckItem("editor", "highlightCurrentLine", ui.hlCurLineChk)
			  << new SettingsCheckItem("editor", "showIndents", ui.showIndentsChk)
			  << new SettingsCheckItem("editor", "replaceTabsWithSpaces", ui.replaceTabsChk)
			  << new SettingsCheckItem("editor", "backspaceUnindents", ui.unindentChk);
	}
	
	Ui::EditorSettingsPage ui;
	ColorButton* curLineColorBtn_;
	ColorButton* markersColorBtn_;
	ColorButton* fontColorBtn_;
	ColorButton* bgColorBtn_;
	ColorButton* braceColorBtn_;
	ColorButton* indentsColorBtn_;
	ColorButton* selectionBgColorBtn_;
};

//#include "ui_AutocompleteSettingsPage.h"

/*class AutocompleteSettingsPage : public QWidget {
public:
	AutocompleteSettingsPage() : QWidget() {
		ui.setupUi(this);
	}
	
	void init(QList<SettingsItem*>& items) {
		items << new SettingsCheckItem("autocomplete", "useDocument", ui.useDocumentChk)
			  << new SettingsCheckItem("autocomplete", "useApis", ui.useApiChk)
			  << new SettingsCheckItem("autocomplete", "replaceWord", ui.replaceWordChk)
			  << new SettingsCheckItem("autocomplete", "caseSensitive", ui.matchCaseChk);
		ui.thresholdSpin->setValue(AutocompleteSettings::threshold());
	}
	
	Ui::AutocompletePage ui;
};*/

class PrintingPage: public QWidget {
public:
	PrintingPage() : QWidget() {
		QVBoxLayout* vBox = new QVBoxLayout(this);
		keepColorsChk_  = new QCheckBox(QObject::tr("Keep syntax highlighting"));
		keepBgColorChk_ = new QCheckBox(QObject::tr("Keep background color"));
		alwaysWrapChk_  = new QCheckBox(QObject::tr("Always wrap text"));
		vBox->addWidget(keepColorsChk_);
		vBox->addWidget(keepBgColorChk_);
		vBox->addWidget(alwaysWrapChk_);
		vBox->addStretch();
		vBox->setMargin(0);
	}
	void init(QList<SettingsItem*>& items) {
		items << new SettingsCheckItem("printing", "keepColors", keepColorsChk_)
			  << new SettingsCheckItem("printing", "keepBgColor", keepBgColorChk_)
			  << new SettingsCheckItem("printing", "alwaysWrap", alwaysWrapChk_);
	}
	QCheckBox* keepColorsChk_;
	QCheckBox* keepBgColorChk_;
	QCheckBox* alwaysWrapChk_;
};

//#include "FileTypesPage.h"
#include "CharsetsSettingsPage.h"
#include "PluginPage.h"
#include "JuffPlugin.h"

/////////////////////////////////////////////////////////////

SettingsDlg::SettingsDlg(QWidget* parent) : QDialog(parent) {
	LOGGER;

	setWindowTitle(tr("Settings"));
	setMinimumSize(600, 400);

	//	create buttons
	okBtn_ = new QPushButton(tr("OK"), this);
	applyBtn_ = new QPushButton(tr("Apply"), this);
	cancelBtn_ = new QPushButton(tr("Cancel"), this);
	connect(okBtn_, SIGNAL(clicked()), SLOT(ok()));
	connect(applyBtn_, SIGNAL(clicked()), SLOT(apply()));
	connect(cancelBtn_, SIGNAL(clicked()), SLOT(reject()));
	//	create multipage
	mp_ = new MultiPage();
	pageMain_ = new MainSettingsPage();
	pageView_ = new ViewSettingsPage();
	pageEditor_ = new EditorSettingsPage();
	pageCharsets_ = new CharsetsSettingsPage();
//	pageAC_ = new AutocompleteSettingsPage();
//	fileTypesPage_ = new FileTypesPage();
	printingPage_ = new PrintingPage();
	mp_->addPage(tr("General"), pageMain_);
	mp_->addPage(tr("View"), pageView_);
	mp_->addPage(tr("Editor"), pageEditor_);
//	mp_->addPage(tr("Autocompletion"), pageAC_);
	mp_->addPage(tr("Charsets"), pageCharsets_);
//	mp_->addPage(tr("File types"), fileTypesPage_);
	mp_->addPage(tr("Printing"), printingPage_);

	//	plugins
	pluginsMainPage_ = new QWidget();
	mp_->addPage(tr("Plugins"), pluginsMainPage_);

	// layouts
	QHBoxLayout* btnLayout = new QHBoxLayout();
	btnLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
	btnLayout->addWidget(okBtn_);
	btnLayout->addWidget(applyBtn_);
	btnLayout->addWidget(cancelBtn_);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(mp_);
	mainLayout->addLayout(btnLayout);
	setLayout(mainLayout);
	
//	init();
}

SettingsDlg::~SettingsDlg() {
	items_.clear();
	delete pageCharsets_;
	delete pageEditor_;
	delete pageMain_;
//	delete pageAC_;
//	delete fileTypesPage_;
	delete pageView_;
	delete pluginsMainPage_;
	delete mp_;
}

void SettingsDlg::init() {
	LOGGER;
	
	if ( SettingsItem::notifier() != 0 )
		connect(SettingsItem::notifier(), SIGNAL(hasChangedItems(bool)), SLOT(somethingChanged(bool)));

	pageMain_->init(items_);
	pageView_->init(items_);
	pageEditor_->init(items_);
//	pageAC_->init(items_);
	printingPage_->init(items_);
	
	//	charsets page
	pageCharsets_->init();
}

void SettingsDlg::addPluginSettingsPage(const QString& name, QWidget* page) {
	PluginPage* plPage = new PluginPage(name, page);
	mp_->addChildPage(tr("Plugins"), name, plPage);
	pluginPages_[name] = plPage;
	plPage->enablePage(PluginSettings::pluginEnabled(name));
}

int SettingsDlg::exec() {
	init();
	return QDialog::exec();
}

void SettingsDlg::apply() {
	foreach (SettingsItem* sItem, items_) {
		sItem->writeValue();
	}

	//	Editor page
	QFont font(pageEditor_->ui.fontCmb->currentFont());
	font.setPointSize(pageEditor_->ui.fontSizeSpin->value());
	EditorSettings::setFont(font);

	if ( pageEditor_->ui.showLineLengthChk->isChecked() ) {
		EditorSettings::set(EditorSettings::LineLengthIndicator, pageEditor_->ui.lineLengthSpin->value());
	}
	else {
		EditorSettings::set(EditorSettings::LineLengthIndicator, -pageEditor_->ui.lineLengthSpin->value());
	}
	// colors
//	TextDocSettings::setMarkersColor(pageEditor_->markersColorBtn_->color());
	EditorSettings::set(EditorSettings::CurLineColor, pageEditor_->curLineColorBtn_->color());
	EditorSettings::set(EditorSettings::DefaultFontColor, pageEditor_->fontColorBtn_->color());
	EditorSettings::set(EditorSettings::DefaultBgColor, pageEditor_->bgColorBtn_->color());
//	TextDocSettings::setMatchedBraceBgColor(pageEditor_->braceColorBtn_->color());
//	TextDocSettings::setIndentsColor(pageEditor_->indentsColorBtn_->color());
	EditorSettings::set(EditorSettings::SelectionBgColor, pageEditor_->selectionBgColorBtn_->color());

	EditorSettings::set(EditorSettings::TabWidth, pageEditor_->ui.tabStopWidthSpin->value());

	//	Autocomplete
//	AutocompleteSettings::setThreshold(pageAC_->ui.thresholdSpin->value());

	//	charsets
	pageCharsets_->applySettings();

	//	plugins
	QStringList plugins = pluginPages_.keys();
	foreach (QString plName, plugins) {
		Log::debug(plName);
		PluginPage* page = pluginPages_[plName];
		if ( page ) {
			Log::debug(QString("Plugin '%1' was %2").arg(plName).arg( page->pageEnabled() ? "ENABLED" : "DISABLED" ));
			PluginSettings::setPluginEnabled(plName, page->pageEnabled());
		}
	}

//	fileTypesPage_->apply();

	emit applied();
}

void SettingsDlg::ok() {
	apply();
	accept();
}

bool SettingsDlg::isPluginEnabled(const QString& pluginName) {
	PluginPage* page = pluginPages_.value(pluginName, 0);
	if (page != 0)
		return page->pageEnabled();
	else
		return false;
}


void SettingsDlg::somethingChanged(bool changed) {
//	JUFFWARNING(QString("Something is changed: ").arg(changed));
}