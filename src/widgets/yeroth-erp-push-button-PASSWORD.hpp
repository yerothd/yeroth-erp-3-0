/*
 * yeroth-erp-push-button-PASSWORD.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_PUSH_BUTTON_PASSWORD_HPP_
#define SRC_WIDGETS_YEROTH_PUSH_BUTTON_PASSWORD_HPP_


#include "src/widgets/yeroth-erp-push-button.hpp"


class YerothPushButtonPASSWORD:public QObject
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS YerothPushButtonPASSWORD(QObject *parent,
                                                    YerothLineEdit *lineEdit_mot_passe = 0,
                                                    YerothPushButton *cacher_mot_de_passe = 0,
                                                    YerothPushButton *voir_mot_de_passe = 0);

    inline virtual ~YerothPushButtonPASSWORD()
    {
    }

    inline YerothLineEdit *get_LINEEDIT_mot_de_passe()
    {
        return _lineEdit_mot_passe;
    }

    inline YerothPushButton *get_PUSH_BUTTON_voir_le_mot_de_passe()
    {
        return _voir_mot_de_passe;
    }

    inline YerothPushButton *get_PUSH_BUTTON_cacher_mot_de_passe()
    {
        return _cacher_mot_de_passe;
    }


    inline void set_LINEEDIT_mot_de_passe(YerothLineEdit *lineEdit_mot_passe)
    {
        _lineEdit_mot_passe = lineEdit_mot_passe;
    }

    inline void set_PUSH_BUTTON_voir_le_mot_de_passe(YerothPushButton *
                                                     voir_mot_de_passe)
    {
        voir_mot_de_passe = _voir_mot_de_passe;
    }

    inline void set_PUSH_BUTTON_cacher_mot_de_passe(YerothPushButton *
                                                    cacher_mot_de_passe)
    {
        _cacher_mot_de_passe = cacher_mot_de_passe;
    }

public slots:
    void voir_le_mot_de_passe();

    void cacher_le_mot_de_passe();

private:

    YerothLineEdit *_lineEdit_mot_passe;

    YerothPushButton *_voir_mot_de_passe;

    YerothPushButton *_cacher_mot_de_passe;
};


#endif /* SRC_WIDGETS_YEROTH_PUSH_BUTTON_PASSWORD_HPP_ */
