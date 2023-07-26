/*
 * YEROTH_TABLEAUX_DE_BORDS_analyse_comparee_mensuelle.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "../yeroth-erp-tableaux-de-bord-window.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-map-COMPLEX-ITEM.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <unistd.h>

#include <QtWidgets/QFileDialog>

#include <QtCore/QDateTime>

#include <QtCore/QMapIterator>

#include <QtCore/QtMath>

#include <QtCore/QPair>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>




void YerothTableauxDeBordWindow::analyse_comparee_mensuelle_VENTES_BENEFICES()
{
    _logger->log("analyse_comparee_mensuelle_VENTES_BENEFICES");

    if (comboBox_mois_debut_chiffre_affaire->currentIndex() >
            comboBox_mois_fin_chiffre_affaire->currentIndex())
    {
        YerothQMessageBox::warning(this, QObject::tr("analyse comparée"),
                                   QObject::tr
                                   ("Le mois de 'début' doit être"
                                    " antérieur au mois de 'fin' !"));
    }

    //"yyyy-MM-dd"; D.toString(YerothUtils::DB_DATE_FORMAT)

    QString annee = comboBox_annee_chiffre_affaire->currentText();
    QString mois_debut = comboBox_mois_debut_chiffre_affaire->currentText();
    QString mois_fin = comboBox_mois_fin_chiffre_affaire->currentText();

    int moisDebut = _moisToNombre[mois_debut];
    int moisFin = _moisToNombre[mois_fin];

    QString dateDebut;
    dateDebut.append(annee).append("-").append(QString::number(moisDebut)).
    append("-01");

    QDate qDateDebut(annee.toInt(), moisDebut, 1);
    QDate qDateFin;

    QString dateFin;
    bool moisFinAjuster = false;

    if (_moisToNombre[YerothTableauxDeBordWindow::MOIS_12] == moisFin)
    {
        dateFin.append(annee).append("-").append(QString::number(moisFin)).
        append("-31");

        qDateFin.setDate(annee.toInt(), moisFin, 31);
    }
    else
    {
        moisFin += 1;
        moisFinAjuster = true;

        dateFin.append(annee).append("-").append(QString::number(moisFin)).
        append("-01");

        qDateFin.setDate(annee.toInt(), moisFin, 1);
    }

    //qDebug() << "calculerChiffresDaffaireMois\n\t" <<
    //                              QString("annee: %1, mois_debut: %2, mois fin ajuster: %3, mois_fin: %4")
    //                                      .arg(annee,
    //                                               QString::number(moisDebut),
    //                                               BOOL_TO_STRING(moisFinAjuster),
    //                                               QString::number(moisFin));

    QString strBeneficeQuery;

    strBeneficeQuery.append(QString
                            ("SELECT %1, %2 FROM %3 WHERE %4 >= '%5' AND ").arg
                            (YerothDatabaseTableColumn::DATE_VENTE,
                             YerothDatabaseTableColumn::MARGE_BENEFICIAIRE,
                             YerothDatabase::STOCKS_VENDU,
                             YerothDatabaseTableColumn::DATE_VENTE,
                             dateDebut));

    if (moisFinAjuster)
    {
        strBeneficeQuery.append(QString("%1 < '%2'").arg
                                (YerothDatabaseTableColumn::DATE_VENTE,
                                 dateFin));
    }
    else
    {
        strBeneficeQuery.append(QString("%1 <= '%2'").arg
                                (YerothDatabaseTableColumn::DATE_VENTE,
                                 dateFin));
    }

    //qDebug() << "analyse_comparee_mensuelle_VENTES_BENEFICES - analyse comparee \n\t" <<
    //                              QString("strBeneficeQuery: %1 | annee: %2, mois_debut: %3, mois fin ajuster: %4, mois_fin: %5")
    //                                      .arg(strBeneficeQuery,
    //                                               annee,
    //                                               QString::number(moisDebut),
    //                                               BOOL_TO_STRING(moisFinAjuster),
    //                                               QString::number(moisFin));


    QString
    strVentesQuery(QString
                   ("SELECT date_vente, (montant_total_vente - montant_tva) "));

    strVentesQuery.append("FROM ").append(YerothDatabase::STOCKS_VENDU).
    append(" WHERE date_vente >= '").append(dateDebut).append("' AND ");

    if (moisFinAjuster)
    {
        strVentesQuery.append("date_vente < '").append(dateFin).append("'");
    }
    else
    {
        strVentesQuery.append("date_vente <= '").append(dateFin).append("'");
    }

    QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

    bool filtreActif = false;

    QString yerothFiltre;

    QString databaseTableColumn;

    QString
    comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText
                                       ());

    QString
    textFromLineEditEvolutionSujets
    (comboBox_evolution_objets_value->currentText());

    if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CAISSIERS))
    {
        filtreActif = true;

        yerothFiltre.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (QObject::tr("filtre: %1 = %2").arg
                             (YerothTableauxDeBordWindow::OBJET_CAISSIERS,
                              textFromLineEditEvolutionSujets)));

        databaseTableColumn = YerothDatabaseTableColumn::NOM_CAISSIER;
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_CATEGORIES))
    {
        filtreActif = true;

        yerothFiltre.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (QObject::tr("filtre: %1 = %2").arg
                             (YerothTableauxDeBordWindow::OBJET_CATEGORIES,
                              textFromLineEditEvolutionSujets)));

        databaseTableColumn = YerothDatabaseTableColumn::CATEGORIE;
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_CLIENTS))
    {
        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_CLIENTS,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn = YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_ARTICLES))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            filtreActif = false;

            yerothFiltre.append(AUCUN_FILTRE);
        }
        else
        {
            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_ARTICLES,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn = YerothDatabaseTableColumn::DESIGNATION;
        }
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_FOURNISSEURS))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                            (YerothTableauxDeBordWindow::OBJET_FOURNISSEURS,
                             textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_QUARTIER))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_QUARTIER,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_VILLES))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_VILLES,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_REGIONS_ETATS))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                            (YerothTableauxDeBordWindow::OBJET_REGIONS_ETATS,
                             textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT;

    }
    //qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
    //                              .arg(databaseTableColumn, comboBox_evolution_objets->currentText());



    if (true == filtreActif)
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            YerothQMessageBox::warning(this,
                                       QObject::tr("paramètre manquant"),
                                       QObject::tr
                                       ("Vous devez spécifier un paramètre dans"
                                        " le champs de texte 'nom' !"));
            return;
        }
        else
        {
            if (!databaseTableColumn.isEmpty())
            {
                strVentesQuery.append(QString(" AND %1 = '%2'").arg
                                      (databaseTableColumn,
                                       YerothUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (textFromLineEditEvolutionSujets)));
            }
        }
    }

    //qDebug() << "++ query: " << strQuery;

    QSqlQuery beneficeQuery;

    QSqlQuery query;

    double montant_total_ventes = 0.0;

    int beneficeQuerySize =
                    YerothUtils::execQuery(beneficeQuery, strBeneficeQuery, _logger);

    int ventesQuerySize =
                    YerothUtils::execQuery(query, strVentesQuery, _logger);

    if (0 >= beneficeQuerySize || 0 >= ventesQuerySize)
    {

        YerothQMessageBox::information(this,
                                       QObject::tr("analyse comparée"),
                                       QObject::
                                       trUtf8(("Il n'y a pas de données correspondante à la requête !\n" "Vérifier que les dates de début et de fin, " "ainsi que l'année sont correctes !")));
        return;
    }

    QMap < int, double >monthToBeneficesTotalAmount;

    QMap < int, double >monthToVentesTotalAmount;

    int curMonth = 0;
    QDate curDate;

    double benefice = 0.0;

    double maxAmountBenefices = 0.0;
    double maxAmountVentes = 0.0;

    double sommeTotaleBeneficesMois = 0.0;
    double sommeTotaleVentesMois = 0.0;

    for (int k = moisDebut; k <= moisFin; ++k)
    {
        sommeTotaleBeneficesMois += monthToBeneficesTotalAmount[k];
        sommeTotaleVentesMois += monthToVentesTotalAmount[k];
    }

    for (int k = 0; k < beneficeQuerySize && beneficeQuery.next(); ++k)
    {
        curDate = beneficeQuery.value(0).toDate();
        curMonth = curDate.month();

        benefice = beneficeQuery.value(1).toDouble();

        //qDebug() << "++ curDate: " << curDate
        //               << ", curMonth: " << curMonth
        //               << ", montant_total_vente: " << montant_total_vente;

        if (0 != curMonth)
        {
            monthToBeneficesTotalAmount[curMonth] += benefice;

            if (maxAmountBenefices < monthToBeneficesTotalAmount[curMonth])
            {
                maxAmountBenefices = monthToBeneficesTotalAmount[curMonth];
            }
            /*qDebug() << ", curMonth: " << curMonth
               << ", montant_total_vente: " << monthToVentesTotalAmount[curMonth]; */
        }
    }

    for (int k = 0; k < ventesQuerySize && query.next(); ++k)
    {
        curDate = query.value(0).toDate();
        curMonth = curDate.month();

        montant_total_ventes = query.value(1).toDouble();

        //qDebug() << "++ curDate: " << curDate
        //               << ", curMonth: " << curMonth
        //               << ", montant_total_vente: " << montant_total_vente;

        if (0 != curMonth)
        {
            monthToVentesTotalAmount[curMonth] += montant_total_ventes;

            if (maxAmountVentes < monthToVentesTotalAmount[curMonth])
            {
                maxAmountVentes = monthToVentesTotalAmount[curMonth];
            }
            /*qDebug() << ", curMonth: " << curMonth
               << ", montant_total_vente: " << monthToVentesTotalAmount[curMonth]; */
        }
    }

    _reportTexFileEndString.clear();

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        _reportTexFileEndString
            .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                     (QString("Details in %1:")
                        .arg(YerothERPConfig::currency)));
    }
    else
    {
        _reportTexFileEndString
            .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                     (QString("Détails in %1:")
                        .arg(YerothERPConfig::currency)));
    }


    _reportTexFileEndString.prepend("\\textbf{").append("}\n");
    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    //Fill in the PDF file which amount of money for which month.

    if (moisFinAjuster)
    {
        moisFin -= 1;
    }

    for (int k = moisDebut; k <= moisFin; ++k)
    {
        _reportTexFileEndString.append("\\item \\textbf{");

        if (YerothMainWindow::LANGUE_ANGLAISE)
        {
            _reportTexFileEndString
                .append(QString("%1}: sales $\\rightarrow %2$, profit $\\rightarrow %3$\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::englishLocale.monthName(k)),
                                     YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                        (GET_CURRENCY_STRING_NUM(monthToVentesTotalAmount[k])),
                                 YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (GET_CURRENCY_STRING_NUM(monthToBeneficesTotalAmount[k]))));
        }
        else
        {
            _reportTexFileEndString
                .append(QString("%1}: ventes $\\rightarrow %2$, bénéfice $\\rightarrow %3$\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::frenchLocale.monthName(k)),
                                     YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                        (GET_CURRENCY_STRING_NUM(monthToVentesTotalAmount[k])),
                                 YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (GET_CURRENCY_STRING_NUM(monthToBeneficesTotalAmount[k]))));

        }
    }

    _reportTexFileEndString.append("\\end{enumerate}\n");

    //qDebug() << "++ END";

    QString barItems;

    double ratioBenefices = 0;
    double ratioVentes = 0;

    double maxAmountAll = maxAmountVentes + maxAmountBenefices;

    const int TICKS = 100;
    const double MAX_RATIO = 900.0;

    for (int k = moisDebut; k <= moisFin; ++k)
    {
        if (monthToVentesTotalAmount.contains(k))
        {
            //qDebug() << QString("month: %1, amount: %2")
            //                                              .arg(QString::number(k),
            //                                                              QString::number(monthToVentesTotalAmount[3], 'f', 2));
            ratioBenefices =
                            (monthToBeneficesTotalAmount[k] * MAX_RATIO) / maxAmountAll;

            ratioVentes =
                            (monthToVentesTotalAmount[k] * MAX_RATIO) / maxAmountAll;

            //qDebug() << QString("++ mois(k): %1, max amount achats: %2, ratio achat: %3, max amount ventes: %4, ratio vente: %5")
            //                              .arg(QString::number(k),
            //                                              QString::number(monthToAchatsTotalAmount[k], 'f', 2),
            //                                              QString::number(ratioAchats, 'f', 2),
            //                                              QString::number(monthToVentesTotalAmount[k], 'f', 2),
            //                                              QString::number(ratio, 'f', 2));

            barItems.append(QString
                            ("\\baritem{%1}{%2}{gray}\n		\\subbaritem{}{%3}{purplish}\n").
                            arg(YerothUtils::
                                LATEX_IN_OUT_handleForeignAccents
                                (GET_MONTH_NAME_LOCALIZED(k)),
                                QString::number(ratioVentes, 'f', 2),
                                QString::number(ratioBenefices, 'f', 2)));
        }
    }


    //qDebug() << "++ barItems: " << barItems;

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

    QString texDocument;
    texDocument.append(YerothUtils::_1a_tex);

    texDocument.replace("YEROTHBARPERGROUP", "\\barspergroup{2}");
    texDocument.replace("YEROTHBARITEMS", barItems);
    texDocument.replace("YEROTHTICKS", QString::number(TICKS));


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument.replace("YEROTHLEGENDANALYSECOMPAREE",
                            "\\diagLegenditem{Ratio of monthly profit.}{purplish}");

        texDocument.replace("YEROTHDIAGRAMMETITRE", "Ratio of monthly income.");

        texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", "Income Level");
    }
    else
    {
        texDocument.replace("YEROTHLEGENDANALYSECOMPAREE",
                            "\\diagLegenditem{Ratio des bénéfices du mois.}{purplish}");

        texDocument.replace("YEROTHDIAGRAMMETITRE",
                            "Ratio du chiffre d'affaire du mois.");

        texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE",
                            "Niveau du chiffre d'affaire");
    }


    QString fileName1(YerothERPConfig::temporaryFilesDir + "/1a.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }
    tmpFile1.close();

    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument2;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.append(YerothUtils::EN_bar_diag_tex);
    }
    else
    {
        texDocument2.append(YerothUtils::FR_bar_diag_tex);
    }


    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(factureDate);

    QString longDateDebut;
    QString longDateFin;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        longDateDebut =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::englishLocale.toString(qDateDebut)));

        longDateFin =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::englishLocale.toString(qDateFin)));
    }
    else
    {
        longDateDebut =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::frenchLocale.toString(qDateDebut)));

        longDateFin =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::frenchLocale.toString(qDateFin)));
    }


    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerothConfig::typeOfFacturation;


    texDocument2.replace("YEROTHPAPERSPEC", "a4paper");

    if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "caissiers")  ||
        YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "catégories") ||
        YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "clients"))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            yerothFiltre.clear();
            yerothFiltre.append(QObject::tr("aucun filtre !"));
        }
    }
    else
    {
        yerothFiltre = YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltre);
    }


    texDocument2.replace("YEROTHMENTION",
                         QObject::tr("[analyse compar\\'ee]"));

    texDocument2.replace("YEROTHFILTRE",
                         yerothFiltre.prepend("''").append("''"));

    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT", longDateDebut);
    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", longDateFin);
    texDocument2.replace("YEROTHCHARTFIN", _reportTexFileEndString);

    texDocument2.replace("YEROTHENTREPRISE",
                         infoEntreprise.getNomCommercial_LATEX());

    texDocument2.replace("YEROTHACTIVITESENTREPRISE",
                         infoEntreprise.getSecteursActivitesTex());

    texDocument2.replace("YEROTHBOITEPOSTALE",
                         infoEntreprise.getBoitePostal());

    texDocument2.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
    texDocument2.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", factureDate);

    texDocument2.replace("YEROTHNOMUTILISATEUR",
                         QString("%1 %2")
                            .arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
                                 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

    texDocument2.replace("YEROTHSUCCURSALE",
                         YerothUtils::LATEX_IN_OUT_handleForeignAccents
						 	 (YerothERPConfig::THIS_SITE_LOCALISATION_NAME));

    texDocument2.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);

    texDocument2.replace("YEROTHCOMPTEBANCAIRENR",
                         infoEntreprise.getNumeroCompteBancaire());

    texDocument2.replace("YEROTHCONTRIBUABLENR",
                         infoEntreprise.getNumeroDeContribuable());

    texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",
                         infoEntreprise.getAgenceCompteBancaireTex());

    texDocument2.replace("1a.tex", fileName1);


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Chart illustrating the income from %1 to %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }
    else
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Diagramme r\\'epr\\'esentatif des chiffres"
                                     " d'affaire du %1 au %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }


    //qDebug() << "++ test: " << texDocument2;

    YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

    QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire"));
    fileName.append(".");

    QString tmpFilePrefix(YerothERPConfig::temporaryFilesDir + "/" + fileName);

    QFile tmpFile(tmpFilePrefix + "tex");

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }

    tmpFile.close();

    //qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;
    QString texRm(tmpFile.fileName().remove(".tex"));
    progArguments << texRm;

    //qDebug() << "++ file name to latex compile bar diag: " << texRm;

    aProcess.start(YerothERPConfig::pathToLatex(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("%1.dvi").arg(texRm);

    aProcess.start(YerothERPConfig::pathToDvips(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("-dALLOWPSTRANSPARENCY");
    progArguments << QString("%1.ps").arg(texRm);
    progArguments << QString("%1.pdf").arg(texRm);

    aProcess.start(YerothERPConfig::pathToPs2Pdf(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

    //emit startPdf(1);
    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();

}


void YerothTableauxDeBordWindow::analyse_comparee_mensuelle_ACHATS_VENTES()
{
    _logger->log("analyse_comparee_mensuelle_ACHATS_VENTES");

    if (comboBox_mois_debut_chiffre_affaire->currentIndex() >
            comboBox_mois_fin_chiffre_affaire->currentIndex())
    {
        YerothQMessageBox::warning(this, QObject::tr("analyse comparée"),
                                   QObject::tr
                                   ("Le mois de 'début' doit être"
                                    " antérieur au mois de 'fin' !"));
    }

    //"yyyy-MM-dd"; D.toString(YerothUtils::DB_DATE_FORMAT)

    QString annee = comboBox_annee_chiffre_affaire->currentText();
    QString mois_debut = comboBox_mois_debut_chiffre_affaire->currentText();
    QString mois_fin = comboBox_mois_fin_chiffre_affaire->currentText();

    int moisDebut = _moisToNombre[mois_debut];
    int moisFin = _moisToNombre[mois_fin];

    QString dateDebut;
    dateDebut.append(annee).append("-").append(QString::number(moisDebut)).
    append("-01");

    QDate qDateDebut(annee.toInt(), moisDebut, 1);
    QDate qDateFin;

    QString dateFin;
    bool moisFinAjuster = false;

    if (_moisToNombre[YerothTableauxDeBordWindow::MOIS_12] == moisFin)
    {
        dateFin.append(annee).append("-").append(QString::number(moisFin)).
        append("-31");

        qDateFin.setDate(annee.toInt(), moisFin, 31);
    }
    else
    {
        moisFin += 1;
        moisFinAjuster = true;

        dateFin.append(annee).append("-").append(QString::number(moisFin)).
        append("-01");

        qDateFin.setDate(annee.toInt(), moisFin, 1);
    }

    //qDebug() << "calculerChiffresDaffaireMois\n\t" <<
    //                          QString("annee: %1, mois_debut: %2, mois fin ajuster: %3, mois_fin: %4")
    //                                  .arg(annee,
    //                                           QString::number(moisDebut),
    //                                           BOOL_TO_STRING(moisFinAjuster),
    //                                           QString::number(moisFin));

    QString strAchatQuery;

    strAchatQuery.append(QString
                         ("SELECT %1, %2, %3 FROM %4 WHERE %5 >= '%6' AND ").
                         arg(YerothDatabaseTableColumn::DATE_ENTREE,
                             YerothDatabaseTableColumn::PRIX_DACHAT,
                             YerothDatabaseTableColumn::QUANTITE_TOTALE,
                             YerothDatabase::ACHATS,
                             YerothDatabaseTableColumn::DATE_ENTREE,
                             dateDebut));

    if (moisFinAjuster)
    {
        strAchatQuery.append(QString("%1 < '%2'").arg
                             (YerothDatabaseTableColumn::DATE_ENTREE,
                              dateFin));
    }
    else
    {
        strAchatQuery.append(QString("%1 <= '%2'").arg
                             (YerothDatabaseTableColumn::DATE_ENTREE,
                              dateFin));
    }

    //qDebug() << "calculerChiffresDaffaireMois - analyse comparee \n\t" <<
    //                          QString("strAchatQuery: %1 | annee: %2, mois_debut: %3, mois fin ajuster: %4, mois_fin: %5")
    //                                  .arg(strAchatQuery,
    //                                           annee,
    //                                           QString::number(moisDebut),
    //                                           BOOL_TO_STRING(moisFinAjuster),
    //                                           QString::number(moisFin));


    QString
    strQuery(QString
             ("SELECT date_vente, (montant_total_vente - montant_tva) "));

    strQuery.append("FROM ").append(YerothDatabase::STOCKS_VENDU).
    append(" WHERE date_vente >= '").append(dateDebut).append("' AND ");

    if (moisFinAjuster)
    {
        strQuery.append("date_vente < '").append(dateFin).append("'");
    }
    else
    {
        strQuery.append("date_vente <= '").append(dateFin).append("'");
    }

    QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

    bool filtreActif = false;

    QString yerothFiltre;

    QString databaseTableColumn;

    QString
    comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText
                                       ());

    QString
    textFromLineEditEvolutionSujets
    (comboBox_evolution_objets_value->currentText());

    if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CAISSIERS))
    {
        filtreActif = true;

        yerothFiltre.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (QObject::tr("filtre: %1 = %2").arg
                             (YerothTableauxDeBordWindow::OBJET_CAISSIERS,
                              textFromLineEditEvolutionSujets)));

        databaseTableColumn = YerothDatabaseTableColumn::NOM_CAISSIER;
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_CATEGORIES))
    {
        filtreActif = true;

        yerothFiltre.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (QObject::tr("filtre: %1 = %2").arg
                             (YerothTableauxDeBordWindow::OBJET_CATEGORIES,
                              textFromLineEditEvolutionSujets)));

        databaseTableColumn = YerothDatabaseTableColumn::CATEGORIE;
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_CLIENTS))
    {
        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_CLIENTS,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn = YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_ARTICLES))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            filtreActif = false;

            yerothFiltre.append(AUCUN_FILTRE);
        }
        else
        {
            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_ARTICLES,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn = YerothDatabaseTableColumn::DESIGNATION;
        }
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_QUARTIER))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_QUARTIER,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_VILLES))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_VILLES,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_REGIONS_ETATS))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                            (YerothTableauxDeBordWindow::OBJET_REGIONS_ETATS,
                             textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_FOURNISSEURS))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                            (YerothTableauxDeBordWindow::OBJET_FOURNISSEURS,
                             textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR;

    }

    //qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
    //                          .arg(databaseTableColumn, comboBox_evolution_objets->currentText());



    if (true == filtreActif)
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            YerothQMessageBox::warning(this,
                                       QObject::tr("paramètre manquant"),
                                       QObject::tr
                                       ("Vous devez spécifier un paramètre dans"
                                        " le champs de texte 'nom' !"));
            return;
        }
        else
        {
            if (!databaseTableColumn.isEmpty())
            {
                strQuery.append(QString(" AND %1 = '%2'").arg
                                (databaseTableColumn,
                                 YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                 (textFromLineEditEvolutionSujets)));
            }
        }
    }

    //qDebug() << "++ query: " << strQuery;

    QSqlQuery achatQuery;

    QSqlQuery query;

    double montant_total_achat = 0.0;

    double montant_total_vente = 0.0;

    int achatQuerySize =
                    YerothUtils::execQuery(achatQuery, strAchatQuery, _logger);

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    if (0 >= achatQuerySize || 0 >= querySize)
    {

        YerothQMessageBox::information(this,
                                       QObject::tr("analyse comparée"),
                                       QObject::
                                       trUtf8(("Il n'y a pas de données correspondante à la requête !\n" "Vérifier que les dates de début et de fin, " "ainsi que l'année sont correctes !")));
        return;
    }

    QMap < int, double >monthToAchatsTotalAmount;

    QMap < int, double >monthToVentesTotalAmount;

    int curMonth = 0;
    QDate curDate;

    double prix_dachat = 0.0;

    double quantite_total_achat = 0.0;

    double maxAmountAchats = 0.0;
    double maxAmount = 0.0;

    double sommeTotalAchatMois = 0.0;
    double sommeTotalVenteMois = 0.0;

    for (int k = moisDebut; k <= moisFin; ++k)
    {
        sommeTotalAchatMois += monthToAchatsTotalAmount[k];
        sommeTotalVenteMois += monthToVentesTotalAmount[k];
    }

    for (int k = 0; k < achatQuerySize && achatQuery.next(); ++k)
    {
        curDate = achatQuery.value(0).toDate();
        curMonth = curDate.month();

        prix_dachat = achatQuery.value(1).toDouble();

        quantite_total_achat = achatQuery.value(2).toDouble();

        montant_total_achat = quantite_total_achat * prix_dachat;

        //qDebug() << "++ curDate: " << curDate
        //               << ", curMonth: " << curMonth
        //               << ", montant_total_vente: " << montant_total_vente;

        if (0 != curMonth)
        {
            monthToAchatsTotalAmount[curMonth] += montant_total_achat;

            if (maxAmountAchats < monthToAchatsTotalAmount[curMonth])
            {
                maxAmountAchats = monthToAchatsTotalAmount[curMonth];
            }
            /*qDebug() << ", curMonth: " << curMonth
               << ", montant_total_vente: " << monthToVentesTotalAmount[curMonth]; */
        }
    }

    for (int k = 0; k < querySize && query.next(); ++k)
    {
        curDate = query.value(0).toDate();
        curMonth = curDate.month();

        montant_total_vente = query.value(1).toDouble();

        //qDebug() << "++ curDate: " << curDate
        //               << ", curMonth: " << curMonth
        //               << ", montant_total_vente: " << montant_total_vente;

        if (0 != curMonth)
        {
            monthToVentesTotalAmount[curMonth] += montant_total_vente;

            if (maxAmount < monthToVentesTotalAmount[curMonth])
            {
                maxAmount = monthToVentesTotalAmount[curMonth];
            }
            /*qDebug() << ", curMonth: " << curMonth
               << ", montant_total_vente: " << monthToVentesTotalAmount[curMonth]; */
        }
    }

    _reportTexFileEndString.clear();


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        _reportTexFileEndString
            .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (QString("Details in %1:")
                            .arg(YerothERPConfig::currency)));
    }
    else
    {
        _reportTexFileEndString
            .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (QString("Détails en %1:")
                            .arg(YerothERPConfig::currency)));
    }


    _reportTexFileEndString.prepend("\\textbf{").append("}\n");
    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    //Fill in the PDF file which amount of money for which month.

    if (moisFinAjuster)
    {
        moisFin -= 1;
    }

    for (int k = moisDebut; k <= moisFin; ++k)
    {
        _reportTexFileEndString.append("\\item \\textbf{");

        if (YerothMainWindow::LANGUE_ANGLAISE)
        {
            _reportTexFileEndString
                .append(QString("%1}: sales $\\rightarrow %2$, purchases $\\rightarrow %3$\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::englishLocale.monthName(k)),
                                 YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (GET_CURRENCY_STRING_NUM(monthToVentesTotalAmount[k])),
                                 YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (GET_CURRENCY_STRING_NUM(monthToAchatsTotalAmount[k]))));
        }
        else
        {
            _reportTexFileEndString
                .append(QString("%1}: ventes $\\rightarrow %2$, achats $\\rightarrow %3$\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::englishLocale.monthName(k)),
                                 YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (GET_CURRENCY_STRING_NUM(monthToVentesTotalAmount[k])),
                                 YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (GET_CURRENCY_STRING_NUM(monthToAchatsTotalAmount[k]))));
        }
    }

    _reportTexFileEndString.append("\\end{enumerate}\n");

    //qDebug() << "++ END";

    QString barItems;

    double ratioAchats = 0;
    double ratio = 0;

    double maxAmountAll = maxAmount + maxAmountAchats;

    const int TICKS = 100;
    const double MAX_RATIO = 900.0;

    for (int k = moisDebut; k <= moisFin; ++k)
    {
        if (monthToVentesTotalAmount.contains(k))
        {
            //qDebug() << QString("month: %1, amount: %2")
            //                                              .arg(QString::number(k),
            //                                                              QString::number(monthToVentesTotalAmount[3], 'f', 2));
            ratioAchats =
                            (monthToAchatsTotalAmount[k] * MAX_RATIO) / maxAmountAll;

            ratio = (monthToVentesTotalAmount[k] * MAX_RATIO) / maxAmountAll;

            //qDebug() << QString("++ mois(k): %1, max amount achats: %2, ratio achat: %3, max amount ventes: %4, ratio vente: %5")
            //                              .arg(QString::number(k),
            //                                              QString::number(monthToAchatsTotalAmount[k], 'f', 2),
            //                                              QString::number(ratioAchats, 'f', 2),
            //                                              QString::number(monthToVentesTotalAmount[k], 'f', 2),
            //                                              QString::number(ratio, 'f', 2));

            barItems.append(QString
                            ("\\baritem{%1}{%2}{gray}\n		\\subbaritem{}{%3}{purplish}\n").
                            arg(YerothUtils::
                                LATEX_IN_OUT_handleForeignAccents
                                (GET_MONTH_NAME_LOCALIZED(k)),
                                QString::number(ratio, 'f', 2),
                                QString::number(ratioAchats, 'f', 2)));
        }
    }


    //qDebug() << "++ barItems: " << barItems;

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

    QString texDocument;
    texDocument.append(YerothUtils::_1a_tex);

    texDocument.replace("YEROTHBARPERGROUP", "\\barspergroup{2}");
    texDocument.replace("YEROTHBARITEMS", barItems);
    texDocument.replace("YEROTHTICKS", QString::number(TICKS));


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument.replace("YEROTHLEGENDANALYSECOMPAREE",
                            "\\diagLegenditem{Ratio of the monthly purchases.}{purplish}");

        texDocument.replace("YEROTHDIAGRAMMETITRE",
                            "Ratio of the monthly income.");

        texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", "Income Level");
    }
    else
    {
        texDocument.replace("YEROTHLEGENDANALYSECOMPAREE",
                            "\\diagLegenditem{Ratio des achats du mois.}{purplish}");

        texDocument.replace("YEROTHDIAGRAMMETITRE",
                            "Ratio du chiffre d'affaire du mois.");

        texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE",
                            "Niveau du chiffre d'affaire");
    }


    QString fileName1(YerothERPConfig::temporaryFilesDir + "/1a.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }
    tmpFile1.close();

    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument2;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.append(YerothUtils::EN_bar_diag_tex);
    }
    else
    {
        texDocument2.append(YerothUtils::FR_bar_diag_tex);
    }


    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(factureDate);

    QString longDateDebut;
    QString longDateFin;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        longDateDebut =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::englishLocale.toString(qDateDebut)));

        longDateFin =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::englishLocale.toString(qDateFin)));
    }
    else
    {
        longDateDebut =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::frenchLocale.toString(qDateDebut)));

        longDateFin =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::frenchLocale.toString(qDateFin)));
    }


    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerothConfig::typeOfFacturation;


    texDocument2.replace("YEROTHPAPERSPEC", "a4paper");

    if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "caissiers")  ||
        YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "catégories") ||
        YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText, "clients"))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            yerothFiltre.clear();
            yerothFiltre.append(QObject::tr("aucun filtre !"));
        }
    }
    else
    {
        yerothFiltre =
                        YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltre);
    }

    texDocument2.replace("YEROTHMENTION",
                         QObject::tr("[analyse compar\\'ee]"));

    texDocument2.replace("YEROTHFILTRE",
                         yerothFiltre.prepend("''").append("''"));

    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT", longDateDebut);
    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", longDateFin);
    texDocument2.replace("YEROTHCHARTFIN", _reportTexFileEndString);

    texDocument2.replace("YEROTHENTREPRISE",
                         infoEntreprise.getNomCommercial_LATEX());

    texDocument2.replace("YEROTHACTIVITESENTREPRISE",
                         infoEntreprise.getSecteursActivitesTex());

    texDocument2.replace("YEROTHBOITEPOSTALE",
                         infoEntreprise.getBoitePostal());

    texDocument2.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
    texDocument2.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", factureDate);

    texDocument2.replace("YEROTHNOMUTILISATEUR",
                         QString("%1 %2")
                            .arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
                                 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

    texDocument2.replace("YEROTHSUCCURSALE",
                         YerothUtils::LATEX_IN_OUT_handleForeignAccents
						 	 (YerothERPConfig::THIS_SITE_LOCALISATION_NAME));

    texDocument2.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument2.replace("YEROTHCOMPTEBANCAIRENR",
                         infoEntreprise.getNumeroCompteBancaire());

    texDocument2.replace("YEROTHCONTRIBUABLENR",
                         infoEntreprise.getNumeroDeContribuable());

    texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",
                         infoEntreprise.getAgenceCompteBancaireTex());

    texDocument2.replace("1a.tex", fileName1);


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Chart illustrating the income from %1 to %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }
    else
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Diagramme r\\'epr\\'esentatif des chiffres"
                                     " d'affaire du %1 au %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }


    //qDebug() << "++ test: " << texDocument2;

    YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

    QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire"));
    fileName.append(".");

    QString tmpFilePrefix(YerothERPConfig::temporaryFilesDir + "/" + fileName);

    QFile tmpFile(tmpFilePrefix + "tex");

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }

    tmpFile.close();

    //qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;
    QString texRm(tmpFile.fileName().remove(".tex"));
    progArguments << texRm;

    //qDebug() << "++ file name to latex compile bar diag: " << texRm;

    aProcess.start(YerothERPConfig::pathToLatex(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("%1.dvi").arg(texRm);

    aProcess.start(YerothERPConfig::pathToDvips(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("-dALLOWPSTRANSPARENCY");
    progArguments << QString("%1.ps").arg(texRm);
    progArguments << QString("%1.pdf").arg(texRm);

    aProcess.start(YerothERPConfig::pathToPs2Pdf(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

    //emit startPdf(1);
    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}
