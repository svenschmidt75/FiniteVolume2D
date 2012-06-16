#include "ComputationalVariable.h"

#include "ComputationalCell.h"


ComputationalVariable::ComputationalVariable(std::shared_ptr<ComputationalCell> const & cell, std::string const & name, Id_t unique_id)
    : cell_(cell), name_(name), unique_id_(unique_id) {}

bool
ComputationalVariable::operator==(ComputationalVariable const & in) const {
    return name_ == in.name_ && cell_ == in.cell_;
}

std::string const &
ComputationalVariable::getName() const {
    return name_;
}

ComputationalVariable::Id_t
ComputationalVariable::id() const {
    return unique_id_;
}

ComputationalCell::Ptr const &
ComputationalVariable::getCell() const {
    return cell_;
}

ComputationalVariable::Ptr
ComputationalVariable::create(std::shared_ptr<ComputationalCell> const & cell, std::string const & name, Id_t unique_id) {
    return ComputationalVariable::Ptr(new ComputationalVariable(cell, name, unique_id));
}
