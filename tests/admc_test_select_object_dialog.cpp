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

#include "admc_test_select_object_dialog.h"

#include "select_object_dialog.h"
#include "console_types/console_object.h"

#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QTreeView>

void ADMCTestSelectObjectDialog::init() {
    ADMCTest::init();

    dialog = new SelectObjectDialog({CLASS_USER}, SelectObjectDialogMultiSelection_Yes, test_arena_dn(), parent_widget);
    dialog->open();
    QVERIFY(QTest::qWaitForWindowExposed(dialog, 1000));

    edit = dialog->findChild<QLineEdit *>("edit");
    QVERIFY(edit != nullptr);

    add_button = dialog->findChild<QPushButton *>("add_button");
    QVERIFY(add_button != nullptr);
}

void ADMCTestSelectObjectDialog::empty() {
    const QList<QString> selected = dialog->get_selected();
    QVERIFY(selected.isEmpty());
}

void ADMCTestSelectObjectDialog::no_matches() {
    const QString dn = test_object_dn(TEST_USER, CLASS_USER);
    const bool create_success = ad.object_add(dn, CLASS_USER);
    QVERIFY(create_success);

    edit->setText("no-match");

    close_message_box_later();
    add_button->click();

    const QList<QString> selected = dialog->get_selected();
    QVERIFY(selected.isEmpty());
}

void ADMCTestSelectObjectDialog::one_match() {
    const QString dn = test_object_dn(TEST_USER, CLASS_USER);
    const bool create_success = ad.object_add(dn, CLASS_USER);
    QVERIFY(create_success);

    edit->setText(TEST_USER);
    add_button->click();

    const QList<QString> selected = dialog->get_selected();
    QVERIFY(selected == QList<QString>({dn}));
}

void ADMCTestSelectObjectDialog::multiple_matches() {
    const QString dn1 = test_object_dn(QString(TEST_USER) + "1", CLASS_USER);
    const bool create_success1 = ad.object_add(dn1, CLASS_USER);
    QVERIFY(create_success1);

    const QString dn2 = test_object_dn(QString(TEST_USER) + "2", CLASS_USER);
    const bool create_success2 = ad.object_add(dn2, CLASS_USER);
    QVERIFY(create_success2);

    edit->setText(TEST_USER);
    add_button->click();

    auto match_dialog = dialog->findChild<SelectObjectMatchDialog *>();
    QVERIFY(match_dialog != nullptr);

    auto match_dialog_view = match_dialog->findChild<QTreeView *>();
    QVERIFY(match_dialog_view != nullptr);

    navigate_until_object(match_dialog_view, dn2, ObjectRole_DN);

    auto ok_button = match_dialog->findChild<QPushButton *>();
    QVERIFY(ok_button != nullptr);
    ok_button->click();

    const QList<QString> selected = dialog->get_selected();
    QVERIFY(selected == QList<QString>({dn2}));
}

QTEST_MAIN(ADMCTestSelectObjectDialog)
