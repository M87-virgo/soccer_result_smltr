#include "teams.h"
#include <QDebug>


teams::teams() = default;
teams::teams(const QString tn, const int &gp, const int &p, const int &gs, const int &ga,
             const int &gd, const int &w, const int &d, const int &l, const double &o, const double &def, const double &ta, const QIcon &f)
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
    teamAbility = ta;
    form = f;
}

void teams::getTeamNameAndQualityValues(const QString name, const double &off = 0.0, const double &def = 0.0, const double &t = 0.0)
{
    teamName = name;
    offense = off;
    defense = def;
    teamAbility = t;
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
        iniPainter.drawPixmap(0, 0, looseImg);
        iniPainter.end();
        form.addPixmap(iniPixmap);
        wldMinusFour = looseImg;
    }
    else
    {
        iniPainter.drawPixmap(0, 0, drawImg);
        iniPainter.end();
        form.addPixmap(iniPixmap);
        wldMinusFour = drawImg;
    }
}
void teams::fillSecFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
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
        secPainter.drawPixmap(wldMinusFour.width(), 0, looseImg);
        secPainter.end();

        frm.addPixmap(secPixmap);
        form.swap(frm);
        wldMinusThree = looseImg;
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
void teams::fillThirdFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
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
        thirdPainter.drawPixmap(2 * winImg.width(), 0, looseImg);
        thirdPainter.end();

        frm.addPixmap(thirdPixmap);
        form.swap(frm);
        wldMinusTwo = looseImg;
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
void teams::fillFourthFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
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
        fourthPainter.drawPixmap(3 * winImg.width(), 0, looseImg);
        fourthPainter.end();

        frm.addPixmap(fourthPixmap);
        form.swap(frm);
        wldMinusOne = looseImg;
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
void teams::fillLastFormIcon(const int &homeGoal, const int &awayGoal, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
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
        lastPainter.drawPixmap(4 * winImg.width(), 0, looseImg);
        lastPainter.end();

        frm.addPixmap(lastPixmap);
        form.swap(frm);

        wldMinusFour = wldMinusThree;
        wldMinusThree = wldMinusTwo;
        wldMinusTwo = wldMinusOne;
        wldMinusOne = looseImg;
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
void teams::setFormIcons(const int &a, const int &b, const QPixmap &winImg, const QPixmap &looseImg, const QPixmap &drawImg)
{   
    int homeGoal = a, awayGoal = b, n = gamesPlayed;
    if(n == 6)
    {
        fillFirstFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
    }
    else if(n == 7)
    {
        fillSecFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
    }
    else if(n == 8)
    {
        fillThirdFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
    }
    else if(n == 9)
    {
        fillFourthFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
    }
    else if(n > 9)
    {
        fillLastFormIcon(homeGoal, awayGoal, winImg, looseImg, drawImg);
    }
}

double teams::convertOffenseValue()
{
    double o = (5.9e-6 * pow(offense, 3) ) - (0.0012156 * pow(offense, 2) ) + (0.1299139 * offense) - 1.6529923;
    return o;
}
double teams::convertDefenseValue()
{
    double d = 2.8e-8 * pow(defense, 4) - (9.612e-6 * pow(defense, 3) ) + (1.188958e-3 * pow(defense, 2) ) - (7.1786300e-2 * defense) + 2.141225096;
    //double d = -(3.5e-6 * pow(defense, 3) ) + (5.344e-4 * pow(defense, 2) ) - (0.0350429 * defense) + 1.7941399;
    //double d = 2.38393 * exp(-0.02503 * defense);
    return d;
}
double teams::convertTeamAbilityValue()
{
    double tav = teamAbility / 100;
    return tav;
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
