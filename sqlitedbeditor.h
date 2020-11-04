#ifndef SQLITEDBEDITOR_H
#define SQLITEDBEDITOR_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QFile>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class SqLiteDBEditor; }
QT_END_NAMESPACE

class SqLiteDBEditor : public QDialog
{
    Q_OBJECT

public:
    explicit SqLiteDBEditor();
    ~SqLiteDBEditor();

    void setUpCellWidgets();
    void setUpNewClubsTblWidget();

    void displayNations();
    void displayClubs();

    void tblLeaguesRowDelete();
    void tblClubsRowDelete();
    void tblNationRivalsRowDelete();
    void tblClubRivalsRowDelete();

    void tblNationsUpdate();
    void tblLeaguesUpdate();
    void tblClubsUpdate();
    void tblNationRivalsUpdate();
    void tblClubRivalsUpdate();

    QString editStringFromNations(QString singleEditString, QString nation);
    QString editStringFromClubs(QString singleEditString, QString clubName);

    void hideInputLayout();
    void showLeagueInputLayout();
    void showClubInputLayout();
    void showNationRivalsLayoutAndSetLook();
    void showClubRivalsLayout();
    void setSizeAndLookOfClubTable();

    void showContinentTable();
    void showSubContinentTable();
    void showNationsTable();
    void nationTableCastToSpinBox();
    void showLeagueTable();
    void showClubTable();
    void showNationRivalsTable();
    void showClubRivalsTable();

    void fillFilterComboBoxes();

private:
    Ui::SqLiteDBEditor *ui;

    QFileInfo dbTblinfo;

    QString styleSheetEditable;
    QString styleSheetNotEditable;
    QString sqlCmdClubTable;
    QString sqlCmdShowNations;

    QSqlQuery sqlCommand;
    QSqlQuery sqlCmd;
    QSqlQuery sqlCmdLeague;
    QSqlQuery sqlCmdLge;
    QSqlQuery sqlCmdClub;

    QSqlQueryModel *model = new QSqlQueryModel();

    QItemSelectionModel *selMdl = new QItemSelectionModel();

    QSortFilterProxyModel *contMdl = new QSortFilterProxyModel(this);
    QSortFilterProxyModel *sucoMdl = new QSortFilterProxyModel(this);
    QSortFilterProxyModel *cntryMdl = new QSortFilterProxyModel(this);
    QSortFilterProxyModel *lMdl = new QSortFilterProxyModel(this);
    QSortFilterProxyModel *cMdl = new QSortFilterProxyModel(this);
    QSortFilterProxyModel *nrMdl = new QSortFilterProxyModel(this);
    QSortFilterProxyModel *crMdl = new QSortFilterProxyModel(this);

private slots:
    void CmbCountryCurrentIndexChanged();
    void CmbLgTblAddNewClubCurrentIndexChanged();
    void TblAddNewClubInsertRow();
    void CmdSaveClicked();
    void CmdSaveRivalsClicked();
    void EdtLeagueNameTextChanged();
    void EdtClubNameTextChanged();
    void TblDataCurrentRowChanged();
    void TblRowClicked(const QModelIndex &index);
    void TblRowDelete();
    void CmdUpdateClicked();
    void on_EdtSearch_textChanged();
    void CmdFilterByLeaguesClicked();
    void CmdNoFilterClubsClicked();
    void CmdFilterContClicked();
    void CmdFilterSubContClicked();
    void CmdNoFilterNatClicked();
    void CmbLeaguesForAllCurrentIndexChanged();
};

#endif // SQLITEDBEDITOR_H
