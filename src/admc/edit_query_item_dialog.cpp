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

#include "edit_query_item_dialog.h"
#include "ui_edit_query_item_dialog.h"

#include "console_impls/query_folder_impl.h"

EditQueryItemDialog::EditQueryItemDialog(QWidget *parent)
: QDialog(parent) {
    ui = new Ui::EditQueryItemDialog();
    ui->setupUi(this);
}

EditQueryItemDialog::~EditQueryItemDialog() {
    delete ui;
}

void EditQueryItemDialog::init(AdConfig *adconfig) {
    ui->edit_query_item_widget->init(adconfig);
}

void EditQueryItemDialog::set_data(const QString &name, const QString &description, const bool scope_is_children, const QByteArray &filter_state) {
    return ui->edit_query_item_widget->set_data(name, description, scope_is_children, filter_state);
}

QString EditQueryItemDialog::name() const {
    return ui->edit_query_item_widget->name();
}

QString EditQueryItemDialog::description() const {
    return ui->edit_query_item_widget->description();
}

QString EditQueryItemDialog::filter() const {
    return ui->edit_query_item_widget->filter();
}

QString EditQueryItemDialog::base() const {
    return ui->edit_query_item_widget->base();
}

bool EditQueryItemDialog::scope_is_children() const {
    return ui->edit_query_item_widget->scope_is_children();
}

QByteArray EditQueryItemDialog::filter_state() const {
    return ui->edit_query_item_widget->filter_state();
}

void EditQueryItemDialog::set_sibling_name_list(const QList<QString> &sibling_name_list_arg) {
    sibling_name_list = sibling_name_list_arg;
}

void EditQueryItemDialog::open() {

    QDialog::open();
}

void EditQueryItemDialog::accept() {
    const QString name = ui->edit_query_item_widget->name();
    const bool name_is_valid = console_query_or_folder_name_is_good(name, sibling_name_list, this);

    if (name_is_valid) {
        QDialog::accept();
    }
}
