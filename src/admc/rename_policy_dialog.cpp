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

#include "rename_policy_dialog.h"
#include "ui_rename_policy_dialog.h"

#include "adldap.h"
#include "globals.h"
#include "rename_object_dialog.h"
#include "settings.h"
#include "status.h"
#include "utils.h"

RenamePolicyDialog::RenamePolicyDialog(AdInterface &ad, const QString &target_dn_arg, QWidget *parent)
: QDialog(parent) {
    ui = new Ui::RenamePolicyDialog();
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    target_dn = target_dn_arg;

    target_name = [&]() {
        const AdObject object = ad.search_object(target_dn);

        return object.get_string(ATTRIBUTE_DISPLAY_NAME);
    }();

    ui->name_edit->setText(target_name);

    settings_setup_dialog_geometry(SETTING_rename_policy_dialog_geometry, this);
}

RenamePolicyDialog::~RenamePolicyDialog() {
    delete ui;
}

void RenamePolicyDialog::accept() {
    AdInterface ad;
    if (ad_failed(ad)) {
        return;
    }

    const QString new_name = ui->name_edit->text();
    const bool apply_success = ad.attribute_replace_string(target_dn, ATTRIBUTE_DISPLAY_NAME, new_name);

    if (apply_success) {
        RenameObjectDialog::success_msg(target_name);
    } else {
        RenameObjectDialog::fail_msg(target_name);
    }

    g_status->display_ad_messages(ad, this);

    if (apply_success) {
        QDialog::accept();
    }
}
