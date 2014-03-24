#include <QtGui>
#include <QTimer>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileInfo>
#include <QIcon>

#include "seafile-applet.h"
#include "account-mgr.h"
#include "api/requests.h"
#include "api/starred-file.h"

#include "starred-files-tab.h"

namespace {

const int kRefreshInterval = 1000 * 60 * 5; // 5 min
const char *kLoadingFaieldLabelName = "loadingFailedText";

enum {
    INDEX_LOADING_VIEW = 0,
    INDEX_LOADING_FAILED_VIEW,
    INDEX_FILES_VIEW
};

}

StarredFilesTab::StarredFilesTab(QWidget *parent)
    : TabView(parent),
      in_refresh_(false)
{
    createStarredFilesListView();
    createLoadingView();
    createLoadingFailedView();

    mStack->insertWidget(INDEX_LOADING_VIEW, loading_view_);
    mStack->insertWidget(INDEX_LOADING_FAILED_VIEW, loading_failed_view_);
    mStack->insertWidget(INDEX_FILES_VIEW, files_list_widget_);

    refresh_timer_ = new QTimer(this);
    connect(refresh_timer_, SIGNAL(timeout()), this, SLOT(refresh()));

    get_starred_files_req_ = NULL;

    refresh();
}

void StarredFilesTab::createStarredFilesListView()
{
    files_list_widget_ = new QListWidget;
}

void StarredFilesTab::createLoadingView()
{
    loading_view_ = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout;
    loading_view_->setLayout(layout);

    QMovie *gif = new QMovie(":/images/loading.gif");
    QLabel *label = new QLabel;
    label->setMovie(gif);
    label->setAlignment(Qt::AlignCenter);
    gif->start();

    layout->addWidget(label);
}

void StarredFilesTab::createLoadingFailedView()
{
    loading_failed_view_ = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout;
    loading_failed_view_->setLayout(layout);

    QLabel *label = new QLabel;
    label->setObjectName(kLoadingFaieldLabelName);
    QString link = QString("<a style=\"color:#777\" href=\"#\">%1</a>").arg(tr("retry"));
    QString label_text = tr("Failed to get starred files information<br/>"
                            "Please %1").arg(link);
    label->setText(label_text);
    label->setAlignment(Qt::AlignCenter);

    connect(label, SIGNAL(linkActivated(const QString&)),
            this, SLOT(refresh()));
    label->installEventFilter(this);

    layout->addWidget(label);
}

void StarredFilesTab::refresh()
{
    if (in_refresh_) {
        return;
    }

    showLoadingView();
    AccountManager *account_mgr = seafApplet->accountManager();

    const std::vector<Account>& accounts = seafApplet->accountManager()->accounts();
    if (accounts.empty()) {
        return;
    }

    in_refresh_ = true;

    if (get_starred_files_req_) {
        delete get_starred_files_req_;
    }

    get_starred_files_req_ = new GetStarredFilesRequest(accounts[0]);
    connect(get_starred_files_req_, SIGNAL(success(const std::vector<StarredFile>&)),
            this, SLOT(refreshStarredFiles(const std::vector<StarredFile>&)));
    connect(get_starred_files_req_, SIGNAL(failed(const ApiError&)),
            this, SLOT(refreshStarredFilesFailed(const ApiError&)));
    get_starred_files_req_->send();
}

void StarredFilesTab::refreshStarredFiles(const std::vector<StarredFile>& files)
{
    in_refresh_ = false;

    get_starred_files_req_->deleteLater();
    get_starred_files_req_ = NULL;

    files_list_widget_->clear();
    for (int i = 0, n = files.size(); i < n; i++) {
        StarredFile file = files[i];
        QIcon icon(":/images/seafile.png"); 
        QString name = QFileInfo(file.path).fileName();
        QListWidgetItem *item = new QListWidgetItem(icon, name);
        files_list_widget_->addItem(item);
    }

    mStack->setCurrentIndex(INDEX_FILES_VIEW);
}

void StarredFilesTab::refreshStarredFilesFailed(const ApiError& error)
{
    qDebug("failed to refresh starred files");
    in_refresh_ = false;

    if (mStack->currentIndex() == INDEX_LOADING_VIEW) {
        mStack->setCurrentIndex(INDEX_LOADING_FAILED_VIEW);
    }
}

void StarredFilesTab::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    refresh_timer_->start(kRefreshInterval);
}

void StarredFilesTab::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
    // TODO
    refresh_timer_->stop();
}

void StarredFilesTab::showLoadingView()
{
    QStackedLayout *stack = (QStackedLayout *)(layout());
    mStack->setCurrentIndex(INDEX_LOADING_VIEW);
}
