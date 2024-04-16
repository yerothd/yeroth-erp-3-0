/*
 * yeroth-erp-qcheckbox.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_QCHECKBOX_HPP_
#define SRC_WIDGETS_YEROTH_QCHECKBOX_HPP_


#include "src/utils/yeroth-erp-utils.hpp"


#include <QtWidgets/QCheckBox>


class YerothERPQCheckBox : public QCheckBox
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	inline YerothERPQCheckBox(QWidget *parent)
	:QCheckBox(parent)
	{
	}

    virtual inline ~YerothERPQCheckBox()
    {
    }

    void setReadOnly(bool isReadOnly);

    void setYerothChecked(bool checked,
    					  const QString &text = YerothUtils::EMPTY_STRING);

    void setYerothEnabled(bool isEnabled);
};

#endif /* SRC_WIDGETS_YEROTH_QCHECKBOX_HPP_ */
