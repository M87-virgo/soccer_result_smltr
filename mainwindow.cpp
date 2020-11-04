#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>
#include <QtAlgorithms>
#include <chrono>
#include <thread>
#include <random>

#include "teams.cpp"


//match schedule functions
QVector<QVector<QPair<int, int> > > MainWindow::fillAllMatchDays(QVector<QPair<int, int> > numOfGamesPerDay, const int &n)
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
QVector<QVector<QPair<int, int> > > MainWindow::moreRounds(QVector<QPair<int, int> > numOfGamesPerDay)
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
    ui -> TblResults -> setStyleSheet("color: white; "
                                      "background-color: #3D3D3D; "
                                      "font: bold 20px Times New Roman");
    ui -> TblResults -> setRowCount(rows);
    ui -> TblResults -> setColumnCount(4);
    ui -> TblResults -> verticalHeader() -> hide();
    QStringList headers = { "Home-Team", "Res", "ult", "Guest" };
    ui -> TblResults -> setHorizontalHeaderLabels(headers);
    ui -> TblResults -> horizontalHeaderItem(1) -> setTextAlignment(Qt::AlignRight);
    ui -> TblResults -> horizontalHeaderItem(2) -> setTextAlignment(Qt::AlignLeft);
    ui -> TblResults -> horizontalHeader() -> setStyleSheet(headerStyle);
    ui -> TblResults -> setStyleSheet("background-color: black; "
                                      "font: bold 19px Georgia");
    ui -> TblResults -> setColumnWidth(0, 290);
    ui -> TblResults -> setColumnWidth(1, 50);
    ui -> TblResults -> setColumnWidth(2, 50);
    ui -> TblResults -> setColumnWidth(3, 290);
}
void MainWindow::fillTable(QVector<QVector<QPair<int, int> > > roundFst, QVector<QPair<int, int> > numOfGamesPerDay)
{
    backAndForth = matchSchedule.size() - 1;
    ui -> CmdBack -> setEnabled(true);
    ui -> CmdContinue -> setDisabled(true);

    double off = 0.0, def = 0.0, ta = 0.0;
    setHeaderForTable();
    eachDay.clear();
    eachDay.resize(matchSchedule.size() );
    resultTable.clear();
    resultTable.resize(teamNameList.size() );
    if(ui -> CmdClubsMode -> isEnabled() )
    {
        sqlCommand.prepare("SELECT offense, defense FROM `nationalteams(countries)` WHERE name_country = ?");
    }
    else
    {
        sqlCommand.prepare("SELECT offense, defense, teamability FROM clubs WHERE name_club = ?");
    }
    for(int i = 0; i < teamNameList.size(); ++i)
    {
        sqlCommand.addBindValue(ui -> LstSelectedTeams -> item(i) -> text() );
        sqlCommand.exec();
        sqlCommand.next();
        off = sqlCommand.value(0).toDouble();
        def = sqlCommand.value(1).toDouble();
        ta = sqlCommand.value(2).toDouble();
        resultTable[i].getTeamNameAndQualityValues(teamNameList.at(i), off, def, ta);
    }

    QString roundFirst = "Matches", roundBack = "Rematches";
    QString temp = roundBack;
    int c = 0, r = 0, day = 1;
    int n_of_round_first = 1, n_of_round_back = 0;
    int rf = n_of_round_first;
    int rb = n_of_round_back;
    double meanHome = 0.0, meanAway = 0.0;

    ui -> TblResults -> setSpan(0, 0, 1, 4);
    ui -> TblResults -> setCellWidget(0, 0, new QLabel);
    qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(0, 0) ) -> setText("Matches 1, Day 1");
    qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(0, 0) ) -> setStyleSheet("color: white; "
                                                                                   "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00A6FF, stop: 0.5 #0098FF, stop: 0.6 #008AFF, stop:1 #00B7FF); "
                                                                                   "font: bold 19px Georgia");
    qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(0, 0) ) -> setAlignment(Qt::AlignCenter);

    ui -> TblResults -> setEditTriggers(QAbstractItemView::AllEditTriggers);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    for(int i = 0; i < matchSchedule.size(); ++i)
    {
        if(c == roundFst.size() && temp == roundBack)
        {
            c = 0;
            ++rb;
            temp = temp + " " + QVariant(rb).toString();
            ui -> TblResults -> setSpan(r, 0, 1 ,4);
            ui -> TblResults -> setCellWidget(r, 0, new QLabel);
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setText(temp  + ", Day " + QVariant(day).toString() );
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setStyleSheet("color: white; "
                                                                                           "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00A6FF, stop: 0.5 #0098FF, stop: 0.6 #008AFF, stop:1 #00B7FF); "
                                                                                           "font: bold 19px Georgia");
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setAlignment(Qt::AlignCenter);
            temp = roundFirst;
        }
        else if(c == roundFst.size() && temp == roundFirst)
        {
            c = 0;
            ++rf;
            temp = temp + " " + QVariant(rf).toString();
            ui -> TblResults -> setSpan(r, 0 ,1 ,4);
            ui -> TblResults -> setCellWidget(r, 0, new QLabel);
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setText(temp + ", Day " + QVariant(day).toString() );
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setStyleSheet("color: white; "
                                                                                           "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00A6FF, stop: 0.5 #0098FF, stop: 0.6 #008AFF, stop:1 #00B7FF); "
                                                                                           "font: bold 19px Georgia");
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setAlignment(Qt::AlignCenter);
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
            ui -> TblResults -> setItem(r, 0, new QTableWidgetItem(teamNameList.at(matchSchedule.at(i).at(j).first - 1) ) );
            ui -> TblResults -> item(r, 0) -> setTextAlignment(Qt::AlignRight);
            ui -> TblResults -> item(r, 0) -> setFlags(ui -> TblResults -> item(r, 0) -> flags() & ~Qt::ItemIsEditable);
            ui -> TblResults -> item(r, 0) -> setTextColor(QColor("black") );
            ui -> TblResults -> setItem(r, 3, new QTableWidgetItem(teamNameList.at(matchSchedule.at(i).at(j).second - 1) ) );
            ui -> TblResults -> item(r, 3) -> setTextAlignment(Qt::AlignLeft);
            ui -> TblResults -> item(r, 3) -> setFlags(ui -> TblResults -> item(r, 3) -> flags() & ~Qt::ItemIsEditable);
            ui -> TblResults -> item(r, 3) -> setTextColor(QColor("black") );

            meanHome = resultTable[matchSchedule.at(i).at(j).first - 1].convertOffenseValue() *
                    (resultTable[matchSchedule.at(i).at(j).second - 1].convertDefenseValue() ) + 0.41;
            randomGoalValue = goals(generator, Dstr::param_type{meanHome});       // random goal value home-team

            meanAway = resultTable[matchSchedule.at(i).at(j).second - 1].convertOffenseValue() *
                    resultTable[matchSchedule.at(i).at(j).first - 1].convertDefenseValue();
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
            else
            {
                ++randomGoalAwayValue;
                if(randomGoalValue != 0)
                {
                    --randomGoalValue;
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
            else
            {
                ++randomGoalValue;
                if(randomGoalAwayValue != 0)
                {
                    --randomGoalAwayValue;
                }
            }
            ui -> TblResults -> setItem(r, 1, new QTableWidgetItem(QVariant(randomGoalValue).toString() ) );
            ui -> TblResults -> item(r, 1) -> setTextAlignment(Qt::AlignCenter);
            ui -> TblResults -> item(r, 1) -> setBackgroundColor(QColor(255, 127, 80, 255) );
            ui -> TblResults -> item(r, 1) -> setTextColor(QColor("black") );

            ui -> TblResults -> setItem(r, 2, new QTableWidgetItem(QVariant(randomGoalAwayValue).toString() ) );
            ui -> TblResults -> item(r, 2) -> setTextAlignment(Qt::AlignCenter);
            ui -> TblResults -> item(r, 2) -> setBackgroundColor(QColor(255, 127, 80, 255) );
            ui -> TblResults -> item(r, 2) -> setTextColor(QColor("black") );

            if(randomGoalValue > randomGoalAwayValue)
            {
                ui -> TblResults -> item(r, 0) -> setBackgroundColor(QColor(170, 255, 0, 255) );
                ui -> TblResults -> item(r, 3) -> setBackgroundColor(QColor(255, 90, 0, 255) );
            }
            else if(randomGoalValue < randomGoalAwayValue)
            {
                ui -> TblResults -> item(r, 0) -> setBackgroundColor(QColor(255, 90, 0, 255) );
                ui -> TblResults -> item(r, 3) -> setBackgroundColor(QColor(170, 255, 0, 255) );
            }
            else
            {
                ui -> TblResults -> item(r, 0) -> setBackgroundColor(Qt::gray);
                ui -> TblResults -> item(r, 3) -> setBackgroundColor(Qt::gray);
            }

            resultTable[matchSchedule.at(i).at(j).first - 1].increasesNumberOfGames();
            resultTable[matchSchedule.at(i).at(j).second - 1].increasesNumberOfGames();

            resultTable[matchSchedule.at(i).at(j).first - 1].calculateGoals(randomGoalValue, randomGoalAwayValue);
            resultTable[matchSchedule.at(i).at(j).second - 1].calculateGoals(randomGoalAwayValue, randomGoalValue);

            if(ui -> TblResults -> item(r, 1) -> text().toInt() > ui -> TblResults -> item(r, 2) -> text().toInt() )
            {
                resultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnWinCase();
                resultTable[matchSchedule.at(i).at(j).second - 1].increaseLooses();
            }
            else if(ui -> TblResults -> item(r, 1) -> text().toInt() < ui -> TblResults -> item(r, 2) -> text().toInt() )
            {
                resultTable[matchSchedule.at(i).at(j).first - 1].increaseLooses();
                resultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnWinCase();
            }
            else
            {
                resultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnDrawCase();
                resultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnDrawCase();
            }
            QPixmap winImg(":/miscRsc/misc_icons/win.jpg");
            QPixmap looseImg(":/miscRsc/misc_icons/loose.jpg");
            QPixmap drawImg(":/miscRsc/misc_icons/draw.jpg");
            resultTable[matchSchedule.at(i).at(j).first - 1].setFormIcons(randomGoalValue, randomGoalAwayValue, winImg, looseImg, drawImg);
            resultTable[matchSchedule.at(i).at(j).second - 1].setFormIcons(randomGoalAwayValue, randomGoalValue, winImg, looseImg, drawImg);
        }
        //eachDay[i] = resultTable;

        if(i < matchSchedule.size() - 1)
        {
            ++r;
            ++day;
            ui -> TblResults -> setSpan(r, 0, 1, 4);
            ui -> TblResults -> setCellWidget(r, 0, new QLabel);
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setText("Day " + QVariant(day).toString() );
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setStyleSheet("color: black; "
                                                                                           "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00ff7f, stop: 0.5 #00D865, stop: 0.6 #00C15A, stop:1 #1EFF7F); "
                                                                                           "font: bold 19px Georgia");
            qobject_cast<QLabel*> (ui -> TblResults -> cellWidget(r, 0) ) -> setAlignment(Qt::AlignCenter);
        }
        ++c;
    }

    int counter = 0;
    std::sort(resultTable.begin(), resultTable.end(),  teams::compare);
    for(teams &t:resultTable)
    {
        t.output(ui -> TblTable, counter);
        ++counter;
    }
    for(int i = 0; i < ui -> TblResults -> rowCount(); ++i)
    {
        ui -> TblResults -> showRow(i);
    }
}
void MainWindow::SpbUpdateTableReturnPressed()
{
    if(ui -> TblResults -> currentColumn() == 1)
    {
        ui -> TblResults -> setCurrentCell(ui -> TblResults -> currentRow(), ui -> TblResults -> currentColumn() + 1);
    }
    else if(ui -> TblResults -> currentColumn() == 2)
    {
        ui -> TblResults -> setCurrentCell(ui -> TblResults -> currentRow(), ui -> TblResults -> currentColumn() - 1);
    }

    ui -> TblTable -> clear();
    setHeaderForTable();

    QVector<teams> updatedResultTable;
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
            goalHome = ui -> TblResults -> item(r, 1) -> text().toInt();
            goalAway = ui -> TblResults -> item(r, 2) -> text().toInt();

            if(goalHome > goalAway)
            {
                ui -> TblResults -> item(r, 0) -> setBackgroundColor(QColor(170, 255, 0, 255) );
                ui -> TblResults -> item(r, 3) -> setBackgroundColor(QColor(255, 90, 0, 255) );
            }
            else if(goalHome < goalAway)
            {
                ui -> TblResults -> item(r, 0) -> setBackgroundColor(QColor(255, 90, 0, 255) );
                ui -> TblResults -> item(r, 3) -> setBackgroundColor(QColor(170, 255, 0, 255) );
            }
            else
            {
                ui -> TblResults -> item(r, 0) -> setBackgroundColor(Qt::gray);
                ui -> TblResults -> item(r, 3) -> setBackgroundColor(Qt::gray);
            }

            updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasesNumberOfGames();
            updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasesNumberOfGames();

            updatedResultTable[matchSchedule.at(i).at(j).first - 1].calculateGoals(goalHome, goalAway);
            updatedResultTable[matchSchedule.at(i).at(j).second - 1].calculateGoals(goalAway, goalHome);

            if(ui -> TblResults -> item(r, 1) -> text().toInt() > ui -> TblResults -> item(r, 2) -> text().toInt() )
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnWinCase();
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increaseLooses();
            }
            else if(ui -> TblResults -> item(r, 1) -> text().toInt() < ui -> TblResults -> item(r, 2) -> text().toInt() )
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increaseLooses();
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnWinCase();
            }
            else
            {
                updatedResultTable[matchSchedule.at(i).at(j).first - 1].increasePointsOnDrawCase();
                updatedResultTable[matchSchedule.at(i).at(j).second - 1].increasePointsOnDrawCase();
            }
            QPixmap winImg(":/miscRsc/misc_icons/win.jpg");
            QPixmap looseImg(":/miscRsc/misc_icons/loose.jpg");
            QPixmap drawImg(":/miscRsc/misc_icons/draw.jpg");
            updatedResultTable[matchSchedule.at(i).at(j).first - 1].setFormIcons(goalHome, goalAway, winImg, looseImg, drawImg);
            updatedResultTable[matchSchedule.at(i).at(j).second - 1].setFormIcons(goalAway, goalHome, winImg, looseImg, drawImg);
        }
        //eachDay[i] = updatedResultTable;

        if(i < backAndForth)//matchSchedule.size() - 1)
        {
            ++r;
        }
    }

    int counter = 0;
    std::sort(updatedResultTable.begin(), updatedResultTable.end(),  teams::compare);
    for(teams &t:updatedResultTable)
    {
        t.output(ui -> TblTable, counter);
        ++counter;
    }

    /*int counter = 0;
    std::sort(eachDay[backAndForth].begin(), eachDay[backAndForth].end(),  teams::compare);
    for(teams &t:eachDay[backAndForth])
    {
        t.output(ui -> TblTable, counter);
        ++counter;
    }*/
}


