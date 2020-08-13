/*
 * ADMC - AD Management Center
 *
 * Copyright (C) 2020 BaseALT Ltd.
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

#ifndef RENAME_DIALOG_H
#define RENAME_DIALOG_H

#include <QString>
#include <QDialog>
#include <QList>

class QLineEdit;
class QVBoxLayout;

class RenameDialog final : public QDialog {
Q_OBJECT

public:
    RenameDialog(const QString &target_arg, QWidget *parent);

private slots:
    void on_accepted();

private:
    struct AttributeEdit {
        QString attribute;
        QLineEdit *edit;
    };

    QString target;
    QVBoxLayout *label_layout;
    QVBoxLayout *edit_layout;
    AttributeEdit name_edit;
    QList<AttributeEdit> edits;

    void add_attribute_edit(const QString &attribute, const QString &label_text);
};

#endif /* RENAME_DIALOG_H */
