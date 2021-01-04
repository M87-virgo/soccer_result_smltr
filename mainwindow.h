/*
 * Copyright © 2020 beyer341@onlinehome.de
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QtSql>
#include <QtWidgets>
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

    QVector<Teams> resultTable = {};
    QVector<QVector<Teams> > eachDay = {};

    void setUpTableDialog();

    bool checkIfListsAreDifferent(QStringList);

    void connectDatabase(QString);

    QVector<QVector<QPair<int, int> > > fillAllMatchDays(const QVector<QPair<int, int> >&, const int&);
    void setUpTable(const int&);
    void setUpLblLegend();
    QVector<QVector<QPair<int, int> > > moreRounds(const QVector<QPair<int, int> >&);
    void fillTable(const QVector<QVector<QPair<int, int> > >&, const QVector<QPair<int, int> >&);
    QVector<Teams> prepareDirectMatchComparsion(QVector<Teams>&, const int&);
    void createMatchSchedule();
    void setHeaderForTable();
    void fillResultTable();

    void writeMatchData(QFile &f, const int &c);
    void readCsvMatchData(QString fileName);

    void showHomeTable();
    void showAwayTable();

private:
    Ui::MainWindow *ui;
    SqLiteDBEditor *meUi{};
    
    QDialog *DlgTableDialog = new QDialog;

    QTableWidget *TblTable = new QTableWidget;
    QTableWidget *TblMatchTable = new QTableWidget;
    QLabel *LblLegend = new QLabel;
    QComboBox *CmbMatches = new QComboBox;
    QPushButton *CmdBack = new QPushButton;
    QPushButton *CmdContinue = new QPushButton;
    QPushButton *CmdCloseDialog = new QPushButton;
    QPushButton *CmdHomeAwayTotal = new QPushButton;


    int numberOfTeams{}, backAndForth = -1, numberOfGamesPerDay{}, homeAwayTotal = 0;

    QSqlDatabase db;

    QSqlQuery sqlCommand;
    QSqlQuery sqlCmd;

    QString headerStyle;

    QStringList teamNameList;

    QVector<QVector<QPair<int, int> > > matchSchedule;

    QPixmap winImg;
    QPixmap lossImg;
    QPixmap drawImg;

private slots:
    void actDBEditorTriggered();
    void actAboutTriggered();
    void actOpenFileTriggered();
    void actSaveListTriggered();
    void actSaveMatchResultsTriggered();
    void showContinents();
    void showSubContinents();
    void showNatTeams();
    void LstSelectedTeamsItemChanged();
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

    void CmbMatchesCurrentIndexChanged();

    void CmdMatchScheduleClicked();
    void CmdFirstDayClicked();
    void CmdContinueClicked();
    void CmdBackClicked();
    void CmdHomeAwayTotalClicked();

    void SpbUpdateTableReturnPressed();
};

#endif // MAINWINDOW_H
