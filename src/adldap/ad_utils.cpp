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

#include "ad_utils.h"

#include "ad_config.h"
#include "ad_display.h"
#include "samba/dom_sid.h"

#include <krb5.h>
#include <ldap.h>

#include <QByteArray>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QLocale>
#include <QString>
#include <QTranslator>

#define GENERALIZED_TIME_FORMAT_STRING "yyyyMMddhhmmss.zZ"
#define UTC_TIME_FORMAT_STRING "yyMMddhhmmss.zZ"

const QDate ntfs_epoch = QDate(1601, 1, 1);

bool large_integer_datetime_is_never(const QString &value) {
    const bool is_never = (value == AD_LARGE_INTEGER_DATETIME_NEVER_1 || value == AD_LARGE_INTEGER_DATETIME_NEVER_2);

    return is_never;
}

QString datetime_qdatetime_to_string(const QString &attribute, const QDateTime &datetime, const AdConfig *adconfig) {
    if (adconfig == nullptr) {
        return QString();
    }

    const AttributeType type = adconfig->get_attribute_type(attribute);

    switch (type) {
        case AttributeType_LargeInteger: {
            const qint64 millis = QDateTime(ntfs_epoch).msecsTo(datetime);
            const qint64 hundred_nanos = millis * MILLIS_TO_100_NANOS;

            return QString::number(hundred_nanos);

            break;
        }
        case AttributeType_UTCTime: {
            return datetime.toString(UTC_TIME_FORMAT_STRING);

            break;
        }
        case AttributeType_GeneralizedTime: {
            return datetime.toString(GENERALIZED_TIME_FORMAT_STRING);

            break;
        }
        default: return "";
    }

    return "";
}

QDateTime datetime_string_to_qdatetime(const QString &attribute, const QString &raw_value, const AdConfig *adconfig) {
    if (adconfig == nullptr) {
        return QDateTime();
    }

    const AttributeType type = adconfig->get_attribute_type(attribute);

    QDateTime datetime = [=]() {
        switch (type) {
            case AttributeType_LargeInteger: {
                const LargeIntegerSubtype subtype = adconfig->get_attribute_large_integer_subtype(attribute);

                if (subtype == LargeIntegerSubtype_Datetime) {
                    QDateTime out = QDateTime(ntfs_epoch);

                    const qint64 hundred_nanos = raw_value.toLongLong();
                    const qint64 millis = hundred_nanos / MILLIS_TO_100_NANOS;
                    out = out.addMSecs(millis);

                    return out;
                } else {
                    break;
                }
            }
            case AttributeType_GeneralizedTime: {
                return QDateTime::fromString(raw_value, GENERALIZED_TIME_FORMAT_STRING);
            }
            case AttributeType_UTCTime: {
                return QDateTime::fromString(raw_value, UTC_TIME_FORMAT_STRING);
            }
            default: break;
        }
        return QDateTime();
    }();

    datetime.setTimeSpec(Qt::UTC);

    return datetime;
}

QString account_option_string(const AccountOption &option) {
    switch (option) {
        case AccountOption_Disabled: return QCoreApplication::translate("ad_utils", "Account disabled");
        case AccountOption_CantChangePassword: return QCoreApplication::translate("ad_utils", "User cannot change password");
        case AccountOption_PasswordExpired: return QCoreApplication::translate("ad_utils", "User must change password on next logon");
        case AccountOption_DontExpirePassword: return QCoreApplication::translate("ad_utils", "Don't expire password");
        case AccountOption_UseDesKey: return QCoreApplication::translate("ad_utils", "Store password using reversible encryption");
        case AccountOption_SmartcardRequired: return QCoreApplication::translate("ad_utils", "Smartcard is required for interactive logon");
        case AccountOption_CantDelegate: return QCoreApplication::translate("ad_utils", "Account is sensitive and cannot be delegated");
        case AccountOption_DontRequirePreauth: return QCoreApplication::translate("ad_utils", "Don't require Kerberos preauthentication");
        case AccountOption_COUNT: return QString("AccountOption_COUNT");
    }

    return "";
}

