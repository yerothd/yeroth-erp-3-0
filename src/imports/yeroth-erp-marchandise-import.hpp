/*
 * yeroth-erp-stock-marchandise.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_MARCHANDISE_IMPORT_WINDOW_HPP_
#define YEROTH_MARCHANDISE_IMPORT_WINDOW_HPP_


#include "src/imports/yeroth-erp-stock-import.hpp"


class YerothERPMarchandiseImport:public YerothERPStockImport
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS YerothERPMarchandiseImport(QStringList &
                                                      aCurCsvFileToImportContentWordList,
                                                      QMap < int,
                                                      YerothERPDatabaseTableColumnInfo *
                                                      > &aCsvContentIdxToDatabaseTableColumnInfo);


    YerothERPMarchandiseImport(YerothPOSAdminWindowsCommons &aCallingWindow,
                               QStringList &
                               aCurCsvFileToImportContentWordList,
                               QMap < int,
                               YerothERPDatabaseTableColumnInfo *
                               > &anIndexToDatabaseTableColumnInfo);

    virtual inline ~YerothERPMarchandiseImport()
    {
    }

    virtual inline int import_csv_file()
    {
        return YerothERPStockImport::import_csv_file();
    }

    virtual inline int import(bool importerParlant = false)
    {
        return YerothERPStockImport::import(importerParlant);
    }

protected:

    virtual enum import_csv_entry_row_return_status import_csv_entry_row(bool importerParlant,
                                                                         QStringList &aCsvFileEntryLine);
};


#endif /* YEROTH_MARCHANDISE_IMPORT_WINDOW_HPP_ */
