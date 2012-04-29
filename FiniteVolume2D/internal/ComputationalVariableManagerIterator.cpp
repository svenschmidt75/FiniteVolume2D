#include "ComputationalVariableManagerIterator.h"

#include <exception>


ComputationalVariableManagerIterator::Variables_t ComputationalVariableManagerIterator::end_data_;


ComputationalVariableManagerIterator::ComputationalVariableManagerIterator(Variables_t const & data) : data_(data), it_(data_.begin()), type_(NON_END) {}

// the "end" of iteration
ComputationalVariableManagerIterator::ComputationalVariableManagerIterator() : data_(end_data_), type_(END) {}

ComputationalVariableManagerIterator::ComputationalVariableManagerIterator(ComputationalVariableManagerIterator const & in)
    : data_(in.data_), item_(in.item_) {}

ComputationalVariableManagerIterator::Item_t const &
ComputationalVariableManagerIterator::operator*() const {
    if (it_ != data_.end()) {
        item_.name      = it_->first;
        item_.flux_eval = it_->second.flux_eval;
        return item_;
    }
    throw std::runtime_error("ComputationalVariableManagerIterator::operator*()");
}

ComputationalVariableManagerIterator::Item_t const *
ComputationalVariableManagerIterator::operator->() const {
    if (it_ != data_.end()) {
        item_.name      = it_->first;
        item_.flux_eval = it_->second.flux_eval;
        return &item_;
    }
    throw std::runtime_error("ComputationalVariableManagerIterator::operator->()");
}

ComputationalVariableManagerIterator const &
ComputationalVariableManagerIterator::operator++() const {
    if (it_ != data_.end()) {
        ++it_;
        if (it_ == data_.end())
            type_ = END;
        return *this;
    }
    throw std::runtime_error("ComputationalVariableManagerIterator::operator++()");
}

bool
ComputationalVariableManagerIterator::operator==(ComputationalVariableManagerIterator const & in) const {
    if (type_ == END) {
        if (in.type_ == END)
            return true;

        return false;
    }
    else if (in.type_ == END)
        return false;

    return it_ == in.it_;
}

bool
ComputationalVariableManagerIterator::operator!=(ComputationalVariableManagerIterator const & in) const {
    return !(*this == in);
}
