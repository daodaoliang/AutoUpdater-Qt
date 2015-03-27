#include "UpdaterWindow.h"
#include "ui_UpdaterWindow.h"

#include "fvupdater.h"
#include "UpdateFileData/UpdateFileData.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>

#ifdef Q_WS_MAC
#include "CoreFoundation/CoreFoundation.h"
#endif

UpdaterWindow::UpdaterWindow(QWidget *parent, bool skipVersionAllowed, bool remindLaterAllowed) :
	QWidget(parent),
    m_ui(new Ui::UpdaterWindow)
{
	m_ui->setupUi(this);

	m_appIconScene = 0;

	if(!skipVersionAllowed)
		m_ui->skipThisVersionButton->hide();
	if(!remindLaterAllowed)
		m_ui->remindMeLaterButton->hide();

	// Delete on close
	setAttribute(Qt::WA_DeleteOnClose, true);

	// Set the "new version is available" string
	QString newVersString = m_ui->newVersionIsAvailableLabel->text().arg(QApplication::applicationName());
	m_ui->newVersionIsAvailableLabel->setText(newVersString);

    _baseManager = new BaseManager(this);


    // Connect buttons
	connect(m_ui->installUpdateButton, SIGNAL(clicked()),
			_baseManager->actionUpdate() , SLOT(InstallUpdate()));
	connect(m_ui->skipThisVersionButton, SIGNAL(clicked()),
			_baseManager->actionUpdate(), SLOT(SkipUpdate()));
	connect(m_ui->remindMeLaterButton, SIGNAL(clicked()),
            _baseManager->actionUpdate(), SLOT(RemindMeLater()));


}

UpdaterWindow::~UpdaterWindow()
{
//	m_ui->releaseNotesWebView->stop();
	delete m_ui;
}

bool UpdaterWindow::UpdateWindowWithCurrentProposedUpdate()
{
	///UpdateFileData* proposedUpdate = FvUpdater::sharedUpdater()->GetProposedUpdate();
	///if (! proposedUpdate) {
	///	return false;
	///}

	///QString downloadString = m_ui->wouldYouLikeToDownloadLabel->text()
///			.arg(QApplication::applicationName(), proposedUpdate->getEnclosureVersion(), QApplication::applicationVersion());
	///m_ui->wouldYouLikeToDownloadLabel->setText(downloadString);

    ///Get change notes from download link and set it inside
    ///m_ui->releaseNotes

//	m_ui->releaseNotesWebView->stop();
//	m_ui->releaseNotesWebView->load(proposedUpdate->GetReleaseNotesLink());

	return true;
}

void UpdaterWindow::closeEvent(QCloseEvent* event)
{
	///FvUpdater::sharedUpdater()->updaterWindowWasClosed();
	event->accept();
}

BaseManager* UpdaterWindow::manager()
{
    return _baseManager;
}

UpdateDownloadProgress* UpdaterWindow::updateDownloadProgress()
{
    return m_ui->updateDownloadProgress;
}

/// Handler
/// Include 	This in handler before shown
///
/// /// FIX with manager Create a new window
///m_updaterWindow = new UpdaterWindow(NULL, skipVersionAllowed, remindLaterAllowed);
///m_updaterWindow->UpdateWindowWithCurrentProposedUpdate();
///m_updaterWindow->show();

