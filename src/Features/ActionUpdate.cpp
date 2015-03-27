#include "ActionUpdate.h"

#include <QSettings>
#include <QApplication>
#include <QDir>
#include <QNetworkReply>

#include "UpdaterWindow.h"
#include "Partials/UpdateDownloadProgress.h"

#define FV_NEW_VERSION_POLICY_KEY  "FVNewVersionPolicy"

ActionUpdate::ActionUpdate(UpdaterWindow* window) : QObject(window),  d(window)
{

}

ActionUpdate::~ActionUpdate()
{

}


void ActionUpdate::SkipUpdate()
{
	qDebug() << "Skip update";

///	UpdateFileData* proposedUpdate = GetProposedUpdate();
///	if (! proposedUpdate) {
///		qWarning() << "Proposed update is NULL (shouldn't be at this point)";
///		return;
///	}

	// Start ignoring this particular version
///	FVIgnoredVersions::IgnoreVersion(proposedUpdate->getEnclosureVersion());

}

void ActionUpdate::RemindMeLater()
{
	// qDebug() << "Remind me later";

    // hide update window
}

void ActionUpdate::UpdateInstallationConfirmed()
{
//	qDebug() << "Confirm update installation";

///	UpdateFileData* proposedUpdate = GetProposedUpdate();
///	if (! proposedUpdate) {
///		qWarning() << "Proposed update is NULL (shouldn't be at this point)";
///		return;
///	}

///	// Open a link
///	if (! QDesktopServices::openUrl(proposedUpdate->getEnclosureUrl())) {
///		showErrorDialog(tr("Unable to open this link in a browser. Please do it manually."), true);
///		return;
///	}

	//hideUpdaterWindow();
	//hideUpdateConfirmationDialog();
}

void ActionUpdate::finishUpdate(QString pathToFinish)
{
	pathToFinish = pathToFinish.isEmpty() ? QCoreApplication::applicationDirPath() : pathToFinish;
	QDir appDir(pathToFinish);
	appDir.setFilter( QDir::Files | QDir::Dirs );

	QFileInfoList dirEntries = appDir.entryInfoList();
	foreach (QFileInfo fi, dirEntries)
	{
		if ( fi.isDir() )
		{
            QString dirname = fi.fileName();
            if ((dirname==".") || (dirname == ".."))
                continue;

			// recursive clean up subdirectory
			finishUpdate(fi.filePath());
		}
		else
		{
			if(fi.suffix()=="oldversion")
				if( !appDir.remove( fi.absoluteFilePath() ) )
					qDebug()<<"Error: Unable to clean up file: "<<fi.absoluteFilePath();

		}
	}	// For each dir entry END
}


void ActionUpdate::decideWhatToDoWithCurrentUpdateProposal()
{
    QSettings settings(QSettings::NativeFormat,
                       QSettings::UserScope,
                       QCoreApplication::organizationDomain(),
                       QCoreApplication::applicationName());

    QString policy = settings.value(FV_NEW_VERSION_POLICY_KEY).toString();
	if(policy == "install")
		InstallUpdate();
	else if(policy == "skip")
		SkipUpdate();
	else
		RemindMeLater();
}


void ActionUpdate::InstallUpdate()
{
	qDebug() << "Install update";
	if(d->manager()->parseUpdate()->getProposedUpdate()==NULL)
	{
		qWarning() << "Abort Update: No update prososed! This should not happen.";
		return;
	}

	//showUpdateConfirmationDialogUpdatedWithCurrentUpdateProposal();
	// Prepare download
	QUrl url = d->manager()->parseUpdate()->getProposedUpdate()->getEnclosureUrl();

	// Check SSL Fingerprint if required
	if(url.scheme()=="https" && !d->manager()->ssl()->m_requiredSslFingerprint.isEmpty())
		if( ! d->manager()->ssl()->checkSslFingerPrint(url) )	// check failed
		{
			qWarning() << "Update aborted.";
			return;
		}

    ///remove below
    QNetworkAccessManager m_qnam;

	// Start Download
	QNetworkReply* reply = m_qnam.get(QNetworkRequest(url));
	connect(reply, SIGNAL(finished()), this, SLOT(httpUpdateDownloadFinished()));

	// Maybe Check request 's return value
	if (reply->error() != QNetworkReply::NoError)
	{
		qDebug()<<"Unable to download the update: "<<reply->errorString();
		return;
	}
	else
		qDebug()<<"OK";


	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), d->updateDownloadProgress(), SLOT(downloadProgress(qint64, qint64) ));
	connect(&m_qnam, SIGNAL(finished(QNetworkReply*)), d->updateDownloadProgress(), SLOT(close()));
	//dlwindow->show();
	emit (updatedFinishedSuccessfully());

    //Don't hide show progress on widget itself...I mean window
}

