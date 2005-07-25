// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2005 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

// $XORP: 

#ifndef __LIBXORP_RANGE_HH__
#define __LIBXORP_RANGE_HH__

#include <sstream>

class IPv4;
class IPv6;

/**
 * @short A template class for implementing linear ranges X..Y
 *
 * We define a linear range by its low and high inclusive boundaries.
 * It's the user's responisibility to ensure that the condition
 * (_low <= _high) always holds!
 */
template <class T>
class Range {
public:
    /**
     * Default constructor
     */
    Range() 			{ _low = _high = 0; }

    /**
     * Constructor from a single value.
     */
    explicit Range(T value)	{ _low = _high = value; }

    /**
     * Constructor from two values.
     */
    explicit Range(T low, T high) {
	_low = low;
	_high = high;
    }

    inline bool operator==(const T& other) const {
	return (_low <= other && _high >= other);
    }

    inline bool operator!=(const T& other) const {
	return (_low > other || _high < other);
    }

    inline bool operator<(const T& other) const {
	return (_high < other);
    }

    inline bool operator<=(const T& other) const {
	return (_low <= other);
    }

    inline bool operator>(const T& other) const {
	return (_low > other);
    }

    inline bool operator>=(const T& other) const {
	return (_high >= other);
    }

    const T& low() const { return _low; }
    const T& high() const { return _high; }

protected:
    T _low;
    T _high;
};

/**
 * @short A linear range class (uint32_t low)..(uint32_t high)
 *
 * Inherits from templatized general Range<uint32_t> class.
 * Provides specialized constructor from string and str() method.
 */
class U32Range: public Range<uint32_t> {
public:
    /**
     * Default constructor
     */
    U32Range() 			{ _low = _high = 0; }

    /**
     * Constructor from a string.
     */
    U32Range(const char *from_cstr) {
	string from_string = string(from_cstr);
	int delim = from_string.find("..", 0);
	if (delim < 0)
	    _low = _high = strtoul(from_cstr, NULL, 10);
	else if (delim > 0 && (from_string.length() - delim > 2)) {
	    _low = strtoul(from_string.substr(0, delim).c_str(), NULL, 10);
	    _high = strtoul(from_string.substr(delim + 2, from_string.length()).c_str(), NULL, 10);
	} else {
	    xorp_throw(InvalidString, "Syntax error");
	}
    }

    /**
     * Convert the range to a human-readable format.
     *
     * @return C++ string.
     */
    string str() const {
	ostringstream os;
	os << _low;
	if (_low < _high)
	    os << ".." << _high;
	return os.str();
    }
};


/**
 * @short A linear IPvX class template (IPvX low)..(IPvX high)
 *
 * Inherits from templatized general Range<IPv4|IPv6> class.
 * Provides a specialized constructor from string and str() method.
 */
template <class T>
class IPvXRange: public Range<T> {
public:
    /**
     * Default constructor
     */
    IPvXRange()			{ _low = _high = 0; }

    /**
     * Constructor from a string.
     */
    IPvXRange(const char *from_cstr) {
	string from_string = string(from_cstr);
	int delim = from_string.find("..", 0);
	if (delim < 0)
	    _low = _high = T(from_cstr);
	else if (delim > 0 && (from_string.length() - delim > 2)) {
	    _low = T(from_string.substr(0, delim).c_str());
	    _high = T(from_string.substr(delim + 2, from_string.length()).c_str());
	} else {
	    xorp_throw(InvalidString, "Syntax error");
	}
    }

    /**
     * Convert the range to a human-readable format.
     *
     * @return C++ string.
     */
    string str() const {
	ostringstream os;
	os << _low.str();
	if (_low < _high)
	    os << ".." << _high.str();
	return os.str();
    }
};

typedef IPvXRange<IPv4> IPv4Range;
typedef IPvXRange<IPv6> IPv6Range;

#endif // __LIBXORP_RANGE_HH__
