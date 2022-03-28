#pragma once

#include <tbb/concurrent_vector.h>

#include "abstract_table_constraint.hpp"

namespace opossum {

/**
 * Container class to define order constraints for tables.
 */
class TableOrderConstraint final : public AbstractTableConstraint {
 public:
  TableOrderConstraint(std::vector<ColumnID> init_determinants, std::vector<ColumnID> init_dependents);

  const std::vector<ColumnID>& determinants() const;
  const std::vector<ColumnID>& dependents() const;

  size_t size() const override;

 protected:
  bool _on_equals(const AbstractTableConstraint& table_constraint) const override;

 private:
  std::vector<ColumnID> _determinants;
  std::vector<ColumnID> _dependents;
};

using TableOrderConstraints = tbb::concurrent_vector<TableOrderConstraint>;

}  // namespace opossum