int account_option_bit(const AccountOption &option) {
    switch (option) {
        case AccountOption_Disabled:
            return 0x00000002;
        case AccountOption_DontExpirePassword:
            return 0x00010000;
        case AccountOption_UseDesKey:
            return 0x00200000;
        case AccountOption_SmartcardRequired:
            return 0x00040000;
        case AccountOption_DontRequirePreauth:
            return 0x00400000;
        case AccountOption_CantDelegate:
            return 0x00100000;

        // NOTE: not all account options can be directly
        // mapped to bits
        case AccountOption_CantChangePassword: return 0;
        case AccountOption_PasswordExpired: return 0;
        case AccountOption_COUNT: return 0;
    }

    return 0;
}

int group_scope_bit(GroupScope scope) {
    switch (scope) {
        case GroupScope_Global: return 0x00000002;
        case GroupScope_DomainLocal: return 0x00000004;
        case GroupScope_Universal: return 0x00000008;
        case GroupScope_COUNT: return 0;
    }
    return 0;
}

QString group_scope_string(GroupScope scope) {
    switch (scope) {
        case GroupScope_Global: return QCoreApplication::translate("ad_utils", "Global");
        case GroupScope_DomainLocal: return QCoreApplication::translate("ad_utils", "Domain Local");
        case GroupScope_Universal: return QCoreApplication::translate("ad_utils", "Universal");
        case GroupScope_COUNT: return "COUNT";
    }
    return "";
}

QString group_type_string(GroupType type) {
    switch (type) {
        case GroupType_Security: return QCoreApplication::translate("ad_utils", "Security");
        case GroupType_Distribution: return QCoreApplication::translate("ad_utils", "Distribution");
        case GroupType_COUNT: return "COUNT";
    }
    return "";
}

// NOTE: need a special translation for Russian where type
// adjectives get suffixes.
QString group_type_string_adjective(GroupType type) {
    switch (type) {
        case GroupType_Security: return QCoreApplication::translate("ad_utils", "Security Group");
        case GroupType_Distribution: return QCoreApplication::translate("ad_utils", "Distribution Group");
        case GroupType_COUNT: return "COUNT";
    }
    return "";
}

QString extract_rid_from_sid(const QByteArray &sid, AdConfig *adconfig) {
    const QString sid_string = attribute_display_value(ATTRIBUTE_OBJECT_SID, sid, adconfig);
    const int cut_index = sid_string.lastIndexOf("-") + 1;
    const QString rid = sid_string.mid(cut_index);

    return rid;
}

bool ad_string_to_bool(const QString &string) {
    return (string == LDAP_BOOL_TRUE);
}

// "CN=foo,CN=bar,DC=domain,DC=com"
// =>
// "CN=foo"
QString dn_get_rdn(const QString &dn) {
    const QStringList exploded_dn = dn.split(',');
    const QString rdn = exploded_dn[0];

    return rdn;
}

// "CN=foo,CN=bar,DC=domain,DC=com"
// =>
// "foo"
QString dn_get_name(const QString &dn) {
    int equals_i = dn.indexOf('=') + 1;
    int comma_i = dn.indexOf(',');
    int segment_length = comma_i - equals_i;

    QString name = dn.mid(equals_i, segment_length);

    return name;
}

QString dn_get_parent(const QString &dn) {
    const int comma_i = dn.indexOf(',');
    const QString parent_dn = dn.mid(comma_i + 1);

    return parent_dn;
}

QString dn_get_parent_canonical(const QString &dn) {
    const QString parent_dn = dn_get_parent(dn);

    return dn_canonical(parent_dn);
}

QString dn_rename(const QString &dn, const QString &new_name) {
    const QStringList exploded_dn = dn.split(',');

    const QString new_rdn = [=]() {
        const QString old_rdn = exploded_dn[0];
        const int prefix_index = old_rdn.indexOf('=') + 1;
        const QString prefix = old_rdn.left(prefix_index);

        return (prefix + new_name);
    }();

    QStringList new_exploded_dn(exploded_dn);
    new_exploded_dn.replace(0, new_rdn);

    const QString new_dn = new_exploded_dn.join(',');

    return new_dn;
}

