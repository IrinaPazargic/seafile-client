#ifndef SEAFILE_CLIENT_UI_STARRED_FILES_TAB_H
#define SEAFILE_CLIENT_UI_STARRED_FILES_TAB_H

#include "tab-view.h"

class QTimer;
class QListWidget;

class StarredFile;
class GetStarredFilesRequest;
class ApiError;

/**
 * The starred files tab
 */
class StarredFilesTab : public TabView {
    Q_OBJECT
public:
    explicit StarredFilesTab(QWidget *parent=0);

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

public slots:
    void refresh();

private slots:
    void refreshStarredFiles(const std::vector<StarredFile>& files);
    void refreshStarredFilesFailed(const ApiError& error);

private:
    void createStarredFilesListView();
    void createLoadingView();
    void createLoadingFailedView();
    void showLoadingView();

    QTimer *refresh_timer_;
    bool in_refresh_;

    QListWidget *files_list_widget_;
    QWidget *loading_view_;
    QWidget *loading_failed_view_;

    GetStarredFilesRequest *get_starred_files_req_;
};

#endif // SEAFILE_CLIENT_UI_STARRED_FILES_TAB_H
