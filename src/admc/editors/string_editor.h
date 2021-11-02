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

#ifndef STRING_EDITOR_H
#define STRING_EDITOR_H

#include "editors/attribute_editor.h"

namespace Ui {
class StringEditor;
}

class StringEditor final : public AttributeEditor {
    Q_OBJECT

public:
    Ui::StringEditor *ui;

    StringEditor(QWidget *parent);
    ~StringEditor();

    void set_attribute(const QString &attribute) override;
    void set_read_only(const bool read_only) override;
    void set_value_list(const QList<QByteArray> &value_list) override;
    QList<QByteArray> get_value_list() const override;
};

#endif /* STRING_EDITOR_H */
