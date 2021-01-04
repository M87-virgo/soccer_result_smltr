/*
 * Copyright © 2020 beyer341@onlinehome.de
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#include "sqlitedbeditor.h"
#include "ui_sqlitedbeditor.h"

#include <QSpinBox>
#include <QShortcut>

#include <QDebug>


void SqLiteDBEditor::TblRowDelete()
{
    if(ui -> LstTables -> currentRow() == 3)
    {
        tblLeaguesRowDelete();
    }
    else if(ui -> LstTables -> currentRow() == 4)
    {
        tblClubsRowDelete();
    }
    else if(ui -> LstTables -> currentRow() == 5)
    {
        tblNationRivalsRowDelete();
    }
    else if(ui -> LstTables -> currentRow() == 6)
    {
        tblClubRivalsRowDelete();
    }
}

void SqLiteDBEditor::tblLeaguesRowDelete()
{
    if(!selMdl -> selectedIndexes().isEmpty() )
    {
        QVector<int> ind;
        for(int i = 0; i < selMdl -> selectedIndexes().size(); ++i)
        {
            ind.push_back(ui -> TblDataLeagues -> model() -> data(selMdl -> selectedIndexes().at(i) ).toInt() );
        }

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete?", "Do you want to delete this record?",
                                            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            for(int i = 0; i < ind.size(); ++i)
            {
                sqlCommand.prepare("DELETE FROM leagues WHERE idleagues = ? ");
                sqlCommand.addBindValue(ind.at(i) );
                sqlCommand.exec();
            }
            TblDataCurrentRowChanged();
        }
        else
        {
            QMessageBox::information(this, "Not Deleted", "No data was deleted.");
        }
    }
    else
    {
        QMessageBox::warning(this, "No selection", "No rows selected!");
    }
}
void SqLiteDBEditor::tblClubsRowDelete()
{
    if(!selMdl -> selectedIndexes().isEmpty() )
    {
        QVector<int> ind;
        for(int i = 0; i < selMdl -> selectedIndexes().size(); ++i)
        {
            ind.push_back(ui -> TblDataClubs -> model() -> data(selMdl -> selectedIndexes().at(i) ).toInt() );
        }

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete?", "Do you want to delete this record?",
                                            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            for(int i = 0; i < ind.size(); ++i)
            {
                sqlCommand.prepare("DELETE FROM clubs WHERE idclubs = ? ");
                sqlCommand.addBindValue(ind.at(i) );
                sqlCommand.exec();
            }
            TblDataCurrentRowChanged();
        }
        else
        {
            QMessageBox::information(this, "Not Deleted", "No data was deleted.");
        }
    }
    else
    {
        QMessageBox::warning(this, "No selection", "No rows selected!");
    }
}
void SqLiteDBEditor::tblNationRivalsRowDelete()
{
    if(!selMdl -> selectedIndexes().isEmpty() )
    {
        QVector<int> ind;
        for(int i = 0; i < selMdl -> selectedIndexes().size(); ++i)
        {
            ind.push_back(ui -> TblDataNatRiv -> model() -> data(selMdl -> selectedIndexes().at(i) ).toInt() );
        }

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete?", "Do you want to delete this record?",
                                            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            for(int i = 0; i < ind.size(); ++i)
            {
                sqlCommand.prepare("DELETE FROM nationalteam_rivals WHERE idnationalteam_rivals = ? ");
                sqlCommand.addBindValue(ind.at(i) );
                sqlCommand.exec();
            }
            TblDataCurrentRowChanged();
        }
        else
        {
            QMessageBox::information(this, "Not Deleted", "No data was deleted.");
        }
    }
    else
    {
        QMessageBox::warning(this, "No selection", "No rows selected!");
    }
}
void SqLiteDBEditor::tblClubRivalsRowDelete()
{
    if(!selMdl -> selectedIndexes().isEmpty() )
    {
        QVector<int> ind;
        for(int i = 0; i < selMdl -> selectedIndexes().size(); ++i)
        {
            ind.push_back(ui -> TblDataRivClub -> model() -> data(selMdl -> selectedIndexes().at(i) ).toInt() );
        }

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete?", "Do you want to delete this record?",
                                            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            for(int i = 0; i < ind.size(); ++i)
            {
                sqlCommand.prepare("DELETE FROM club_rivals WHERE idclub_rivals = ? ");
                sqlCommand.addBindValue(ind.at(i) );
                sqlCommand.exec();
            }
            TblDataCurrentRowChanged();
        }
        else
        {
            QMessageBox::information(this, "Not Deleted", "No data was deleted.");
        }
    }
    else
    {
        QMessageBox::warning(this, "No selection", "No rows selected!");
    }
}

void SqLiteDBEditor::CmdUpdateClicked()
{
    if(ui -> LstTables -> currentRow() == 2)
    {
        tblNationsUpdate();

        dbTblinfo.setFile("mydb.db");
        ui -> LblLastUpdate -> setText(dbTblinfo.lastModified().toString() );
    }
    else if(ui -> LstTables -> currentRow() == 3)
    {
        tblLeaguesUpdate();
        fillFilterComboBoxes();

        dbTblinfo.setFile("mydb.db");
        ui -> LblLastUpdate -> setText(dbTblinfo.lastModified().toString() );
    }
    else if(ui -> LstTables -> currentRow() == 4)
    {
        tblClubsUpdate();

        dbTblinfo.setFile("mydb.db");
        ui -> LblLastUpdate -> setText(dbTblinfo.lastModified().toString() );
    }
    else if(ui -> LstTables -> currentRow() == 5)
    {
        tblNationRivalsUpdate();

        dbTblinfo.setFile("mydb.db");
        ui -> LblLastUpdate -> setText(dbTblinfo.lastModified().toString() );
    }
    else if(ui -> LstTables -> currentRow() == 6)
    {
        tblClubRivalsUpdate();

        dbTblinfo.setFile("mydb.db");
        ui -> LblLastUpdate -> setText(dbTblinfo.lastModified().toString() );
    }
}

QString SqLiteDBEditor::editStringFromNations(QString singleEditString, QString nation)
{
    QString totEdt;
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        if(qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> text() !=  ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 4) ).toString() )
        {
            nation = ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 1) ).toString();
            singleEditString = QString("Offensive Value from %1: ").arg(nation)
                 + ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 4) ).toString()
                 + " --> "
                 + qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> text()
                 + "\n";

            totEdt += singleEditString;
        }
        if(qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> text() !=  ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 5) ).toString() )
        {
            nation = ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 1) ).toString();
            singleEditString = QString("Defensive Value from %1: ").arg(nation)
                 + ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 5) ).toString()
                 + " --> "
                 + qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> text()
                 + "\n";

            totEdt += singleEditString;
        }
        if(qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> text() !=  ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 6) ).toString() )
        {
            nation = ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 1) ).toString();
            singleEditString = QString("Team Ability Value from %1: ").arg(nation)
                 + ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 6) ).toString()
                 + " --> "
                 + qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> text()
                 + "\n";

            totEdt += singleEditString;
        }
    }
    return totEdt;
}
void SqLiteDBEditor::tblNationsUpdate()
{
    QString snglEdt;
    QString nation;
    QString totEdt = editStringFromNations(snglEdt, nation);
    if(totEdt.isEmpty() )
    {
        QMessageBox::information(this, "No edits", "No edits found.");
    }
    else
    {
        QMessageBox::StandardButton rply = QMessageBox::question(this, "Update?", QString("Do you want to save the following edit? :\n\n%1").arg(totEdt),
                                            QMessageBox::Yes | QMessageBox::No);
        if(rply == QMessageBox::Yes)
        {
            for(int i = 0; i < model -> rowCount(); ++i)
            {
                if(qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> value() !=  ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 4) ).toInt() )
                {
                    int off = qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> value();

                    sqlCommand.prepare("UPDATE `nationalteams(countries)` SET offensive = ? WHERE fifa_code = ? ");
                    sqlCommand.addBindValue(off);
                    sqlCommand.addBindValue(ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 0) ).toString() );
                    sqlCommand.exec();
                }
                if(qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> value() !=  ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 5) ).toInt())
                {
                    int def = qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> value();

                    sqlCommand.prepare("UPDATE `nationalteams(countries)` SET defensive = ? WHERE fifa_code = ? ");
                    sqlCommand.addBindValue(def);
                    sqlCommand.addBindValue(ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 0) ).toString() );
                    sqlCommand.exec();
                }
                if(qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> value() !=  ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 6) ).toInt() )
                {
                    int ta = qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> value();

                    sqlCommand.prepare("UPDATE `nationalteams(countries)` SET teamability = ? WHERE fifa_code = ? ");
                    sqlCommand.addBindValue(ta);
                    sqlCommand.addBindValue(ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 0) ).toString() );
                    sqlCommand.exec();
                }
            }
            totEdt.clear();
        }
        else
        {
            QMessageBox::information(this, "No Update", "No edit was saved.");
        }
    }
}
void SqLiteDBEditor::tblLeaguesUpdate()
{
    QString edit;
    QString totalEdit;
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        if(qobject_cast<QLineEdit*> (ui -> TblDataLeagues -> indexWidget(lMdl -> index(i, 1) ) ) -> text() !=  ui -> TblDataLeagues -> model() -> data(lMdl -> index(i, 1) ).toString() )
        {
            edit = ui -> TblDataLeagues -> model() -> data(lMdl -> index(i, 1) ).toString()
                 + " --> "
                 + qobject_cast<QLineEdit*> (ui -> TblDataLeagues -> indexWidget(lMdl -> index(i, 1) ) ) -> text()
                 + "\n";

            totalEdit += edit;
        }
    }
    if(totalEdit.isEmpty() )
    {
        QMessageBox::information(this, "No edits", "No edits found.");
    }
    else
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Update?", QString("Do you want to save the following edit? :\n\n%1").arg(totalEdit),
                                            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            for(int i = 0; i < model -> rowCount(); ++i)
            {
                if(qobject_cast<QLineEdit*> (ui -> TblDataLeagues -> indexWidget(lMdl -> index(i, 1) ) ) -> text() !=  ui -> TblDataLeagues -> model() -> data(lMdl -> index(i, 1) ).toString() )
                {
                    QString leagueName = qobject_cast<QLineEdit*> (ui -> TblDataLeagues -> indexWidget(lMdl -> index(i, 1) ) ) -> text();

                    sqlCommand.prepare("UPDATE leagues SET name_league = ? WHERE idleagues = ? ");
                    sqlCommand.addBindValue(leagueName);
                    sqlCommand.addBindValue(ui -> TblDataLeagues -> model() -> data(lMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
            }
            totalEdit.clear();
        }
        else
        {
            QMessageBox::information(this, "No Update", "No edit was saved.");
        }
    }
}

QString SqLiteDBEditor::editStringFromClubs(QString singleEditString, QString clubName)
{
    QString totalEdt;
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        if(qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> text() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 1) ).toString() )
        {
            singleEditString = ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 1) ).toString()
                 + " --> "
                 + qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> text()
                 + "\n";

            totalEdt += singleEditString;
        }
        if(qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> text() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 2) ).toString() )
        {
            clubName = qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> text();
            singleEditString = QString("Offensive Value from %1: ").arg(clubName)
                 + ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 2) ).toString()
                 + " --> "
                 + qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> text()
                 + "\n";

            totalEdt += singleEditString;
        }
        if(qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> text() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 3) ).toString() )
        {
            clubName = qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> text();
            singleEditString = QString("Defensive Value from %1: ").arg(clubName)
                 + ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 3) ).toString()
                 + " --> "
                 + qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> text()
                 + "\n";

            totalEdt += singleEditString;
        }
        if(qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> text() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 4) ).toString() )
        {
            clubName = qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> text();
            singleEditString = QString("Team Ability Value from %1: ").arg(clubName)
                 + ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 4) ).toString()
                 + " --> "
                 + qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> text()
                 + "\n";

            totalEdt += singleEditString;
        }
        if(qobject_cast<QComboBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 5) ) ) -> currentText() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 5) ).toString() )
        {
            clubName = qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> text();
            singleEditString = QString("%1 change League from: ").arg(clubName)
                 + ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 5) ).toString()
                 + " to "
                 + qobject_cast<QComboBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 5) ) ) -> currentText()
                 + "\n";

            totalEdt += singleEditString;
        }
    }
    return totalEdt;
}
void SqLiteDBEditor::tblClubsUpdate()
{
    QString singleEdt;
    QString club;
    QString totalEdt = editStringFromClubs(singleEdt, club);
    if(totalEdt.isEmpty() )
    {
        QMessageBox::information(this, "No edits", "No edits found.");
    }
    else
    {
        QMessageBox::StandardButton rply = QMessageBox::question(this, "Update?", QString("Do you want to save the following edit? :\n\n%1").arg(totalEdt),
                                            QMessageBox::Yes | QMessageBox::No);
        if(rply == QMessageBox::Yes)
        {
            for(int i = 0; i < model -> rowCount(); ++i)
            {
                if(qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> text() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 1) ).toString() )
                {
                    QString clubName = qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> text();

                    sqlCommand.prepare("UPDATE clubs SET name_club = ? WHERE idclubs = ? ");
                    sqlCommand.addBindValue(clubName);
                    sqlCommand.addBindValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
                if(qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> value() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 2) ).toInt() )
                {
                    int off = qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> value();

                    sqlCommand.prepare("UPDATE clubs SET offensive = ? WHERE idclubs = ? ");
                    sqlCommand.addBindValue(off);
                    sqlCommand.addBindValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
                if(qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> value() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 3) ).toInt())
                {
                    int def = qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> value();

                    sqlCommand.prepare("UPDATE clubs SET defensive = ? WHERE idclubs = ? ");
                    sqlCommand.addBindValue(def);
                    sqlCommand.addBindValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
                if(qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> value() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 4) ).toInt() )
                {
                    int ta = qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> value();

                    sqlCommand.prepare("UPDATE clubs SET teamability = ? WHERE idclubs = ? ");
                    sqlCommand.addBindValue(ta);
                    sqlCommand.addBindValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
                if(qobject_cast<QComboBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 5) ) ) -> currentText() !=  ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 5) ).toInt() )
                {
                    QString league = qobject_cast<QComboBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 5) ) ) -> currentText();

                    sqlCommand.prepare("UPDATE clubs SET leagues_idleagues = (SELECT idleagues FROM leagues WHERE name_league = ?) WHERE idclubs = ? ");
                    sqlCommand.addBindValue(league);
                    sqlCommand.addBindValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
            }
            totalEdt.clear();
        }
        else
        {
            QMessageBox::information(this, "No Update", "No edit was saved.");
        }
    }
}
void SqLiteDBEditor::tblNationRivalsUpdate()
{
    QString edt;
    QString allEdt;
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        if(qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 1) ) ) -> currentText() !=  ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 1) ).toString() )
        {
            edt = ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 1) ).toString()
                 + " --> "
                 + qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 1) ) ) -> currentText()
                 + "\n";

            allEdt += edt;
        }
        if(qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 2) ) ) -> currentText() !=  ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 2) ).toString() )
        {
            edt = ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 2) ).toString()
                 + " --> "
                 + qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 2) ) ) -> currentText()
                 + "\n";

            allEdt += edt;
        }
    }
    if(allEdt.isEmpty() )
    {
        QMessageBox::information(this, "No edits", "No edits found.");
    }
    else
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Update?", QString("Do you want to save the following edit? :\n\n%1").arg(allEdt),
                                            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            for(int i = 0; i < model -> rowCount(); ++i)
            {
                if(qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 1) ) ) -> currentText() !=  ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 1) ).toString() )
                {
                    QString country = qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 1) ) ) -> currentText();

                    sqlCommand.prepare("UPDATE nationalteam_rivals SET team_fifa_code = (SELECT fifa_code FROM `nationalteams(countries)` WHERE name_country = ?) WHERE idnationalteam_rivals = ? ");
                    sqlCommand.addBindValue(country);
                    sqlCommand.addBindValue(ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
                if(qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 2) ) ) -> currentText() !=  ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 2) ).toString() )
                {
                    QString country = qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 2) ) ) -> currentText();

                    sqlCommand.prepare("UPDATE nationalteam_rivals SET rival_fifa_code = (SELECT fifa_code FROM `nationalteams(countries)` WHERE name_country = ?) WHERE idnationalteam_rivals = ? ");
                    sqlCommand.addBindValue(country);
                    sqlCommand.addBindValue(ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
            }
            allEdt.clear();
        }
        else
        {
            QMessageBox::information(this, "No Update", "No edit was saved.");
        }
    }
}
void SqLiteDBEditor::tblClubRivalsUpdate()
{
    QString mod;
    QString totalMod;
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        if(qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 1) ) ) -> currentText() !=  ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 1) ).toString() )
        {
            mod = ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 1) ).toString()
                 + " --> "
                 + qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 1) ) ) -> currentText()
                 + "\n";

            totalMod += mod;
        }
        if(qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 2) ) ) -> currentText() !=  ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 2) ).toString() )
        {
            mod = ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 2) ).toString()
                 + " --> "
                 + qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 2) ) ) -> currentText()
                 + "\n";

            totalMod += mod;
        }
    }
    if(totalMod.isEmpty() )
    {
        QMessageBox::information(this, "No edits", "No edits found.");
    }
    else
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Update?", QString("Do you want to save the following edit? :\n\n%1").arg(totalMod),
                                            QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            for(int i = 0; i < model -> rowCount(); ++i)
            {
                if(qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 1) ) ) -> currentText() !=  ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 1) ).toString() )
                {
                    QString club = qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 1) ) ) -> currentText();

                    sqlCommand.prepare("UPDATE club_rivals SET clubs_idclubs = (SELECT idclubs FROM clubs WHERE name_club = ?) WHERE idclub_rivals = ? ");
                    sqlCommand.addBindValue(club);
                    sqlCommand.addBindValue(ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
                if(qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 2) ) ) -> currentText() !=  ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 2) ).toString() )
                {
                    QString club = qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 2) ) ) -> currentText();

                    sqlCommand.prepare("UPDATE club_rivals SET clubs_idclubs1 = (SELECT idclubs FROM clubs WHERE name_club = ?) WHERE idclub_rivals = ? ");
                    sqlCommand.addBindValue(club);
                    sqlCommand.addBindValue(ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 0) ).toInt() );
                    sqlCommand.exec();
                }
            }
            totalMod.clear();
        }
        else
        {
            QMessageBox::information(this, "No Update", "No edit was saved.");
        }
    }
}

void SqLiteDBEditor::TblRowClicked(const QModelIndex &index)
{
    if(ui -> LstTables -> currentRow() == 2)
    {
        ui -> TblDataNations -> setCurrentIndex(cntryMdl -> index(index.row(), 0) );
        ui -> TblDataNations -> selectRow(index.row() );
    }
    else if(ui -> LstTables -> currentRow() == 3)
    {
        ui -> TblDataLeagues -> setCurrentIndex(lMdl -> index(index.row(), 0) );
        ui -> TblDataLeagues -> selectRow(index.row() );
    }
    else if(ui -> LstTables -> currentRow() == 4)
    {
        ui -> TblDataClubs -> setCurrentIndex(cMdl -> index(index.row(), 0) );
        ui -> TblDataClubs -> selectRow(index.row() );
    }
    else if(ui -> LstTables -> currentRow() == 5)
    {
        ui -> TblDataNatRiv -> setCurrentIndex(nrMdl -> index(index.row(), 0) );
        ui -> TblDataNatRiv -> selectRow(index.row() );
    }
    else if(ui -> LstTables -> currentRow() == 6)
    {
        ui -> TblDataRivClub -> setCurrentIndex(crMdl -> index(index.row(), 0) );
        ui -> TblDataRivClub -> selectRow(index.row() );
    }
}

void SqLiteDBEditor::TblDataCurrentRowChanged()
{
    if(ui -> LstTables -> currentRow() < 3)
    {
        hideInputLayout();
    }

    if(ui -> LstTables -> currentRow() == 0)
    {
        showContinentTable();
    }
    else if(ui -> LstTables -> currentRow() == 1)
    {
        showSubContinentTable();
    }
    else if(ui -> LstTables -> currentRow() == 2)
    {   
        sqlCmdShowNations = "SELECT `nationalteams(countries)`.fifa_code AS `FIFA CODE`, `nationalteams(countries)`.name_country AS Country, "
                            "subcontinents.name_subcontinent AS Subcontinent, continents.name_continent AS Continent, `nationalteams(countries)`.offensive AS Off, "
                            "`nationalteams(countries)`.defensive AS Def, `nationalteams(countries)`.teamability AS TeamAbility FROM `nationalteams(countries)` "
                            "INNER JOIN subcontinents ON `nationalteams(countries)`.subcontinents_idsubcontinents = subcontinents.idsubcontinents "
                            "INNER JOIN continents ON `nationalteams(countries)`.continents_idcontinents = continents.idcontinents "
                            "WHERE subcontinents.name_subcontinent = '%1'";
        sqlCmdShowNations = sqlCmdShowNations.arg(ui -> CmbSubContinents -> currentText() );
        ui -> CmdNoFilterSubCont -> setText(QString("show all nations\nfrom\n%1").arg(ui -> CmbContinents -> currentText() ) );
        showNationsTable();
    }
    else if(ui -> LstTables -> currentRow() == 3)
    {
        showLeagueTable();
        showLeagueInputLayout();
    }
    else if(ui -> LstTables -> currentRow() == 4)
    {
        sqlCmdClubTable = "SELECT clubs.idclubs AS ClubId, clubs.name_club AS Club, clubs.offensive AS Off, clubs.defensive AS Def, clubs.teamability AS Teamability, "
                            "leagues.name_league AS League, `nationalteams(countries)`.name_country AS Country "
                            "FROM clubs INNER JOIN leagues ON clubs.leagues_idleagues = leagues.idleagues INNER JOIN `nationalteams(countries)` ON "
                            "leagues.fifa_code = `nationalteams(countries)`.fifa_code INNER JOIN subcontinents ON leagues.subcontinents_idsubcontinents = subcontinents.idsubcontinents "
                            "INNER JOIN continents ON leagues.continents_idcontinents = continents.idcontinents WHERE leagues.name_league = '%1'";
        sqlCmdClubTable = sqlCmdClubTable.arg(ui -> CmbLeaguesFilter -> currentText() );
        showClubTable();
        setSizeAndLookOfClubTable();
        showClubInputLayout();
    }
    else if(ui -> LstTables -> currentRow() == 5)
    {
        showNationRivalsTable();
        showNationRivalsLayoutAndSetLook();
    }
    else if(ui -> LstTables -> currentRow() == 6)
    {
        showClubRivalsTable();
        showClubRivalsLayout();
    }
}
void SqLiteDBEditor::CmbLeaguesFilterCurrentIndexChanged()
{
    sqlCmdClubTable = QString ("SELECT clubs.idclubs AS ClubId, clubs.name_club AS Club, clubs.offensive AS Off, clubs.defensive AS Def, clubs.teamability AS Teamability, "
                              "leagues.name_league AS League, `nationalteams(countries)`.name_country AS Country "
                              "FROM clubs INNER JOIN leagues ON clubs.leagues_idleagues = leagues.idleagues INNER JOIN `nationalteams(countries)` ON "
                              "leagues.fifa_code = `nationalteams(countries)`.fifa_code INNER JOIN subcontinents ON leagues.subcontinents_idsubcontinents = subcontinents.idsubcontinents "
                              "INNER JOIN continents ON leagues.continents_idcontinents = continents.idcontinents WHERE leagues.name_league = '%1'").arg(ui -> CmbLeaguesFilter -> currentText() );
    showClubTable();
    setSizeAndLookOfClubTable();
    ui -> CmdNoFilterClubs -> setEnabled(true);
    ui -> LblNumberOfRows -> setText(QString("%1 Teams").arg(ui -> TblDataClubs -> model() -> rowCount() ) );
}
void SqLiteDBEditor::CmdNoFilterClubsClicked()
{
    sqlCmdClubTable = "SELECT clubs.idclubs AS ClubId, clubs.name_club AS Club, clubs.offensive AS Off, clubs.defensive AS Def, clubs.teamability AS Teamability, "
                        "leagues.name_league AS League, `nationalteams(countries)`.name_country AS Country "
                        "FROM clubs INNER JOIN leagues ON clubs.leagues_idleagues = leagues.idleagues INNER JOIN `nationalteams(countries)` ON "
                        "leagues.fifa_code = `nationalteams(countries)`.fifa_code INNER JOIN subcontinents ON leagues.subcontinents_idsubcontinents = subcontinents.idsubcontinents "
                        "INNER JOIN continents ON leagues.continents_idcontinents = continents.idcontinents";
    showClubTable();
    setSizeAndLookOfClubTable();
    ui -> CmdNoFilterClubs -> setDisabled(true);
    ui -> LblNumberOfRows -> setText(QString("%1 Teams").arg(ui -> TblDataClubs -> model() -> rowCount() ) );
}
void SqLiteDBEditor::CmdNoFilterSubContClicked()
{
    sqlCmdShowNations = QString ("SELECT `nationalteams(countries)`.fifa_code AS `FIFA CODE`, `nationalteams(countries)`.name_country AS Country, "
                        "subcontinents.name_subcontinent AS Subcontinent, continents.name_continent AS Continent, `nationalteams(countries)`.offensive AS Off, "
                        "`nationalteams(countries)`.defensive AS Def, `nationalteams(countries)`.teamability AS TeamAbility FROM `nationalteams(countries)` "
                        "INNER JOIN subcontinents ON `nationalteams(countries)`.subcontinents_idsubcontinents = subcontinents.idsubcontinents "
                        "INNER JOIN continents ON `nationalteams(countries)`.continents_idcontinents = continents.idcontinents WHERE "
                        "continents.name_continent = '%1'").arg(ui -> CmbContinents -> currentText() );
    showNationsTable();
    ui -> CmdNoFilterSubCont -> setDisabled(true);
    ui -> LblNumberOfRows -> setText(QString("%1 Teams").arg(ui -> TblDataNations -> model() -> rowCount() ) );
}

void SqLiteDBEditor::hideInputLayout()
{
    ui -> TblAddNewClubs -> setHidden(true);
    ui -> CmbLeaguesForAll -> hide();

    ui -> LblAddLeague -> setHidden(true);
    ui -> LblLeagueName -> setHidden(true);
    ui -> LblCountry -> setHidden(true);
    ui -> LblFiCo -> setHidden(true);
    ui -> LblSubCon -> setHidden(true);
    ui -> LblCon -> setHidden(true);
    ui -> EdtLeagueName -> setHidden(true);
    ui -> CmbCountry -> setHidden(true);
    ui -> LblFifaCode -> setHidden(true);
    ui -> LblSubCont -> setHidden(true);
    ui -> LblContin -> setHidden(true);
    ui -> EdtSearch -> setHidden(true);
    ui -> LblSearch -> setHidden(true);
    ui -> CmdSave -> setHidden(true);

    ui -> LblAddClub -> setHidden(true);

    ui -> LblAddNatRival -> setHidden(true);
    ui -> LblNatTeam -> setHidden(true);
    ui -> CmbNatTeam -> setHidden(true);
    ui -> LblRival -> setHidden(true);
    ui -> CmbNatRival -> setHidden(true);
    ui -> CmdSaveRivals -> setHidden(true);

    ui -> LblAddClubRival -> setHidden(true);
    ui -> LblClubTeam -> setHidden(true);
    ui -> CmbClub -> setHidden(true);
    ui -> LblClubRival -> setHidden(true);
    ui -> CmbClubRival -> setHidden(true);
    ui -> CmdUpdate -> setDisabled(true);

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterClubs -> hide();
}
void SqLiteDBEditor::showLeagueInputLayout()
{
    ui -> TblAddNewClubs -> setHidden(true);
    ui -> CmbLeaguesForAll -> hide();

    ui -> LblNumberOfRows -> hide();
    ui -> LblAddLeague -> setHidden(false);
    ui -> LblLeagueName -> setHidden(false);
    ui -> LblCountry -> setHidden(false);
    ui -> LblFiCo -> setHidden(false);
    ui -> LblSubCon -> setHidden(false);
    ui -> LblCon -> setHidden(false);
    ui -> EdtLeagueName -> setHidden(false);
    ui -> CmbCountry -> setHidden(false);
    ui -> LblFifaCode -> setHidden(false);
    ui -> LblSubCont -> setHidden(false);
    ui -> LblContin -> setHidden(false);
    ui -> EdtSearch -> setHidden(false);
    ui -> LblSearch -> setHidden(false);
    ui -> CmdSave -> setHidden(false);

    ui -> LblAddClub -> setHidden(true);

    ui -> LblAddNatRival -> setHidden(true);
    ui -> LblNatTeam -> setHidden(true);
    ui -> CmbNatTeam -> setHidden(true);
    ui -> LblRival -> setHidden(true);
    ui -> CmbNatRival -> setHidden(true);
    ui -> CmdSaveRivals -> setHidden(true);

    ui -> LblAddClubRival -> setHidden(true);
    ui -> LblClubTeam -> setHidden(true);
    ui -> CmbClub -> setHidden(true);
    ui -> LblClubRival -> setHidden(true);
    ui -> CmbClubRival -> setHidden(true);
    ui -> CmdUpdate -> setEnabled(true);

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterClubs -> hide();
}
void SqLiteDBEditor::showClubInputLayout()
{    
    ui -> TblAddNewClubs -> setHidden(false);
    ui -> CmbLeaguesForAll -> show();

    ui -> LblAddClub -> setHidden(false);
    ui -> LblNumberOfRows -> show();

    ui -> LblAddLeague -> setHidden(true);
    ui -> LblLeagueName -> setHidden(true);
    ui -> LblCountry -> setHidden(true);
    ui -> LblFiCo -> setHidden(true);
    ui -> LblSubCon -> setHidden(true);
    ui -> LblCon -> setHidden(true);
    ui -> EdtLeagueName -> setHidden(true);
    ui -> CmbCountry -> setHidden(true);
    ui -> LblFifaCode -> setHidden(true);
    ui -> LblSubCont -> setHidden(true);
    ui -> LblContin -> setHidden(true);
    ui -> EdtSearch -> setHidden(true);
    ui -> LblSearch -> setHidden(true);
    ui -> CmdSave -> setHidden(false);

    ui -> LblAddNatRival -> setHidden(true);
    ui -> LblNatTeam -> setHidden(true);
    ui -> CmbNatTeam -> setHidden(true);
    ui -> LblRival -> setHidden(true);
    ui -> CmbNatRival -> setHidden(true);
    ui -> CmdSaveRivals -> setHidden(true);

    ui -> LblAddClubRival -> setHidden(true);
    ui -> LblClubTeam -> setHidden(true);
    ui -> CmbClub -> setHidden(true);
    ui -> LblClubRival -> setHidden(true);
    ui -> CmbClubRival -> setHidden(true);
    ui -> CmdUpdate -> setEnabled(true);
}
void SqLiteDBEditor::showNationRivalsLayoutAndSetLook()
{
    ui -> TblDataNatRiv -> setColumnWidth(0, 90);
    ui -> TblDataNatRiv -> setColumnWidth(1, 350);
    ui -> TblDataNatRiv -> setColumnWidth(2, 350);
    ui -> TblDataNatRiv -> setStyleSheet(styleSheetEditable);

    ui -> LblNumberOfRows -> hide();
    ui -> TblAddNewClubs -> setHidden(true);
    ui -> CmbLeaguesForAll -> hide();

    ui -> LblAddNatRival -> setHidden(false);
    ui -> LblNatTeam -> setHidden(false);
    ui -> CmbNatTeam -> setHidden(false);
    ui -> LblRival -> setHidden(false);
    ui -> CmbNatRival -> setHidden(false);
    ui -> CmdSaveRivals -> setHidden(false);


    ui -> LblAddLeague -> setHidden(true);
    ui -> LblLeagueName -> setHidden(true);
    ui -> LblCountry -> setHidden(true);
    ui -> LblFiCo -> setHidden(true);
    ui -> LblSubCon -> setHidden(true);
    ui -> LblCon -> setHidden(true);
    ui -> EdtLeagueName -> setHidden(true);
    ui -> CmbCountry -> setHidden(true);
    ui -> LblFifaCode -> setHidden(true);
    ui -> LblSubCont -> setHidden(true);
    ui -> LblContin -> setHidden(true);
    ui -> EdtSearch -> setHidden(true);
    ui -> LblSearch -> setHidden(true);
    ui -> CmdSave -> setHidden(true);

    ui -> LblAddClub -> setHidden(true);

    ui -> LblAddClubRival -> setHidden(true);
    ui -> LblClubTeam -> setHidden(true);
    ui -> CmbClub -> setHidden(true);
    ui -> LblClubRival -> setHidden(true);
    ui -> CmbClubRival -> setHidden(true);
    ui -> CmdUpdate -> setEnabled(true);

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterClubs -> hide();
    ui -> CmdNoFilterSubCont -> hide();
}
void SqLiteDBEditor::setSizeAndLookOfClubTable()
{
    ui -> TblDataClubs -> setColumnWidth(0, 90);
    ui -> TblDataClubs -> setColumnWidth(1, 200);
    ui -> TblDataClubs -> setColumnWidth(2, 100);
    ui -> TblDataClubs -> setColumnWidth(3, 100);
    ui -> TblDataClubs -> setColumnWidth(4, 140);
    ui -> TblDataClubs -> setColumnWidth(5, 300);
    ui -> TblDataClubs -> setColumnWidth(6, 320);
    ui -> TblDataClubs -> setStyleSheet(styleSheetEditable);
}
void SqLiteDBEditor::showClubRivalsLayout()
{
    ui -> TblAddNewClubs -> setHidden(true);
    ui -> CmbLeaguesForAll -> hide();

    ui -> LblNumberOfRows -> hide();
    ui -> LblAddClubRival -> setHidden(false);
    ui -> LblClubTeam -> setHidden(false);
    ui -> CmbClub -> setHidden(false);
    ui -> LblClubRival -> setHidden(false);
    ui -> CmbClubRival -> setHidden(false);
    ui -> CmdSaveRivals -> setHidden(false);

    ui -> LblAddLeague -> setHidden(true);
    ui -> LblLeagueName -> setHidden(true);
    ui -> LblCountry -> setHidden(true);
    ui -> LblFiCo -> setHidden(true);
    ui -> LblSubCon -> setHidden(true);
    ui -> LblCon -> setHidden(true);
    ui -> EdtLeagueName -> setHidden(true);
    ui -> CmbCountry -> setHidden(true);
    ui -> LblFifaCode -> setHidden(true);
    ui -> LblSubCont -> setHidden(true);
    ui -> LblContin -> setHidden(true);
    ui -> EdtSearch -> setHidden(true);
    ui -> LblSearch -> setHidden(true);
    ui -> CmdSave -> setHidden(true);

    ui -> LblAddClub -> setHidden(true);

    ui -> LblAddNatRival -> setHidden(true);
    ui -> LblNatTeam -> setHidden(true);
    ui -> CmbNatTeam -> setHidden(true);
    ui -> LblRival -> setHidden(true);
    ui -> CmbNatRival -> setHidden(true);
    ui -> CmdUpdate -> setEnabled(true);

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterClubs -> hide();
    ui -> CmdNoFilterSubCont -> hide();
}

void SqLiteDBEditor::showContinentTable()
{
    ui -> TblDataCont -> raise();
    ui -> TblDataCont -> setHidden(false);

    ui -> TblDataSubCont -> setHidden(true);
    ui -> TblDataNations -> setHidden(true);
    ui -> TblDataLeagues -> setHidden(true);
    ui -> TblDataClubs -> setHidden(true);
    ui -> TblDataNatRiv -> setHidden(true);
    ui -> TblDataRivClub -> setHidden(true);

    ui -> LblNumberOfRows -> hide();

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterClubs -> hide();
    ui -> CmdNoFilterSubCont -> hide();

    sqlCommand.exec("SELECT name_continent AS Continent FROM continents");
    model -> setQuery(sqlCommand);

    contMdl -> setDynamicSortFilter(true);
    contMdl -> setSourceModel(model);

    ui -> TblDataCont -> setModel(contMdl);
    ui -> TblDataCont -> setColumnWidth(0, 250);
    ui -> TblDataCont -> setStyleSheet(styleSheetNotEditable);
}
void SqLiteDBEditor::showSubContinentTable()
{
    ui -> TblDataSubCont -> raise();
    ui -> TblDataSubCont -> setHidden(false);

    ui -> LblNumberOfRows -> hide();

    ui -> TblDataCont -> setHidden(true);
    ui -> TblDataNations -> setHidden(true);
    ui -> TblDataLeagues -> setHidden(true);
    ui -> TblDataClubs -> setHidden(true);
    ui -> TblDataNatRiv -> setHidden(true);
    ui -> TblDataRivClub -> setHidden(true);

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterClubs -> hide();
    ui -> CmdNoFilterSubCont -> hide();

    sqlCommand.exec("SELECT subcontinents.name_subcontinent AS Subcontinent, continents.name_continent AS Continent FROM subcontinents "
                       "INNER JOIN continents ON subcontinents.continents_idcontinents = continents.idcontinents");
    model -> setQuery(sqlCommand);

    sucoMdl -> setDynamicSortFilter(true);
    sucoMdl -> setSourceModel(model);

    ui -> TblDataSubCont -> setModel(sucoMdl);
    ui -> TblDataSubCont -> setColumnWidth(0, 180);
    ui -> TblDataSubCont -> setColumnWidth(1, 250);

    ui -> TblDataSubCont -> setStyleSheet(styleSheetNotEditable);
}
void SqLiteDBEditor::showNationsTable()
{
    ui -> TblDataNations -> raise();
    ui -> TblDataNations -> setHidden(false);

    ui -> LblNumberOfRows -> show();

    ui -> TblDataCont -> setHidden(true);
    ui -> TblDataSubCont -> setHidden(true);
    ui -> TblDataLeagues -> setHidden(true);
    ui -> TblDataClubs -> setHidden(true);
    ui -> TblDataNatRiv -> setHidden(true);
    ui -> TblDataRivClub -> setHidden(true);

    ui -> CmdUpdate -> setEnabled(true);

    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterClubs -> hide();

    ui -> CmbContinents -> show();
    ui -> CmbSubContinents -> show();
    ui -> CmdNoFilterNat -> show();
    ui -> CmdNoFilterSubCont -> show();
    ui -> CmdNoFilterNat -> setEnabled(true);

    sqlCommand.exec(sqlCmdShowNations);
    model -> setQuery(sqlCommand);

    cntryMdl -> setSourceModel(model);
    ui -> TblDataNations -> setModel(cntryMdl);
    selMdl -> setModel(cntryMdl);
    ui -> TblDataNations -> setSelectionModel(selMdl);

    nationTableCastToSpinBox();

    cntryMdl -> setDynamicSortFilter(true);

    ui -> TblDataNations -> setColumnWidth(0, 137);
    ui -> TblDataNations -> setColumnWidth(1, 320);
    ui -> TblDataNations -> setColumnWidth(2, 180);
    ui -> TblDataNations -> setColumnWidth(3, 260);
    ui -> TblDataNations -> setColumnWidth(4, 100);
    ui -> TblDataNations -> setColumnWidth(5, 100);
    ui -> TblDataNations -> setColumnWidth(6, 150);
    ui -> TblDataNations -> setStyleSheet(styleSheetEditable);
    ui -> LblNumberOfRows -> setText(QString("%1 Teams").arg(ui -> TblDataNations -> model() -> rowCount() ) );
}
void SqLiteDBEditor::CmbFilterContCurrentIndexChanged()
{
    sqlCmdShowNations = QString ("SELECT `nationalteams(countries)`.fifa_code AS `FIFA CODE`, `nationalteams(countries)`.name_country AS Country, "
                        "subcontinents.name_subcontinent AS Subcontinent, continents.name_continent AS Continent, `nationalteams(countries)`.offensive AS Off, "
                        "`nationalteams(countries)`.defensive AS Def, `nationalteams(countries)`.teamability AS TeamAbility FROM `nationalteams(countries)` "
                        "INNER JOIN subcontinents ON `nationalteams(countries)`.subcontinents_idsubcontinents = subcontinents.idsubcontinents "
                        "INNER JOIN continents ON `nationalteams(countries)`.continents_idcontinents = continents.idcontinents WHERE "
                        "continents.name_continent = '%1'").arg(ui -> CmbContinents -> currentText() );
    showNationsTable();
    sqlCmd.exec(QString("SELECT name_subcontinent FROM subcontinents WHERE continents_idcontinents = "
                        "(SELECT idcontinents FROM continents WHERE name_continent = '%1')").arg(ui -> CmbContinents -> currentText() ) );
    ui -> CmbSubContinents -> clear();
    while(sqlCmd.next() )
    {
        QString nameOfSubCont = sqlCmd.value(0).toString();
        ui -> CmbSubContinents -> addItem(nameOfSubCont);
    }
    ui -> CmdNoFilterNat -> setEnabled(true);
    ui -> CmdNoFilterSubCont -> setEnabled(true);
    ui -> CmdNoFilterSubCont -> setText(QString("show all nations\nfrom\n%1").arg(ui -> CmbContinents -> currentText() ) );
    ui -> LblNumberOfRows -> setText(QString("%1 Teams").arg(ui -> TblDataNations -> model() -> rowCount() ) );
}
void SqLiteDBEditor::CmbFilterSubContCurrentIndexChanged()
{
    sqlCmdShowNations = QString ("SELECT `nationalteams(countries)`.fifa_code AS `FIFA CODE`, `nationalteams(countries)`.name_country AS Country, "
                                 "subcontinents.name_subcontinent AS Subcontinent, continents.name_continent AS Continent, `nationalteams(countries)`.offensive AS Off, "
                                 "`nationalteams(countries)`.defensive AS Def, `nationalteams(countries)`.teamability AS TeamAbility FROM `nationalteams(countries)` "
                                 "INNER JOIN subcontinents ON `nationalteams(countries)`.subcontinents_idsubcontinents = subcontinents.idsubcontinents "
                                 "INNER JOIN continents ON `nationalteams(countries)`.continents_idcontinents = continents.idcontinents WHERE "
                                 "subcontinents.name_subcontinent = '%1'").arg(ui -> CmbSubContinents -> currentText() );
    showNationsTable();
    ui -> CmdNoFilterSubCont -> setEnabled(true);
    ui -> LblNumberOfRows -> setText(QString("%1 Teams").arg(ui -> TblDataNations -> model() -> rowCount() ) );
}
void SqLiteDBEditor::CmdNoFilterNatClicked()
{
    sqlCmdShowNations = "SELECT `nationalteams(countries)`.fifa_code AS `FIFA CODE`, `nationalteams(countries)`.name_country AS Country, "
                                "subcontinents.name_subcontinent AS Subcontinent, continents.name_continent AS Continent, `nationalteams(countries)`.offensive AS Off, "
                                "`nationalteams(countries)`.defensive AS Def, `nationalteams(countries)`.teamability AS TeamAbility FROM `nationalteams(countries)` "
                                "INNER JOIN subcontinents ON `nationalteams(countries)`.subcontinents_idsubcontinents = subcontinents.idsubcontinents "
                                "INNER JOIN continents ON `nationalteams(countries)`.continents_idcontinents = continents.idcontinents ";
    showNationsTable();
    ui -> CmdNoFilterNat -> setDisabled(true);
    ui -> CmdNoFilterSubCont -> setEnabled(true);
    ui -> LblNumberOfRows -> setText(QString("%1 Teams").arg(ui -> TblDataNations -> model() -> rowCount() ) );
}

void SqLiteDBEditor::nationTableCastToSpinBox()
{
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        ui -> TblDataNations -> setIndexWidget(cntryMdl -> index(i, 4), new QSpinBox);
        ui -> TblDataNations -> setIndexWidget(cntryMdl -> index(i, 5), new QSpinBox);
        ui -> TblDataNations -> setIndexWidget(cntryMdl -> index(i, 6), new QSpinBox);

        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> setStyleSheet("color: black; "
                                                                                                                  "background-color: coral; "
                                                                                                         "font: 21px Times New Roman");
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> setAlignment(Qt::AlignCenter);
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> setValue(ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 4) ).toInt() );
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> setMinimum(20);
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 4) ) ) -> setMaximum(99);

        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> setStyleSheet("color: black; "
                                                                                                                  "background-color: coral; "
                                                                                                         "font: 21px Times New Roman");
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> setAlignment(Qt::AlignCenter);
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> setValue(ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 5) ).toInt() );
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> setMinimum(20);
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 5) ) ) -> setMaximum(99);

        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> setStyleSheet("color: black; "
                                                                                                                  "background-color: coral; "
                                                                                                         "font: 21px Times New Roman");
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> setAlignment(Qt::AlignCenter);
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> setValue(ui -> TblDataNations -> model() -> data(cntryMdl -> index(i, 6) ).toInt() );
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> setMinimum(20);
        qobject_cast<QSpinBox*> (ui -> TblDataNations -> indexWidget(cntryMdl -> index(i, 6) ) ) -> setMaximum(99);
    }
}
void SqLiteDBEditor::showLeagueTable()
{
    ui -> EdtSearch -> clear();
    ui -> EdtLeagueName -> clear();

    ui -> TblDataLeagues -> raise();
    ui -> TblDataLeagues -> setHidden(false);

    ui -> LblNumberOfRows -> hide();

    ui -> TblDataCont -> setHidden(true);
    ui -> TblDataSubCont -> setHidden(true);
    ui -> TblDataNations -> setHidden(true);
    ui -> TblDataClubs -> setHidden(true);
    ui -> TblDataNatRiv -> setHidden(true);
    ui -> TblDataRivClub -> setHidden(true);

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterClubs -> hide();
    ui -> CmdNoFilterSubCont -> hide();

    sqlCommand.exec("SELECT leagues.idleagues AS LeagueID, leagues.name_league AS League, `nationalteams(countries)`.name_country AS Country "
                        "FROM leagues INNER JOIN `nationalteams(countries)` ON leagues.fifa_code = `nationalteams(countries)`.fifa_code");
    model -> setQuery(sqlCommand);

    lMdl -> setSourceModel(model);
    ui -> TblDataLeagues -> setModel(lMdl);
    selMdl -> setModel(lMdl);
    ui -> TblDataLeagues -> setSelectionModel(selMdl);

    for(int i = 0; i < model -> rowCount(); ++i)
    {
        ui -> TblDataLeagues -> setIndexWidget(lMdl -> index(i, 1), new QLineEdit);

        qobject_cast<QLineEdit*> (ui -> TblDataLeagues -> indexWidget(lMdl -> index(i, 1) ) ) -> setStyleSheet("color: black; "
                                                                                                               "background-color: lightcoral; "
                                                                                                            "font: 21px Times New Roman");
        qobject_cast<QLineEdit*> (ui -> TblDataLeagues -> indexWidget(lMdl -> index(i, 1) ) ) -> setText(ui -> TblDataLeagues -> model() -> data(lMdl -> index(i, 1) ).toString() );

    }
    lMdl -> setDynamicSortFilter(true);

    ui -> TblDataLeagues -> setColumnWidth(0, 120);
    ui -> TblDataLeagues -> setColumnWidth(1, 300);
    ui -> TblDataLeagues -> setColumnWidth(2, 320);
    ui -> TblDataLeagues -> setStyleSheet(styleSheetEditable);
}
void SqLiteDBEditor::showClubTable()
{
    ui -> TblDataClubs -> raise();
    ui -> TblDataClubs -> setHidden(false);

    ui -> LblNumberOfRows -> show();

    ui -> TblDataCont -> setHidden(true);
    ui -> TblDataSubCont -> setHidden(true);
    ui -> TblDataNations -> setHidden(true);
    ui -> TblDataLeagues -> setHidden(true);
    ui -> TblDataNatRiv -> setHidden(true);
    ui -> TblDataRivClub -> setHidden(true);

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterSubCont -> hide();

    ui -> CmbLeaguesFilter -> show();
    ui -> CmdNoFilterClubs -> show();
    ui -> CmdNoFilterClubs -> setEnabled(true);

    sqlCommand.exec(sqlCmdClubTable);
    model -> setQuery(sqlCommand);

    cMdl -> setSourceModel(model);
    ui -> TblDataClubs -> setModel(cMdl);
    selMdl -> setModel(cMdl);
    ui -> TblDataClubs -> setSelectionModel(selMdl);

    QSqlQuery sqlShowLeaguesPerCountry;
    sqlShowLeaguesPerCountry.prepare("SELECT name_league FROM leagues WHERE fifa_code = "
                                     "(SELECT `nationalteams(countries)`.fifa_code FROM `nationalteams(countries)` WHERE name_country = ?) ");
    QString leagueFromCountry;
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        ui -> TblDataClubs -> setIndexWidget(cMdl -> index(i, 1), new QLineEdit);
        ui -> TblDataClubs -> setIndexWidget(cMdl -> index(i, 2), new QSpinBox);
        ui -> TblDataClubs -> setIndexWidget(cMdl -> index(i, 3), new QSpinBox);
        ui -> TblDataClubs -> setIndexWidget(cMdl -> index(i, 4), new QSpinBox);
        ui -> TblDataClubs -> setIndexWidget(cMdl -> index(i, 5), new QComboBox);

        qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> setStyleSheet("color: black; "
                                                                                                             "background-color: lightcoral; "
                                                                                                          "font: 21px Times New Roman");
        qobject_cast<QLineEdit*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 1) ) ) -> setText(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 1) ).toString() );

        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> setStyleSheet("color: black; "
                                                                                                            "background-color: coral; "
                                                                                                         "font: 21px Times New Roman");
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> setAlignment(Qt::AlignCenter);
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> setValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 2) ).toInt() );
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> setMinimum(20);
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 2) ) ) -> setMaximum(99);

        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> setStyleSheet("color: black; "
                                                                                                            "background-color: coral; "
                                                                                                         "font: 21px Times New Roman");
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> setAlignment(Qt::AlignCenter);
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> setValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 3) ).toInt() );
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> setMinimum(20);
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 3) ) ) -> setMaximum(99);

        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> setStyleSheet("color: black; "
                                                                                                            "background-color: coral; "
                                                                                                         "font: 21px Times New Roman");
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> setAlignment(Qt::AlignCenter);
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> setValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 4) ).toInt() );
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> setMinimum(20);
        qobject_cast<QSpinBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 4) ) ) -> setMaximum(99);

        qobject_cast<QComboBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 5) ) ) ->
                setStyleSheet("color: black; "
                              "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 coral, stop: 1 indianred); "
                              "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 blue, stop: 1 lightblue); "
                              "font: 21px Times New Roman");
        sqlShowLeaguesPerCountry.addBindValue(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 6) ).toString() );
        sqlShowLeaguesPerCountry.exec();
        while(sqlShowLeaguesPerCountry.next() )
        {
            leagueFromCountry = "";
            leagueFromCountry += sqlShowLeaguesPerCountry.value(0).toString();
            qobject_cast<QComboBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 5) ) ) -> addItem(leagueFromCountry);
        }
        qobject_cast<QComboBox*> (ui -> TblDataClubs -> indexWidget(cMdl -> index(i, 5) ) ) -> setCurrentText(ui -> TblDataClubs -> model() -> data(cMdl -> index(i, 5) ).toString() );
    }
    cMdl -> setDynamicSortFilter(true);

    ui -> TblAddNewClubs -> clear();
    setUpNewClubsTblWidget();
    connect(qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(0, 4) ), SIGNAL(currentIndexChanged(int) ), SLOT(CmbLgTblAddNewClubCurrentIndexChanged() ) );
    connect(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(0, 0) ), SIGNAL(returnPressed() ), SLOT(TblAddNewClubInsertRow() ) );
    connect(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(0, 0) ), SIGNAL(textChanged(QString) ), SLOT(EdtClubNameTextChanged() ) );
    ui -> LblNumberOfRows -> setText(QString("%1 Teams").arg(ui -> TblDataClubs -> model() -> rowCount() ) );
}
void SqLiteDBEditor::showNationRivalsTable()
{
    ui -> TblDataNatRiv -> raise();
    ui -> TblDataNatRiv -> setHidden(false);

    ui -> TblDataCont -> setHidden(true);
    ui -> TblDataSubCont -> setHidden(true);
    ui -> TblDataNations -> setHidden(true);
    ui -> TblDataLeagues -> setHidden(true);
    ui -> TblDataClubs -> setHidden(true);
    ui -> TblDataRivClub -> setHidden(true);

    sqlCommand.exec("SELECT idnationalteam_rivals AS id_NRi, nc.name_country AS Team, nac.name_country AS Rival FROM nationalteam_rivals AS nriv "
                       "JOIN `nationalteams(countries)` AS nc ON nriv.team_fifa_code = nc.fifa_code "
                       "JOIN `nationalteams(countries)` AS nac ON nriv.rival_fifa_code = nac.fifa_code");
    model -> setQuery(sqlCommand);

    nrMdl -> setSourceModel(model);
    ui -> TblDataNatRiv -> setModel(nrMdl);
    selMdl -> setModel(nrMdl);
    ui -> TblDataNatRiv -> setSelectionModel(selMdl);

    QString path = ":/rsc/icon_flags/";
    QString nation;
    QSqlQuery nationsQuery;
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        ui -> TblDataNatRiv -> setIndexWidget(nrMdl -> index(i, 1), new QComboBox);
        ui -> TblDataNatRiv -> setIndexWidget(nrMdl -> index(i, 2), new QComboBox);

        nationsQuery.exec("SELECT name_country FROM `nationalteams(countries)`");
        while(nationsQuery.next() )
        {
            nation = "";
            nation += nationsQuery.value(0).toString();
            qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 1) ) ) -> addItem(QIcon(path + QString(nation + ".png") ), nation);
            qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 2) ) ) -> addItem(QIcon(path + QString(nation + ".png") ), nation);
        }
        qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 1) ) ) -> setMaximumWidth(350);
        qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 2) ) ) -> setMaximumWidth(350);
        qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 1) ) ) -> setStyleSheet("color: black; "
                                                                                                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 aliceblue, stop: 1 lightcyan); "
                                                                                                               "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 blue, stop: 1 lightblue); "
                                                                                                               "font: 21px Times New Roman");
        qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 1) ) ) -> setCurrentText(ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 1) ).toString() );

        qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 2) ) ) -> setStyleSheet("color: black; "
                                                                                                               "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 honeydew, stop: 1 lightgreen); "
                                                                                                               "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 blue, stop: 1 lightblue); "
                                                                                                               "font: 21px Times New Roman");
        qobject_cast<QComboBox*> (ui -> TblDataNatRiv -> indexWidget(nrMdl -> index(i, 2) ) ) -> setCurrentText(ui -> TblDataNatRiv -> model() -> data(nrMdl -> index(i, 2) ).toString() );
    }
    nrMdl -> setDynamicSortFilter(true);
}
void SqLiteDBEditor::showClubRivalsTable()
{
    ui -> TblDataRivClub -> raise();
    ui -> TblDataRivClub -> setHidden(false);

    ui -> TblDataCont -> setHidden(true);
    ui -> TblDataSubCont -> setHidden(true);
    ui -> TblDataNations -> setHidden(true);
    ui -> TblDataLeagues -> setHidden(true);
    ui -> TblDataClubs -> setHidden(true);
    ui -> TblDataNatRiv -> setHidden(true);

    sqlCommand.exec("SELECT idclub_rivals AS id_CRi, cl.name_club AS Team, clu.name_club AS Rival FROM club_rivals AS criv "
                       "JOIN clubs AS cl ON criv.clubs_idclubs = cl.idclubs "
                       "JOIN clubs AS clu ON criv.clubs_idclubs1 = clu.idclubs");
    model -> setQuery(sqlCommand);

    crMdl -> setSourceModel(model);
    ui -> TblDataRivClub -> setModel(crMdl);
    selMdl -> setModel(crMdl);
    ui -> TblDataRivClub -> setSelectionModel(selMdl);

    QString club;
    QSqlQuery clubsQuery;
    for(int i = 0; i < model -> rowCount(); ++i)
    {
        ui -> TblDataRivClub -> setIndexWidget(crMdl -> index(i, 1), new QComboBox);
        ui -> TblDataRivClub -> setIndexWidget(crMdl -> index(i, 2), new QComboBox);

        clubsQuery.exec("SELECT name_club FROM clubs");
        while(clubsQuery.next() )
        {
            club = "";
            club += clubsQuery.value(0).toString();
            qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 1) ) ) -> addItem(club);
            qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 2) ) ) -> addItem(club);
        }

        qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 1) ) ) -> setMaximumWidth(250);
        qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 2) ) ) -> setMaximumWidth(250);
        qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 1) ) ) -> setStyleSheet("color: black; "
                                                                                                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 aliceblue, stop: 1 lightcyan); "
                                                                                                                "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 blue, stop: 1 lightblue); "
                                                                                                                "font: 21px Times New Roman");
        qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 1) ) ) -> setCurrentText(ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 1) ).toString() );

        qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 2) ) ) -> setStyleSheet("color: black; "
                                                                                                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 honeydew, stop: 1 lightgreen); "
                                                                                                                "selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 blue, stop: 1 lightblue); "
                                                                                                                "font: 21px Times New Roman");
        qobject_cast<QComboBox*> (ui -> TblDataRivClub -> indexWidget(crMdl -> index(i, 2) ) ) -> setCurrentText(ui -> TblDataRivClub -> model() -> data(crMdl -> index(i, 2) ).toString() );

    }
    crMdl -> setDynamicSortFilter(true);

    ui -> TblDataRivClub -> setColumnWidth(0, 90);
    ui -> TblDataRivClub -> setColumnWidth(1, 250);
    ui -> TblDataRivClub -> setColumnWidth(2, 250);
    ui -> TblDataRivClub -> setStyleSheet(styleSheetEditable);
}


//output functions
void SqLiteDBEditor::displayNations()
{
    QString disp;
    QString path = ":/rsc/icon_flags/";
    while(sqlCommand.next() )
    {
        disp = "";
        disp += sqlCommand.value(0).toString();

        ui -> CmbCountry -> addItem(QIcon(path + QString(disp + ".png") ), disp);

        ui -> CmbNatTeam -> addItem(QIcon(path + QString(disp + ".png") ), disp);
        ui -> CmbNatRival -> addItem(QIcon(path + QString(disp + ".png") ), disp);
    }
}

void SqLiteDBEditor::displayClubs()
{
    QString dispClub;
    while(sqlCmdClub.next() )
    {
        dispClub = "";
        dispClub += sqlCmdClub.value(0).toString();

        ui -> CmbClub -> addItem(dispClub);
        ui -> CmbClubRival -> addItem(dispClub);
    }
}

//search
void SqLiteDBEditor::EdtSearchTextChanged()
{
    ui -> CmbCountry -> clear();

    sqlCommand.prepare("SELECT name_country FROM `nationalteams(countries)` WHERE name_country LIKE ? ORDER BY name_country ASC");
    sqlCommand.addBindValue(ui -> EdtSearch -> text() + "%" );
    sqlCommand.exec();
    displayNations();
}

void SqLiteDBEditor::EdtLeagueNameTextChanged()
{   
    QRegExp rx("[0-9A-Z.\\-]*\\s?[\\w\\-]+\\s?[\\w\\-]+\\s?[\\w\\-]+\\s?[\\w\\-]+");
    rx.setPatternSyntax(QRegExp::RegExp);
    if(rx.exactMatch(ui -> EdtLeagueName -> text() ) )
    {
        ui -> CmdSave -> setEnabled(true);
    }
    else
    {
        ui -> CmdSave -> setDisabled(true);
    }
}
void SqLiteDBEditor::EdtClubNameTextChanged()
{
    QRegExp rx("[0-9A-Z.\\-]*\\s?[\\w\\-]+\\s?[\\w\\-]+\\s?[\\w\\-]+\\s?[\\w\\-]+");
    rx.setPatternSyntax(QRegExp::RegExp);
    for(int i = 0; i < ui -> TblAddNewClubs -> rowCount(); ++i)
    {
        if(rx.exactMatch(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(i, 0) ) -> text() ) )
        {
            ui -> CmdSave -> setEnabled(true);
        }
        else
        {
            ui -> CmdSave -> setDisabled(true);
        }
    }

    for(int i = 0; i < ui -> TblAddNewClubs -> rowCount(); ++i)
    {
        if(!rx.exactMatch(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(i, 0) ) -> text() ) )
        {
            ui -> CmdSave -> setDisabled(true);
        }
    }
}

//save data("insert into...")
void SqLiteDBEditor::CmdSaveClicked()
{
    if(ui -> LstTables -> currentRow() == 3)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Save?", "Do you want to save your data?",
                                            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(reply == QMessageBox::Save)
        {
            sqlCommand.prepare("INSERT INTO `leagues` (`name_league`, `fifa_code`, `subcontinents_idsubcontinents`, `continents_idcontinents`) "
                               "VALUES (?, ?, (SELECT subcontinents_idsubcontinents FROM "
                               "`nationalteams(countries)` WHERE fifa_code = ?), "
                               "(SELECT continents_idcontinents FROM `nationalteams(countries)` WHERE fifa_code = ?) )");
            sqlCommand.addBindValue(ui -> EdtLeagueName -> text() );
            sqlCommand.addBindValue(ui -> LblFifaCode -> text() );
            sqlCommand.addBindValue(ui -> LblFifaCode -> text() );
            sqlCommand.addBindValue(ui -> LblFifaCode -> text() );
            sqlCommand.exec();
            TblDataCurrentRowChanged();
            fillFilterComboBoxes();
            ui -> EdtLeagueName -> clear();
        }
        else if(reply == QMessageBox::Discard)
        {
            QMessageBox::information(this, "Not accomplished", "Operation was not accomplished");
        }
        else
        {
            QMessageBox::information(this, "Canceled", "Procedure was canceled");
        }
    }
    else if(ui -> LstTables -> currentRow() == 4)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Save?", "Do you want to save your data?",
                                            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(reply == QMessageBox::Save)
        {
            for(int i = 0; i < ui -> TblAddNewClubs -> rowCount(); ++i)
            {
                sqlCommand.prepare("INSERT INTO `clubs` (`name_club`, `offensive`, `defensive`, `teamability`, "
                                   "`leagues_idleagues`, `fifa_code`, `subcontinents_idsubcontinents`, `continents_idcontinents`) "
                                   "VALUES (?, ?, ?, ?, (SELECT idleagues FROM leagues WHERE name_league = ? ), "
                                   "(SELECT fifa_code FROM `nationalteams(countries)` WHERE name_country = ? ), "
                                   "(SELECT subcontinents_idsubcontinents FROM `nationalteams(countries)` WHERE name_country = ?), "
                                   "(SELECT continents_idcontinents FROM `nationalteams(countries)` WHERE name_country = ?) )");
                sqlCommand.addBindValue(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(i, 0) ) -> text() );
                sqlCommand.addBindValue(qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(i, 1) ) -> text() );
                sqlCommand.addBindValue(qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(i, 2) ) -> text() );
                sqlCommand.addBindValue(qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(i, 3) ) -> text() );
                sqlCommand.addBindValue(qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(i, 4) ) -> currentText() );
                sqlCommand.addBindValue(qobject_cast<QLabel*> (ui -> TblAddNewClubs -> cellWidget(i, 5) ) -> text() );
                sqlCommand.addBindValue(qobject_cast<QLabel*> (ui -> TblAddNewClubs -> cellWidget(i, 5) ) -> text() );
                sqlCommand.addBindValue(qobject_cast<QLabel*> (ui -> TblAddNewClubs -> cellWidget(i, 5) ) -> text() );
                sqlCommand.exec();
            }
            TblDataCurrentRowChanged();
            ui -> CmbClub -> clear();
            ui -> CmbClubRival -> clear();
            sqlCmdClub.exec("SELECT name_club FROM clubs ORDER BY name_club ASC");
            displayClubs();

            ui -> TblAddNewClubs -> clear();
            setUpNewClubsTblWidget();
            connect(qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(0, 4) ), SIGNAL(currentIndexChanged(int) ), SLOT(CmbLgTblAddNewClubCurrentIndexChanged() ) );
            connect(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(0, 0) ), SIGNAL(returnPressed() ), SLOT(TblAddNewClubInsertRow() ) );
            connect(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(0, 0) ), SIGNAL(textChanged(QString) ), SLOT(EdtClubNameTextChanged() ) );
            ui -> CmdSave -> setDisabled(true);
        }
        else if(reply == QMessageBox::Discard)
        {
            QMessageBox::information(this, "Not accomplished", "Operation was not accomplished");
        }
        else
        {
            QMessageBox::information(this, "Canceled", "Procedure was canceled");
        }
    }
    else
    {
        QMessageBox::information(this, "Select the right Table", "If you want to save your new LEAGUE, select the League Table. "
                                                                 "For new Club, select the Club Table.");
    }
}
void SqLiteDBEditor::CmdSaveRivalsClicked()
{
    if(ui -> LstTables -> currentRow() == 5)
    {
        if(ui -> CmbNatTeam -> currentIndex() == ui -> CmbNatRival -> currentIndex() )
        {
            QMessageBox::warning(this, "Same Teams", "You cannot add two identic rivals!");
        }
        else
        {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Save?", "Do you want to save your data?",
                                                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            if(reply == QMessageBox::Save)
            {
                sqlCommand.prepare("INSERT INTO `nationalteam_rivals` (`team_fifa_code`, `rival_fifa_code`) "
                                   "VALUES ( (SELECT fifa_code FROM `nationalteams(countries)` WHERE name_country = ?), "
                                   "(SELECT fifa_code FROM `nationalteams(countries)` WHERE name_country = ?) )");
                sqlCommand.addBindValue(ui -> CmbNatTeam -> currentText() );
                sqlCommand.addBindValue(ui -> CmbNatRival -> currentText() );
                sqlCommand.exec();
                TblDataCurrentRowChanged();
            }
            else if(reply == QMessageBox::Discard)
            {
                QMessageBox::information(this, "Not accomplished", "Operation was not accomplished");
            }
            else
            {
                QMessageBox::information(this, "Canceled", "Procedure was canceled");
            }
        }
    }
    else if(ui -> LstTables -> currentRow() == 6)
    {
        if(ui -> CmbClub -> currentIndex() == ui -> CmbClubRival -> currentIndex() )
        {
            QMessageBox::warning(this, "Same Teams", "You cannot add two identic rivals!");
        }
        else
        {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Save?", "Do you want to save your data?",
                                                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

            if(reply == QMessageBox::Save)
            {
                sqlCommand.prepare("INSERT INTO `club_rivals` (`clubs_idclubs`, `clubs_idclubs1`) "
                                   "VALUES ( (SELECT idclubs FROM clubs WHERE name_club = ?), (SELECT idclubs FROM clubs WHERE name_club = ?) )");
                sqlCommand.addBindValue(ui -> CmbClub -> currentText() );
                sqlCommand.addBindValue(ui -> CmbClubRival -> currentText() );
                sqlCommand.exec();
                TblDataCurrentRowChanged();
            }
            else if(reply == QMessageBox::Discard)
            {
                QMessageBox::information(this, "Not accomplished", "Operation was not accomplished");
            }
            else
            {
                QMessageBox::information(this, "Canceled", "Procedure was canceled");
            }
        }
    }
    else
    {
        QMessageBox::information(this, "Select the right Table", "If you want to save your new LEAGUE, select the League Table. "
                                                                 "For new Club, select the Club Table.");
    }
}

void SqLiteDBEditor::CmbCountryCurrentIndexChanged()
{
    sqlCmd = sqlCommand;
    sqlCmd.prepare("SELECT `nationalteams(countries)`.fifa_code, subcontinents.name_subcontinent, "
                   "continents.name_continent FROM `nationalteams(countries)` "
                   "INNER JOIN subcontinents ON `nationalteams(countries)`.subcontinents_idsubcontinents = idsubcontinents "
                   "INNER JOIN continents ON `nationalteams(countries)`.continents_idcontinents = idcontinents where name_country = ? ");
    sqlCmd.addBindValue(ui -> CmbCountry -> currentText() );
    sqlCmd.exec();

    QString disp;
    while(sqlCmd.next() )
    {
        disp = sqlCmd.value(0).toString();
        ui -> LblFifaCode -> setText(disp);

        disp = sqlCmd.value(1).toString();
        ui -> LblSubCont -> setText(disp);

        disp = sqlCmd.value(2).toString();
        ui -> LblContin -> setText(disp);
    }
}

void SqLiteDBEditor::CmbLeaguesForAllCurrentIndexChanged()
{
    for(int i = 0; i < ui -> TblAddNewClubs -> rowCount(); ++i)
    {
        qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(i, 4) ) -> setCurrentIndex(ui -> CmbLeaguesForAll -> currentIndex() );
    }
}
void SqLiteDBEditor::CmbLgTblAddNewClubCurrentIndexChanged()
{
    for(int i = 0; i < ui -> TblAddNewClubs -> rowCount(); ++i)
    {
        sqlCmdLeague.prepare("SELECT nt.name_country FROM `nationalteams(countries)` AS nt INNER JOIN leagues ON nt.fifa_code = leagues.fifa_code WHERE name_league = ? ");
        sqlCmdLeague.addBindValue(qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(i, 4) ) -> currentText() );
        sqlCmdLeague.exec();
        QString disp;
        while(sqlCmdLeague.next() )
        {
            disp = sqlCmdLeague.value(0).toString();
            qobject_cast<QLabel*> (ui -> TblAddNewClubs -> cellWidget(i, 5) ) -> setText(disp);
        }
    }
}
void SqLiteDBEditor::TblAddNewClubInsertRow()
{
    ui -> CmdSave -> setDisabled(true);

    QString widgetStyle = "background-color: dimgrey";
    int row = ui -> TblAddNewClubs -> rowCount();
    ui -> TblAddNewClubs -> insertRow(ui -> TblAddNewClubs -> rowCount() );

    ui -> TblAddNewClubs -> setCellWidget(row, 0, new QLineEdit);
    ui -> TblAddNewClubs -> setCellWidget(row, 1, new QSpinBox);
    ui -> TblAddNewClubs -> setCellWidget(row, 2, new QSpinBox);
    ui -> TblAddNewClubs -> setCellWidget(row, 3, new QSpinBox);
    ui -> TblAddNewClubs -> setCellWidget(row, 4, new QComboBox);
    ui -> TblAddNewClubs -> setCellWidget(row, 5, new QLabel);

    if(ui -> TblAddNewClubs -> rowCount() % 2 == 0)
    {
        qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(row, 0) ) -> setStyleSheet(widgetStyle);
        qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 1) ) -> setStyleSheet(widgetStyle);
        qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 2) ) -> setStyleSheet(widgetStyle);
        qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 3) ) -> setStyleSheet(widgetStyle);
        qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(row, 4) ) -> setStyleSheet(widgetStyle);
        qobject_cast<QLabel*> (ui -> TblAddNewClubs -> cellWidget(row, 5) ) -> setStyleSheet(widgetStyle);
    }
    qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(row, 0) ) -> setText("\"Enter the club-name here\"");
    qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(row, 0) ) -> setFocus();
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 1) ) -> setMinimum(20);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 1) ) -> setMaximum(99);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 1) ) -> setValue(50);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 1) ) -> setAlignment(Qt::AlignCenter);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 2) ) -> setMinimum(20);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 2) ) -> setMaximum(99);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 2) ) -> setValue(50);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 2) ) -> setAlignment(Qt::AlignCenter);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 3) ) -> setMinimum(20);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 3) ) -> setMaximum(99);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 3) ) -> setValue(50);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(row, 3) ) -> setAlignment(Qt::AlignCenter);
    ui -> TblAddNewClubs -> setEditTriggers(QAbstractItemView::AllEditTriggers);
    sqlCmdLeague.exec("SELECT name_league FROM leagues ORDER BY name_league ASC");
    QString dispLeague;
    while(sqlCmdLeague.next() )
    {
        dispLeague = "";
        dispLeague += sqlCmdLeague.value(0).toString();
        qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(row, 4) ) -> addItem(dispLeague);
    }

    sqlCmdLeague.prepare("SELECT nt.name_country FROM `nationalteams(countries)` AS nt INNER JOIN leagues ON nt.fifa_code = leagues.fifa_code WHERE name_league = ? ");
    sqlCmdLeague.addBindValue(qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(row, 4) ) -> currentText() );
    sqlCmdLeague.exec();
    QString disp;
    while(sqlCmdLeague.next() )
    {
        disp = sqlCmdLeague.value(0).toString();
        qobject_cast<QLabel*> (ui -> TblAddNewClubs -> cellWidget(row, 5) ) -> setText(disp);
    }

    for(int i = 0; i < ui -> TblAddNewClubs -> rowCount(); ++i)
    {
        connect(qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(i, 4) ), SIGNAL(currentIndexChanged(int) ), SLOT(CmbLgTblAddNewClubCurrentIndexChanged() ) );
        connect(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(i, 0) ), SIGNAL(textChanged(QString) ), SLOT(EdtClubNameTextChanged() ) );
    }
    qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(ui -> TblAddNewClubs -> rowCount() - 2, 0) ) -> disconnect();
    connect(qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(ui -> TblAddNewClubs -> rowCount() - 1, 0) ), SIGNAL(returnPressed() ), SLOT(TblAddNewClubInsertRow() ) );
}
void SqLiteDBEditor::setUpCellWidgets()
{
    ui -> TblAddNewClubs -> setCellWidget(0, 0, new QLineEdit);
    ui -> TblAddNewClubs -> setCellWidget(0, 1, new QSpinBox);
    ui -> TblAddNewClubs -> setCellWidget(0, 2, new QSpinBox);
    ui -> TblAddNewClubs -> setCellWidget(0, 3, new QSpinBox);
    ui -> TblAddNewClubs -> setCellWidget(0, 4, new QComboBox);
    ui -> TblAddNewClubs -> setCellWidget(0, 5, new QLabel);

    qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(0, 0) ) -> setText("\"Enter the club-name here\"");
    qobject_cast<QLineEdit*> (ui -> TblAddNewClubs -> cellWidget(0, 0) ) -> setSelection(0, 0);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 1) ) -> setMinimum(20);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 1) ) -> setMaximum(99);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 1) ) -> setValue(50);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 1) ) -> setAlignment(Qt::AlignCenter);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 2) ) -> setMinimum(20);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 2) ) -> setMaximum(99);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 2) ) -> setValue(50);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 2) ) -> setAlignment(Qt::AlignCenter);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 3) ) -> setMinimum(20);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 3) ) -> setMaximum(99);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 3) ) -> setValue(50);
    qobject_cast<QSpinBox*> (ui -> TblAddNewClubs -> cellWidget(0, 3) ) -> setAlignment(Qt::AlignCenter);
    ui -> TblAddNewClubs -> setEditTriggers(QAbstractItemView::AllEditTriggers);
    sqlCmdLeague.exec("SELECT name_league FROM leagues ORDER BY name_league ASC");
    QString dispLeague;
    while(sqlCmdLeague.next() )
    {
        dispLeague = "";
        dispLeague += sqlCmdLeague.value(0).toString();
        qobject_cast<QComboBox*> (ui -> TblAddNewClubs -> cellWidget(0, 4) ) -> addItem(dispLeague);
    }
    CmbLgTblAddNewClubCurrentIndexChanged();
}
void SqLiteDBEditor::setUpNewClubsTblWidget()
{
    ui -> TblAddNewClubs -> setStyleSheet("background-color: #3D3D3D; "
                                          "font: bold 20px Times New Roman");
    ui -> TblAddNewClubs -> setRowCount(1);
    ui -> TblAddNewClubs -> setColumnCount(6);
    ui -> TblAddNewClubs -> verticalHeader() -> hide();

    QStringList headers = { "Clubname", "Off", "Def", "Team\nability", "Leaguename", "Country" };
    ui -> TblAddNewClubs -> setHorizontalHeaderLabels(headers);
    ui -> TblAddNewClubs -> horizontalHeader() -> setStyleSheet("QHeaderView::section { height: 40px; "
                                                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FF7972, stop: 0.5 #FF4F4C, stop: 0.6 #FF282C, stop: 1 #f08080); "
                                                                "color: black; "
                                                                "padding-left: 4px; "
                                                                "border: 1px solid #f08080; "
                                                                "font: bold 22px Georgia } "
                                                                "QTableView::item { border-top: 1px solid #f08080; "
                                                                "border-bottom: 1px solid #f08080; "
                                                                "border-left: 1px solid #f08080; "
                                                                "border-right: 1px solid #f08080 }");
    ui -> TblAddNewClubs -> setColumnWidth(0, 381);
    ui -> TblAddNewClubs -> setColumnWidth(1, 81);
    ui -> TblAddNewClubs -> setColumnWidth(2, 81);
    ui -> TblAddNewClubs -> setColumnWidth(3, 115);
    ui -> TblAddNewClubs -> setColumnWidth(4, 320);
    ui -> TblAddNewClubs -> setColumnWidth(5, 411);
    setUpCellWidgets();
}

void SqLiteDBEditor::fillFilterComboBoxes()
{
    QString disp = {};
    sqlCmd.exec("SELECT name_continent FROM continents");
    ui -> CmbContinents -> clear();
    while(sqlCmd.next() )
    {
        disp = sqlCmd.value(0).toString();
        ui -> CmbContinents -> addItem(disp);
    }

    sqlCmd.exec(QString("SELECT name_subcontinent FROM subcontinents WHERE continents_idcontinents = "
                        "(SELECT idcontinents FROM continents WHERE name_continent = '%1')").arg(ui -> CmbContinents -> currentText() ) );
    ui -> CmbSubContinents -> clear();
    while(sqlCmd.next() )
    {
        disp = sqlCmd.value(0).toString();
        ui -> CmbSubContinents -> addItem(disp);
    }

    sqlCmd.exec("SELECT name_league FROM leagues ORDER BY name_league ASC");
    ui -> CmbLeaguesFilter -> clear();
    ui -> CmbLeaguesForAll -> clear();
    while(sqlCmd.next() )
    {
        disp = sqlCmd.value(0).toString();
        ui -> CmbLeaguesFilter -> addItem(disp);
        ui -> CmbLeaguesForAll -> addItem(disp);
    }
}
SqLiteDBEditor::SqLiteDBEditor() : QDialog(), ui(new Ui::SqLiteDBEditor)
{
    ui -> setupUi(this);
    this -> setStyleSheet("QToolTip { background-color: lightgrey; "
                          "color: black; "
                          "border: none } "
                          "QWidget { color: lightblue; "
                          "background-color: #404040 }");

    dbTblinfo.setFile("mydb.db");
    ui -> LblLastUpdate -> setText(dbTblinfo.lastModified().toString() );

    styleSheetEditable = "QTableView { alternate-background-color: white; "
                         "background: lightgrey; "
                         "color: black } "
                         "QTableView::item:selected { color: white; "
                                 "background-color: blue } "
                                 "QHeaderView::section { height: 30px; "
                                 "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FF7972, stop: 0.5 #FF4F4C, stop: 0.6 #FF282C, stop: 1 #f08080); "
                                 "color: black; "
                                 "padding-left: 4px; "
                                 "border: 1px solid #f08080; "
                                 "font: bold 22px Georgia } "
                                 "QTableView::item { border-top: 1px solid #f08080; "
                                 "border-bottom: 1px solid #f08080; "
                                 "border-left: 1px solid #f08080; "
                                 "border-right: 1px solid #f08080 }";

    styleSheetNotEditable = "QTableView { alternate-background-color: dimgrey; "
                            "background: black } "
                            "QHeaderView::section { height: 30px; "
                                    "background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2BE2FF, stop: 0.5 #1ECEFF, stop: 0.6 #2BBBFF, stop:1 #1CECFF); "
                                    "color: black; "
                                    "padding-left: 4px; "
                                    "border: 1px solid #4CD5FF; "
                                    "font: bold 22px Georgia }";
    setUpNewClubsTblWidget();
    showContinentTable();

    ui -> LstTables -> clear();
    ui -> LstTables -> addItem("Continents");
    ui -> LstTables -> addItem("Subcontinents");
    ui -> LstTables -> addItem("Countries");
    ui -> LstTables -> addItem("Leagues");
    ui -> LstTables -> addItem("Clubs");
    ui -> LstTables -> addItem("Nationalteam-Rivals");
    ui -> LstTables -> addItem("Club-Rivals");

    ui -> LstTables -> setCurrentRow(0);
    ui -> LstTables -> setFocus();

    ui -> CmbCountry -> clear();
    ui -> CmbNatTeam -> clear();
    ui -> CmbNatRival -> clear();
    sqlCommand.exec("SELECT name_country FROM `nationalteams(countries)` ORDER BY name_country ASC");
    displayNations(); 

    ui -> CmbClub -> clear();
    ui -> CmbClubRival -> clear();
    sqlCmdClub = sqlCommand;
    sqlCmdClub.exec("SELECT name_club FROM clubs ORDER BY name_club ASC");
    displayClubs();

    sqlCmd = sqlCommand;
    sqlCmd.prepare("SELECT `nationalteams(countries)`.fifa_code, subcontinents.name_subcontinent, "
                   "continents.name_continent FROM `nationalteams(countries)` "
                   "INNER JOIN subcontinents ON `nationalteams(countries)`.subcontinents_idsubcontinents = idsubcontinents "
                   "INNER JOIN continents ON `nationalteams(countries)`.continents_idcontinents = idcontinents where name_country = ? ");
    sqlCmd.addBindValue(ui -> CmbCountry -> currentText() );
    sqlCmd.exec();

    QString disp;
    while(sqlCmd.next() )
    {
        disp = sqlCmd.value(0).toString();
        ui -> LblFifaCode -> setText(disp);

        disp = sqlCmd.value(1).toString();
        ui -> LblSubCont -> setText(disp);

        disp = sqlCmd.value(2).toString();
        ui -> LblContin -> setText(disp);
    }

    fillFilterComboBoxes();

    ui -> TblAddNewClubs -> hide();
    ui -> CmbLeaguesForAll -> hide();

    ui -> LblNumberOfRows -> hide();
    ui -> LblAddLeague -> setHidden(true);
    ui -> LblLeagueName -> setHidden(true);
    ui -> LblCountry -> setHidden(true);
    ui -> LblFiCo -> setHidden(true);
    ui -> LblSubCon -> setHidden(true);
    ui -> LblCon -> setHidden(true);
    ui -> EdtLeagueName -> setHidden(true);
    ui -> CmbCountry -> setHidden(true);
    ui -> LblFifaCode -> setHidden(true);
    ui -> LblSubCont -> setHidden(true);
    ui -> LblContin -> setHidden(true);
    ui -> EdtSearch -> setHidden(true);
    ui -> LblSearch -> setHidden(true);
    ui -> CmdSave -> setHidden(true);

    ui -> LblAddClub -> setHidden(true);

    ui -> LblAddNatRival -> setHidden(true);
    ui -> LblNatTeam -> setHidden(true);
    ui -> CmbNatTeam -> setHidden(true);
    ui -> LblRival -> setHidden(true);
    ui -> CmbNatRival -> setHidden(true);
    ui -> CmdSaveRivals -> setHidden(true);

    ui -> LblAddClubRival -> setHidden(true);
    ui -> LblClubTeam -> setHidden(true);
    ui -> CmbClub -> setHidden(true);
    ui -> LblClubRival -> setHidden(true);
    ui -> CmbClubRival -> setHidden(true);
    ui -> CmdUpdate -> setDisabled(true);

    ui -> CmbContinents -> hide();
    ui -> CmbSubContinents -> hide();
    ui -> CmbLeaguesFilter -> hide();
    ui -> CmdNoFilterSubCont -> hide();
    ui -> CmdNoFilterNat -> hide();
    ui -> CmdNoFilterClubs -> hide();
    ui -> CmdNoFilterClubs -> setEnabled(true);
    ui -> CmdNoFilterNat -> setDisabled(true);

    //___________________________________________
    ui -> LblAddClub -> move(30, 530);

    ui -> CmdSaveRivals -> move(740, 580);

    ui -> LblAddNatRival -> move(80, 570);
    ui -> LblNatTeam -> move(30, 620);
    ui -> CmbNatTeam -> move(180, 620);
    ui -> LblRival -> move(100, 650);
    ui -> CmbNatRival -> move(180, 650);

    ui -> LblAddClubRival -> move(80, 570);
    ui -> LblClubTeam -> move(100, 620);
    ui -> CmbClub -> move(180, 620);
    ui -> LblClubRival -> move(100, 650);
    ui -> CmbClubRival -> move(180, 650);
    //_________________________________________

    ui -> CmdSave -> setDisabled(true);
    ui -> CmdSave -> setStyleSheet("QPushButton:disabled { border-image: url(:/miscRsc/misc_icons/filesave_128x128_disabled) 3 10 3 10; "
                                   "border-top: 3px transparent; "
                                   "border-bottom: 3px transparent; "
                                   "border-right: 10px transparent; "
                                   "border-left: 10px transparent } "
                                   "QPushButton:enabled { border-image: url(:/miscRsc/misc_icons/filesave_128x128) 3 10 3 10; "
                                   "border-top: 3px transparent; "
                                   "border-bottom: 3px transparent; "
                                   "border-right: 10px transparent; "
                                   "border-left: 10px transparent } "
                                   "QPushButton:pressed { border-image: url(:/miscRsc/misc_icons/filesave_128x128_pressed) 2 9 2 9; "
                                   "border-top: 2px transparent; "
                                   "border-bottom: 2px transparent; "
                                   "border-right: 9px transparent; "
                                   "border-left: 9px transparent }");

    ui -> CmdSaveRivals -> setStyleSheet("QPushButton:disabled { border-image: url(:/miscRsc/misc_icons/filesave_128x128_disabled) 3 10 3 10; "
                                         "border-top: 3px transparent; "
                                         "border-bottom: 3px transparent; "
                                         "border-right: 10px transparent; "
                                         "border-left: 10px transparent } "
                                         "QPushButton:enabled { border-image: url(:/miscRsc/misc_icons/filesave_128x128) 3 10 3 10; "
                                         "border-top: 3px transparent; "
                                         "border-bottom: 3px transparent; "
                                         "border-right: 10px transparent; "
                                         "border-left: 10px transparent } "
                                         "QPushButton:pressed { border-image: url(:/miscRsc/misc_icons/filesave_128x128_pressed) 2 9 2 9; "
                                         "border-top: 2px transparent; "
                                         "border-bottom: 2px transparent; "
                                         "border-right: 9px transparent; "
                                         "border-left: 9px transparent }");

    ui -> CmdNoFilterClubs -> setStyleSheet(" :disabled { color: grey; "
                                            "background: dimgrey }");
    ui -> CmdNoFilterNat -> setStyleSheet(" :disabled { color: grey; "
                                            "background: dimgrey }");
    ui -> CmdNoFilterSubCont -> setStyleSheet(" :disabled { color: grey; "
                                              "background: dimgrey }");

    ui -> LstTables -> setStyleSheet("QListView::item:selected:active { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 lightskyblue, stop: 1 blue) } "
                                     "QListView::item:hover { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 gainsboro, stop: 1 grey) } "
                                     "QListWidget { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 khaki, stop: 0.5 gold, stop: 1 lightsalmon); "
                                     "color: blue }");

    connect(ui -> EdtSearch, SIGNAL(textChanged(QString) ), SLOT(EdtSearchTextChanged() ) );
    connect(ui -> LstTables, SIGNAL(currentRowChanged(int) ), SLOT(TblDataCurrentRowChanged() ) );
    connect(ui -> CmbCountry, SIGNAL(currentIndexChanged(int) ), SLOT(CmbCountryCurrentIndexChanged() ) );

    connect(ui -> EdtLeagueName, SIGNAL(textChanged(QString) ), SLOT(EdtLeagueNameTextChanged() ) );

    connect(ui -> CmdSave, SIGNAL(clicked() ), SLOT(CmdSaveClicked() ) );
    connect(ui -> CmdSaveRivals, SIGNAL(clicked() ), SLOT(CmdSaveRivalsClicked() ) );
    connect(ui -> CmdUpdate, SIGNAL(clicked() ), SLOT(CmdUpdateClicked() ) );
    connect(ui -> CmbLeaguesFilter, SIGNAL(currentIndexChanged(int) ), SLOT(CmbLeaguesFilterCurrentIndexChanged() ) );
    connect(ui -> CmdNoFilterClubs, SIGNAL(clicked() ), SLOT(CmdNoFilterClubsClicked() ) );
    connect(ui -> CmdNoFilterSubCont, SIGNAL(clicked() ), SLOT(CmdNoFilterSubContClicked() ) );
    connect(ui -> CmdNoFilterNat, SIGNAL(clicked() ), SLOT(CmdNoFilterNatClicked() ) );
    connect(ui -> CmbContinents, SIGNAL(currentIndexChanged(int) ), SLOT(CmbFilterContCurrentIndexChanged() ) );
    connect(ui -> CmbSubContinents, SIGNAL(currentIndexChanged(int) ), SLOT(CmbFilterSubContCurrentIndexChanged() ) );

    connect(ui -> CmbLeaguesForAll, SIGNAL(currentIndexChanged(int) ), SLOT(CmbLeaguesForAllCurrentIndexChanged() ) );

    QShortcut* delL = new QShortcut(Qt::Key_Delete, ui -> TblDataLeagues);
    QShortcut* delC = new QShortcut(Qt::Key_Delete, ui -> TblDataClubs);
    QShortcut* delNatRiv = new QShortcut(Qt::Key_Delete, ui -> TblDataNatRiv);
    QShortcut* delClubRiv = new QShortcut(Qt::Key_Delete, ui -> TblDataRivClub);

    connect(delL, SIGNAL(activated() ), SLOT(TblRowDelete() ) );
    connect(delC, SIGNAL(activated() ), SLOT(TblRowDelete() ) );
    connect(delNatRiv, SIGNAL(activated() ), SLOT(TblRowDelete() ) );
    connect(delClubRiv, SIGNAL(activated() ), SLOT(TblRowDelete() ) );

    connect(ui -> TblDataNations, SIGNAL(clicked(QModelIndex) ), SLOT(TblRowClicked(QModelIndex) ) );
    connect(ui -> TblDataLeagues, SIGNAL(clicked(QModelIndex) ), SLOT(TblRowClicked(QModelIndex) ) );
    connect(ui -> TblDataClubs, SIGNAL(clicked(QModelIndex) ), SLOT(TblRowClicked(QModelIndex) ) );
    connect(ui -> TblDataNatRiv, SIGNAL(clicked(QModelIndex) ), SLOT(TblRowClicked(QModelIndex) ) );
    connect(ui -> TblDataRivClub, SIGNAL(clicked(QModelIndex) ), SLOT(TblRowClicked(QModelIndex) ) );
}

SqLiteDBEditor::~SqLiteDBEditor()
{
    delete ui;
}
