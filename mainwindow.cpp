/*
 * Copyright © 2020 beyer341@onlinehome.de
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>
#include <QtAlgorithms>
#include <QFileDialog>
#include <chrono>
#include <random>

#include "teams.cpp"

void MainWindow::CmbMatchesCurrentIndexChanged()
{
    for(int i = 0; i < TblMatchTable -> rowCount(); ++i)
    {
        if(!TblMatchTable -> cellWidget(i, 0) )
        { 
            if(CmbMatches -> currentText() == QString("%1 vs. %2").arg(TblMatchTable -> item(i, 0) -> text() ).arg(TblMatchTable -> item(i, 3) -> text() ) )
            {
                TblMatchTable -> setSelectionMode(QAbstractItemView::ContiguousSelection);
                TblMatchTable -> setSelectionBehavior(QAbstractItemView::SelectRows);
                TblMatchTable -> selectRow(i);
            }
        }
    }
}

//match schedule functions
QVector<QVector<QPair<int, int> > > MainWindow::fillAllMatchDays(const QVector<QPair<int, int> > &numOfGamesPerDay, const int &n)
{
    for(int i = 1; i < matchSchedule.size(); ++i)
    {
        if(i % 2 != 0)
        {
            matchSchedule[i][0].first = n;
        }
        else
        {
            matchSchedule[i][0].second = n;
        }
    }

    for(int j = 1; j < matchSchedule.size(); ++j)
    {
        for (int i = 1; i < numOfGamesPerDay.size(); ++i)
        {
             matchSchedule[j][i].first = matchSchedule[j - 1][i].first + (n/2);
             matchSchedule[j][i].second = matchSchedule[j - 1][i].second + (n/2);
             if( (matchSchedule[j - 1][i].first + (n/2) ) > matchSchedule.size() )
             {
                 matchSchedule[j][i].first = matchSchedule[j - 1][i].first - ( (n/2) - 1);
             }

             if( (matchSchedule[j - 1][i].second + (n/2) ) > matchSchedule.size() )
             {
                 matchSchedule[j][i].second = matchSchedule[j - 1][i].second - ( (n/2) - 1);
             }
        }
        if(j % 2 != 0)
        {
            matchSchedule[j][0].second = matchSchedule[j - 1][ numOfGamesPerDay.size() - 1 ].second;
        }
        else
        {
            matchSchedule[j][0].first = matchSchedule[j - 1][ numOfGamesPerDay.size() - 1 ].second;
        }
    }
    return matchSchedule;
}
QVector<QVector<QPair<int, int> > > MainWindow::moreRounds(const QVector<QPair<int, int> > &numOfGamesPerDay)
{
    QVector<QVector<QPair<int, int> > > round;
    QVector<QVector<QPair<int, int> > > backRound;
    round = matchSchedule;
    backRound = matchSchedule;
    if(ui -> SpbNumberOfRounds -> value() > 1)
    {
        for(int j = 0; j < matchSchedule.size(); ++j)
        {
            for(int i = 0; i < numOfGamesPerDay.size(); ++i)
            {
                qSwap(backRound[j][i].first, backRound[j][i].second);
            }
        }
        for(int i = 1; i < ui -> SpbNumberOfRounds -> value(); ++i)
        {
            if(i % 2 == 0)
            {
                matchSchedule.append(round);
            }
            else
            {
                matchSchedule.append(backRound);
            }
        }
    }
    return matchSchedule;
}
void MainWindow::setUpTable(const int &rows)
{
    TblMatchTable -> setRowCount(rows);
    TblMatchTable -> setColumnCount(4);
    TblMatchTable -> verticalHeader() -> hide();

    QStringList headers = { "Home-Team", "Res", "ult", "Guest" };
    TblMatchTable -> setHorizontalHeaderLabels(headers);
    TblMatchTable -> horizontalHeaderItem(1) -> setTextAlignment(Qt::AlignRight);
    TblMatchTable -> horizontalHeaderItem(2) -> setTextAlignment(Qt::AlignLeft);
    TblMatchTable -> horizontalHeader() -> setStyleSheet(headerStyle);
    TblMatchTable -> setStyleSheet("background-color: grey; "
                                      "font: bold 19px Georgia");
    for(int i = 0; i < TblMatchTable -> rowCount(); ++i)
    {
        TblMatchTable -> setRowHeight(i, 18);
    }
    TblMatchTable -> setColumnWidth(0, 288);
    TblMatchTable -> setColumnWidth(1, 50);
    TblMatchTable -> setColumnWidth(2, 50);
    TblMatchTable -> setColumnWidth(3, 288);
}
void MainWindow::setUpLblLegend()
{
    LblLegend -> setStyleSheet("color: lightblue; "
                            "background-color: black; "
                            "font: bold 17px Georgia");

    LblLegend -> setText("Pts = Points; GF = Goals for; GA = Goals against; GD = Goal difference; MP = Match played; "
                         "W = Win; L = Loss; D = Draw");
    LblLegend -> setFixedHeight(35);
}
void MainWindow::fillTable(const QVector<QVector<QPair<int, int> > > &roundFst, const QVector<QPair<int, int> > &numOfGamesPerDay)
{
    backAndForth = matchSchedule.size() - 1;
    CmdBack -> setEnabled(true);
    CmdContinue -> setDisabled(true);

    double off = 0.0, def = 0.0, tabl = 0.0;
    setHeaderForTable();
    eachDay.clear();
    eachDay.resize(matchSchedule.size() );
    resultTable.clear();
    resultTable.resize(teamNameList.size() );
    if(ui -> CmdClubsMode -> isEnabled() )
    {
        sqlCommand.prepare("SELECT offensive, defensive, teamability FROM `nationalteams(countries)` WHERE name_country = ?");
    }
    else
    {
        sqlCommand.prepare("SELECT offensive, defensive, teamability FROM clubs WHERE name_club = ?");
    }
    for(int i = 0; i < teamNameList.size(); ++i)
    {
        sqlCommand.addBindValue(teamNameList.at(i) );
        sqlCommand.exec();
        sqlCommand.next();
        off = sqlCommand.value(0).toDouble();
        def = sqlCommand.value(1).toDouble();
        tabl = sqlCommand.value(2).toDouble();
        resultTable[i].getTeamNameAndQualityValues(teamNameList.at(i), off, def, tabl);
    }

    QString roundFirst = "[Matches", roundBack = "[Rematches";
    QString temp = roundBack;
    int c = 0, r = 0, day = 1;
    int nOfRoundFirst = 1, nOfRoundBack = 0;
    int rf = nOfRoundFirst;
    int rb = nOfRoundBack;
    double meanHome = 0.0, meanAway = 0.0;

    TblMatchTable -> setSpan(0, 0, 1, 4);
    TblMatchTable -> setCellWidget(0, 0, new QLabel);
    qobject_cast<QLabel*> (TblMatchTable -> cellWidget(0, 0) ) -> setText("[Matches 1, Day 1]");
    qobject_cast<QLabel*> (TblMatchTable -> cellWidget(0, 0) ) -> setStyleSheet("color: black; "
                                                                                   "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7FFFFF, stop: 0.5 #38FFFF, stop: 0.6 #30DDDD, stop:1 #C2FFFF); "
                                                                                   "font: bold 19px Georgia");
    qobject_cast<QLabel*> (TblMatchTable -> cellWidget(0, 0) ) -> setAlignment(Qt::AlignCenter);
    TblMatchTable -> setEditTriggers(QAbstractItemView::AllEditTriggers);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    CmbMatches -> disconnect();
    CmbMatches -> clear();

    for(int i = 0; i < matchSchedule.size(); ++i)
    {
        if(c == roundFst.size() && temp == roundBack)
        {
            c = 0;
            ++rb;
            temp = temp + " " + QVariant(rb).toString();
            TblMatchTable -> setSpan(r, 0, 1 ,4);
            TblMatchTable -> setCellWidget(r, 0, new QLabel);
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setText(temp  + ", Day " + QVariant(day).toString() + "]" );
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setStyleSheet("color: black; "
                                                                                    "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7FFFFF, stop: 0.5 #38FFFF, stop: 0.6 #30DDDD, stop:1 #C2FFFF); "
                                                                                    "font: bold 19px Georgia");
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setAlignment(Qt::AlignCenter);
            temp = roundFirst;
        }
        else if(c == roundFst.size() && temp == roundFirst)
        {
            c = 0;
            ++rf;
            temp = temp + " " + QVariant(rf).toString();
            TblMatchTable -> setSpan(r, 0, 1 ,4);
            TblMatchTable -> setCellWidget(r, 0, new QLabel);
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setText(temp + ", Day " + QVariant(day).toString() + "]" );
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setStyleSheet("color: black; "
                                                                                   "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7FFFFF, stop: 0.5 #38FFFF, stop: 0.6 #30DDDD, stop:1 #C2FFFF); "
                                                                                   "font: bold 19px Georgia");
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setAlignment(Qt::AlignCenter);
            temp = roundBack;
        }

        int randomGoalValue = 0, randomGoalAwayValue = 0;
        bool effectTeamAbilityHome = false, effectTeamAbilityAway = false;
        using BerDstr = std::bernoulli_distribution;
        using Dstr = std::poisson_distribution<int>;
        Dstr goals{};
        BerDstr taEffect{};
        for(int j = 0; j < numOfGamesPerDay.size(); ++j)
        {
            ++r;
            TblMatchTable -> setItem(r, 0, new QTableWidgetItem(teamNameList.at(matchSchedule.at(i).at(j).first - 1) ) );
            TblMatchTable -> item(r, 0) -> setTextAlignment(Qt::AlignRight);
            TblMatchTable -> item(r, 0) -> setFlags(TblMatchTable -> item(r, 0) -> flags() & ~Qt::ItemIsEditable);
            TblMatchTable -> item(r, 0) -> setTextColor(QColor("black") );
            TblMatchTable -> setItem(r, 3, new QTableWidgetItem(teamNameList.at(matchSchedule.at(i).at(j).second - 1) ) );
            TblMatchTable -> item(r, 3) -> setTextAlignment(Qt::AlignLeft);
            TblMatchTable -> item(r, 3) -> setFlags(TblMatchTable -> item(r, 3) -> flags() & ~Qt::ItemIsEditable);
            TblMatchTable -> item(r, 3) -> setTextColor(QColor("black") );

            CmbMatches -> addItem(TblMatchTable -> item(r, 0) -> text() + " vs. " + TblMatchTable -> item(r, 3) -> text() );

            meanHome = (resultTable[matchSchedule.at(i).at(j).first - 1].convertOffensiveValue() *
                    resultTable[matchSchedule.at(i).at(j).second - 1].convertDefensiveValue() ) + 0.41;
            randomGoalValue = goals(generator, Dstr::param_type{meanHome});       // random goal value home-team

            meanAway = resultTable[matchSchedule.at(i).at(j).second - 1].convertOffensiveValue() *
                    resultTable[matchSchedule.at(i).at(j).first - 1].convertDefensiveValue();
            randomGoalAwayValue = goals(generator, Dstr::param_type{meanAway});       // random goal value guest-team

            effectTeamAbilityHome = taEffect(generator, BerDstr::param_type{resultTable[matchSchedule.at(i).at(j).first - 1].convertTeamAbilityValue()});
            effectTeamAbilityAway = taEffect(generator, BerDstr::param_type{resultTable[matchSchedule.at(i).at(j).second - 1].convertTeamAbilityValue()});
            if(effectTeamAbilityHome)
            {
                ++randomGoalValue;
                if(randomGoalAwayValue != 0)
                {
                    --randomGoalAwayValue;
                }
            }
            if(effectTeamAbilityAway)
            {
                ++randomGoalAwayValue;
                if(randomGoalValue != 0)
                {
                    --randomGoalValue;
                }
            }

            TblMatchTable -> setItem(r, 1, new QTableWidgetItem(QVariant(randomGoalValue).toString() ) );
            TblMatchTable -> item(r, 1) -> setTextAlignment(Qt::AlignCenter);
            TblMatchTable -> item(r, 1) -> setBackgroundColor(QColor("lightblue") );
            TblMatchTable -> item(r, 1) -> setTextColor(QColor("black") );

            TblMatchTable -> setItem(r, 2, new QTableWidgetItem(QVariant(randomGoalAwayValue).toString() ) );
            TblMatchTable -> item(r, 2) -> setTextAlignment(Qt::AlignCenter);
            TblMatchTable -> item(r, 2) -> setBackgroundColor(QColor("lightblue") );
            TblMatchTable -> item(r, 2) -> setTextColor(QColor("black") );

            if(randomGoalValue > randomGoalAwayValue)
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(QColor(170, 255, 0, 255) );
                TblMatchTable -> item(r, 3) -> setBackgroundColor(QColor(255, 90, 0, 255) );
            }
            else if(randomGoalValue < randomGoalAwayValue)
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(QColor(255, 90, 0, 255) );
                TblMatchTable -> item(r, 3) -> setBackgroundColor(QColor(170, 255, 0, 255) );
            }
            else
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(Qt::gray);
                TblMatchTable -> item(r, 3) -> setBackgroundColor(Qt::gray);
            }

            resultTable[matchSchedule.at(i).at(j).first - 1].increasesNumberOfGames();
            resultTable[matchSchedule.at(i).at(j).second - 1].increasesNumberOfGames();

            resultTable[matchSchedule.at(i).at(j).first - 1].calculateGoals(randomGoalValue, randomGoalAwayValue);
            resultTable[matchSchedule.at(i).at(j).second - 1].calculateGoals(randomGoalAwayValue, randomGoalValue);

            if(randomGoalValue > randomGoalAwayValue)
            {
                resultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnWinCase();
                resultTable[matchSchedule.at(i).at(j).second - 1].increaseLoss();
            }
            else if(randomGoalValue < randomGoalAwayValue)
            {
                resultTable[matchSchedule.at(i).at(j).first - 1].increaseLoss();
                resultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnWinCase();
            }
            else
            {
                resultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnDrawCase();
                resultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnDrawCase();
            }

            resultTable[matchSchedule.at(i).at(j).first - 1].setFormIcons(randomGoalValue, randomGoalAwayValue, winImg, lossImg, drawImg);
            resultTable[matchSchedule.at(i).at(j).second - 1].setFormIcons(randomGoalAwayValue, randomGoalValue, winImg, lossImg, drawImg);
        }

        if(i < matchSchedule.size() - 1)
        {
            ++r;
            ++day;
            TblMatchTable -> setSpan(r, 0, 1, 4);
            TblMatchTable -> setCellWidget(r, 0, new QLabel);
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setText("Day " + QVariant(day).toString() );
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setStyleSheet("color: white; "
                                                                                           "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00A6FF, stop: 0.5 #0098FF, stop: 0.6 #008AFF, stop:1 #00B7FF); "
                                                                                           "font: bold 19px Georgia");
            qobject_cast<QLabel*> (TblMatchTable -> cellWidget(r, 0) ) -> setAlignment(Qt::AlignCenter);
        }
        ++c;
    }

    resultTable = prepareDirectMatchComparsion(resultTable, numOfGamesPerDay.size() );
    int counter = 0;
    std::sort(resultTable.begin(), resultTable.end(),  Teams::compare);
    for(Teams &t:resultTable)
    {
        t.output(*TblTable, counter);
        ++counter;
    }
    for(int i = 0; i < TblMatchTable -> rowCount(); ++i)
    {
        TblMatchTable -> showRow(i);
    }
}
void MainWindow::SpbUpdateTableReturnPressed()
{
    if(TblMatchTable -> currentColumn() == 1)
    {
        TblMatchTable -> setCurrentCell(TblMatchTable -> currentRow(), TblMatchTable -> currentColumn() + 1);
    }
    else if(TblMatchTable -> currentColumn() == 2)
    {
        TblMatchTable -> setCurrentCell(TblMatchTable -> currentRow(), TblMatchTable -> currentColumn() - 1);
    }

    TblTable -> clear();
    setHeaderForTable();

    QVector<Teams> updatedResultTable;
    updatedResultTable.resize(teamNameList.size() );
    for(int i = 0; i < teamNameList.size(); ++i)
    {
        updatedResultTable[i].getTeamNameAndQualityValues(teamNameList.at(i) );
    }
    int goalHome = 0, goalAway = 0, r = 0, gamesPerDay = 0;
    if(teamNameList.size() % 2 != 0)
    {
        gamesPerDay = (teamNameList.size() - 1) / 2;
    }
    else
    {
        gamesPerDay = teamNameList.size() / 2;
    }
    for(int i = 0; i < backAndForth + 1; ++i)
    {
        for(int j = 0; j < gamesPerDay; ++j)
        {
            ++r;
            goalHome = TblMatchTable -> item(r, 1) -> text().toInt();
            goalAway = TblMatchTable -> item(r, 2) -> text().toInt();
            if(goalHome > goalAway)
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(QColor(170, 255, 0, 255) );
                TblMatchTable -> item(r, 3) -> setBackgroundColor(QColor(255, 90, 0, 255) );
            }
            else if(goalHome < goalAway)
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(QColor(255, 90, 0, 255) );
                TblMatchTable -> item(r, 3) -> setBackgroundColor(QColor(170, 255, 0, 255) );
            }
            else
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(Qt::gray);
                TblMatchTable -> item(r, 3) -> setBackgroundColor(Qt::gray);
            }
            updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasesNumberOfGames();
            updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasesNumberOfGames();

            updatedResultTable[matchSchedule.at(i).at(j).first - 1].calculateGoals(goalHome, goalAway);
            updatedResultTable[matchSchedule.at(i).at(j).second - 1].calculateGoals(goalAway, goalHome);

            if(TblMatchTable -> item(r, 1) -> text().toInt() > TblMatchTable -> item(r, 2) -> text().toInt() )
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnWinCase();
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increaseLoss();
            }
            else if(TblMatchTable -> item(r, 1) -> text().toInt() < TblMatchTable -> item(r, 2) -> text().toInt() )
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increaseLoss();
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnWinCase();
            }
            else
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnDrawCase();
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnDrawCase();
            }

            updatedResultTable[matchSchedule.at(i).at(j).first - 1].setFormIcons(goalHome, goalAway, winImg, lossImg, drawImg);
            updatedResultTable[matchSchedule.at(i).at(j).second - 1].setFormIcons(goalAway, goalHome, winImg, lossImg, drawImg);
        }

        if(i < backAndForth)
        {
            ++r;
        }
    }

    updatedResultTable = prepareDirectMatchComparsion(updatedResultTable, gamesPerDay);
    int counter = 0;
    std::sort(updatedResultTable.begin(), updatedResultTable.end(), Teams::compare);
    for(Teams &t:updatedResultTable)
    {
        t.output(*TblTable, counter);
        ++counter;
    }
}
void MainWindow::showHomeTable()
{
    if(TblMatchTable -> currentColumn() == 1)
    {
        TblMatchTable -> setCurrentCell(TblMatchTable -> currentRow(), TblMatchTable -> currentColumn() + 1);
    }
    else if(TblMatchTable -> currentColumn() == 2)
    {
        TblMatchTable -> setCurrentCell(TblMatchTable -> currentRow(), TblMatchTable -> currentColumn() - 1);
    }

    TblTable -> clear();
    setHeaderForTable();

    QVector<Teams> updatedResultTable;
    updatedResultTable.resize(teamNameList.size() );
    for(int i = 0; i < teamNameList.size(); ++i)
    {
        updatedResultTable[i].getTeamNameAndQualityValues(teamNameList.at(i) );
    }
    int goalHome = 0, goalAway = 0, r = 0, gamesPerDay = 0;
    if(teamNameList.size() % 2 != 0)
    {
        gamesPerDay = (teamNameList.size() - 1) / 2;
    }
    else
    {
        gamesPerDay = teamNameList.size() / 2;
    }
    for(int i = 0; i < backAndForth + 1; ++i)
    {
        for(int j = 0; j < gamesPerDay; ++j)
        {
            ++r;
            goalHome = TblMatchTable -> item(r, 1) -> text().toInt();
            goalAway = TblMatchTable -> item(r, 2) -> text().toInt();
            if(goalHome > goalAway)
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(QColor(170, 255, 0, 255) );
                TblMatchTable -> item(r, 3) -> setBackgroundColor(QColor(255, 90, 0, 255) );
            }
            else if(goalHome < goalAway)
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(QColor(255, 90, 0, 255) );
                TblMatchTable -> item(r, 3) -> setBackgroundColor(QColor(170, 255, 0, 255) );
            }
            else
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(Qt::gray);
                TblMatchTable -> item(r, 3) -> setBackgroundColor(Qt::gray);
            }
            updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasesNumberOfGames();

            updatedResultTable[matchSchedule.at(i).at(j).first - 1].calculateGoals(goalHome, goalAway);

            if(TblMatchTable -> item(r, 1) -> text().toInt() > TblMatchTable -> item(r, 2) -> text().toInt() )
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnWinCase();
            }
            else if(TblMatchTable -> item(r, 1) -> text().toInt() < TblMatchTable -> item(r, 2) -> text().toInt() )
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increaseLoss();
            }
            else
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnDrawCase();
            }

            updatedResultTable[matchSchedule.at(i).at(j).first - 1].setFormIcons(goalHome, goalAway, winImg, lossImg, drawImg);
        }

        if(i < backAndForth)
        {
            ++r;
        }
    }

    updatedResultTable = prepareDirectMatchComparsion(updatedResultTable, gamesPerDay);
    int counter = 0;
    std::sort(updatedResultTable.begin(), updatedResultTable.end(), Teams::compare);
    for(Teams &t:updatedResultTable)
    {
        t.output(*TblTable, counter);
        ++counter;
    }
}
void MainWindow::showAwayTable()
{
    if(TblMatchTable -> currentColumn() == 1)
    {
        TblMatchTable -> setCurrentCell(TblMatchTable -> currentRow(), TblMatchTable -> currentColumn() + 1);
    }
    else if(TblMatchTable -> currentColumn() == 2)
    {
        TblMatchTable -> setCurrentCell(TblMatchTable -> currentRow(), TblMatchTable -> currentColumn() - 1);
    }

    TblTable -> clear();
    setHeaderForTable();

    QVector<Teams> updatedResultTable;
    updatedResultTable.resize(teamNameList.size() );
    for(int i = 0; i < teamNameList.size(); ++i)
    {
        updatedResultTable[i].getTeamNameAndQualityValues(teamNameList.at(i) );
    }
    int goalHome = 0, goalAway = 0, r = 0, gamesPerDay = 0;
    if(teamNameList.size() % 2 != 0)
    {
        gamesPerDay = (teamNameList.size() - 1) / 2;
    }
    else
    {
        gamesPerDay = teamNameList.size() / 2;
    }
    for(int i = 0; i < backAndForth + 1; ++i)
    {
        for(int j = 0; j < gamesPerDay; ++j)
        {
            ++r;
            goalHome = TblMatchTable -> item(r, 1) -> text().toInt();
            goalAway = TblMatchTable -> item(r, 2) -> text().toInt();
            if(goalHome > goalAway)
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(QColor(170, 255, 0, 255) );
                TblMatchTable -> item(r, 3) -> setBackgroundColor(QColor(255, 90, 0, 255) );
            }
            else if(goalHome < goalAway)
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(QColor(255, 90, 0, 255) );
                TblMatchTable -> item(r, 3) -> setBackgroundColor(QColor(170, 255, 0, 255) );
            }
            else
            {
                TblMatchTable -> item(r, 0) -> setBackgroundColor(Qt::gray);
                TblMatchTable -> item(r, 3) -> setBackgroundColor(Qt::gray);
            }
            updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasesNumberOfGames();

            updatedResultTable[matchSchedule.at(i).at(j).second - 1].calculateGoals(goalAway, goalHome);

            if(TblMatchTable -> item(r, 1) -> text().toInt() > TblMatchTable -> item(r, 2) -> text().toInt() )
            {
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increaseLoss();
            }
            else if(TblMatchTable -> item(r, 1) -> text().toInt() < TblMatchTable -> item(r, 2) -> text().toInt() )
            {
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnWinCase();
            }
            else
            {
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnDrawCase();
            }

            updatedResultTable[matchSchedule.at(i).at(j).second - 1].setFormIcons(goalAway, goalHome, winImg, lossImg, drawImg);
        }

        if(i < backAndForth)
        {
            ++r;
        }
    }

    updatedResultTable = prepareDirectMatchComparsion(updatedResultTable, gamesPerDay);
    int counter = 0;
    std::sort(updatedResultTable.begin(), updatedResultTable.end(), Teams::compare);
    for(Teams &t:updatedResultTable)
    {
        t.output(*TblTable, counter);
        ++counter;
    }
}
void MainWindow::CmdHomeAwayTotalClicked()
{
    if(homeAwayTotal == 0)
    {
        showHomeTable();
        ++homeAwayTotal;
    }
    else if(homeAwayTotal == 1)
    {
        showAwayTable();
        ++homeAwayTotal;
    }
    else
    {
        SpbUpdateTableReturnPressed();
        homeAwayTotal = 0;
    }
}

QVector<Teams> MainWindow::prepareDirectMatchComparsion(QVector<Teams> &resultTable, const int &gamesPerDay)
{
    QString homeTeamName, guestName;
    int homeGoal = 0, guestGoal = 0, goalDiffHome = 0, pointsHome = 0, goalDiffGuest = 0, pointsGuest = 0;
    for(int i = 0; i < backAndForth + 1; ++i)
    {
        for(int j = 0; j < gamesPerDay; ++j)
        {
            goalDiffHome = resultTable[matchSchedule.at(i).at(j).first - 1].returnGoalDiff();
            goalDiffGuest = resultTable[matchSchedule.at(i).at(j).second - 1].returnGoalDiff();
            pointsHome = resultTable[matchSchedule.at(i).at(j).first - 1].returnPoints();
            pointsGuest = resultTable[matchSchedule.at(i).at(j).second - 1].returnPoints();
            homeTeamName = resultTable[matchSchedule.at(i).at(j).first - 1].returnTeamName();
            guestName = resultTable[matchSchedule.at(i).at(j).second - 1].returnTeamName();
            if(pointsHome == pointsGuest && goalDiffHome == goalDiffGuest)
            {
                for(int k = 0; k < TblMatchTable -> rowCount(); ++k)
                {
                    if(TblMatchTable -> cellWidget(k, 0) )
                    {
                        continue;
                    }

                    if(pointsHome == pointsGuest && goalDiffHome == goalDiffGuest
                            && (homeTeamName == TblMatchTable -> item(k, 0) -> text() && guestName == TblMatchTable -> item(k, 3) -> text() ) )
                    {
                        homeGoal = TblMatchTable -> item(k, 1) -> text().toInt();
                        guestGoal = TblMatchTable -> item(k, 2) -> text().toInt();

                        resultTable[matchSchedule.at(i).at(j).first - 1].getGoalsForDirectMatchComparsion(homeGoal);
                        resultTable[matchSchedule.at(i).at(j).second - 1].getGoalsForDirectMatchComparsion(guestGoal);

                        resultTable[matchSchedule.at(i).at(j).second - 1].getAwayGoalsForDirectMatchComparsion(guestGoal);
                    }
                }
            }
        }
    }
    return resultTable;
}


//db connection
void MainWindow::connectDatabase(QString databaseName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);
    db.close();

    if(db.open() )
    {
        QMessageBox::information(this, "Connection", "Database connected successfully!");

        ui -> actionDB_Editor -> setEnabled(true);
        ui -> CmdClubsMode -> setEnabled(true);
    }
    else
    {
        QSqlError error = db.lastError();
        QMessageBox::information(this, "Connection", error.text() );
    }

    sqlCommand = QSqlQuery(db);
    sqlCmd = sqlCommand;
    ui -> CmdNatTeamsMode -> setDisabled(true);
    ui -> LstContinents -> clear();
    sqlCommand.exec("SELECT name_continent FROM continents ORDER BY idcontinents ASC");
    showContinents();

    ui -> LayoutSetRounds -> show();
}

//selection / undo buttons
void MainWindow::CmdUndoSelectedTeamClicked()
{
    if(ui -> LstSelectedTeams -> count() != 0)
    {
        TblTable -> clear();
        TblMatchTable -> clear();
        TblMatchTable -> setRowCount(0);
        while(!ui -> LstSelectedTeams -> selectedItems().isEmpty() )
        {
            for(int i = 0; i < ui -> LstSelectedTeams -> selectedItems().size(); ++i)
            {
                ui -> LstSelectedTeams -> takeItem(ui -> LstSelectedTeams -> row(ui -> LstSelectedTeams -> selectedItems().at(i) ) );
            }
        }
        LstSubContsCurrentRowChanged();
        if(ui -> LstSelectedTeams -> count() < 2)
        {
            ui -> actionSaveTeamList -> setDisabled(true);
            ui -> actionSaveMatchResults -> setDisabled(true);
            ui -> CmdFirstDay -> setDisabled(true);
        }
    }
    else
    {
        QMessageBox::information(this, "Empty list", "List is emtpy. No team can be selected to remove from the list.");
    }
}
void MainWindow::CmdClearAllClicked()
{
    if(ui -> LstSelectedTeams -> count() != 0)
    {
        ui -> actionSaveMatchResults -> setDisabled(true);
        teamNameList.clear();
        ui -> SpbNumberOfTeams -> setMinimum(2);
        ui -> SpbNumberOfTeams -> setValue(2);
        backAndForth = -1;
        TblTable -> clear();
        TblMatchTable -> clear();
        ui -> LstSelectedTeams -> clear();
        LstSubContsCurrentRowChanged();
        ui -> actionSaveTeamList -> setDisabled(true);
        ui -> CmdClearAll -> setDisabled(true);
        ui -> CmdFirstDay -> setDisabled(true);
        CmdBack -> setDisabled(true);
        DlgTableDialog -> close();
    }
}
void MainWindow::CmdTeamSelectionClicked()
{
    if(!ui -> LstNatTeams -> selectedItems().isEmpty() )
    {
        TblTable -> clear();
        TblMatchTable -> clear();
        TblMatchTable -> setRowCount(0);
        for(int i = 0; i < ui -> LstNatTeams -> selectedItems().size(); ++i)
        {
            ui -> LstSelectedTeams -> addItem(ui -> LstNatTeams -> selectedItems().at(i) -> text() );
        }
        LstSubContsCurrentRowChanged();
        if(ui -> LstSelectedTeams -> count() > 1)
        {
            ui -> actionSaveTeamList -> setEnabled(true);
            ui -> CmdClearAll -> setEnabled(true);
            ui -> CmdFirstDay -> setEnabled(true);
        }
    }
    else
    {
        QMessageBox::information(this, "No Selection", "Currently no Team is selected!");
    }
}
void MainWindow::CmdSelectWholeSubCoClicked()
{
    if(ui -> LstNatTeams -> count() != 0)
    {
        TblTable -> clear();
        TblMatchTable -> clear();
        TblMatchTable -> setRowCount(0);
        ui -> CmdFirstDay -> setEnabled(true);
        backAndForth = 0;

        for(int i = 0; i < ui -> LstNatTeams -> count(); ++i)
        {
            ui -> LstSelectedTeams -> addItem(ui -> LstNatTeams -> item(i) -> text() );
        }
        ui -> LstNatTeams -> clear();
        ui -> actionSaveTeamList -> setEnabled(true);
        ui -> CmdClearAll -> setEnabled(true);
    }
    else
    {
        QMessageBox::information(this, "Empty list", "Left list is emtpy. No team will be add.");
    }
}
void MainWindow::CmdSelectWholeContClicked()
{
    if(ui -> LstContinents -> count() != 0 && !ui -> LstContinents -> selectedItems().isEmpty() )
    {
        TblTable -> clear();
        TblMatchTable -> clear();
        TblMatchTable -> setRowCount(0);
        ui -> CmdFirstDay -> setEnabled(true);
        backAndForth = 0;

        sqlCommand.prepare("SELECT name_country FROM `nationalteams(countries)` WHERE continents_idcontinents = ?");
        sqlCommand.addBindValue(ui -> LstContinents -> currentRow() + 1);
        sqlCommand.exec();
        QString nation;
        QStringList nat;
        while(sqlCommand.next() )
        {
            nation = "";
            nation += sqlCommand.value(0).toString();
            nat.push_back(nation);
            ui -> LstSelectedTeams -> addItem(nation);
        }
        QStringList preList;
        for(int i = 0; i < ui -> LstSelectedTeams -> count(); ++i)
        {
            preList.push_back(ui -> LstSelectedTeams -> item(i) -> text() );
        }
        preList.removeDuplicates();
        ui -> LstSelectedTeams -> clear();
        for(const QString &L : preList)
        {
            ui -> LstSelectedTeams -> addItem(L);
        }
        LstSubContsCurrentRowChanged();
        preList.clear();
        ui -> actionSaveTeamList -> setEnabled(true);
        ui -> CmdClearAll -> setEnabled(true);
    }
    else if(ui -> LstContinents -> count() == 0 || ui -> LstContinents -> selectedItems().isEmpty() || !ui -> LstContinents -> hasFocus() )
    {
        QMessageBox::information(this, "Empty list or no selection", "Continent-List is emtpy or no continent is selected. No team will be add.");
    }
}

void MainWindow::createMatchSchedule()
{
    TblMatchTable -> clear();
    TblMatchTable -> clearSpans();

    numberOfTeams = ui -> LstSelectedTeams -> count();
    int n = numberOfTeams;
    if(ui -> LstSelectedTeams -> count() % 2 != 0)
    {
        ++n;    //add dummy
    }

    QVector<QPair<int, int> > numOfGamesPerDay;
    numOfGamesPerDay.resize(n/2);
    matchSchedule.resize(n - 1);
    for(int i = 0; i < matchSchedule.size(); ++i)
    {
        matchSchedule[i].resize(numOfGamesPerDay.size() );
    }

    //fill first matchday
    for(int i = 0; i < (n/2); ++i)
    {
        numOfGamesPerDay[i].first = i + 1;
        numOfGamesPerDay[i].second = n - i;
    }
    matchSchedule[0] = numOfGamesPerDay;

    matchSchedule = fillAllMatchDays(numOfGamesPerDay, n);

    //delete dummy
    if(numberOfTeams % 2 != 0)
    {
        numOfGamesPerDay.resize( (n/2) - 1);
        for(int i = 0; i < matchSchedule.size(); ++i)
        {
            matchSchedule[i].erase(matchSchedule[i].begin() );
        }
    }

    //shuffle matches within the matchday
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    for(int i = 0; i < matchSchedule.size(); ++i)
    {
        shuffle(matchSchedule[i].begin(), matchSchedule[i].end(), generator);
    }

    //more rounds?
    QVector<QVector<QPair<int, int> > > roundFst;
    roundFst = matchSchedule;
    matchSchedule = moreRounds(numOfGamesPerDay);

    int rows = ( ( (ui -> LstSelectedTeams -> count() * (ui -> LstSelectedTeams -> count() - 1) ) / 2) * ui -> SpbNumberOfRounds -> value() ) + matchSchedule.size();
    setUpTable(rows);

    teamNameList.clear();
    for(int i = 0; i < ui -> LstSelectedTeams -> count(); ++i)
    {
        teamNameList.append(ui -> LstSelectedTeams -> item(i) -> text() );
    }
    shuffle(teamNameList.begin(), teamNameList.end(), generator);

    fillTable(roundFst, numOfGamesPerDay);
    numberOfGamesPerDay = numOfGamesPerDay.size();
}

//match matchSchedule button
void MainWindow::CmdMatchScheduleClicked()
{
    if(ui -> LstSelectedTeams -> count() > 1)
    {
        DlgTableDialog -> close();
        createMatchSchedule();
        if(backAndForth == 0)
        {
            CmdBack -> setDisabled(true);
        }

        setUpTableDialog();
    }
    else
    {
        QMessageBox::warning(this, "Too few teams", "Too few teams in list! You have to choose at least two teams.");
    }
}
void MainWindow::setUpTableDialog()
{
    DlgTableDialog -> setWindowTitle("Match-Schedule with results and Table");
    TblTable -> setFixedWidth(1118);
    TblTable -> setMaximumHeight(775);
    TblTable -> setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    TblTable -> adjustSize();
    TblTable -> setSortingEnabled(true);
    TblMatchTable -> setFixedWidth(705);
    TblMatchTable -> setMaximumHeight(1000);
    TblMatchTable -> setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    TblMatchTable -> adjustSize();

    CmdBack -> setGeometry(1600, 900, 64, 64);
    CmdContinue -> setGeometry(1655, 900, 64, 64);
    CmdCloseDialog -> setGeometry(1750, 900, 64, 64);
    CmdHomeAwayTotal -> setGeometry(1628, 820, 64, 64);
    CmdHomeAwayTotal -> setText("HAT");

    LblLegend -> setParent(DlgTableDialog);
    TblMatchTable -> setParent(DlgTableDialog);
    TblTable -> setParent(DlgTableDialog);
    CmbMatches -> setParent(DlgTableDialog);
    CmdBack -> setParent(DlgTableDialog);
    CmdContinue -> setParent(DlgTableDialog);
    CmdCloseDialog -> setParent(DlgTableDialog);
    CmdHomeAwayTotal -> setParent(DlgTableDialog);

    CmbMatches -> setFixedHeight(35);
    LblLegend -> setFixedWidth(TblTable -> width() );

    TblMatchTable -> move(0, 0);
    TblTable -> move(1825 - TblTable -> width(), LblLegend -> height() );
    LblLegend -> move(1825 - TblTable -> width(), 0);
    CmbMatches -> move(1825 - TblTable -> width(), TblTable -> height() + LblLegend -> height() );

    DlgTableDialog -> setFixedWidth(TblMatchTable -> width() + TblTable -> width() );
    DlgTableDialog -> setFixedHeight(1000);
    DlgTableDialog -> setWindowFlags(Qt::Window);
    ui -> actionSaveMatchResults -> setEnabled(true);
    DlgTableDialog -> show();
    connect(CmbMatches, SIGNAL(currentIndexChanged(int) ), SLOT(CmbMatchesCurrentIndexChanged() ) );
}

void MainWindow::CmdAddRandomTeamsClicked()
{
    ui -> actionSaveTeamList -> setEnabled(true);
    ui -> CmdFirstDay -> setEnabled(true);
    if(ui -> CmdClubsMode -> isEnabled() )
    {
        sqlCmd.exec("SELECT name_country FROM `nationalteams(countries)`");
    }
    else
    {
        sqlCmd.exec("SELECT name_club FROM clubs");
    }

    QStringList partTeams;
    QStringList withoutDuplicates;
    while(sqlCmd.next() )
    {
        partTeams.push_back(sqlCmd.value(0).toString() );
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    using Udistr = std::uniform_int_distribution<int>;
    Udistr rval{};
    int randIndex = 0;
    if(partTeams.size() >= ui -> SpbNumberOfTeams -> value() )
    {
        for(int i = 0; i < ui -> SpbNumberOfTeams -> value(); ++i)
        {
            if(partTeams.size() == 0)
            {
                break;
            }
            randIndex = rval(generator, Udistr::param_type{0, partTeams.size() - 1} );
            ui -> LstSelectedTeams -> addItem(partTeams.at(randIndex) );
            partTeams.removeAt(randIndex);
        }

        for(int i = 0; i < ui -> LstSelectedTeams -> count(); ++i)
        {
            withoutDuplicates.push_back(ui -> LstSelectedTeams -> item(i)->text() );
        }
        withoutDuplicates.removeDuplicates();
        ui -> LstSelectedTeams -> clear();
        ui -> LstSelectedTeams -> addItems(withoutDuplicates);
        showNatTeams();
        ui -> CmdClearAll -> setEnabled(true);

        ui -> SpbNumberOfTeams -> setMinimum(1);
        ui -> SpbNumberOfTeams -> setValue(1);
    }
    else
    {
        QMessageBox::warning(this, "Decrease value", "The number of teams in the database\nis less than the number you have specified");
    }
}

//fill tables_____________________________________________________________
void MainWindow::showContinents()
{
    QString disp;
    while(sqlCommand.next() )
    {
        disp = "";
        disp += sqlCommand.value(0).toString();

        ui -> LstContinents -> addItem(disp);
    }
}
void MainWindow::showSubContinents()
{
    QString disp;
    while(sqlCommand.next() )
    {
        disp = "";
        disp += sqlCommand.value(0).toString();

        ui -> LstSubCont -> addItem(disp);
    }
}
void MainWindow::showNatTeams()
{
    QString disp;
    while(sqlCommand.next() )
    {
        disp = "";
        disp += sqlCommand.value(0).toString();
        ui -> LstNatTeams -> addItem(disp);
    }

    if(ui -> LstSelectedTeams -> count() > 0)
    {
        for(int i = 0; i < ui -> LstSelectedTeams -> count(); ++i)
        {
            for (int k = 0; k < ui -> LstNatTeams -> count(); ++k)
            {
                if(ui -> LstSelectedTeams -> item(i) -> text() == ui -> LstNatTeams -> item(k) -> text() )
                {
                    ui -> LstNatTeams -> takeItem(k);
                }
            }
        }
    }
}

void MainWindow::CmdNatTeamsModeClicked()
{
    ui -> LstContinents -> resize(241, 171);

    ui -> LblContinents -> setText("Continents");
    ui -> LblNatTeams -> setText("Nationalteams");
    ui -> CmdClearAll -> setDisabled(true);
    ui -> CmdNatTeamsMode -> setDisabled(true);
    ui -> CmdClubsMode -> setEnabled(true);
    ui -> CmdSelectWholeCont -> setEnabled(true);
    ui -> LblSubCont -> show();
    ui -> LstSubCont -> show();
    ui -> LstContinents -> clear();
    ui -> LstNatTeams -> clear();
    ui -> LstSelectedTeams -> clear();
    sqlCommand.exec("SELECT name_continent FROM continents ORDER BY idcontinents ASC");
    showContinents();
}
void MainWindow::CmdClubsModeClicked()
{
    ui -> LstContinents -> resize(300, 350);

    ui -> LblContinents -> setText("Leagues");
    ui -> LblNatTeams -> setText("Clubs");
    ui -> CmdClearAll -> setDisabled(true);
    ui -> CmdNatTeamsMode -> setEnabled(true);
    ui -> CmdClubsMode -> setDisabled(true);
    ui -> CmdSelectWholeCont -> setDisabled(true);
    ui -> LblSubCont -> hide();
    ui -> LstSubCont -> hide();
    ui -> LstContinents -> clear();
    ui -> LstSelectedTeams -> clear();
    sqlCommand.exec("SELECT name_league FROM leagues ORDER BY name_league ASC");
    QString league;
    while(sqlCommand.next() )
    {
        league = "";
        league += sqlCommand.value(0).toString();

        ui -> LstContinents -> addItem(league);
    }
}

void MainWindow::writeMatchData(QFile &f, const int &rowCounter)
{
    QTextStream out(&f);
    if(TblMatchTable -> cellWidget(rowCounter, 0) )
    {
        out << qobject_cast<QLabel*>(TblMatchTable -> cellWidget(rowCounter, 0) ) -> text() << endl;
    }
    else
    {
        out << TblMatchTable -> item(rowCounter, 0) -> text() << ";" << TblMatchTable -> item(rowCounter, 1) -> text() << ";"
            << TblMatchTable -> item(rowCounter, 2) -> text() << ";" << TblMatchTable -> item(rowCounter, 3) -> text() << endl;
    }
}
void MainWindow::readCsvMatchData(QString fileName)
{
    QFile csv(fileName);
    QTextStream in(&csv);
    QString line;
    teamNameList.clear();
    matchSchedule.clear();
    int row = 0, lineCounter = 0, roundCounter = 0;
    if (!csv.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File not exists";
    }
    else
    {
        while(!in.atEnd() )
        {
            line = in.readLine();
            QStringList item = line.split(";");
            if(item.size() != 1)
            {
                teamNameList.push_back(item.at(0) );
                teamNameList.push_back(item.at(3) );
            }
            if(item.size() == 1 && item.at(0).at(0) == "[")
            {
                ++roundCounter;
            }
            ++lineCounter;
        }
        csv.close();

        teamNameList.removeDuplicates();
        numberOfTeams = teamNameList.size();
        matchSchedule.resize( (teamNameList.size() - 1) * roundCounter);
        backAndForth = matchSchedule.size() - 1;
        eachDay.clear();
        eachDay.resize(matchSchedule.size() );
        int gamesPerDay = 0, dayCounter = 0, matchCounter = 0;
        if(teamNameList.size() % 2 != 0)
        {
            gamesPerDay = (teamNameList.size() - 1) / 2;
        }
        else
        {
            gamesPerDay = teamNameList.size() / 2;
        }
        numberOfGamesPerDay = gamesPerDay;
        for(int i = 0; i < matchSchedule.size(); ++i)
        {
            matchSchedule[i].resize(gamesPerDay);
        }

        csv.open(QFile::ReadOnly | QFile::Text);
        setUpTable(lineCounter);
        TblMatchTable -> setEditTriggers(QAbstractItemView::AllEditTriggers);
        while(!in.atEnd() )
        {   
            line = in.readLine();
            QStringList item = line.split(";");
            if(item.size() == 1)
            {
                matchCounter = 0;
                TblMatchTable -> setSpan(row, 0, 1, 4);
                TblMatchTable -> setCellWidget(row, 0, new QLabel);
                qobject_cast<QLabel*> (TblMatchTable -> cellWidget(row, 0) ) -> setText(item.at(0) );
                if(item.at(0).at(0) == "[")
                {
                    qobject_cast<QLabel*> (TblMatchTable -> cellWidget(row, 0) ) -> setStyleSheet("color: black; "
                                                                                                   "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7FFFFF, stop: 0.5 #38FFFF, stop: 0.6 #30DDDD, stop:1 #C2FFFF); "
                                                                                                   "font: bold 19px Georgia");
                }
                else
                {
                    qobject_cast<QLabel*> (TblMatchTable -> cellWidget(row, 0) ) -> setStyleSheet("color: white; "
                                                                                                   "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00A6FF, stop: 0.5 #0098FF, stop: 0.6 #008AFF, stop:1 #00B7FF); "
                                                                                                   "font: bold 19px Georgia");
                }
                qobject_cast<QLabel*> (TblMatchTable -> cellWidget(row, 0) ) -> setAlignment(Qt::AlignCenter);
                ++dayCounter;
            }
            else
            {

                matchSchedule[dayCounter - 1][matchCounter].first = teamNameList.indexOf(item.at(0) ) + 1;
                matchSchedule[dayCounter - 1][matchCounter].second = teamNameList.indexOf(item.at(3) ) + 1;
                ++matchCounter;

                TblMatchTable -> setItem(row, 0, new QTableWidgetItem(item.at(0) ) );
                TblMatchTable -> item(row, 0) -> setTextAlignment(Qt::AlignRight);
                TblMatchTable -> item(row, 0) -> setFlags(TblMatchTable -> item(row, 0) -> flags() & ~Qt::ItemIsEditable);
                TblMatchTable -> item(row, 0) -> setTextColor(QColor("black") );
                TblMatchTable -> setItem(row, 3, new QTableWidgetItem(item.at(3) ) );
                TblMatchTable -> item(row, 3) -> setTextAlignment(Qt::AlignLeft);
                TblMatchTable -> item(row, 3) -> setFlags(TblMatchTable -> item(row, 3) -> flags() & ~Qt::ItemIsEditable);
                TblMatchTable -> item(row, 3) -> setTextColor(QColor("black") );

                TblMatchTable -> setItem(row, 1, new QTableWidgetItem(item.at(1) ) );
                TblMatchTable -> item(row, 1) -> setTextAlignment(Qt::AlignCenter);
                TblMatchTable -> item(row, 1) -> setBackgroundColor(QColor("lightblue") );
                TblMatchTable -> item(row, 1) -> setTextColor(QColor("black") );
                TblMatchTable -> setItem(row, 2, new QTableWidgetItem(item.at(2) ) );
                TblMatchTable -> item(row, 2) -> setTextAlignment(Qt::AlignCenter);
                TblMatchTable -> item(row, 2) -> setBackgroundColor(QColor("lightblue") );
                TblMatchTable -> item(row, 2) -> setTextColor(QColor("black") );

                if(item.at(1).toInt() > item.at(2).toInt() )
                {
                    TblMatchTable -> item(row, 0) -> setBackgroundColor(QColor(170, 255, 0, 255) );
                    TblMatchTable -> item(row, 3) -> setBackgroundColor(QColor(255, 90, 0, 255) );
                }
                else if(item.at(1).toInt() < item.at(2).toInt() )
                {
                    TblMatchTable -> item(row, 0) -> setBackgroundColor(QColor(255, 90, 0, 255) );
                    TblMatchTable -> item(row, 3) -> setBackgroundColor(QColor(170, 255, 0, 255) );
                }
                else
                {
                    TblMatchTable -> item(row, 0) -> setBackgroundColor(Qt::gray);
                    TblMatchTable -> item(row, 3) -> setBackgroundColor(Qt::gray);
                }
                CmbMatches -> addItem(TblMatchTable -> item(row, 0) -> text() + " vs. " + TblMatchTable -> item(row, 3) -> text() );
            }
            ++row;
        }
        csv.close();
    }
}
//menu trigger buttons
void MainWindow::actOpenFileTriggered()
{
    const QFileDialog::Options options = QFlag(QFileDialog::ReadOnly);
    QString selectedFilter;
    const char *fileFormat;
    if(ui -> CmdClubsMode -> isEnabled() )
    {
        fileFormat = "Data Base Files (*.db);;Comma-separated values File (*.csv);;Nationalteams List Files (*.natt)";
    }
    else
    {
        fileFormat = "Data Base Files (*.db);;Comma-separated values File (*.csv);;Clubs List Files (*.clubs)";
    }
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open another Database or select a Team-List"),
                                "",
                                tr(fileFormat),
                                &selectedFilter,
                                options);
    if(!fileName.isEmpty() && fileName.endsWith("b") )
    {
        connectDatabase(fileName);
    }

    if(!fileName.isEmpty() && (fileName.endsWith("t") || fileName.endsWith("s") ) )
    {
        ui -> LstSelectedTeams -> clear();
        QFile list(fileName);
        QString line;
        if(list.open(QFile::ReadOnly | QFile::Text) )
        {
            QTextStream txt(&list);
            while(!txt.atEnd())
            {
                line = txt.readLine();
                if(ui -> CmdClubsMode -> isEnabled() )
                {
                    sqlCommand.prepare("SELECT name_country FROM `nationalteams(countries)` WHERE fifa_code = ?");
                }
                else
                {
                    sqlCommand.prepare("SELECT name_club FROM clubs WHERE idclubs = ?");
                }
                sqlCommand.addBindValue(line);
                sqlCommand.exec();
                while(sqlCommand.next() )
                {
                    ui -> LstSelectedTeams -> addItem(sqlCommand.value(0).toString() );
                }
            }
            list.close();
            showNatTeams();
        }
    }

    if(!fileName.isEmpty() && (fileName.endsWith("v") ) )
    {
        CmdBack -> setEnabled(true);
        CmdContinue -> setDisabled(true);
        ui -> LstSelectedTeams -> clear();
        TblMatchTable -> clear();
        CmbMatches -> clear();
        readCsvMatchData(fileName);
        SpbUpdateTableReturnPressed();
        setUpTableDialog();
    }
}
void MainWindow::actSaveListTriggered()
{
    const QFileDialog::Options options = QFlag(QFileDialog::ReadOnly);
    QString selectedFilter;
    const char *fileFormat;
    if(ui -> CmdClubsMode -> isEnabled() )
    {
        fileFormat = "Nationalteams List Files (*.natt)";
    }
    else
    {
        fileFormat = "Clubs List Files (*.clubs)";
    }
    QString fileName = QFileDialog::getSaveFileName(this,
                                tr("Save Team-List"),
                                "",
                                tr(fileFormat),
                                &selectedFilter,
                                options);
    if(!fileName.isEmpty() )
    {
        if(ui -> CmdClubsMode -> isEnabled() )
        {
            sqlCommand.prepare("SELECT fifa_code FROM `nationalteams(countries)` WHERE name_country = ?");
        }
        else
        {
            sqlCommand.prepare("SELECT idclubs FROM clubs WHERE name_club = ?");
        }

        QStringList id;
        for(int i = 0; i < ui -> LstSelectedTeams -> count(); ++i)
        {
            sqlCommand.addBindValue(ui -> LstSelectedTeams -> item(i) -> text() );
            sqlCommand.exec();
            while(sqlCommand.next() )
            {
                id.push_back(sqlCommand.value(0).toString() );
            }
        }

        QFile list(fileName);
        QTextStream txt(&list);
        if(list.open(QIODevice::WriteOnly | QFile::Text) )
        {
            for(int i = 0; i < id.size(); ++i)
            {
                txt << id.at(i) << endl;
            }
        }
        list.close();
    }
}
void MainWindow::actSaveMatchResultsTriggered()
{
    const QFileDialog::Options options = QFlag(QFileDialog::ReadOnly);
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                tr("Save Team-List"),
                                "",
                                tr("Comma-separated values File (*.csv)"),
                                &selectedFilter,
                                options);

    if(!fileName.isEmpty() && TblMatchTable -> rowCount() != 0)
    {
        QFile csvFile(fileName);
        if(csvFile.open(QIODevice::WriteOnly | QFile::Text) )
        {
            for(int row = 0; row < TblMatchTable -> rowCount(); ++row)
            {
                writeMatchData(csvFile, row);
            }
        }
        csvFile.close();
    }
    else
    {
        QMessageBox::warning(this, "Empty or Modification", "Either the Match-Table is empty or you\nmodified the Participants-List");
    }
}
void MainWindow::actDBEditorTriggered()
{
    meUi = new SqLiteDBEditor();
    meUi -> setAttribute(Qt::WA_DeleteOnClose);
    meUi -> show();
}
void MainWindow::actAboutTriggered()
{
    QMessageBox::aboutQt(this, "About");
}

void MainWindow::LstSelectedTeamsItemChanged()
{
    TblTable -> clear();
    TblMatchTable -> clear();
    TblMatchTable -> setRowCount(0);
    if(ui -> LstSelectedTeams -> count() > 1)
    {
        ui -> actionSaveTeamList -> setEnabled(true);
        ui -> CmdFirstDay -> setEnabled(true);
    }
}
//current row changed signal
void MainWindow::LstContinentsCurrentRowChanged()
{
    if(ui -> CmdClubsMode -> isEnabled() )
    {
        ui -> LstSubCont -> clear();
        sqlCommand.prepare("SELECT name_subcontinent FROM subcontinents WHERE continents_idcontinents = "
                        "(SELECT idcontinents FROM continents WHERE name_continent = ?) ORDER BY name_subcontinent ASC");
        sqlCommand.addBindValue(ui -> LstContinents -> currentItem() -> text() );
        sqlCommand.exec();
        showSubContinents();

        ui -> LstSubCont -> setCurrentRow(0);
    }
    else
    {
        ui -> LstSubCont -> clear();
        ui -> LstNatTeams -> clear();
        sqlCommand.prepare("SELECT name_club FROM clubs WHERE leagues_idleagues = "
                           "(SELECT idleagues FROM leagues WHERE name_league = ?)");
        sqlCommand.addBindValue(ui -> LstContinents -> currentItem() -> text() );
        sqlCommand.exec();
        showNatTeams();
    }
}
void MainWindow::LstSubContsCurrentRowChanged()
{
    if(ui -> CmdClubsMode -> isEnabled() )
    {
        ui -> LstNatTeams -> clear();
        sqlCommand.prepare("SELECT name_country FROM `nationalteams(countries)` WHERE subcontinents_idsubcontinents = "
                        "(SELECT idsubcontinents FROM subcontinents WHERE name_subcontinent = ?) ORDER BY name_country ASC");
        if(!ui -> LstSubCont -> selectedItems().isEmpty() )
        {
            sqlCommand.addBindValue(ui -> LstSubCont -> currentItem() -> text() );
            sqlCommand.exec();
            showNatTeams();
        }
    }
    else
    {
        ui -> LstNatTeams -> clear();
        sqlCommand.prepare("SELECT name_club FROM clubs WHERE leagues_idleagues = "
                           "(SELECT idleagues FROM leagues WHERE name_league = ?)");
        if(!ui -> LstContinents -> selectedItems().isEmpty() )
        {
            sqlCommand.addBindValue(ui -> LstContinents -> currentItem() -> text() );
            sqlCommand.exec();
            showNatTeams();
        }
    }
}


void MainWindow::setHeaderForTable()
{
    TblTable -> setAlternatingRowColors(true);
    TblTable -> setStyleSheet("QTableView::item { border-top: 1px solid #404040; "
                           "border-bottom: 1px solid #404040 } "
                           "QTableView { color: black; "
                           "background-color: lightcyan; "
                           "alternate-background-color: lightblue; "
                           "font: bold 17px Georgia }");
    TblTable -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    TblTable -> setRowCount(numberOfTeams);
    TblTable -> setColumnCount(11);
    for(int i = 0; i < TblTable -> rowCount(); ++i)
    {
        TblTable -> setRowHeight(i, 18);
    }
    TblTable -> setShowGrid(false);
    TblTable -> verticalHeader() -> hide();
    QStringList headers = { "Pos", "Team", "Pts", "GF", "GA", "GD", "MP", "W", "D", "L", "Last 5" };
    TblTable -> setHorizontalHeaderLabels(headers);
    TblTable -> horizontalHeader() -> setStyleSheet("QHeaderView::section { height: 30px; "
                                                          "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2BE2FF, stop: 0.5 #1ECEFF, stop: 0.6 #2BBBFF, stop:1 #1CECFF); "
                                                          "color: black; "
                                                          "padding-left: 1px; "
                                                          "border: 1px none; "
                                                          "font: bold 20px Georgia } ");

    TblTable -> setColumnWidth(0, 60);
    TblTable -> setColumnWidth(1, 350);
    TblTable -> setColumnWidth(2, 80);
    TblTable -> setColumnWidth(3, 80);
    TblTable -> setColumnWidth(4, 80);
    TblTable -> setColumnWidth(5, 90);
    TblTable -> setColumnWidth(6, 60);
    TblTable -> setColumnWidth(7, 60);
    TblTable -> setColumnWidth(8, 60);
    TblTable -> setColumnWidth(9, 60);
    TblTable -> setColumnWidth(10, 110);

    for(int i = 0; i < TblTable -> rowCount(); ++i)
    {
        TblTable -> setItem(i, 0, new QTableWidgetItem(QVariant(i + 1).toString() + "." ) );
        TblTable -> item(i, 0) -> setTextAlignment(Qt::AlignCenter);
    }
}


bool MainWindow::checkIfListsAreDifferent(QStringList list)
{
    bool listsAreDifferent = false;
    if(list.size() == teamNameList.size() )
    {
        for(int i = 0; i < list.size(); ++i)
        {
            for(int j = 0; j < list.size(); ++j)
            {
                if(list.at(i) == teamNameList.at(j) )
                {
                    listsAreDifferent = false;
                    break;
                }
                else
                {
                    listsAreDifferent = true;
                }
            }
        }
    }
    else
    {
        listsAreDifferent = true;
    }
    return listsAreDifferent;
}
void MainWindow::CmdContinueClicked()
{
    if(ui -> LstSelectedTeams -> count() > 1)
    {  
        ui -> CmdFirstDay -> setDisabled(true);
        QStringList newList;
        for(int i = 0; i < ui -> LstSelectedTeams -> count(); ++i)
        {
            newList.push_back(ui -> LstSelectedTeams -> item(i) -> text() );
        }

        bool listsAreDifferent = checkIfListsAreDifferent(newList);
        if( (backAndForth == -1 && eachDay.isEmpty() ) || listsAreDifferent)
        {
            CmdMatchScheduleClicked();
            backAndForth = -1;
        }

        ++backAndForth;
        SpbUpdateTableReturnPressed();
        if(backAndForth == eachDay.size() - 1)
        {
            CmdContinue -> setDisabled(true);
        }
        else
        {
            CmdContinue -> setEnabled(true);
        }

        if(backAndForth > 0)
        {
            CmdBack -> setEnabled(true);
        }
        else
        {
            CmdBack -> setDisabled(true);
        }

        for(int i = 0; i < TblMatchTable -> rowCount(); ++i)
        {
            TblMatchTable -> showRow(i);
        }
        for(int i = (numberOfGamesPerDay + 1) * (backAndForth + 1); i < TblMatchTable -> rowCount(); ++i)
        {
            TblMatchTable -> hideRow(i);
        }
    }
    else if(teamNameList.size() > 1)
    {
        ++backAndForth;
        SpbUpdateTableReturnPressed();
        if(backAndForth == eachDay.size() - 1)
        {
            CmdContinue -> setDisabled(true);
        }
        else
        {
            CmdContinue -> setEnabled(true);
        }

        if(backAndForth > 0)
        {
            CmdBack -> setEnabled(true);
        }
        else
        {
            CmdBack -> setDisabled(true);
        }

        for(int i = 0; i < TblMatchTable -> rowCount(); ++i)
        {
            TblMatchTable -> showRow(i);
        }
        for(int i = (numberOfGamesPerDay + 1) * (backAndForth + 1); i < TblMatchTable -> rowCount(); ++i)
        {
            TblMatchTable -> hideRow(i);
        }
    }
    else
    {
        QMessageBox::warning(this, "Too few teams", "Too few teams in list! You have to choose at least two teams.");
    }
}
void MainWindow::CmdBackClicked()
{
    QStringList newList;
    for(int i = 0; i < ui -> LstSelectedTeams -> count(); ++i)
    {
        newList.push_back(ui -> LstSelectedTeams -> item(i) -> text() );
    }
    bool listsAreDifferent = checkIfListsAreDifferent(newList);
    if(!listsAreDifferent || (!teamNameList.empty() && ui -> LstSelectedTeams -> count() == 0) )
    {
        if(backAndForth < eachDay.size() )
        {
            CmdContinue -> setEnabled(true);
        }

        --backAndForth;
        if(backAndForth == 0 )
        {
            CmdBack -> setDisabled(true);
        }
        else
        {
            CmdBack -> setEnabled(true);
        }

        SpbUpdateTableReturnPressed();

        for(int i = 0; i < TblMatchTable -> rowCount(); ++i)
        {
            TblMatchTable -> showRow(i);
        }
        for(int i = (numberOfGamesPerDay + 1) * (backAndForth + 1); i < TblMatchTable -> rowCount(); ++i)
        {
            TblMatchTable -> hideRow(i);
        }
    }
    else if(listsAreDifferent)
    {
        CmdMatchScheduleClicked();
        backAndForth = 1;
    }
}
void MainWindow::CmdFirstDayClicked()
{
    backAndForth = -1;
    CmdContinueClicked();
    DlgTableDialog -> show();
}

//constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    setUpLblLegend();

    winImg.load(":/miscRsc/misc_icons/win.jpg");
    lossImg.load(":/miscRsc/misc_icons/loss.jpg");
    drawImg.load(":/miscRsc/misc_icons/draw.jpg");

    ui -> actionDB_Editor -> setDisabled(true);
    ui -> actionSaveTeamList -> setDisabled(true);
    ui -> actionSaveMatchResults -> setDisabled(true);

    ui -> CmdUndoWholeSubCo -> setDisabled(true);
    ui -> CmdUndoWholeCont -> setDisabled(true);

    ui -> LayoutSetRounds -> hide();

    headerStyle = "QHeaderView::section { height: 30px; "
                  "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2BE2FF, stop: 0.5 #1ECEFF, stop: 0.6 #2BBBFF, stop:1 #1CECFF); "
                  "color: black; "
                  "padding-left: 1px; "
                  "border: 1px none; "
                  "font: bold 20px Georgia } "
                  "QHeaderView::section:middle { height: 30px; "
                  "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #FF7972, stop: 0.5 #FF4F4C, stop: 0.6 #FF282C, stop: 1 #f08080); "
                  "color: black; "
                  "padding-left: 1px; "
                  "border: 1px none; "
                  "font: bold 20px Georgia } ";

    ui -> LstContinents -> setStyleSheet("QListView::item:hover { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ABAFE5, stop: 1 #8588B2) } "
                                         "QListWidget { background: black }");
    ui -> LstSubCont -> setStyleSheet("QListView::item:hover { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ABAFE5, stop: 1 #8588B2) } "
                                         "QListWidget { background: black }");
    ui -> LstNatTeams -> setStyleSheet("QListView::item:hover { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ABAFE5, stop: 1 #8588B2) } "
                                         "QListWidget { background: black }");
    ui -> LstSelectedTeams -> setStyleSheet("QListView::item:hover { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ABAFE5, stop: 1 #8588B2) } "
                                         "QListWidget { background: black }");
    ui -> SpbNumberOfRounds -> setStyleSheet("color: black; "
                                             "background-color: lightgrey");
    ui -> SpbNumberOfTeams -> setStyleSheet("color: black; "
                                             "background-color: lightgrey");

    ui -> CmdClubsMode -> setDisabled(true);
    ui -> CmdNatTeamsMode -> setDisabled(true);
    ui -> CmdFirstDay -> setDisabled(true);
    CmdBack -> setDisabled(true);
    ui -> CmdClearAll -> setDisabled(true);
    ui -> CmdClubsMode -> setStyleSheet(" :disabled { color: grey; "
                                        "background: dimgrey }");
    ui -> CmdNatTeamsMode -> setStyleSheet(" :disabled { color: grey; "
                                           "background: dimgrey }");
    ui -> CmdSelectWholeCont -> setStyleSheet(" :disabled { color: grey; "
                                              "background: dimgrey }");
    CmdCloseDialog -> setStyleSheet("QPushButton:disabled { border-image: url(:/miscRsc/misc_icons/closeDlg-disabled) 3 10 3 10; "
                                    "border-top: 3px transparent; "
                                    "border-bottom: 3px transparent; "
                                    "border-right: 10px transparent; "
                                    "border-left: 10px transparent } "
                                    "QPushButton:enabled { border-image: url(:/miscRsc/misc_icons/closeDlg) 3 10 3 10; "
                                    "border-top: 3px transparent; "
                                    "border-bottom: 3px transparent; "
                                    "border-right: 10px transparent; "
                                    "border-left: 10px transparent } "
                                    "QPushButton:hover { border-image: url(:/miscRsc/misc_icons/closeDlg-hover) 4 11 4 11; "
                                    "border-top: 4px transparent; "
                                    "border-bottom: 4px transparent; "
                                    "border-right: 11px transparent; "
                                    "border-left: 11px transparent }"
                                    "QPushButton:pressed { border-image: url(:/miscRsc/misc_icons/closeDlg-pressed) 2 9 2 9; "
                                    "border-top: 2px transparent; "
                                    "border-bottom: 2px transparent; "
                                    "border-right: 9px transparent; "
                                    "border-left: 9px transparent }");
    CmdContinue -> setStyleSheet("QPushButton:disabled { border-image: url(:/miscRsc/misc_icons/arrow-continue-disabled) 3 10 3 10; "
                                       "border-top: 3px transparent; "
                                       "border-bottom: 3px transparent; "
                                       "border-right: 10px transparent; "
                                       "border-left: 10px transparent } "
                                       "QPushButton:enabled { border-image: url(:/miscRsc/misc_icons/arrow-continue) 3 10 3 10; "
                                       "border-top: 3px transparent; "
                                       "border-bottom: 3px transparent; "
                                       "border-right: 10px transparent; "
                                       "border-left: 10px transparent } "
                                       "QPushButton:hover { border-image: url(:/miscRsc/misc_icons/arrow-continue-hover) 4 11 4 11; "
                                       "border-top: 4px transparent; "
                                       "border-bottom: 4px transparent; "
                                       "border-right: 11px transparent; "
                                       "border-left: 11px transparent }"
                                       "QPushButton:pressed { border-image: url(:/miscRsc/misc_icons/arrow-continue-pressed) 2 9 2 9; "
                                       "border-top: 2px transparent; "
                                       "border-bottom: 2px transparent; "
                                       "border-right: 9px transparent; "
                                       "border-left: 9px transparent }");
    CmdBack -> setStyleSheet("QPushButton:disabled { border-image: url(:/miscRsc/misc_icons/arrow-back-disabled) 3 10 3 10; "
                                   "border-top: 3px transparent; "
                                   "border-bottom: 3px transparent; "
                                   "border-right: 10px transparent; "
                                   "border-left: 10px transparent } "
                                   "QPushButton:enabled { border-image: url(:/miscRsc/misc_icons/arrow-back) 3 10 3 10; "
                                   "border-top: 3px transparent; "
                                   "border-bottom: 3px transparent; "
                                   "border-right: 10px transparent; "
                                   "border-left: 10px transparent } "
                                   "QPushButton:hover { border-image: url(:/miscRsc/misc_icons/arrow-back-hover) 4 11 4 11; "
                                   "border-top: 4px transparent; "
                                   "border-bottom: 4px transparent; "
                                   "border-right: 11px transparent; "
                                   "border-left: 11px transparent } "
                                   "QPushButton:pressed { border-image: url(:/miscRsc/misc_icons/arrow-back-pressed) 2 9 2 9; "
                                   "border-top: 2px transparent; "
                                   "border-bottom: 2px transparent; "
                                   "border-right: 9px transparent; "
                                   "border-left: 9px transparent; }");
    ui -> CmdUndoWholeSubCo -> setStyleSheet(" :disabled { color: grey; "
                                             "background: dimgrey }");
    ui -> CmdUndoWholeCont -> setStyleSheet(" :disabled { color: grey; "
                                            "background: dimgrey }");
    ui -> CmdClearAll -> setStyleSheet(" :disabled { color: grey; "
                                       "background: dimgrey }");
    ui -> CmdFirstDay -> setStyleSheet(" :disabled { color: grey; "
                                       "background: dimgrey }");

    CmbMatches -> setStyleSheet("color: black; "
                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 coral, stop: 1 indianred); "
                             "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 blue, stop: 1 lightblue); "
                             "font: 18px Georgia");

    /*ui -> CmdDBCon -> setStyleSheet(" :enabled { color: white; "
                                    "border-Image: url(:/miscRsc/misc_icons/button.png) 3 10 3 10; "
                                    "border-top: 3px transparent; "
                                    "border-bottom: 3px transparent; "
                                    "border-right: 10px transparent; "
                                    "border-left: 10px transparent } "
                                    " :disabled { color: grey;"
                                    "border-Image: url(:/miscRsc/misc_icons/button_disabled.png) 3 10 3 10;"
                                    "border-top: 3px transparent;"
                                    "border-bottom: 3px transparent;"
                                    "border-right: 10px transparent; "
                                    "border-left: 10px transparent }"
                                    " :pressed { color: white; "
                                    "border-Image: url(:/miscRsc/misc_icons/button_pressed.png) 3 10 3 10; border-top: 3px transparent; "
                                    "border-bottom: 3px transparent; "
                                    "border-right: 10px transparent; "
                                    "border-left: 10px transparent }");*/
    CmdBack -> setFocusPolicy(Qt::NoFocus);
    CmdContinue -> setFocusPolicy(Qt::NoFocus);

    ui -> LblGitHubLink -> setOpenExternalLinks(true);
    ui -> LblGitHubLink -> setText("<a href=https://github.com/M87-virgo/soccer_result_smltr>GitHub Project Link</a>");
    ui -> LblGitHubLink -> setAlignment(Qt::AlignCenter);
    ui -> LblGitHubLink -> setStyleSheet("background: lightgrey");

    QString databaseName = "mydb.db";
    connectDatabase(databaseName);

    connect(ui -> actionDB_Editor, SIGNAL(triggered() ), SLOT(actDBEditorTriggered() ) );
    connect(ui -> actionAbout, SIGNAL(triggered() ), SLOT(actAboutTriggered() ) );
    connect(ui -> actionOpenFile, SIGNAL(triggered() ), SLOT(actOpenFileTriggered() ) );
    connect(ui -> actionSaveTeamList, SIGNAL(triggered() ), SLOT(actSaveListTriggered() ) );
    connect(ui -> actionSaveMatchResults, SIGNAL(triggered() ), SLOT(actSaveMatchResultsTriggered() ) );

    connect(ui -> LstSelectedTeams, SIGNAL(itemChanged(QListWidgetItem*) ), SLOT(LstSelectedTeamsItemChanged() ) );
    connect(ui -> LstContinents, SIGNAL(itemSelectionChanged() ), SLOT(LstContinentsCurrentRowChanged() ) );
    connect(ui -> LstSubCont, SIGNAL(itemSelectionChanged() ), SLOT(LstSubContsCurrentRowChanged() ) );

    connect(ui -> CmdNatTeamsMode, SIGNAL(clicked() ), SLOT(CmdNatTeamsModeClicked() ) );
    connect(ui -> CmdClubsMode, SIGNAL(clicked() ), SLOT(CmdClubsModeClicked() ) );

    connect(ui -> CmdUndoSelectedTeam, SIGNAL(clicked() ), SLOT(CmdUndoSelectedTeamClicked() ) );
    connect(ui -> CmdTeamSelection, SIGNAL(clicked() ), SLOT(CmdTeamSelectionClicked() ) );
    connect(ui -> CmdSelectWholeSubCo, SIGNAL(clicked() ), SLOT(CmdSelectWholeSubCoClicked() ) );
    connect(ui -> CmdSelectWholeCont, SIGNAL(clicked() ), SLOT(CmdSelectWholeContClicked() ) );
    connect(ui -> CmdClearAll, SIGNAL(clicked() ), SLOT(CmdClearAllClicked() ) );
    connect(ui -> CmdMatchSchedule, SIGNAL(clicked() ), SLOT(CmdMatchScheduleClicked() ) );
    connect(ui -> CmdAddRandomTeams, SIGNAL(clicked() ), SLOT(CmdAddRandomTeamsClicked() ) );

    QShortcut* ret = new QShortcut(Qt::Key_Return, TblMatchTable);
    connect(ret, SIGNAL(activated() ), SLOT(SpbUpdateTableReturnPressed() ) );
    connect(CmdHomeAwayTotal, SIGNAL(clicked() ), SLOT(CmdHomeAwayTotalClicked() ) );

    connect(CmdContinue, SIGNAL(clicked() ), SLOT(CmdContinueClicked() ) );
    connect(ui -> CmdFirstDay, SIGNAL(clicked() ), SLOT(CmdFirstDayClicked() ) );
    connect(CmdBack, SIGNAL(clicked() ), SLOT(CmdBackClicked() ) );
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}
