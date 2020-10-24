#include "teams.h"
#include <QDebug>

teams::teams() = default;
teams::teams(const QString tn, const int &gp, const int &p, const int &gs, const int &ga,
             const int &gd, const int &w, const int &d, const int &l, const double &o, const double &def, QIcon f)
{
    teamName = tn;
    gamesPlayed = gp;
    points = p;
    goalsScored = gs;
    goalsAgainst = ga;
    goalDiff = gd;
    wins = w;
    draws = d;
    looses = l;
    offense = o;
    defense = def;
    form = f;
}

void teams::getOffenseValue(const double &off)
{
    offense = off;
}
void teams::getDefenseValue(const double &def)
{
    defense = def;
}
void teams::getTeamNames(const QString name)
{
    teamName = name;
}
void teams::increasesNumberOfGames()
{
    ++gamesPlayed;
}
void teams::increasePointsOnWinCase()
{
    points += 3;
    ++wins;
}
void teams::increasePointsOnDrawCase()
{
    ++points;
    ++draws;
}
void teams::increaseLooses()
{
    ++looses;
}
void teams::calculateGoals(const int &goals, const int &goalsAgnst)
{
    goalsScored += goals;
    goalsAgainst += goalsAgnst;
    goalDiff = goalsScored - goalsAgainst;
}

bool teams::compare(const teams &a, const teams &b)
{
    if(a.points != b.points)
    {
        return a.points > b.points;
    }
    else if(a.goalDiff != b.goalDiff)
    {
        return a.goalDiff > b.goalDiff;
    }
    else
    {
        return a.goalsScored > b.goalsScored;
    }
}

