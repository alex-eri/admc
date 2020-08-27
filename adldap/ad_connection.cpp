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

//#include "config.h"
#include "ad_connection.h"

#include <iostream>

namespace adldap {

AdConnection::AdConnection() {
}

int AdConnection::connect(std::string uri_arg, std::string domain) {
    uri = uri_arg;

    // "DOMAIN.COM" => "DC=DOMAIN,DC=COM"
    search_base = domain;
    for (size_t i = 0; i < search_base.length(); i++) {
        search_base[i] = std::tolower(search_base[i]);
    }
    search_base = "DC=" + search_base;
    const std::string erase_str = ".";
    const std::string insert_str = ",DC=";
    size_t pos = search_base.find(erase_str);
    if (pos != std::string::npos) {
        search_base.erase(pos, erase_str.length());
        search_base.insert(pos, insert_str);
    }
    
    const int result = ad_login(this->uri.c_str(), &ldap_connection);
    return result;
}

bool AdConnection::is_connected() {
    if (nullptr != this->ldap_connection) {
        return true;
    }
    return false;
}

int AdConnection::get_ldap_result() const {
    const int result = ad_get_ldap_result(ldap_connection);
    return result;
}

std::string AdConnection::get_search_base() const {
    return search_base;
}

std::string AdConnection::get_uri() const {
    return uri;
}

int AdConnection::add(const char *dn, const char **objectClass) {
    return ad_add(ldap_connection, dn, objectClass);
}

int AdConnection::object_delete(const char *dn) {
    return ad_delete(ldap_connection, dn);
}

int AdConnection::search(const char *filter, char ***dn_list) {
    return ad_search(ldap_connection, filter, this->search_base.c_str(), dn_list);
}

int AdConnection::attribute_add(const char *dn, const char *attribute, const char *value) {
    return ad_attribute_add(ldap_connection, dn, attribute, value);
}

int AdConnection::attribute_add_binary(const char *dn, const char *attribute, const char *data, int data_length) {
    return ad_attribute_add_binary(ldap_connection, dn, attribute, data, data_length);
}

int AdConnection::attribute_replace(const char *dn, const char *attribute, const char *value) {
    return ad_attribute_replace(ldap_connection, dn, attribute, value);
}

int AdConnection::attribute_replace_binary(const char *dn, const char *attribute, const char *data, int data_length) {
    return ad_attribute_replace_binary(ldap_connection, dn, attribute, data, data_length);
}

int AdConnection::attribute_delete(const char *dn, const char *attribute, const char *value) {
    return ad_attribute_delete(ldap_connection, dn, attribute, value);
}

int AdConnection::get_all_attributes(const char *dn, char ****attributes) {
    return ad_get_all_attributes(ldap_connection, dn, attributes);
}

int AdConnection::rename(const char *dn, const char *new_name) {
    return ad_rename(ldap_connection, dn, new_name);
}

int AdConnection::move(const char *current_dn, const char *new_container) {
    return ad_move(ldap_connection, current_dn, new_container);
}

int AdConnection::list(const char *dn, char ***dn_list) {
    return ad_list(ldap_connection, dn, dn_list);
}

} /* namespace adldap */

