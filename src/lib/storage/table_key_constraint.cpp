#include "table_key_constraint.hpp"

namespace opossum {

TableKeyConstraint::TableKeyConstraint(std::unordered_set<ColumnID> init_columns, KeyConstraintType init_key_type)
    : AbstractTableConstraint(TableConstraintType::Key, std::move(init_columns)), _key_type(init_key_type) {}

KeyConstraintType TableKeyConstraint::key_type() const { return _key_type; }

bool TableKeyConstraint::_on_equals(const AbstractTableConstraint& table_constraint) const {
  DebugAssert(dynamic_cast<const TableKeyConstraint*>(&table_constraint),
              "Different table_constraint type should have been caught by AbstractTableConstraint::operator==");
  return key_type() == static_cast<const TableKeyConstraint&>(table_constraint).key_type();
}

size_t TableKeyConstraint::size() const {
  size_t size = 0;
  size += sizeof(AbstractTableConstraint);
  size += sizeof(KeyConstraintType);

  return size;
}

}  // namespace opossum
