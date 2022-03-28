#pragma once

#include <tbb/concurrent_vector.h>

#include "abstract_table_constraint.hpp"

namespace opossum {

enum class KeyConstraintType { PRIMARY_KEY, UNIQUE };

/**
 * Container class to define uniqueness constraints for tables.
 * As defined by SQL, two types of keys are supported: PRIMARY KEY and UNIQUE keys.
 */
class TableKeyConstraint final : public AbstractTableConstraint {
 public:
  TableKeyConstraint(std::unordered_set<ColumnID> init_columns, KeyConstraintType init_key_type);

  KeyConstraintType key_type() const;

  size_t size() const override;

 protected:
  bool _on_equals(const AbstractTableConstraint& table_constraint) const override;

 private:
  KeyConstraintType _key_type;
};

using TableKeyConstraints = tbb::concurrent_vector<TableKeyConstraint>;

}  // namespace opossum
