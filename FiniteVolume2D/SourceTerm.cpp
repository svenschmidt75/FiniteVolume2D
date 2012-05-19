#include "SourceTerm.h"


SourceTerm::SourceTerm() : value_(0) {}

void
SourceTerm::operator+=(double const value) {
    value_ += value;
}
