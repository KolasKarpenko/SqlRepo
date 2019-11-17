// GenerateDal.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <sqlitesession/Session.h>
#include <uuid.h>
#include <TableCppClass.h>
#include <Schema.h>
#include <DataSchema.h>

int main(int argc, char* argv[])
{
	try {
		repo::schema::RegisterSchema();

		const std::string outPath = argc > 1 ? argv[1] : "./";

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

		fileCpp << "/*" << std::endl;
		fileCpp << "This file is auto-generated" << std::endl;
		fileCpp << "*/" << std::endl << std::endl;
		fileCpp << "#include \"" << headerName << "\"" << std::endl;
		fileCpp << "#include <Tools.h>" << std::endl;
		fileCpp << "namespace repo" << std::endl;
		fileCpp << std::endl;
		fileCpp << "{" << std::endl;

		for (const auto& schema : repo::schema::AllSchemas) {
			fileH << "namespace " << schema.first << std::endl;
			fileH << "{" << std::endl << std::endl;
			fileCpp << "namespace " << schema.first << std::endl;
			fileCpp << "{" << std::endl << std::endl;

			repo::sqlite::Session session = repo::sqlite::Session::Get(schema.first);
			schema.second->UpdateSchema(session);

			std::vector<std::string> tableList;
			session.ExecSql("select name from sqlite_master where type='table' and name not like 'sqlite_%';", [&tableList](const repo::IRow& row) {
				const repo::SqlString& name = row.Data("name");
				if (!name.IsNull())
					tableList.push_back(name.Data());
			});

			for (const auto& table : tableList) {
				repo::cpp::TableCppClass tableCpp(session, table);

				fileH << tableCpp.GetHeaderText();
				fileH << std::endl;

				fileCpp << tableCpp.GetCppText(headerName);
				fileCpp << std::endl;
			}

			fileH << "} //namespace " << schema.first << std::endl;
			fileCpp << "} //namespace " << schema.first << std::endl;
		}

		fileH << "} //namespace repo" << std::endl;

		fileCpp << "} //namespace repo" << std::endl;

		fileH.close();
		fileCpp.close();
	}
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}

