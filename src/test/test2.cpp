#include "storage/table.hpp"

int main() {
	opossum::table t;

	t.add_column("a", opossum::column_type::int_type);
	t.add_column("b", opossum::column_type::float_type);
	t.add_column("c", opossum::column_type::string_type);

	t.append({123, 456.7, "test"});

	t.print();
}