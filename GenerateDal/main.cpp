// GenerateDal.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <sqlitesession/Session.h>
#include <uuid.h>
#include <TableCppClass.h>
#include <Schema.h>
#include <DataSchemaDeploy.h>

int main(int argc, char* argv[])
{
	try {
		repo::schema::RegisterSchema();

		if (argc < 2) {
			std::cout << "Syntax: ./GenerateDal <data_file_name> <ouput_dir_path>";
			return 0;
		}

		repo::sqlite::Session session = repo::sqlite::Session::Get(argv[1]);
		//TODO Update different schemas with different namespaces
		repo::schema::ProductSchema.UpdateSchema(session);

		const std::string outPath = argc > 2 ? argv[2] : "./";

		std::vector<std::string> tableList;
		session.ExecSql("select name from sqlite_master where type='table' and name not like 'sqlite_%';", [&tableList](const repo::IRow& row) {
			const repo::SqlString& name = row.Data("name");
			if (!name.IsNull())
				tableList.push_back(name.Data());
		});

		std::ofstream fileH;
		std::ofstream fileCpp;
		const std::string headerName = "Dal.h";
		const std::string cppName = "Dal.cpp";
		fileH.open(outPath + "/" + headerName);
		fileCpp.open(outPath + "/" + cppName);

		fileH << "/*" << std::endl;
		fileH << "This file is auto-generated" << std::endl;
		fileH << "*/" << std::endl << std::endl;
		fileH << "#pragma once" << std::endl << std::endl;
		fileH << "#include <json/json.h>" << std::endl;
		fileH << "#include <IRow.h>" << std::endl;
		fileH << "#include <NewObjectPatch.h>" << std::endl;
		fileH << "#include <DeleteObjectPatch.h>" << std::endl;
		fileH << "#include <TransactionPatch.h>" << std::endl;
		fileH << std::endl;
		fileH << "namespace repo" << std::endl;
		fileH << "{" << std::endl;
		fileH << "namespace dal" << std::endl;
		fileH << "{" << std::endl << std::endl;

		fileCpp << "/*" << std::endl;
		fileCpp << "This file is auto-generated" << std::endl;
		fileCpp << "*/" << std::endl << std::endl;
		fileCpp << "#include \"" << headerName << "\"" << std::endl;
		fileCpp << "namespace repo" << std::endl;
		fileCpp << "{" << std::endl;
		fileCpp << "namespace dal" << std::endl;
		fileCpp << "{" << std::endl << std::endl;

		for (const auto& table : tableList) {
			repo::cpp::TableCppClass tableCpp(session, table);

			fileH << tableCpp.GetHeaderText();
			fileH << std::endl;

			fileCpp << tableCpp.GetCppText(headerName);
			fileCpp << std::endl;
		}

		fileH << "} //namespace dal" << std::endl;
		fileH << "} //namespace repo" << std::endl;

		fileCpp << "} //namespace dal" << std::endl;
		fileCpp << "} //namespace repo" << std::endl;

		fileH.close();
		fileCpp.close();
	}
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}