//db connection
void MainWindow::CmdDBConClicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydb.db");

    if(db.open() )
    {
        QMessageBox::information(this, "Connection", "Database connected successfully!");
        ui -> CmdDBCon -> setHidden(true);

        ui -> actionDB_Editor -> setEnabled(true);
        ui -> CmdClubsMode -> setEnabled(true);
    }
    else
    {
        QSqlError error = db.lastError();
        QMessageBox::information(this, "Connection", error.text() );
    }

    sqlCommand = QSqlQuery(db);
    ui -> CmdNatTeamsMode -> setDisabled(true);
    ui -> LstContinents -> clear();
    sqlCommand.exec("SELECT name_continent FROM continents ORDER BY idcontinents ASC");
    showContinents();

    ui -> LblRounds -> show();
    ui -> SpbNumberOfRounds -> show();
    ui -> CmdMatchSchedule -> show();
}

//selection / undo buttons
void MainWindow::CmdUndoSelectedTeamClicked()
{
    if(ui -> LstSelectedTeams -> count() != 0)
    {
        while(!ui -> LstSelectedTeams -> selectedItems().isEmpty() )
        {
            for(int i = 0; i < ui -> LstSelectedTeams -> selectedItems().size(); ++i)
            {
                ui -> LstSelectedTeams -> takeItem(ui -> LstSelectedTeams -> row(ui -> LstSelectedTeams -> selectedItems().at(i) ) );
            }
        }
        LstSubContsCurrentRowChanged();
    }
    else
    {
        QMessageBox::information(this, "Empty list", "List is emtpy. No team will be moved.");
    }
}
void MainWindow::CmdClearAllClicked()
{
    if(ui -> LstSelectedTeams -> count() != 0 )
    {
        ui -> LstSelectedTeams -> clear();
        LstSubContsCurrentRowChanged();
    }
}
void MainWindow::CmdTeamSelectionClicked()
{
    if(!ui -> LstNatTeams -> selectedItems().isEmpty() )
    {
        for(int i = 0; i < ui -> LstNatTeams -> selectedItems().size(); ++i)
        {
            ui -> LstSelectedTeams -> addItem(ui -> LstNatTeams -> selectedItems().at(i) -> text() );
        }
        LstSubContsCurrentRowChanged();
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
        for(int i = 0; i < ui -> LstNatTeams -> count(); ++i)
        {
            ui -> LstSelectedTeams -> addItem(ui -> LstNatTeams -> item(i) -> text() );
        }
        ui -> LstNatTeams -> clear();
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
    }
    else if(ui -> LstContinents -> count() == 0 || ui -> LstContinents -> selectedItems().isEmpty() || !ui -> LstContinents -> hasFocus() )
    {
        QMessageBox::information(this, "Empty list or no selection", "Continent-List is emtpy or no continent is selected. No team will be add.");
    }
}

