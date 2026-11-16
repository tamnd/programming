// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

#pragma once

#include <exception>
#include <string>

// parse_port returns the port number in text. It throws std::invalid_argument
// or std::out_of_range.
int parse_port(const std::string &text);

// read_port returns the port number. On an error, it throws a
// std::runtime_error with the place, nested with the error of parse_port.
int read_port(const std::string &where, const std::string &text);

// chain joins the message of e and the messages of the nested exceptions.
std::string chain(const std::exception &e);

// check_setting returns "port N", or "error: " and the chain of messages.
std::string check_setting(const std::string &where, const std::string &text);
