/*
 * This file is part of PowerDNS or dnsdist.
 * Copyright -- PowerDNS.COM B.V. and its contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * In addition, for the avoidance of any doubt, permission is granted to
 * link this program with OpenSSL and to (re)distribute the binaries
 * produced as the result of such linking.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include "svc-records.hh"
#include "misc.hh"
#include "base64.hh"

const std::map<std::string, SvcParam::SvcParamKey> SvcParam::SvcParams = {
  {"mandatory", SvcParam::SvcParamKey::mandatory},
  {"alpn", SvcParam::SvcParamKey::alpn},
  {"no-default-alpn", SvcParam::SvcParamKey::no_default_alpn},
  {"port", SvcParam::SvcParamKey::port},
  {"ipv4hint", SvcParam::SvcParamKey::ipv4hint},
  {"ech", SvcParam::SvcParamKey::ech},
  {"ipv6hint", SvcParam::SvcParamKey::ipv6hint}
};

SvcParam::SvcParamKey SvcParam::keyFromString(const std::string& k) {
  auto it = SvcParams.find(k);
  if (it != SvcParams.end()) {
    return it->second;
  }
  if (k.substr(0, 3) == "key") {
    try {
      return SvcParam::SvcParamKey(pdns_stou(k.substr(3)));
    }
    catch (...) {
    }
  }
  throw std::invalid_argument("SvcParam '" + k + "'is not recognized or in keyNNNN format");
}

std::string SvcParam::keyToString(const SvcParam::SvcParamKey& k) {
  auto ret = std::find_if(SvcParams.begin(), SvcParams.end(), [&](const std::pair<std::string, SvcParam::SvcParamKey>& e) { return e.second == k; });
  if (ret != SvcParams.end()) {
    return ret->first;
  }
  return "key" + std::to_string(k);
}

SvcParam::SvcParam() {};

SvcParam::SvcParam(const SvcParamKey &key) {
  d_key = key;
  if (d_key != SvcParamKey::no_default_alpn) {
    throw std::invalid_argument("can not create non-empty SvcParam for key '" + keyToString(key) + "'");
  }
}

SvcParam::SvcParam(const SvcParamKey &key, const std::string &value) {
  d_key = key;
  if (d_key != SvcParamKey::ech && d_key < 7) {
    throw std::invalid_argument("can not create SvcParam for " + keyToString(key) + " with a string value");
  }
  if (d_key == SvcParamKey::ech) {
    std::string d;
    // TODO check Base64 decode
    d_ech = value;
    return;
  }
  d_value = value;
}

SvcParam::SvcParam(const SvcParamKey &key, std::vector<std::string> &&value) {
  d_key = key;
  if (d_key != SvcParamKey::alpn) {
    throw std::invalid_argument("can not create SvcParam for " + keyToString(key) + " with a string-set value");
  }
  if (d_key == SvcParamKey::alpn) {
    d_alpn = std::move(value);
  }
}

SvcParam::SvcParam(const SvcParamKey &key, std::set<std::string> &&value) {
  d_key = key;
  if (d_key != SvcParamKey::mandatory) {
    throw std::invalid_argument("can not create SvcParam for " + keyToString(key) + " with a string-set value");
  }
  if (d_key == SvcParamKey::mandatory) {
    for (auto const &v: value) {
      d_mandatory.insert(keyFromString(v));
    }
  }
}

SvcParam::SvcParam(const SvcParamKey &key, std::set<SvcParam::SvcParamKey> &&value) {
  d_key = key;
  if (d_key != SvcParamKey::mandatory) {
    throw std::invalid_argument("can not create SvcParam for " + keyToString(key) + " with a string-set value");
  }
  d_mandatory = std::move(value);
}

SvcParam::SvcParam(const SvcParamKey &key, std::vector<ComboAddress> &&value) {
  d_key = key;
  if (d_key != SvcParamKey::ipv6hint && d_key != SvcParamKey::ipv4hint) {
    throw std::invalid_argument("can not create SvcParam for " + keyToString(key) + " with an IP address value");
  }
  for (auto const &addr : value) {
    if (d_key == SvcParam::ipv6hint && !addr.isIPv6()) {
      throw std::invalid_argument("non-IPv6 address ('" + addr.toString() + "') passed for " + keyToString(key));
    }
    if (d_key == SvcParam::ipv4hint && !addr.isIPv4()) {
      throw std::invalid_argument("non-IPv4 address ('" + addr.toString() + "') passed for " + keyToString(key));
    }
  }
  d_ipHints = std::move(value);
}

SvcParam::SvcParam(const SvcParamKey &key, const uint16_t value) {
  d_key = key;
  if (d_key != SvcParamKey::port) {
    throw std::invalid_argument("can not create SvcParam for " + keyToString(key) + " with an port value");
  }
  d_port = value;
}

//! This ensures an std::set<SvcParam> will be sorted by key (section 2.2 mandates this for wire format)
bool SvcParam::operator<(const SvcParam& other) const {
  return this->d_key < other.getKey();
}

const std::vector<ComboAddress>& SvcParam::getIPHints() const {
  if (d_key != SvcParamKey::ipv6hint && d_key != SvcParamKey::ipv4hint) {
    throw std::invalid_argument("getIPHints called for non-IP address key '" + keyToString(d_key) + "'");
  }
  return d_ipHints;
}

uint16_t SvcParam::getPort() const {
  if (d_key != SvcParam::port) {
    throw std::invalid_argument("getPort called for non-port key '" + keyToString(d_key) + "'");
  }
  return d_port;
}

const std::vector<std::string>& SvcParam::getALPN() const {
  if (d_key != SvcParam::alpn) {
    throw std::invalid_argument("getALPN called for non-alpn key '" + keyToString(d_key) + "'");
  }
  return d_alpn;
}

const std::set<SvcParam::SvcParamKey>& SvcParam::getMandatory() const {
  if (d_key != SvcParam::mandatory) {
    throw std::invalid_argument("getMandatory called for non-mandatory key '" + keyToString(d_key) + "'");
  }
  return d_mandatory;
}

const std::string& SvcParam::getECH() const {
  if (d_key != SvcParam::ech) {
    throw std::invalid_argument("getECH called for non-ech key '" + keyToString(d_key) + "'");
  }
  return d_ech;
}

const std::string& SvcParam::getValue() const {
  if (d_key < 7) {
    throw std::invalid_argument("getValue called for non-single value key '" + keyToString(d_key) + "'");
  }
  return d_value;
}
