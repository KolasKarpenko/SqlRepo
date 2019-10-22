#pragma once

#include <TransactionPatch.h>
#include <DataSchema.h>
#include <CreateTablePatch.h>
#include <DropTablePatch.h>
#include <AddTableColumnPatch.h>
#include <RenameTablePatch.h>
#include <RenameColumnPatch.h>
#include <NewObjectPatch.h>

namespace repo
{
namespace schema
{
	static DataSchema ProductSchema("ProductSchema");
	static std::map<std::string, const DataSchema*> AllSchemas;

	static void RegisterSchema() {
		TransactionPatch createProductData(201905261113);
		CreateTablePatch createProductTable("Product");
		createProductTable.AddColumn("name", TableColumnStruct::Type::text, "Product", true);
		NewObjectPatch createProduct("Product");
		createProduct.SetValue("name", SqlString("Product1"));
		createProductData.AddPatch(createProductTable);
		createProductData.AddPatch(createProduct);
		ProductSchema.Register(createProductData);

		CreateTablePatch createInspectionTable(201905261123, "Inspection", "Product");
		createInspectionTable.AddColumn("name", TableColumnStruct::Type::text, "Inspection", true);
		ProductSchema.Register(createInspectionTable);

		CreateTablePatch createRegionTable(201905261135, "Region", "Inspection");
		createRegionTable.AddColumn("name", TableColumnStruct::Type::text, "Region", true);
		ProductSchema.Register(createRegionTable);

		AddTableColumnPatch productDescriptionColumn(201905262011, "Product");
		productDescriptionColumn.AddColumn("description", TableColumnStruct::Type::text);
		ProductSchema.Register(productDescriptionColumn);

		AllSchemas[ProductSchema.GetName()] = &ProductSchema;
	}
}
}