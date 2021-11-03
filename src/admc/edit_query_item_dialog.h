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

#ifndef EDIT_QUERY_ITEM_DIALOG_H
#define EDIT_QUERY_ITEM_DIALOG_H

#include <QDialog>

class EditQueryItemWidget;
class AdConfig;

namespace Ui {
class EditQueryItemDialog;
}

class EditQueryItemDialog : public QDialog {
    Q_OBJECT

public:
    Ui::EditQueryItemDialog *ui;

    EditQueryItemDialog(QWidget *parent);
    ~EditQueryItemDialog();

    void init(AdConfig *adconfig);

    void set_data(const QString &name, const QString &description, const bool scope_is_children, const QByteArray &filter_state);

    QString name() const;
    QString description() const;
    QString filter() const;
    QString base() const;
    bool scope_is_children() const;
    QByteArray filter_state() const;

    void set_sibling_name_list(const QList<QString> &sibling_name_list_arg);

    void open() override;
    void accept() override;

private:
    QList<QString> sibling_name_list;
};

#endif /* EDIT_QUERY_ITEM_DIALOG_H */
