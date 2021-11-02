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

#ifndef MULTI_EDITOR_H
#define MULTI_EDITOR_H

#include "editors/attribute_editor.h"

class QListWidgetItem;

enum MultiEditorType {
    MultiEditorType_String,
    MultiEditorType_Octet,
    MultiEditorType_Datetime,
};

namespace Ui {
class MultiEditor;
}

class MultiEditor final : public AttributeEditor {
    Q_OBJECT

public:
    Ui::MultiEditor *ui;

    MultiEditor(QWidget *parent);
    ~MultiEditor();

    void set_attribute(const QString &attribute) override;
    void set_read_only(const bool read_only) override;

    void set_value_list(const QList<QByteArray> &values) override;
    QList<QByteArray> get_value_list() const override;

private:
    AttributeEditor *add_string_editor;
    AttributeEditor *add_bool_editor;
    AttributeEditor *edit_string_editor;
    AttributeEditor *edit_octet_editor;
    AttributeEditor *edit_datetime_editor;
    QListWidgetItem *edited_item;

    void on_add_button();
    void on_remove_button();
    void on_item_double_clicked(QListWidgetItem *item);
    void add_value(const QByteArray value);
    QString bytes_to_string(const QByteArray bytes) const;
    QByteArray string_to_bytes(const QString string) const;
    MultiEditorType get_editor_type() const;
    void on_add_string_editor_accepted();
    void on_add_bool_editor_accepted();
    void add_values_from_editor(AttributeEditor *editor);
    void on_edit_string_editor_accepted();
    void on_edit_octet_editor_accepted();
    void on_edit_datetime_editor_accepted();
    void edit_values_from_editor(AttributeEditor *editor);
};

#endif /* MULTI_EDITOR_H */
