/*
 * Name  : SourceTerm
 * Path  : 
 * Use   : Contains the constant source term, i.e. that contribution
 *         that is not linked to any cell.
 * Author: Sven Schmidt
 * Date  : 05/19/2012
 */
#pragma once


class SourceTerm {
public:
    SourceTerm();

    void operator+=(double const value);

private:
    double value_;
};
