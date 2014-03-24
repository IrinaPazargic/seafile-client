#ifndef SEAFILE_CLIENT_UI_SEAFILE_TAB_BAR_H
#define SEAFILE_CLIENT_UI_SEAFILE_TAB_BAR_H

#include <QTabBar>

class QPaintEvent;
class QVBoxLayout;
class QStackedLayout;

/**
 * Custom tabbar used in the custom tab widget
 */ 
class SeafileTabBar : public QTabBar {
    Q_OBJECT
public:
    SeafileTabBar(QWidget *parent=0);

    void paintEvent(QPaintEvent *event);
};

/**
 * Custom tab widget, allow the tabbar to expand fully
 */ 
class SeafileTabWidget : public QWidget {
    Q_OBJECT
public:
    SeafileTabWidget(QWidget *parent=0);

    void addTab(QWidget *tab, const QIcon& icon, const QString& text);

    void adjustTabsWidth(int full_width);

    int currentIndex() const;

private:
    QVBoxLayout *layout_;
    
    SeafileTabBar *tabbar_;

    QWidget *pane_;

    QStackedLayout *stack_;
};


#endif // SEAFILE_CLIENT_UI_SEAFILE_TAB_BAR_H
