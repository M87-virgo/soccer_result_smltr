#ifndef TEAMS_H
#define TEAMS_H

#include <QString>
#include <QTableWidget>
#include <QPainter>

#include <cmath>

class teams
{
    private:
        QString teamName = {};
        int gamesPlayed = 0;
        int points = 0;
        int goalsScored = 0;
        int goalsAgainst = 0;
        int goalsScoredSum = 0;
        int goalsAgainstSum = 0;
        int goalDiff = 0;
        int wins = 0;
        int draws = 0;
        int looses = 0;

        double offense = 0.0;
        double defense = 0.0;
        double teamAbility = 0.0;

        QIcon form;

        QPixmap wldMinusFour = {};
        QPixmap wldMinusThree = {};
        QPixmap wldMinusTwo = {};
        QPixmap wldMinusOne = {};

    public:
        teams();
        teams(const QString, const int&, const int&, const int&, const int&,
              const int&, const int&, const int&, const int&, const double&, const double&, QIcon);

        void getTeamNameAndQualityValues(const QString, const double&, const double&, const double&);
        void increasesNumberOfGames();
        void increasePointsOnWinCase();
        void increasePointsOnDrawCase();
        void increaseLooses();
        void calculateGoals(const int&, const int&);
        void setFormIcons(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);

        static bool compare(const teams &a, const teams &b);
        void output(QTableWidget*, const int&);

        void fillFirstFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);
        void fillSecFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);
        void fillThirdFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);
        void fillFourthFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);
        void fillLastFormIcon(const int&, const int&, const QPixmap&, const QPixmap&, const QPixmap&);

        double convertOffenseValue();
        double convertDefenseValue();
        double convertTeamAbilityValue();
};

#endif // TEAMS_H