void MainWindow::createMatchSchedule()
{
    ui -> TblResults -> clear();
    ui -> TblResults -> clearSpans();

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
    fillTable(roundFst, numOfGamesPerDay);
    numberOfGamesPerDay = numOfGamesPerDay.size();
}

//match matchSchedule button
void MainWindow::CmdMatchScheduleClicked()
{
    if(ui -> LstSelectedTeams -> count() > 1)
    {
        createMatchSchedule();
        ui -> LblMatchScedule -> show();
        ui -> LblEndTable -> show();
        ui -> TblResults -> show();
        ui -> TblTable -> show();
    }
    else
    {
        QMessageBox::warning(this, "Too few teams", "Too few teams in list! You have to choose at least two teams.");
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
    ui -> LstContinents -> resize(300, 280);

    ui -> LblContinents -> setText("Leagues");
    ui -> LblNatTeams -> setText("Clubs");
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

//menu trigger buttons
void MainWindow::actDBEditorTriggered()
{
    meUi = new SqLiteDBEditor();
    meUi -> setAttribute(Qt::WA_DeleteOnClose);
    //meUi -> setWindowFlag(Qt::FramelessWindowHint, true);
    meUi -> show();
}
void MainWindow::actAboutTriggered()
{
    QMessageBox::aboutQt(this, "About");
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
        sqlCommand.addBindValue(ui -> LstSubCont -> currentItem() -> text() );
        sqlCommand.exec();
        showNatTeams();
    }
    else
    {
        ui -> LstNatTeams -> clear();
        sqlCommand.prepare("SELECT name_club FROM clubs WHERE leagues_idleagues = "
                           "(SELECT idleagues FROM leagues WHERE name_league = ?)");
        sqlCommand.addBindValue(ui -> LstContinents -> currentItem() -> text() );
        sqlCommand.exec();
        showNatTeams();
    }
}


void MainWindow::setHeaderForTable()
{
    ui -> TblTable -> setStyleSheet("color: black; "
                                    "background-color: lightblue; "
                                      "font: bold 19px Georgia");
    ui -> TblTable -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui -> TblTable -> setRowCount(numberOfTeams);
    ui -> TblTable -> setColumnCount(11);
    ui -> TblTable -> verticalHeader() -> hide();
    QStringList headers = { "Pos", "Team", "P", "G", "GA", "GD", "GP", "W", "D", "L", "FORM" };
    ui -> TblTable -> setHorizontalHeaderLabels(headers);
    ui -> TblTable -> horizontalHeader() -> setStyleSheet("QHeaderView::section { height: 30px; "
                                                          "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2BE2FF, stop: 0.5 #1ECEFF, stop: 0.6 #2BBBFF, stop:1 #1CECFF); "
                                                          "color: black; "
                                                          "padding-left: 1px; "
                                                          "border: 1px none; "
                                                          "font: bold 20px Georgia } ");

    ui -> TblTable -> setColumnWidth(0, 60);
    ui -> TblTable -> setColumnWidth(1, 350);
    ui -> TblTable -> setColumnWidth(2, 80);
    ui -> TblTable -> setColumnWidth(3, 80);
    ui -> TblTable -> setColumnWidth(4, 80);
    ui -> TblTable -> setColumnWidth(5, 90);
    ui -> TblTable -> setColumnWidth(6, 60);
    ui -> TblTable -> setColumnWidth(7, 60);
    ui -> TblTable -> setColumnWidth(8, 60);
    ui -> TblTable -> setColumnWidth(9, 60);
    ui -> TblTable -> setColumnWidth(10, 110);

    for(int i = 0; i < ui -> TblTable -> rowCount(); ++i)
    {
        ui -> TblTable -> setItem(i, 0, new QTableWidgetItem(QVariant(i + 1).toString() + "." ) );
    }
}

void MainWindow::CmdContinueClicked()
{
    if(ui -> LstSelectedTeams -> count() > 1)
    {
        QStringList newList;
        for(int i = 0; i < ui -> LstSelectedTeams -> count(); ++i)
        {
            newList.push_back(ui -> LstSelectedTeams -> item(i) -> text() );
        }
        if( (backAndForth == -1 && eachDay.isEmpty() ) || teamNameList != newList)
        {
            CmdMatchScheduleClicked();
            backAndForth = -1;
        }

        ++backAndForth;
        /*int i = backAndForth;
        int counter = 0;
        std::sort(eachDay[i].begin(), eachDay[i].end(),  teams::compare);
        for(teams &t:eachDay[i])
        {
            t.output(ui -> TblTable, counter);
            ++counter;
        }*/
        SpbUpdateTableReturnPressed();
        if(backAndForth == eachDay.size() - 1)
        {
            ui -> CmdContinue -> setDisabled(true);
        }
        else
        {
            ui -> CmdContinue -> setEnabled(true);
        }

        if(backAndForth > 0)
        {
            ui -> CmdBack -> setEnabled(true);
        }
        else
        {
            ui -> CmdBack -> setDisabled(true);
        }

        for(int i = 0; i < ui -> TblResults -> rowCount(); ++i)
        {
            ui -> TblResults -> showRow(i);
        }
        for(int i = (numberOfGamesPerDay + 1) * (backAndForth + 1); i < ui -> TblResults -> rowCount(); ++i)
        {
            ui -> TblResults -> hideRow(i);
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
    if(teamNameList != newList)
    {
        CmdMatchScheduleClicked();
        backAndForth = 1;
    }

    if(backAndForth < eachDay.size() )
    {
        ui -> CmdContinue -> setEnabled(true);
    }

    --backAndForth;
    if(backAndForth == 0 )
    {
        ui -> CmdBack -> setDisabled(true);
    }
    else
    {
        ui -> CmdBack -> setEnabled(true);
    }

    /*int i = backAndForth;
    int counter = 0;
    std::sort(eachDay[i].begin(), eachDay[i].end(),  teams::compare);
    for(teams &t:eachDay[i])
    {
        t.output(ui -> TblTable, counter);
        ++counter;
    }*/
    SpbUpdateTableReturnPressed();

    for(int i = 0; i < ui -> TblResults -> rowCount(); ++i)
    {
        ui -> TblResults -> showRow(i);
    }
    for(int i = (numberOfGamesPerDay + 1) * (backAndForth + 1); i < ui -> TblResults -> rowCount(); ++i)
    {
        ui -> TblResults -> hideRow(i);
    }
}
//constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui -> setupUi(this);
    this -> setStyleSheet("color: lightblue; "
                          "background-color: #404040");

    ui -> actionDB_Editor -> setDisabled(true);

    ui -> LblMatchScedule -> hide();
    ui -> LblRounds -> hide();
    ui -> TblResults -> hide();
    ui -> TblTable -> hide();
    ui -> LblEndTable -> hide();
    ui -> SpbNumberOfRounds -> hide();
    ui -> CmdMatchSchedule -> hide();

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
    ui -> SpbNumberOfRounds -> setStyleSheet("background-color: black");

    ui -> CmdClubsMode -> setDisabled(true);
    ui -> CmdNatTeamsMode -> setDisabled(true);
    ui -> CmdBack -> setDisabled(true);
    ui -> CmdClubsMode -> setStyleSheet(" :disabled { color: grey; "
                                        "background: dimgrey }");
    ui -> CmdNatTeamsMode -> setStyleSheet(" :disabled { color: grey; "
                                           "background: dimgrey }");
    ui -> CmdSelectWholeCont -> setStyleSheet(" :disabled { color: grey; "
                                              "background: dimgrey }");
    ui -> CmdContinue -> setStyleSheet(" :disabled { color: grey; "
                                       "background: dimgrey }");
    ui -> CmdBack -> setStyleSheet(" :disabled { color: grey; "
                                   "background: dimgrey }");

    /*ui -> CmdDBCon -> setStyleSheet(" :enabled { border-style: outset; border-width: 2px; border-radius: 10px; "
                                    "border-color: beige; min-width: 10em; padding: 6px; color: white; background-color: "
                                    "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4595C6, "
                                    "stop: 0.4 #3E87B2, stop: 0.7 #357499, stop: 1 #489DCE) } :pressed "
                                    "{ background-color: qradialgradient(spread:pad, cx: 0.5, cy: 0.5, radius: 0.5, fx: 0.5, fy: 0.5, "
                                    "stop: 0.0795 rgba(0, 147, 185, 255), stop: 1 rgba(0, 85, 183, 255) ); border: 3px solid black; border-radius: 10px }");*/

    ui -> CmdDBCon -> setStyleSheet(" :enabled { color: white; "
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
                                    "border-left: 10px transparent }");

    /*ui -> CmdDBCon -> setStyleSheet(" :enabled { background-color: qradialgradient(spread:pad, cx:0.499807, "
                                    "cy:0.489, radius:0.5, fx:0.499, fy:0.488909, stop:0.0795455 rgba(0, 147, 185, 255), "
                                    "stop:1 rgba(30, 30, 30, 255) ); border: 5px solid black; border-radius: 15px }");*/

    ui -> LblGitHubLink -> setOpenExternalLinks(true);
    ui -> LblGitHubLink -> setText("<a href=https://github.com/M87-virgo/soccer_result_smltr>GitHub Project Link</a>");
    ui -> LblGitHubLink -> setAlignment(Qt::AlignCenter);
    ui -> LblGitHubLink -> setStyleSheet("background: lightgrey");

    connect(ui -> actionDB_Editor, SIGNAL(triggered() ), SLOT(actDBEditorTriggered() ) );
    connect(ui -> actionAbout, SIGNAL(triggered() ), SLOT(actAboutTriggered() ) );

    connect(ui -> LstContinents, SIGNAL(itemSelectionChanged() ), SLOT(LstContinentsCurrentRowChanged() ) );
    connect(ui -> LstSubCont, SIGNAL(itemSelectionChanged() ), SLOT(LstSubContsCurrentRowChanged() ) );

    connect(ui -> CmdNatTeamsMode, SIGNAL(clicked() ), SLOT(CmdNatTeamsModeClicked() ) );
    connect(ui -> CmdClubsMode, SIGNAL(clicked() ), SLOT(CmdClubsModeClicked() ) );
    connect(ui -> CmdDBCon, SIGNAL(clicked() ), SLOT(CmdDBConClicked() ) );

    connect(ui -> CmdUndoSelectedTeam, SIGNAL(clicked() ), SLOT(CmdUndoSelectedTeamClicked() ) );
    connect(ui -> CmdTeamSelection, SIGNAL(clicked() ), SLOT(CmdTeamSelectionClicked() ) );
    connect(ui -> CmdSelectWholeSubCo, SIGNAL(clicked() ), SLOT(CmdSelectWholeSubCoClicked() ) );
    connect(ui -> CmdSelectWholeCont, SIGNAL(clicked() ), SLOT(CmdSelectWholeContClicked() ) );
    connect(ui -> CmdClearAll, SIGNAL(clicked() ), SLOT(CmdClearAllClicked() ) );
    connect(ui -> CmdMatchSchedule, SIGNAL(clicked() ), SLOT(CmdMatchScheduleClicked() ) );

    QShortcut* ret = new QShortcut(Qt::Key_Return, ui -> TblResults);
    connect(ret, SIGNAL(activated() ), SLOT(SpbUpdateTableReturnPressed() ) );

    connect(ui -> CmdContinue, SIGNAL(clicked() ), SLOT(CmdContinueClicked() ) );
    connect(ui -> CmdBack, SIGNAL(clicked() ), SLOT(CmdBackClicked() ) );
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

