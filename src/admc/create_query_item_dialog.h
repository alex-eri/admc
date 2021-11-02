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

#ifndef CREATE_QUERY_ITEM_DIALOG_H
#define CREATE_QUERY_ITEM_DIALOG_H

#include <QDialog>

class EditQueryItemWidget;

namespace Ui {
class CreateQueryItemDialog;
}

class CreateQueryItemDialog : public QDialog {
    Q_OBJECT

public:
    Ui::CreateQueryItemDialog *ui;

    CreateQueryItemDialog(QWidget *parent);
    ~CreateQueryItemDialog();

    EditQueryItemWidget *edit_widget() const;

    void set_sibling_name_list(const QList<QString> &list);

    void open() override;
    void accept() override;

private:
    QList<QString> sibling_name_list;
};

#endif /* CREATE_QUERY_ITEM_DIALOG_H */
