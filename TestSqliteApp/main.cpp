// SqliteTestApp.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sqlitesession/Session.h>
#include <Dal.h>
#include <Schema.h>
#include <DataSchema.h>
#include <TransactionPatch.h>
#include <PatchFactory.h>
#include <JsonReader.h>

repo::ProductSchema::Product selectProduct(repo::ISession& s)
{
	std::stringstream selectProduct;
	selectProduct << "select * from " << repo::ProductSchema::Product::TableName << " limit 1;";
	repo::ProductSchema::Product result;
	bool hasProduct = false;
	s.ExecSql(selectProduct.str().c_str(), [&result, &hasProduct](const repo::IRow& row) {
		result = row;
		hasProduct = true;
	});

	if (!hasProduct) {
		result.Set_name("Product 1");
		repo::TransactionPatch tr;
		result.Save(tr);
		tr.ApplyJournal(s);
	}

	return result;
}

void insertData(repo::ISession& s)
{
	repo::TransactionPatch tr;

	const repo::ProductSchema::Product product = selectProduct(s);

	repo::ProductSchema::Inspection inspection1;
	inspection1.Set_parentId(product.Get_id());
	inspection1.Set_name("inspection 1");
	inspection1.Save(tr);

	repo::ProductSchema::Region region1;
	region1.Set_parentId(inspection1.Get_id());
	region1.Set_name("region 1");
	region1.Save(tr);

	repo::ProductSchema::Region region2;
	region2.Set_parentId(inspection1.Get_id());
	region2.Set_name("region 2");
	region2.Save(tr);

	repo::ProductSchema::Inspection inspection2;
	inspection2.Set_parentId(product.Get_id());
	inspection2.Set_name("inspection 2");
	inspection2.Save(tr);

	repo::ProductSchema::Region region3;
	region3.Set_parentId(inspection2.Get_id());
	region3.Set_name("region 3");
	region3.Save(tr);

	tr.ApplyJournal(s);
}

void copyData(repo::ISession& s)
{
	std::stringstream selectRegion3;
	selectRegion3 << "select * from " << repo::ProductSchema::Region::TableName << " where " << repo::ProductSchema::Region::Field::name << " = 'region 3' limit 1";
	s.ExecSql(selectRegion3.str().c_str(), [&s](const repo::IRow& row) {
		repo::TransactionPatch tr;
		repo::ProductSchema::Region r(row);
		repo::ProductSchema::Region copy = r.Copy();
		copy.Set_name("copy region 3");
		copy.Save(tr);
		tr.ApplyJournal(s);
	});
}

void editData(repo::ISession& s)
{
	std::stringstream selectProduct1;
	selectProduct1 << "select * from " << repo::ProductSchema::Product::TableName << " where " << repo::ProductSchema::Product::Field::name << " = 'Product 1' limit 1;";
	s.ExecSql(selectProduct1.str().c_str(), [&s](const repo::IRow& row) {
		repo::TransactionPatch tr;
		repo::ProductSchema::Product p(row);
		p.Set_description(repo::SqlString("test product"));
		p.Save(tr);
		tr.ApplyJournal(s);
	});
}

void selectData(repo::ISession& s)
{
	std::cout << "Products" << std::endl;
	std::cout << "------------------------" << std::endl;
	std::cout << selectProduct(s).ToJson() << std::endl;

	std::cout << "Inspections" << std::endl;
	std::stringstream selectInspections;
	selectInspections << "select * from " << repo::ProductSchema::Inspection::TableName;
	s.ExecSql(selectInspections.str().c_str(), [](const repo::IRow& row) {
		repo::ProductSchema::Inspection i(row);
		std::cout << "------------------------" << std::endl;
		std::cout << i.ToJson() << std::endl;
	});

	std::cout << "Regions" << std::endl;
	std::stringstream selectRegions;
	selectRegions << "select * from " << repo::ProductSchema::Region::TableName;
	s.ExecSql(selectRegions.str().c_str(), [](const repo::IRow& row) {
		repo::ProductSchema::Region r(row);
		std::cout << "------------------------" << std::endl;
		std::cout << r.ToJson() << std::endl;
	});
}

void selectJournal(repo::ISession& s)
{
	std::stringstream selectJournal;
	selectJournal << "select * from " << repo::ProductSchema::private_Journal::TableName << " order by id;";
	std::cout << "Journal" << std::endl;
	s.ExecSql(selectJournal.str().c_str(), [](const repo::IRow& row) {
		const repo::ProductSchema::private_Journal t(row);
		std::cout << "------------------------" << std::endl;
		std::cout << t.Get_id() << ": " << t.Get_time() << ": " << t.Get_patchJson().Data() << std::endl;
	});
}

int main(int argc, char* argv[])
{
	try {
		repo::schema::RegisterSchema();
		repo::sqlite::Session s = repo::sqlite::Session::Get("test.repo");
		repo::schema::ProductSchema.UpdateSchema(s);

		//insertData(s);
		//editData(s);
		//copyData(s);

		/*
		repo::sqlite::Session s2 = repo::sqlite::Session::Get("test_deep_copy.repo");
		repo::schema::ProductSchema.UpdateSchema(s2);
		auto deepCopy = selectProduct(s).DeepCopy(s);
		repo::TransactionPatch tr;
		deepCopy.Save(tr);
		tr.ApplyJournal(s2);
		*/
		

		selectData(s);
		//selectJournal(s);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}

