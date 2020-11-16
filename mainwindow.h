#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QtSql>
#include "sqlitedbeditor.h"

#include "teams.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<teams> resultTable = {};
    QVector<QVector<teams> > eachDay = {};

    void connectDatabase(QString);

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

    int numberOfTeams{}, backAndForth = -1, numberOfGamesPerDay{};

    QSqlDatabase db;

    QSqlQuery sqlCommand;
    QSqlQuery sqlCmd;

    QString headerStyle;

    QStringList teamNameList;

    QVector<QVector<QPair<int, int> > > matchSchedule;

private slots:
    void actDBEditorTriggered();
    void actAboutTriggered();
    void actOpenFileTriggered();
    void actSaveListTriggered();
    void showContinents();
    void showSubContinents();
    void showNatTeams();
    void LstContinentsCurrentRowChanged();
    void LstSubContsCurrentRowChanged();
    void CmdAddRandomTeamsClicked();
    void CmdNatTeamsModeClicked();
    void CmdClubsModeClicked();
    void CmdTeamSelectionClicked();
    void CmdUndoSelectedTeamClicked();
    void CmdSelectWholeSubCoClicked();
    //void CmdUndoWholeSubCoClicked();
    void CmdSelectWholeContClicked();
    //void CmdUndoWholeContClicked();
    void CmdClearAllClicked();

    void CmdMatchScheduleClicked();
    void CmdContinueClicked();
    void CmdBackClicked();

    void SpbUpdateTableReturnPressed();
};

#endif // MAINWINDOW_H
