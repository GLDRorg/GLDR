/**
 *  @file oglplus/site-config.hpp
 *  @brief System configuration options
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_SITE_CONFIG_120305_HPP
#define OGLPLUS_SITE_CONFIG_120305_HPP

#define OGLPLUS_USE_GLCOREARB_H 0
#define OGLPLUS_USE_GL3_H 0
#define OGLPLUS_USE_GLEW 1
#define OGLPLUS_USE_GL3W 0

#if !OGLPLUS_USE_BOOST_CONFIG
#define OGLPLUS_NO_SCOPED_ENUMS 0
#define OGLPLUS_NO_VARIADIC_MACROS 0
#define OGLPLUS_NO_VARIADIC_TEMPLATES 1
#define OGLPLUS_NO_UNIFIED_INITIALIZATION_SYNTAX 1
#define OGLPLUS_NO_INITIALIZER_LISTS 1
#define OGLPLUS_NO_DEFAULTED_FUNCTIONS 1
#define OGLPLUS_NO_DELETED_FUNCTIONS 1
#define OGLPLUS_NO_EXPLICIT_CONVERSION_OPERATORS 1
#define OGLPLUS_NO_FUNCTION_TEMPLATE_DEFAULT_ARGS 1
#define OGLPLUS_NO_USER_DEFINED_LITERALS 1
#define OGLPLUS_NO_CONSTEXPR 1
#define OGLPLUS_NO_NOEXCEPT 1
#define OGLPLUS_NO_LAMBDAS 0
#define OGLPLUS_NO_NULLPTR 0

#define OGLPLUS_NO_CHRONO 0
#endif

#pragma warning(disable : 4244)

#endif
