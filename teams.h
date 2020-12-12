/*
 * Copyright © 2020 beyer341@onlinehome.de
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#ifndef TEAMS_H
#define TEAMS_H

#include <QString>
#include <QTableWidget>
#include <QPainter>

#include <cmath>

class Teams
{
    private:
        QString teamName = {};
        int gamesPlayed = 0;
        int points = 0;
        int goalsScoredSum = 0;
        int goalsAgainstSum = 0;
        int goalsScored = 0;
        int goalsAway = 0;
        int goalDiff = 0;
        int wins = 0;
        int draws = 0;
        int loss = 0;

        double offensive = 0.0;
        double defensive = 0.0;
        double teamAbility = 0.0;

        QIcon form;

        QPixmap wldMinusFour = {};
        QPixmap wldMinusThree = {};
        QPixmap wldMinusTwo = {};
        QPixmap wldMinusOne = {};

    public:
        Teams();
        Teams(const QString, const int&, const int&, const int&, const int&, const int&, const int&,
              const int&, const int&, const int&, const int&, const double&, const double&, const double&, const QIcon&);

        void getTeamNameAndQualityValues(const QString, const double&, const double&, const double&);
        void increasesNumberOfGames();
        int returnNumberOfGames();
        void increasePointsOnWinCase();
        void increasePointsOnDrawCase();
        void increaseLoss();
        void calculateGoals(const int&, const int&);
        void setFormIcons(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);

        static bool compare(const Teams &a, const Teams &b);
        void getGoalsForDirectMatchComparsion(const int&);
        void getAwayGoalsForDirectMatchComparsion(const int&);
        void output(QTableWidget&, const int&);
        int returnPoints();
        int returnGoalDiff();
        QString returnTeamName();

        void fillFirstFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);
        void fillSecFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);
        void fillThirdFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);
        void fillFourthFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);
        void fillLastFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);

        double convertOffensiveValue();
        double convertDefensiveValue();
        double convertTeamAbilityValue();
};

#endif // TEAMS_H
