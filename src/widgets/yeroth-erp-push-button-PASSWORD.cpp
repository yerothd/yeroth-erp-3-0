/*
 * yeroth-erp-push-button-PASSWORD.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-push-button-PASSWORD.hpp"


YerothPushButtonPASSWORD::YerothPushButtonPASSWORD(QObject *parent,
                                                   YerothLineEdit *
                                                   lineEdit_mot_passe
                                                   /* = 0 */,
                                                   YerothPushButton *
                                                   cacher_mot_de_passe
                                                   /* = 0 */,
                                                   YerothPushButton *
                                                   voir_mot_de_passe
                                                   /* = 0 */)
    :

    QObject(parent), _lineEdit_mot_passe(lineEdit_mot_passe),
    _cacher_mot_de_passe(cacher_mot_de_passe),
    _voir_mot_de_passe(voir_mot_de_passe)
{
    if (0 != _cacher_mot_de_passe)
    {
        _cacher_mot_de_passe->enable(this, SLOT(cacher_le_mot_de_passe()));
        _cacher_mot_de_passe->setEnabled(false);
    }

    if (0 != _voir_mot_de_passe)
    {
        _voir_mot_de_passe->enable(this, SLOT(voir_le_mot_de_passe()));
        _voir_mot_de_passe->setEnabled(true);
    }
}


void YerothPushButtonPASSWORD::cacher_le_mot_de_passe()
{
    if (0 != _lineEdit_mot_passe)
    {
        _lineEdit_mot_passe->setEchoMode(QLineEdit::Password);
    }

    if (0 != _cacher_mot_de_passe)
    {
        _cacher_mot_de_passe->setEnabled(false);
    }

    if (0 != _voir_mot_de_passe)
    {
        _voir_mot_de_passe->setEnabled(true);
    }
}


void YerothPushButtonPASSWORD::voir_le_mot_de_passe()
{
    if (0 != _lineEdit_mot_passe)
    {
        _lineEdit_mot_passe->setEchoMode(QLineEdit::Normal);
    }

    if (0 != _cacher_mot_de_passe)
    {
        _cacher_mot_de_passe->setEnabled(true);
    }

    if (0 != _voir_mot_de_passe)
    {
        _voir_mot_de_passe->setEnabled(false);
    }
}
