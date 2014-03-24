#ifndef SEAFILE_CLIENT_UI_ACTIVITIES_TAB_H
#define SEAFILE_CLIENT_UI_ACTIVITIES_TAB_H

#include "tab-view.h"

class QTimer;
class ApiError;

/**
 * The starred files tab
 */
class ActivitiesTab : public TabView {
    Q_OBJECT
public:
    explicit ActivitiesTab(QWidget *parent=0);

public slots:
    void refresh();
};

#endif // SEAFILE_CLIENT_UI_ACTIVITIES_TAB_H