QString dn_move(const QString &dn, const QString &new_parent_dn) {
    const QString rdn = dn_get_rdn(dn);
    const QString new_dn = QString("%1,%2").arg(rdn, new_parent_dn);

    return new_dn;
}

// "CN=foo,CN=bar,DC=domain,DC=com"
// =>
// "domain.com/bar/foo"
QString dn_canonical(const QString &dn) {
    char *canonical_cstr = ldap_dn2ad_canonical(cstr(dn));
    const QString canonical = QString(canonical_cstr);
    ldap_memfree(canonical_cstr);

    return canonical;
}

QString dn_from_name_and_parent(const QString &name, const QString &parent, const QString &object_class) {
    const QString suffix = [object_class]() {
        if (object_class == CLASS_OU) {
            return "OU";
        } else {
            return "CN";
        }
    }();
    const QString dn = QString("%1=%2,%3").arg(suffix, name, parent);

    return dn;
}

QString get_default_domain_from_krb5() {
    krb5_error_code result;
    krb5_context context;
    char *realm_cstr = NULL;

    auto cleanup = [&]() {
        krb5_free_default_realm(context, realm_cstr);
        krb5_free_context(context);
    };

    result = krb5_init_context(&context);
    if (result) {
        qDebug() << "Failed to init krb5 context";

        cleanup();
        return QString();
    }

    result = krb5_get_default_realm(context, &realm_cstr);
    if (result) {
        qDebug() << "Failed to get default realm";

        cleanup();
        return QString();
    }

    const QString out = QString(realm_cstr);

    cleanup();

    return out;
}

// Transform domain to domain DN
// "DOMAIN.COM" => "DC=domain,DC=com"
QString domain_to_domain_dn(const QString &domain) {
    QString domain_dn = domain;

    domain_dn = domain_dn.toLower();
    domain_dn = "DC=" + domain_dn;
    domain_dn = domain_dn.replace(".", ",DC=");

    return domain_dn;
}

int bit_set(int bitmask, int bit, bool set) {
    if (set) {
        return bitmask | bit;
    } else {
        return bitmask & ~bit;
    }
}

bool bit_is_set(int bitmask, int bit) {
    return ((bitmask & bit) != 0);
}

const char *cstr(const QString &qstr) {
    static QList<QByteArray> buffer;

    const QByteArray bytes = qstr.toUtf8();
    buffer.append(bytes);

    // Limit buffer to 100 strings
    if (buffer.size() > 100) {
        buffer.removeAt(0);
    }

    // NOTE: return data of bytes in buffer NOT the temp local bytes
    return buffer.last().constData();
}

bool load_adldap_translation(QTranslator &translator, const QLocale &locale) {
    return translator.load(locale, "adldap", "_", ":/adldap");
}

QByteArray guid_string_to_bytes(const QString &guid_string) {
    const QList<QByteArray> segment_list = [&]() {
        QList<QByteArray> out;

        const QList<QString> string_segment_list = guid_string.split('-');

        for (const QString &string_segment : string_segment_list) {
            const QByteArray segment = QByteArray::fromHex(string_segment.toLatin1());
            out.append(segment);
        }

        std::reverse(out[0].begin(), out[0].end());
        std::reverse(out[1].begin(), out[1].end());
        std::reverse(out[2].begin(), out[2].end());

        return out;
    }();

    const QByteArray guid_bytes = [&]() {
        QByteArray out;

        for (const QByteArray &segment : segment_list) {
            out.append(segment);
        }

        return out;
    }();

    return guid_bytes;
}

QByteArray sid_string_to_bytes(const QString &sid_string) {
    dom_sid sid;
    string_to_sid(&sid, cstr(sid_string));

    const QByteArray sid_bytes = QByteArray((char *) &sid, sizeof(dom_sid));

    return sid_bytes;
}
