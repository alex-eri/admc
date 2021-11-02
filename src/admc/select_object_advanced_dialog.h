/*
 * ADMC - AD Management Center
 *
 * Copyright (C) 2020-2021 BaseALT Ltd.
 * Copyright (C) 2020-2021 Dmitry Degtyarev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SELECT_OBJECT_ADVANCED_DIALOG_H
#define SELECT_OBJECT_ADVANCED_DIALOG_H

/**
 * Find objects and select them. 
 */

#include <QDialog>

namespace Ui {
class SelectObjectAdvancedDialog;
}

class SelectObjectAdvancedDialog final : public QDialog {
    Q_OBJECT

public:
    Ui::SelectObjectAdvancedDialog *ui;

    SelectObjectAdvancedDialog(const QList<QString> classes, QWidget *parent);
    ~SelectObjectAdvancedDialog();

    QList<QString> get_selected_dns() const;
};

#endif /* SELECT_OBJECT_ADVANCED_DIALOG_H */