void teams::fillFirstFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
{
    if(homeGoal > awayGoal)
    {
        QPixmap iniPixmap(64, 64);
        iniPixmap.fill();
        QPainter iniPainter(&iniPixmap);
        iniPainter.drawPixmap(0, 0, winImg);
        form.addPixmap(iniPixmap);
        wldMinusFour = winImg;
    }
    else if(homeGoal < awayGoal)
    {
        QPixmap iniPixmap(64, 64);
        iniPixmap.fill();
        QPainter iniPainter(&iniPixmap);
        iniPainter.drawPixmap(0, 0, looseImg);
        form.addPixmap(iniPixmap);
        wldMinusFour = looseImg;
    }
    else
    {
        QPixmap iniPixmap(64, 64);
        iniPixmap.fill();
        QPainter iniPainter(&iniPixmap);
        iniPainter.drawPixmap(0, 0, drawImg);
        form.addPixmap(iniPixmap);
        wldMinusFour = drawImg;
    }
}
void teams::fillSecFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
{
    if(homeGoal > awayGoal)
    {
        QPixmap secPixmap(128, 64);
        secPixmap.fill();
        QPainter secPainter(&secPixmap);
        secPainter.drawPixmap(0, 0, wldMinusFour);
        secPainter.drawPixmap(wldMinusFour.width(), 0, winImg);
        QIcon frm;
        frm.addPixmap(secPixmap);
        form.swap(frm);
        wldMinusThree = winImg;
    }
    else if(homeGoal < awayGoal)
    {
        QPixmap secPixmap(128, 64);
        secPixmap.fill();
        QPainter secPainter(&secPixmap);
        secPainter.drawPixmap(0, 0, wldMinusFour);
        secPainter.drawPixmap(wldMinusFour.width(), 0, looseImg);
        QIcon frm;
        frm.addPixmap(secPixmap);
        form.swap(frm);
        wldMinusThree = looseImg;
    }
    else
    {
        QPixmap secPixmap(128, 64);
        secPixmap.fill();
        QPainter secPainter(&secPixmap);
        secPainter.drawPixmap(0, 0, wldMinusFour);
        secPainter.drawPixmap(wldMinusFour.width(), 0, drawImg);
        QIcon frm;
        frm.addPixmap(secPixmap);
        form.swap(frm);
        wldMinusThree = drawImg;
    }
}
void teams::fillThirdFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
{
    if(homeGoal > awayGoal)
    {
        QPixmap thirdPixmap(192, 64);
        thirdPixmap.fill();
        QPainter thirdPainter(&thirdPixmap);
        thirdPainter.drawPixmap(0, 0, wldMinusFour);
        thirdPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        thirdPainter.drawPixmap(2 * winImg.width(), 0, winImg);
        QIcon frm;
        frm.addPixmap(thirdPixmap);
        form.swap(frm);
        wldMinusTwo = winImg;
    }
    else if(homeGoal < awayGoal)
    {
        QPixmap thirdPixmap(192, 64);
        thirdPixmap.fill();
        QPainter thirdPainter(&thirdPixmap);
        thirdPainter.drawPixmap(0, 0, wldMinusFour);
        thirdPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        thirdPainter.drawPixmap(2 * winImg.width(), 0, looseImg);
        QIcon frm;
        frm.addPixmap(thirdPixmap);
        form.swap(frm);
        wldMinusTwo = looseImg;
    }
    else
    {
        QPixmap thirdPixmap(192, 64);
        thirdPixmap.fill();
        QPainter thirdPainter(&thirdPixmap);
        thirdPainter.drawPixmap(0, 0, wldMinusFour);
        thirdPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        thirdPainter.drawPixmap(2 * winImg.width(), 0, drawImg);
        QIcon frm;
        frm.addPixmap(thirdPixmap);
        form.swap(frm);
        wldMinusTwo = drawImg;
    }
}
void teams::fillFourthFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
{
    if(homeGoal > awayGoal)
    {
        QPixmap fourthPixmap(256, 64);
        fourthPixmap.fill();
        QPainter fourthPainter(&fourthPixmap);
        fourthPainter.drawPixmap(0, 0, wldMinusFour);
        fourthPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        fourthPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        fourthPainter.drawPixmap(3 * winImg.width(), 0, winImg);
        QIcon frm;
        frm.addPixmap(fourthPixmap);
        form.swap(frm);
        wldMinusOne = winImg;
    }
    else if(homeGoal < awayGoal)
    {
        QPixmap fourthPixmap(256, 64);
        fourthPixmap.fill();
        QPainter fourthPainter(&fourthPixmap);
        fourthPainter.drawPixmap(0, 0, wldMinusFour);
        fourthPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        fourthPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        fourthPainter.drawPixmap(3 * winImg.width(), 0, looseImg);
        QIcon frm;
        frm.addPixmap(fourthPixmap);
        form.swap(frm);
        wldMinusOne = looseImg;
    }
    else
    {
        QPixmap fourthPixmap(256, 64);
        fourthPixmap.fill();
        QPainter fourthPainter(&fourthPixmap);
        fourthPainter.drawPixmap(0, 0, wldMinusFour);
        fourthPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        fourthPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        fourthPainter.drawPixmap(3 * winImg.width(), 0, drawImg);
        QIcon frm;
        frm.addPixmap(fourthPixmap);
        form.swap(frm);
        wldMinusOne = drawImg;
    }
}
void teams::fillLastFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
{
    if(homeGoal > awayGoal)
    {
        QPixmap lastPixmap(320, 64);
        lastPixmap.fill();
        QPainter lastPainter(&lastPixmap);
        lastPainter.drawPixmap(0, 0, wldMinusFour);
        lastPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        lastPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        lastPainter.drawPixmap(3 * winImg.width(), 0, wldMinusOne);
        lastPainter.drawPixmap(4 * winImg.width(), 0, winImg);
        QIcon frm;
        frm.addPixmap(lastPixmap);
        form.swap(frm);
    }
    else if(homeGoal < awayGoal)
    {
        QPixmap lastPixmap(320, 64);
        lastPixmap.fill();
        QPainter lastPainter(&lastPixmap);
        lastPainter.drawPixmap(0, 0, wldMinusFour);
        lastPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        lastPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        lastPainter.drawPixmap(3 * winImg.width(), 0, wldMinusOne);
        lastPainter.drawPixmap(4 * winImg.width(), 0, looseImg);
        QIcon frm;
        frm.addPixmap(lastPixmap);
        form.swap(frm);
    }
    else
    {
        QPixmap lastPixmap(320, 64);
        lastPixmap.fill();
        QPainter lastPainter(&lastPixmap);
        lastPainter.drawPixmap(0, 0, wldMinusFour);
        lastPainter.drawPixmap(winImg.width(), 0, wldMinusThree);
        lastPainter.drawPixmap(2 * winImg.width(), 0, wldMinusTwo);
        lastPainter.drawPixmap(3 * winImg.width(), 0, wldMinusOne);
        lastPainter.drawPixmap(4 * winImg.width(), 0, drawImg);
        QIcon frm;
        frm.addPixmap(lastPixmap);
        form.swap(frm);
    }
}
void teams::setFormIcons(const int &a, const int &b, const int &numberOfGamesPerTeam)
{
    int homeGoal = a;
    int awayGoal = b;

    int n = numberOfGamesPerTeam;
    QPixmap winImg(":/miscRsc/misc_icons/win.png");
    QPixmap looseImg(":/miscRsc/misc_icons/loose.png");
    QPixmap drawImg(":/miscRsc/misc_icons/draw.png");
    if(gamesPlayed == n && n == 6)
    {
        fillFirstFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
    }
    else if(n == 7)
    {
        if(gamesPlayed == n - 1)
        {
            fillFirstFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n)
        {
            fillSecFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
    }
    else if(n == 8)
    {
        if(gamesPlayed == n - 2)
        {
            fillFirstFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n - 1)
        {
            fillSecFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n)
        {
            fillThirdFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
    }
    else if(n == 9)
    {
        if(gamesPlayed == n - 3)
        {
            fillFirstFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n - 2)
        {
            fillSecFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n - 1)
        {
            fillThirdFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n)
        {
            fillFourthFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
    }
    else if(n > 9)
    {
        if(gamesPlayed == n - 4)
        {
            fillFirstFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n - 3)
        {
            fillSecFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n - 2)
        {
            fillThirdFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n - 1)
        {
            fillFourthFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
        else if(gamesPlayed == n)
        {
            fillLastFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
        }
    }
}

double teams::convertOffenseValue()
{
    double o = (5.9e-6 * pow(offense, 3) ) - (0.0012156 * pow(offense, 2) ) + (0.1299139 * offense) - 1.6529923;
    return o;
}
double teams::convertDefenseValue()
{
    //double d = -(2.4e-6 * pow(defense, 3) ) + (9.047e-4 * pow(defense, 2) ) - (0.1393443 * defense) + 7.4175298;
    double d = 2.38393 * exp(-0.02503 * defense);
    return d;
}

void teams::output(QTableWidget *tableWidget, const int &counter)
{
    tableWidget -> setItem(counter, 1, new QTableWidgetItem(teamName) );
    tableWidget -> setItem(counter, 2, new QTableWidgetItem(QVariant(points).toString() ) );
    tableWidget -> setItem(counter, 3, new QTableWidgetItem(QVariant(goalsScored).toString() ) );
    tableWidget -> setItem(counter, 4, new QTableWidgetItem(QVariant(goalsAgainst).toString() ) );
    tableWidget -> setItem(counter, 5, new QTableWidgetItem(QVariant(goalDiff).toString() ) );
    tableWidget -> setItem(counter, 6, new QTableWidgetItem(QVariant(gamesPlayed).toString() ) );
    tableWidget -> setItem(counter, 7, new QTableWidgetItem(QVariant(wins).toString() ) );
    tableWidget -> setItem(counter, 8, new QTableWidgetItem(QVariant(draws).toString() ) );
    tableWidget -> setItem(counter, 9, new QTableWidgetItem(QVariant(looses).toString() ) );
    tableWidget -> setItem(counter, 10, new QTableWidgetItem() );
    tableWidget -> item(counter, 10) -> setIcon(form);
    tableWidget -> setIconSize(QSize(96, 64) );
}
