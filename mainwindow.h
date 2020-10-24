#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QtSql>
#include "sqlitedbeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<QVector<QPair<int, int> > > fillAllMatchDays(QVector<QPair<int, int> >, const int&);
    void setUpTable(const int&);
    QVector<QVector<QPair<int, int> > > moreRounds(QVector<QPair<int, int> >);
    void fillTable(QVector<QVector<QPair<int, int> > >, QVector<QPair<int, int> >);
    void createMatchSchedule();
    void setHeaderForTable();
    void fillResultTable();

private:
    Ui::MainWindow *ui;
    SqLiteDBEditor *meUi{};

    int numberOfTeams{};

    QSqlDatabase db;

    QSqlQuery sqlCommand;

    QString headerStyle;

    QStringList teamNameList;

    QVector<QVector<QPair<int, int> > > matchSchedule;

private slots:
    void actDBEditorTriggered();
    void actAboutTriggered();
    void showContinents();
    void showSubContinents();
    void showNatTeams();
    void LstContinentsCurrentRowChanged();
    void LstSubContsCurrentRowChanged();
    void CmdNatTeamsModeClicked();
    void CmdClubsModeClicked();
    void CmdDBConClicked();
    void CmdTeamSelectionClicked();
    void CmdUndoSelectedTeamClicked();
    void CmdSelectWholeSubCoClicked();
    //void CmdUndoWholeSubCoClicked();
    void CmdSelectWholeContClicked();
    //void CmdUndoWholeContClicked();
    void CmdClearAllClicked();

    void CmdMatchScheduleClicked();

    void SpbUpdateTableEditingFinished();
};

#endif // MAINWINDOW_H
