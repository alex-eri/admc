/*
 * ADMC - AD Management Center
 *
 * Copyright (C) 2020-2022 BaseALT Ltd.
 * Copyright (C) 2020-2022 Dmitry Degtyarev
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

#include "policy_ou_results_widget.h"
#include "ui_policy_ou_results_widget.h"

#include "console_widget/console_widget.h"

#include <QModelIndex>

PolicyOUResultsWidget::PolicyOUResultsWidget(ConsoleWidget *console_arg)
: QWidget(console_arg) {
    ui = new Ui::PolicyOUResultsWidget();
    ui->setupUi(this);

    console = console_arg;
    ui->inheritance_widget->set_console(console);
    ui->links_widget->set_console(console);

    connect(ui->links_widget, &LinkedPoliciesWidget::gplink_changed,
            this, &PolicyOUResultsWidget::update_inheritance_widget);
}

PolicyOUResultsWidget::~PolicyOUResultsWidget() {
    delete ui;
}

void PolicyOUResultsWidget::update(const QModelIndex &index) {
    update_inheritance_widget(index);
    update_links_widget(index);
}

void PolicyOUResultsWidget::update_inheritance_widget(const QModelIndex &index) {
    ui->inheritance_widget->update(index);
}

void PolicyOUResultsWidget::update_links_widget(const QModelIndex &index) {
    ui->links_widget->update(index);
}
