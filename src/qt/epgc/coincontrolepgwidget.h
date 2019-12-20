// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef COINCONTROLEPGWIDGET_H
#define COINCONTROLEPGWIDGET_H

#include <QDialog>

namespace Ui {
class CoinControlEpgWidget;
}

class CoinControlEpgWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CoinControlEpgWidget(QWidget *parent = nullptr);
    ~CoinControlEpgWidget();

private:
    Ui::CoinControlEpgWidget *ui;
};

#endif // COINCONTROLEPGWIDGET_H
