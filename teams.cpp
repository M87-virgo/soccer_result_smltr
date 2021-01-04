/*
 * Copyright © 2020 beyer341@onlinehome.de
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#include "teams.h"


Teams::Teams() = default;
Teams::Teams(const QString tn, const int &gp, const int &p, const int &gss, const int &gas, const int &gsngl, const int &ga,
             const int &gd, const int &w, const int &d, const int &l, const double &o, const double &def, const double &ta, const QIcon &f)
{
    teamName = tn;
    gamesPlayed = gp;
    points = p;
    goalsScoredSum = gss;
    goalsAgainstSum = gas;
    goalsScored = gsngl;
    goalsAway = ga;
    goalDiff = gd;
    wins = w;
    draws = d;
    loss = l;
    offensive = o;
    defensive = def;
    teamAbility = ta;
    form = f;
}

void Teams::getTeamNameAndQualityValues(const QString name, const double &off = 0.0, const double &def = 0.0, const double &t = 0.0)
{
    teamName = name;
    offensive = off;
    defensive = def;
    teamAbility = t;
}
QString Teams::returnTeamName()
{
    return teamName;
}
void Teams::increasesNumberOfGames()
{
    ++gamesPlayed;
}
void Teams::increasePointsOnWinCase()
{
    points += 3;
    ++wins;
}
void Teams::increasePointsOnDrawCase()
{
    ++points;
    ++draws;
}
int Teams::returnNumberOfGames()
{
    return gamesPlayed;
}
int Teams::returnPoints()
{
    return points;
}
void Teams::increaseLoss()
{
    ++loss;
}
void Teams::calculateGoals(const int &goals, const int &goalsAgnst)
{
    goalsScoredSum += goals;
    goalsAgainstSum += goalsAgnst;
    goalDiff = goalsScoredSum - goalsAgainstSum;
}
int Teams::returnGoalDiff()
{
    return goalDiff;
}

bool Teams::compare(const Teams &a, const Teams &b)
{
    if(a.points != b.points)
    {
        return a.points > b.points;
    }
    else if(a.goalDiff != b.goalDiff)
    {
        return a.goalDiff > b.goalDiff;
    }
    else if(a.goalsScored != b.goalsScored)
    {
        return a.goalsScored > b.goalsScored;
    }
    else if(a.goalsAway != b.goalsAway)
    {
        return a.goalsAway > b.goalsAway;
    }
    else
    {
        return a.goalsScoredSum > b.goalsScoredSum;
    }
}

void Teams::getGoalsForDirectMatchComparsion(const int &homeGoal)
{
    goalsScored += homeGoal;
}
void Teams::getAwayGoalsForDirectMatchComparsion(const int &guestGoals)
{
    goalsAway += guestGoals;
}

void Teams::fillFirstFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &lossImg, const QPixmap &drawImg)
{
    QPixmap iniPixmap(64, 64);
    iniPixmap.fill(Qt::transparent);
    QPainter iniPainter(&iniPixmap);

    if(homeGoal > awayGoal)
    { 
        iniPainter.drawPixmap(0, 0, winImg);
        iniPainter.end();
        form.addPixmap(iniPixmap);
        wldMinusFour = winImg;
    }
    else if(homeGoal < awayGoal)
    {
        iniPainter.drawPixmap(0, 0, lossImg);
        iniPainter.end();
        form.addPixmap(iniPixmap);
        wldMinusFour = lossImg;
    }
    else
    {
        iniPainter.drawPixmap(0, 0, drawImg);
        iniPainter.end();
        form.addPixmap(iniPixmap);
        wldMinusFour = drawImg;
    }
}
void Teams::fillSecFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &lossImg, const QPixmap &drawImg)
{
    QPixmap secPixmap(128, 64);
    secPixmap.fill(Qt::transparent);
    QPainter secPainter(&secPixmap);
    QIcon frm;

    if(homeGoal > awayGoal)
    {
        secPainter.drawPixmap(0, 0, wldMinusFour);
        secPainter.drawPixmap(wldMinusFour.width(), 0, winImg);
        secPainter.end();

        frm.addPixmap(secPixmap);
        form.swap(frm);
        wldMinusThree = winImg;

    }
    else if(homeGoal < awayGoal)
    {
        secPainter.drawPixmap(0, 0, wldMinusFour);
        secPainter.drawPixmap(wldMinusFour.width(), 0, lossImg);
        secPainter.end();

        frm.addPixmap(secPixmap);
        form.swap(frm);
        wldMinusThree = lossImg;
    }
    else
    {
        secPainter.drawPixmap(0, 0, wldMinusFour);
        secPainter.drawPixmap(wldMinusFour.width(), 0, drawImg);
        secPainter.end();

        frm.addPixmap(secPixmap);
        form.swap(frm);
        wldMinusThree = drawImg;
    }
}
void Teams::fillThirdFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &lossImg, const QPixmap &drawImg)
{
    QPixmap thirdPixmap(192, 64);
    thirdPixmap.fill(Qt::transparent);
    QPainter thirdPainter(&thirdPixmap);
    QIcon frm;

    if(homeGoal > awayGoal)
    {
        thirdPainter.drawPixmap(0, 0, wldMinusFour);
        thirdPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        thirdPainter.drawPixmap(2 * winImg.width(), 0, winImg);
        thirdPainter.end();

        frm.addPixmap(thirdPixmap);
        form.swap(frm);
        wldMinusTwo = winImg;
    }
    else if(homeGoal < awayGoal)
    {
        thirdPainter.drawPixmap(0, 0, wldMinusFour);
        thirdPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        thirdPainter.drawPixmap(2 * winImg.width(), 0, lossImg);
        thirdPainter.end();

        frm.addPixmap(thirdPixmap);
        form.swap(frm);
        wldMinusTwo = lossImg;
    }
    else
    {
        thirdPainter.drawPixmap(0, 0, wldMinusFour);
        thirdPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        thirdPainter.drawPixmap(2 * winImg.width(), 0, drawImg);
        thirdPainter.end();

        frm.addPixmap(thirdPixmap);
        form.swap(frm);
        wldMinusTwo = drawImg;
    }
}
void Teams::fillFourthFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &lossImg, const QPixmap &drawImg)
{
    QPixmap fourthPixmap(256, 64);
    fourthPixmap.fill(Qt::transparent);
    QPainter fourthPainter(&fourthPixmap);
    QIcon frm;

    if(homeGoal > awayGoal)
    {
        fourthPainter.drawPixmap(0, 0, wldMinusFour);
        fourthPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        fourthPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        fourthPainter.drawPixmap(3 * winImg.width(), 0, winImg);
        fourthPainter.end();

        frm.addPixmap(fourthPixmap);
        form.swap(frm);
        wldMinusOne = winImg;
    }
    else if(homeGoal < awayGoal)
    {
        fourthPainter.drawPixmap(0, 0, wldMinusFour);
        fourthPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        fourthPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        fourthPainter.drawPixmap(3 * winImg.width(), 0, lossImg);
        fourthPainter.end();

        frm.addPixmap(fourthPixmap);
        form.swap(frm);
        wldMinusOne = lossImg;
    }
    else
    {
        fourthPainter.drawPixmap(0, 0, wldMinusFour);
        fourthPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        fourthPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        fourthPainter.drawPixmap(3 * winImg.width(), 0, drawImg);
        fourthPainter.end();

        frm.addPixmap(fourthPixmap);
        form.swap(frm);
        wldMinusOne = drawImg;
    }
}
void Teams::fillLastFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &lossImg, const QPixmap &drawImg)
{
    QPixmap lastPixmap(320, 64);
    lastPixmap.fill(Qt::transparent);
    QPainter lastPainter(&lastPixmap);
    QIcon frm;

    if(homeGoal > awayGoal)
    {
        lastPainter.drawPixmap(0, 0, wldMinusFour);
        lastPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        lastPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        lastPainter.drawPixmap(3 * winImg.width(), 0, wldMinusOne);
        lastPainter.drawPixmap(4 * winImg.width(), 0, winImg);
        lastPainter.end();

        frm.addPixmap(lastPixmap);
        form.swap(frm);

        wldMinusFour = wldMinusThree;
        wldMinusThree = wldMinusTwo;
        wldMinusTwo = wldMinusOne;
        wldMinusOne = winImg;
    }
    else if(homeGoal < awayGoal)
    {
        lastPainter.drawPixmap(0, 0, wldMinusFour);
        lastPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        lastPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        lastPainter.drawPixmap(3 * winImg.width(), 0, wldMinusOne);
        lastPainter.drawPixmap(4 * winImg.width(), 0, lossImg);
        lastPainter.end();

        frm.addPixmap(lastPixmap);
        form.swap(frm);

        wldMinusFour = wldMinusThree;
        wldMinusThree = wldMinusTwo;
        wldMinusTwo = wldMinusOne;
        wldMinusOne = lossImg;
    }
    else
    {
        lastPainter.drawPixmap(0, 0, wldMinusFour);
        lastPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        lastPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        lastPainter.drawPixmap(3 * winImg.width(), 0, wldMinusOne);
        lastPainter.drawPixmap(4 * winImg.width(), 0, drawImg);
        lastPainter.end();

        frm.addPixmap(lastPixmap);
        form.swap(frm);

        wldMinusFour = wldMinusThree;
        wldMinusThree = wldMinusTwo;
        wldMinusTwo = wldMinusOne;
        wldMinusOne = drawImg;
    }
}
void Teams::setFormIcons(const int &a, const int &b, const QPixmap &winImg, const QPixmap &lossImg, const QPixmap &drawImg)
{   
    int homeGoal = a, awayGoal = b, n = gamesPlayed;
    if(n == 6)
    {
        fillFirstFormIcon(homeGoal, awayGoal, winImg, lossImg, drawImg);
    }
    else if(n == 7)
    {
        fillSecFormIcon(homeGoal, awayGoal, winImg, lossImg, drawImg);
    }
    else if(n == 8)
    {
        fillThirdFormIcon(homeGoal, awayGoal, winImg, lossImg, drawImg);
    }
    else if(n == 9)
    {
        fillFourthFormIcon(homeGoal, awayGoal, winImg, lossImg, drawImg);
    }
    else if(n > 9)
    {
        fillLastFormIcon(homeGoal, awayGoal, winImg, lossImg, drawImg);
    }
}

double Teams::retOffensive()
{
    return offensive;
}
double Teams::retDefensive()
{
    return defensive;
}
double Teams::convertOffensiveValue()
{
    double o = (0.0000059 * pow(offensive, 3) ) - (0.0012156 * pow(offensive, 2) ) + (0.1299139 * offensive) - 1.6529923;
    return o;
}
double Teams::convertDefensiveValue()
{
    double d = 0.000000028 * pow(defensive, 4) - (0.000009612 * pow(defensive, 3) ) + (0.001188958 * pow(defensive, 2) ) - (0.0717863 * defensive) + 2.141225096;
    //double d = -(3.5e-6 * pow(defensive, 3) ) + (5.344e-4 * pow(defensive, 2) ) - (0.0350429 * defensive) + 1.7941399;
    //double d = 2.38393 * exp(-0.02503 * defensive);
    return d;
}
double Teams::convertTeamAbilityValue()
{
    double tav = teamAbility / 100;
    return tav;
}

void Teams::output(QTableWidget &tableWidget, const int &counter)
{
    tableWidget.setItem(counter, 1, new QTableWidgetItem(teamName) );
    tableWidget.setItem(counter, 2, new QTableWidgetItem(QVariant(points).toString() ) );
    tableWidget.setItem(counter, 3, new QTableWidgetItem(QVariant(goalsScoredSum).toString() ) );
    tableWidget.setItem(counter, 4, new QTableWidgetItem(QVariant(goalsAgainstSum).toString() ) );
    tableWidget.setItem(counter, 5, new QTableWidgetItem(QVariant(goalDiff).toString() ) );
    tableWidget.setItem(counter, 6, new QTableWidgetItem(QVariant(gamesPlayed).toString() ) );
    tableWidget.setItem(counter, 7, new QTableWidgetItem(QVariant(wins).toString() ) );
    tableWidget.setItem(counter, 8, new QTableWidgetItem(QVariant(draws).toString() ) );
    tableWidget.setItem(counter, 9, new QTableWidgetItem(QVariant(loss).toString() ) );
    tableWidget.setItem(counter, 10, new QTableWidgetItem() );
    tableWidget.item(counter, 10) -> setIcon(form);
    tableWidget.setIconSize(QSize(96, 64) );

    for(int j = 2; j < tableWidget.columnCount(); ++j)
    {
        tableWidget.item(counter, j) -> setTextAlignment(Qt::AlignCenter);
    }
}
