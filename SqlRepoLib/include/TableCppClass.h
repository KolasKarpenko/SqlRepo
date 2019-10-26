#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include "ISession.h"
#include "TableColumnStruct.h"

namespace repo {
namespace cpp {

class TableCppClass
{
public:
	struct Column
	{
		Column(const IRow& row, const std::string& tableName);

		void Getter(std::stringstream& ss) const;
		void ImplementGetter(std::stringstream& ss) const;
		void Setter(std::stringstream& ss) const;
		void ImplementSetter(std::stringstream& ss) const;
		void Member(std::stringstream& ss) const;
		void DefaultValue(std::stringstream& ss) const;
		void ImplementDefaultValue(std::stringstream& ss) const;
		std::string GetCType() const;
		std::string GetFullType() const;
		void GetRowMethod(std::stringstream& ss, bool notNull = false) const;

		repo::TableColumnStruct columnStruct;
		const std::string tableName;
	};

	TableCppClass(ISession& session, const std::string& tableName);

	std::string GetHeaderText() const;
	std::string GetCppText(const std::string& headerPath) const;

private:
	void BeginClass(std::stringstream& ss) const;
	void Constructor(std::stringstream& ss) const;
	void ImplementConstructor(std::stringstream& ss) const;
	void EndClass(std::stringstream& ss) const;
	void Public(std::stringstream& ss) const;
	void SaveMethod(std::stringstream& ss) const;
	void DeleteMethod(std::stringstream& ss) const;
	void ToJsonMethod(std::stringstream& ss) const;
	void FromJsonMethod(std::stringstream& ss) const;
	void CopyMethod(std::stringstream& ss) const;
	void DeepCopyMethod(std::stringstream& ss) const;
	void Private(std::stringstream& ss) const;
	void ImplementNames(std::stringstream& ss) const;
	void ImplementSave(std::stringstream& ss) const;
	void ImplementDelete(std::stringstream& ss) const;
	void ImplementToJson(std::stringstream& ss) const;
	void ImplementFromJson(std::stringstream& ss) const;
	void ImplementCopy(std::stringstream& ss) const;
	void ImplementDeepCopy(std::stringstream& ss) const;
	bool IsTablePrivate() const;

	const std::string m_name;
	std::vector<Column> m_columns;
	std::string m_parentName;
	repo::ISession& m_session;
};

}
}

